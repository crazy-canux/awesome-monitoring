#include "nagios_common_snmp.h"

int init_plugins_gettext(){
	if (setlocale( LC_ALL, "" )==NULL) return 1;
	bindtextdomain( PACKAGE, LOCALEDIR );
	textdomain( PACKAGE );
	return 0;
}

void init_options() {
	o_host=NULL;
	o_verb=0;
	error=NULL; 
	o_port=161;
	o_timeout=0; /* timeout (3s) in uS */
	retries=3;
	snmp_bulk=0;
	o_v2c=0;
	o_help=o_verbose=o_version=0;
	o_community=NULL;
	o_login=NULL;
	o_passwd=NULL;
	o_privpasswd=NULL;
	o_protocols[0]=NULL;o_protocols[1]=NULL;
	o_authproto=NULL;o_privproto=NULL;
	/* Init of output variable */
	output[0]=0;
	output_overflow=0;
	p_output=NULL;
}

int check_common_options(){
	
	if (o_help==1) return 2;  
	if (o_version==1) return 3;  
	o_verb=(int)o_verbose; 
	
	if (o_host==NULL) { printf(_("Set hostname or IP !\n"));return 1;}

	if ( (o_community==NULL) && (o_login==NULL || o_passwd==NULL)) 
	{printf(_("Put snmp login info!\n"));return 1;}
	
	if ((o_login!=NULL || o_passwd!=NULL) && (o_community!=NULL || o_v2c==1) )
	{ printf (_("Can't mix snmp v1,2c,3 protocols!\n")); return 1;}

	if (o_protocols[0]!=NULL) {
		if (o_login==NULL || o_passwd==NULL) 
		{ printf(_("Put snmp V3 login info with protocols!\n")); return 1;}
		o_authproto=strdup(o_protocols[0]);  /*  Auth protocol */
		if (o_protocols[1]!=NULL) { /*  Priv  protocol */
			if (o_privpasswd==NULL) {printf(_("Put snmp V3 priv login info with priv protocols!\n")); return 1;}			
			o_privproto=strdup(o_protocols[1]);
		}  		
	}
	/* put default for snmpv3 protocols */
	if (o_authproto ==NULL) o_authproto=strdup("md5");
	if (o_privproto ==NULL) o_privproto=strdup("des");
	
	/* set snmp version depending on options */
	o_snmpver= SNMP_V1;
	if (o_v2c==1) o_snmpver= SNMP_V2C;
	if (o_login!=NULL) {
		if (o_privpasswd!=NULL) o_snmpver=SNMP_V3_PRIV;
		else o_snmpver=SNMP_V3_NOPRIV;
	}
	
	/* Put timeout in uS if set in options */
	if (o_timeout!=0) o_timeout *= 1000000;	
	else o_timeout=TIMEOUT_DEFAULT*1000000;
	
	return 0;
}

void copy_oid( oid* dst,size_t *dst_len, oid*src, size_t src_len) {
	int i;
	for (i=0;i<src_len;i++) 
	dst[i]=src[i];
	*dst_len=src_len;
}

void copy_oid_index( oid* dst,size_t *dst_len, oid*src, size_t src_len, int index) {
	int i;
	for (i=0;i<src_len;i++) 
	dst[i]=src[i];
	dst[src_len]=index;
	*dst_len=src_len+1;
}

snmp_table* search_oid(oid* base_oid,size_t base_oid_len,snmp_table * val_table) {
	int found_oid=0; /*int i;*/
	while ((val_table != NULL) && (found_oid==0)) {
		/*printf ("Query OID 1 (%i):",base_oid_len);for (i=0;i<base_oid_len;i++) printf(".%lu",base_oid[i]);printf("\n");
		printf ("Query OID 2 (%i):",val_table->name_length);for (i=0;i<val_table->name_length;i++) printf(".%lu",val_table->name[i]);printf("\n\n");*/
		if ((base_oid[base_oid_len-1]==val_table->name[val_table->name_length-1]) 
			&& (snmp_oid_compare(base_oid,base_oid_len,val_table->name,val_table->name_length) == 0)) found_oid=1;
		else
			val_table=val_table->next;
	}
	if (found_oid==0) return NULL;	
	return val_table;
}

int get_value_int(oid* base_oid,size_t base_oid_len, int index_oid,snmp_table * val_table,int *returned_val) {
	base_oid[base_oid_len++]=(oid) index_oid;
	if ((val_table=search_oid(base_oid,base_oid_len,val_table)) == NULL) return 1;
	*returned_val=val_table->value_l;
	return 0;
}
int get_value_ulint(oid* base_oid,size_t base_oid_len, int index_oid,snmp_table * val_table,unsigned long *returned_val) {
	base_oid[base_oid_len++]=(oid) index_oid;
	if ((val_table=search_oid(base_oid,base_oid_len,val_table)) == NULL) return 1;
	*returned_val=val_table->value_l;
	return 0;
}

int get_value_long(oid* base_oid,size_t base_oid_len, int index_oid,snmp_table * val_table,unsigned long long *returned_val) {
	base_oid[base_oid_len++]=(oid) index_oid;
	if ((val_table=search_oid(base_oid,base_oid_len,val_table)) == NULL) return 1;
	*returned_val=val_table->value_ull;
	return 0;
}

int init_snmp_session() {

	switch (o_snmpver) {
	case SNMP_V1:
		if (o_verb) printf ("V1 login : %s\n",o_community);
		snmp_bulk=0;
		ss=init_session_v1(&session,o_host,o_community, o_port,o_timeout,retries,&error);			
		break;
	case SNMP_V2C:
		if (o_verb) printf ("V2c login : %s\n",o_community);
		snmp_bulk=1;
		ss=init_session_v2(&session,o_host,o_community,o_port, o_timeout,retries,&error);	
		break;
	case SNMP_V3_PRIV:
		snmp_bulk=1;
		if (o_verb) printf ("V3 AuthPriv login : %s with %s,%s\n",o_login,o_authproto,o_privproto);
		ss = init_session_v3 ( &session,o_host,o_login,o_passwd,o_authproto,o_privpasswd,o_privproto,1,o_port,o_timeout,retries,&error);
		break;
	case SNMP_V3_NOPRIV:
		snmp_bulk=1;
		if (o_verb) printf ("V3 AuthNoPriv login : %s with %s\n",o_login,o_authproto);
		ss = init_session_v3 ( &session,o_host,o_login,o_passwd,o_authproto,o_privpasswd,o_privproto,0, o_port, o_timeout,retries,&error);
		break;
	default:
		error=strdup(_("Unknown snmp protocol\n"));
		return 1;
	}
	if (ss==NULL) return 1;
	return 0;
}

netsnmp_session * init_session_v1 (
netsnmp_session * session,
char* host, char* community,
int port, long int timeout,
int retries,
char** error)
{
	netsnmp_session * ss;
	char * host_port;
	asprintf(&host_port, "%s:%i",host,port);
	
	init_snmp("Nagios_snmp");
	snmp_sess_init( session );              /* set up defaults */
	session->peername = strdup(host_port);		/* host:port */
	session->version = SNMP_VERSION_1;		/* set the SNMP version number */
	session->timeout=timeout;				/* set timeout (in uS) */
	session->retries=retries;				/* Set number of retries (* timeout for total timeout) */
	session->community = (u_char*) strdup(community);	/* set the SNMPv1 community name used for authentication */
	session->community_len = strlen((char *)session->community);

	/*Open the session */
	SOCK_STARTUP; 								/* for Windows */
	ss = snmp_open(session);                     /* establish the session */
	/* Get error in case something went wrong */
	if (ss==NULL) snmp_error(session, &session->s_errno, &session->s_snmp_errno, error);
	free(host_port);
	return ss;	
}

netsnmp_session * init_session_v2 (
netsnmp_session * session,
char* host, char* community,
int port, long int timeout,
int retries,
char** error)
{
	netsnmp_session * ss;
	char * host_port;
	asprintf(&host_port, "%s:%i",host,port);
	
	init_snmp("Nagios_snmp");
	snmp_sess_init( session );              /* set up defaults */
	session->peername = strdup(host_port);		/* host */
	session->version = SNMP_VERSION_2c;		/* set the SNMP version number */
	session->timeout=timeout;				/* set timeout (in uS) */
	session->retries=retries;				/* Set number of retries (* timeout for total timeout) */
	session->community = (u_char*) strdup(community);	/* set the SNMPv1 community name used for authentication */
	session->community_len = strlen((char *)session->community);

	/*Open the session */
	SOCK_STARTUP; 								/* for Windows */
	ss = snmp_open(session);                     /* establish the session */
	/* Get error in case something went wrong */
	if (ss==NULL) snmp_error(session, &session->s_errno, &session->s_snmp_errno, error);
	free(host_port);
	return ss;	
}

netsnmp_session * init_session_v3 (
netsnmp_session * session,
char* host, 
char* login, char* passwd, char* authproto, /*  Login/pass & proto */
char* privpass, char* privproto,	   /*  encryption key & proto */
int priv,				   /*  == 1 : use AuthPriv */
int port, long int timeout,
int retries,
char** error)
{       
	netsnmp_session * ss;
	char * host_port; 
	asprintf(&host_port, "%s:%i",host,port);
	
	init_snmp("Nagios_snmp");
	snmp_sess_init( session );              /* set up defaults */
	session->peername = strdup(host_port);          /* host */
	session->version = SNMP_VERSION_3;             /* set the SNMP version number */
	session->timeout=timeout;                               /* set timeout (in uS) */
	session->retries=retries;                               /* Set number of retries (* timeout for total timeout) */
	
	session->securityName = strdup(login);
	session->securityNameLen = strlen(session->securityName);

	/* set the authentication method to MD5 */
	if (strcmp(authproto,"md5")==0) {
		session->securityAuthProto = usmHMACMD5AuthProtocol;
		session->securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
	} else {
		if (strcmp(authproto,"sha")==0) {
			session->securityAuthProto = usmHMACSHA1AuthProtocol;
			session->securityAuthProtoLen = sizeof(usmHMACSHA1AuthProtocol)/sizeof(oid);
		} else {
			*error=strdup("Unknown snmpv3 auth protocol.");
			return NULL;
		}	
	}
	if (priv==1) {
		/* set the security level */
		session->securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;
		session->securityPrivKeyLen = USM_PRIV_KU_LEN;
		if (strcmp(privproto,"des")==0) {
			session->securityPrivProto = usmDESPrivProtocol;
			session->securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
		} else {
			if (strcmp(privproto,"aes")==0) {
#ifdef usmAESPrivProtocol
                                session->securityPrivProto = usmAESPrivProtocol;
			       session->securityPrivProtoLen = USM_PRIV_PROTO_AES_LEN;
#else
                              *error=strdup(_("AES protocol not supported on this system."));
                                return NULL;
#endif
			} else {
				*error=strdup(_("Unknown snmpv3 auth protocol."));
				return NULL;
			}	
		}
		
		if (generate_Ku(session->securityAuthProto,
					session->securityAuthProtoLen,
					(u_char *) privpass, strlen(privpass),
					session->securityPrivKey,
					&session->securityPrivKeyLen) != SNMPERR_SUCCESS) {
			
			*error=strdup(_("Error generating a key (Ku) from the privacy pass phrase."));
			free(host_port);
			return NULL;
		}
	} else {
		/* set the security level */
		session->securityLevel = SNMP_SEC_LEVEL_AUTHNOPRIV;
	}
	session->securityAuthKeyLen = USM_AUTH_KU_LEN;
	if (generate_Ku(session->securityAuthProto,
				session->securityAuthProtoLen,
				(u_char *) passwd, strlen(passwd),
				session->securityAuthKey,
				&session->securityAuthKeyLen) != SNMPERR_SUCCESS) {
		*error=strdup(_("Error generating key (Ku) from authentication pass phrase."));
		free(host_port);
		return NULL;
	}
	
	/*Open the session */
	SOCK_STARTUP;                                                           /* for Windows */
	ss = snmp_open(session);                     /* establish the session */
	/* Get error in case something went wrong */
	if (ss==NULL) snmp_error(session, &session->s_errno, &session->s_snmp_errno, error);
	free(host_port);
	return ss;
}


int process_snmp_var( /* return code : 0:OK, 1 : Type not supported, 2: No such object / endof mib,  3 : mem problems */
netsnmp_variable_list *vars_ret, /* response variable */
snmp_table* oid_list) /* snmp tableto fill */
{
	int i;
	/* for (i=0;i<oid_list->name_length;i++) printf(".%lu",oid_list->name[i]); */
	/* printf(" : %lu\n", oid_list->value_l); */	
	switch (vars_ret->type) {
	case ASN_OCTET_STR:
		if ((oid_list->value=(char *)malloc(vars_ret->val_len+1))==NULL) return 3;		
		memcpy(oid_list->value, vars_ret->val.string, vars_ret->val_len);
		oid_list->value[vars_ret->val_len] = '\0';  
		break;
	case ASN_INTEGER:
		oid_list->value_l=*vars_ret->val.integer;
		/* printf("Integer : %lu\n", oid_list->value_l); */
		break;
	case ASN_COUNTER64:
		oid_list->value_ull = ((unsigned long long) vars_ret->val.counter64->high) << 32;
		oid_list->value_ull += vars_ret->val.counter64->low;
		/* printf("counter64 : %llu\n", oid_list->value_ull); */
		break;
	case ASN_BIT_STR:
		if ((oid_list->value=(char *)malloc(vars_ret->val_len+1))==NULL) return 3;		
		memcpy(oid_list->value, vars_ret->val.bitstring, vars_ret->val_len);
		oid_list->value[vars_ret->val_len] = '\0';  /* useless for bitstrings, but lost the size  */		
		break;
	case ASN_OBJECT_ID:
		copy_oid(oid_list->value_oid,&oid_list->value_oid_length,vars_ret->val.objid,vars_ret->val_len / sizeof(oid));
		break;
	case ASN_TIMETICKS:
	case ASN_GAUGE:
	case ASN_COUNTER:
	case ASN_UINTEGER:
		oid_list->value_ull = (unsigned long) *vars_ret->val.integer;
		/* printf("TIME/GAUGE/COUNTER/UINT : %llu\n", oid_list->value_ull); */
		break;
	case ASN_IPADDRESS:
		asprintf(&oid_list->value,"%d.%d.%d.%d",vars_ret->val.string[0], vars_ret->val.string[1], vars_ret->val.string[2], vars_ret->val.string[3]);
		break;
	case ASN_NULL:
		oid_list->value_l=0;
		oid_list->value_ull=0;
		break;
	case SNMP_NOSUCHOBJECT:
	case SNMP_NOSUCHINSTANCE:
	case SNMP_ENDOFMIBVIEW:
		return 2;
		break;
	default: /* opaque types and other */
		for (i=0;i<vars_ret->name_length;i++) printf(".%lu",vars_ret->name[i]);
		printf(_(" : type not supported : %i\n"),(int) vars_ret->type);
		return 1;
	}
	copy_oid(oid_list->name,&oid_list->name_length,vars_ret->name,vars_ret->name_length);
	return 0;
}


snmp_table * get_snmp_oids (   
struct snmp_session * session,  
oid table_oid[MAX_NUM_OID][MAX_OID_LEN], size_t table_oid_len[MAX_NUM_OID],
int num_oid,char** error)   
{
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	int status,i;
	int j,num_fragment;
	snmp_table* oid_list, *oid_list_cur;
	netsnmp_variable_list *vars_ret;
	int processed;

	oid_list_cur=oid_list=NULL;
	
	num_fragment=(int) num_oid/MAX_NUM_OID_PDU;
	if (o_verb==1 && num_fragment!=0) printf ("Fragmentation of OID requests in %i pdu\n",num_fragment);
	processed=0;
	for (j=0;j<=num_fragment;j++) {
		pdu = snmp_pdu_create(SNMP_MSG_GET); /*  create a get pdu (v1,v2c,v3)	} */
	    /* Treating oid  ,j*MAX_NUM_OID_PDU    to oid     min(num_oid,MAX_NUM_OID_PDU*(j+1)) */
		for (i=j*MAX_NUM_OID_PDU;i<min(num_oid,MAX_NUM_OID_PDU*(j+1));i++) {  /*  fill with oid */
			snmp_add_null_var(pdu, table_oid[i], table_oid_len[i]); 
		}

		status=snmp_synch_response(session, pdu, &response); /*  Send request */
		if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) {
			for(vars_ret = response->variables; vars_ret; vars_ret = vars_ret->next_variable) {
				/*print_variable(vars_ret->name, vars_ret->name_length, vars_ret); */
				if (processed==1) {
					if ((oid_list_cur->next=malloc(sizeof(snmp_table)))==NULL) return NULL;
					oid_list_cur=oid_list_cur->next;
					processed=0;
				} else if (oid_list_cur == NULL)  {
					if ((oid_list_cur=oid_list=malloc(sizeof(snmp_table)))==NULL) return NULL; 
				}
				oid_list_cur->next=NULL;
				if ((i=process_snmp_var(vars_ret,oid_list_cur)) == 0 ) 
					processed=1;
				else if (i==3) return NULL;
				else if (i==2) break;
			}
		} else {  /* Error getting response */          
			if (status == STAT_SUCCESS) 
			asprintf(error, "(packet) : %s",snmp_errstring(response->errstat));
			else 
			snmp_error(session, &session->s_errno, &session->s_snmp_errno, error);
			return NULL;
		}
		if (response) snmp_free_pdu(response); /*  free the answer */
	}
	return oid_list;

}

snmp_table * get_snmp_table (
struct snmp_session * session,
oid * table_oid, size_t table_oid_len,
int bulk_read,char** error)
{
	struct snmp_pdu *pdu;
	struct snmp_pdu *response;
	int status,i,table_parsed,var_parsed,processed;
	snmp_table* oid_list, *oid_list_cur;
	netsnmp_variable_list *vars_ret;
	oid current_oid[MAX_OID_LEN];
	size_t current_oid_len;

	oid_list_cur=oid_list=NULL;
	copy_oid(current_oid,&current_oid_len,table_oid,table_oid_len);

	table_parsed=1;
	processed=0;
	while (table_parsed) { /* parse the table */		
		if (bulk_read==1) {
			pdu = snmp_pdu_create(SNMP_MSG_GETBULK); /*  create a getbulk pdu (v2c & v3) */
			pdu->errindex=25; /*  max-repetition */
			pdu->errstat=0; /*  non repeaters */
		} else {
			pdu = snmp_pdu_create(SNMP_MSG_GETNEXT); /*  create a getnext pdu (v1,v2c,v3) */
		}
		snmp_add_null_var(pdu, current_oid, current_oid_len); /*  fill with oid */
		status = snmp_synch_response(session, pdu, &response); /*  Send request */

		/*Process the response. */
		if (status == STAT_SUCCESS && response->errstat == SNMP_ERR_NOERROR) { /*  SUCESS !!! */
			vars_ret=response->variables;
			var_parsed=1;
			/* process all the responses */
			while (var_parsed) {
				/* Check if still in tree OID */
				if (netsnmp_oid_is_subtree(table_oid, table_oid_len,vars_ret->name,vars_ret->name_length)!=0) 
				{ table_parsed=0; break;}	
				/*print_variable(vars_ret->name, vars_ret->name_length, vars_ret); */
				if (processed==1) {
					if ((oid_list_cur->next=malloc(sizeof(snmp_table)))==NULL) return NULL;
					oid_list_cur=oid_list_cur->next;
					processed=0;
				} else if (oid_list_cur == NULL)  {
					if ((oid_list_cur=oid_list=malloc(sizeof(snmp_table)))==NULL) return NULL; 
				}
				oid_list_cur->next=NULL;
				if ((i=process_snmp_var(vars_ret,oid_list_cur)) == 0 ) 
					processed=1;
				else if (i==3) return NULL;
				else if (i==2) break;
				if (vars_ret->next_variable != NULL ) vars_ret=vars_ret->next_variable; else var_parsed=0;
			}
			if ((vars_ret->type != SNMP_ENDOFMIBVIEW) && (vars_ret->type != SNMP_NOSUCHOBJECT) 
					&& (vars_ret->type != SNMP_NOSUCHINSTANCE)) {
				copy_oid(current_oid,&current_oid_len, vars_ret->name,vars_ret->name_length);
			}
			else table_parsed = 0;	
		} else {  /* Error getting response */		
			if (status == STAT_SUCCESS) 
			asprintf(error, "(packet) : %s",snmp_errstring(response->errstat));
			else
			snmp_error(session, &session->s_errno, &session->s_snmp_errno, error);
			return NULL;
		}
		if (response) snmp_free_pdu(response); /*  free the answer */
	}
	return oid_list;
}


int convert_to_float(char* in_string,float *val) {

	char tmp[1];
	if (in_string==NULL) return 1;
	if (sscanf (in_string, "%f%c", val, tmp) == 1)
	return 0;
	else
	return 1;
}

int float_array(char* in_string,float* out_array, int max_array) {
	int num_array=0;
	char *saveptr;
	char *str1;

	if (in_string==NULL) return -1;
	/*  separators are : "," and "%" to get rid of % in values. */
	str1=strtok_r (in_string, ",%", &saveptr);
	while ((str1!=NULL)&&(num_array<max_array)) {
		if (sscanf (str1, "%f", &out_array[num_array]) == 1) {
			num_array++;
		} else {
			return -2;
		}
		str1=strtok_r (NULL, ",%", &saveptr);
	}
	return num_array;
}

int int_array(char* in_string,int* out_array, int max_array) {
	int num_array=0;
	char *saveptr;
	char *str1;

	if (in_string==NULL) return -1;
	/*  separators are : "," and "%" to get rid of % in values. */
	str1=strtok_r (in_string, ",%", &saveptr);
	if (str1==NULL) {
		if (sscanf (in_string, "%i", &out_array[num_array]) == 1) {
			num_array++;
		} else {
			return -2;
		}
	} else {
		while ((str1!=NULL)&&(num_array<max_array)) {
			if (sscanf (str1, "%i", &out_array[num_array]) == 1) {
				num_array++;
			} else {
				return -2;
			}
			str1=strtok_r (NULL, ",%", &saveptr);
		}
	}
	return num_array;
}

int string_array(char* in_string,char** out_array,int max_array) {
	int num_array=0;
	char *saveptr;
	char *str1;

	if (in_string==NULL) return -1;
	/*  separator is  "," . */
	str1=strtok_r (in_string, ",", &saveptr);
	while ((str1!=NULL)&&(num_array<max_array)) {
		out_array[num_array++]=strdup(str1);
		str1=strtok_r (NULL, ",", &saveptr);
	}
	return num_array;
}

int get_options (int narg, char* const valarg[],options_nagsnmp *liste, int n_liste)
{
	int i,c;
	static struct option long_options[50];
	int long_options_n=0;
	char short_options[200];
	int short_options_n=0;
	int option_index = 0;
	int option_found=-1;
	
	/*  remove error output in case of unknown option */
	opterr=0;

	for (c=0;c<n_liste;c++) {
		if (liste[c].o_short!=0) { 
			short_options[short_options_n++]=liste[c].o_short;
			if (liste[c].type != 0) short_options[short_options_n++]=':';
			if (liste[c].type >= 8) short_options[short_options_n++]=':';
		}	
		if (liste[c].o_long!=NULL) {
			long_options[long_options_n].name=strdup(liste[c].o_long);
			long_options[long_options_n].has_arg= 
			(liste[c].type == 0) ? 0 : ((liste[c].type & 8)==8) ? 2 : 1;
			long_options[long_options_n].flag=0;
			long_options[long_options_n++].val= (liste[c].o_short!=0) ? liste[c].o_short : 0;
		}
	}
	long_options[long_options_n].name=0;
	long_options[long_options_n].has_arg=0;
	long_options[long_options_n].flag=0;
	long_options[long_options_n++].val=0;
	short_options[short_options_n]=0;
	/* printf("Options : #%s#\n",short_options); */
	while (1)
	{
		option_found=-1;
		c = getopt_long (narg, valarg, short_options,
		long_options, &option_index);
		/* printf("getopt_long returned : %i / %c \n",c,c); */
		if (c == EOF) break;
		if (c==0) {
			/* printf ("Long option %s", long_options[option_index].name); */
			/* if (optarg) printf (" avec argument %s", optarg);  */
			/* printf ("\n");	 */
			for (i=0;(i<n_liste) && (option_found==-1);i++) {
				if (!strcmp(long_options[option_index].name,liste[i].o_long)) {
					option_found=i;
				}
			}
		} else {
			for (i=0;(i<n_liste) && (option_found==-1);i++) {
				if (c==liste[i].o_short) {
					option_found=i;
					/* printf ("Option %c",liste[i].o_short); */
					/* if (optarg != NULL) printf (" val: %s",optarg); */
					/* printf("\n");				 */
				}
			}
		}
		if (option_found==-1) {
			printf(_("Unkown option or missing argument (code 0%o)\n"),c);return 1;
		} else {
			if (liste[option_found].type==0) { /*  no options */
				liste[option_found].present=1;
				if (liste[option_found].intvalue != NULL) { /* fill with 1 if present */
					*liste[option_found].intvalue=1;
				}
			} else if ((optarg==NULL) && ((liste[option_found].type & 8)==8)) {
				liste[option_found].present=3;
				liste[option_found].n_array=0;
			} else {
				if ((liste[option_found].type & 1)==1) { /*  number */
					if (liste[option_found].ivalue==NULL) { /* Integer */
						if ((liste[option_found].type & 4)==4) {
							if ((liste[option_found].n_array=int_array(optarg,liste[option_found].intvalue,liste[option_found].n_array))>0)
							liste[option_found].present=1;
							else 
							liste[option_found].present=0;	
						} else {
							if ((optarg!=NULL) && (sscanf (optarg, "%i", liste[option_found].intvalue) == 1)) 
							liste[option_found].present=1;
							else
							liste[option_found].present=0;	
						}
					} else {
						if ((liste[option_found].type & 4)==4) {
							if ((liste[option_found].n_array=float_array(optarg,liste[option_found].ivalue,liste[option_found].n_array))>0)
							liste[option_found].present=1;
							else 
							liste[option_found].present=0;	
						} else {
							if ((optarg!=NULL) && (sscanf (optarg, "%f", liste[option_found].ivalue) == 1)) 
							liste[option_found].present=1;
							else
							liste[option_found].present=0;	
						}
					}
				} else if ((liste[option_found].type & 2)==2) { /*  string */
					if ((liste[option_found].type & 4)==4) { /*  array */
						if ((liste[option_found].n_array=string_array(optarg,liste[option_found].cvalue,liste[option_found].n_array)) > 0) 
						liste[option_found].present=1;
						else
						liste[option_found].present=0;	
					} else {
						if (optarg !=NULL) {
							if ((*liste[option_found].cvalue=strdup(optarg))==NULL) return 1;
							liste[option_found].present=1;
						} else {
							liste[option_found].present=0;	
						}
					}
				}
			}		
		}
	}

	if (optind < narg)
	{
		printf (_("Too much arguments (only options are needed) : \n"));
		while (optind < narg)
		printf ("%s ", valarg[optind++]);
		printf ("\n");
		return 1;
	}
	return 0;	
}

/**************************** Utilities ***********************************/

void convert_text(char *string) { /* convert special chars in strings to "_" (OK : 0-9A-Za-z_.-, )*/
	int i=0;
	if (string==NULL) return;
	while (string[i]!=0) {
		/*0-9 : 48-57
		A-Z : 65-90
		a-z : 97-122
		_.-, : 95,46,45,44*/
		if (string[i]<44) { string[i++]=95; continue;}
		if (string[i]==47) { string[i++]=95; continue;}
		if (string[i]<48) { string[i++]=95; continue;}
		
		if (string[i]>122) { string[i++]=95; continue;}
		if (string[i]>=97) {i++;continue;}
		if (string[i]>90) { string[i++]=95; continue;}
		if (string[i]>=65) { i++;continue;}
		if (string[i]>=57) { string[i++]=95;continue;}
		i++;
	}
}

void subst_chars(char* string, const char* remove, char replace) {
	char*pos;
	
	pos=string;
	while((pos=strpbrk(pos,remove))!=NULL) {
		*pos++=replace;
	}
}
void remove_chars(char* string, const char* remove) {
	char*pos,*pos2;
	
	pos=pos2=string;
	while((pos2=strpbrk(pos,remove))!=NULL) {
		while (pos!=pos2) *string++=*pos++;
		pos2++;pos++;
		if (*pos ==0) break;
	}
	while (*pos!=0) *string++=*pos++;
	*string=0;
}

int write_file( char* file_name, int rows, int max_rows, int items, unsigned long long int values[MAX_ROWS][MAX_ITEMS]) {
	FILE* FICHIER;
	int i,j,start;
	
	if ((FICHIER=fopen(file_name,"w")) == NULL) return 1;
	if (rows > max_rows ) start = rows - max_rows ; 
	else start = 0;
	for (i=0;i<rows;i++) {
		for (j=0;j<items-1;j++) fprintf(FICHIER,"%llu:",values[i][j]);		
		fprintf(FICHIER,"%llu\n",values[i][items-1]);
	}
	fclose(FICHIER);
	return 0;
}

/************************************************************
			temporary file  utilities 
*************************************************************/

int read_file( char* file_name, int *rows, int items, unsigned long long int values[MAX_ROWS][MAX_ITEMS]) {
	FILE* FICHIER;
	char buffer[500],buffer2[100];
	char *pos,*pos2,*endptr;
	int i;
	
	if ((FICHIER=fopen(file_name,"r")) == NULL) return 1;
	*rows=0;
	while (fscanf(FICHIER,"%s\n",buffer)!=EOF) {
		pos=buffer;
		for (i=0;i<items;i++) {
		  pos2=pos;
		  while (*pos2!=':' && *pos2 != 0) pos2++;
		  if ((*pos2==0)&&(i!=(items-1))) return 1;
		  memcpy(buffer2,pos,pos2-pos);
		  buffer2[pos2-pos]=0;
		  values[*rows][i]=strtoll(buffer2,&endptr,10); /* TODO test ==LLONG_MIN) return 1; */
		  pos=pos2+1;		  
		}
		(*rows)++;
	}
	fclose(FICHIER);
	return 0;
}

/************************************************************
			Cache  utilities 
*************************************************************/
/* read cache file 
INPUT : 
desc
num : number of data columns
ttl : time to live of values in seconds
OUTPUT :
0 : read OK, 1 : no file or TTL expired, 2: timeout, 3: memory problems
num : number of values
descr : description table
index : index table
*/
int read_cache(snmp_table **descT,int num, long unsigned int ttl) {
	FILE * FICHIER;
	char file_name[MAX_FILE_LENGTH]=BASE_CACHE;
	char lock_file_name[MAX_FILE_LENGTH];
	char buffer[500],buffer2[100];
	char *pos2,*pos,*endptr;
	long unsigned timeout,timenow;
	int rows,i;
	snmp_table **tempT;
	
	strcat(file_name,"/");
	strncat(file_name,o_host,50);
	strcat(file_name,".");
	strcat(file_name,plugin_name);
	strcpy(lock_file_name,file_name);
	strcat(lock_file_name,".lock");
	if (!access(lock_file_name, F_OK)) { /* file exists */
		sleep(1);
		timeout=0;
		while (!access(lock_file_name, F_OK) && timeout++ < 2) sleep(1);
		if (timeout == 3) return 2;
	}
	timenow=time(NULL);
	if ((FICHIER=fopen(file_name,"r")) == NULL) return 1;
	if (fscanf(FICHIER,"%lu\n",&timeout)==EOF) {fclose(FICHIER);return 1;}
	if (timenow-timeout>ttl) {fclose(FICHIER);return 1;}
	tempT=malloc(sizeof(snmp_table *)*num);
	for (i=0;i<num;i++) {
		if ((tempT[i]=malloc(sizeof(snmp_table)))==NULL) {fclose(FICHIER);return 3;}
		descT[i]=tempT[i];
	}
	rows=0;
	while (fgets(buffer,500,FICHIER)!=NULL) {
		if (rows!=0) {
			for (i=0;i<num;i++) {
				if ((tempT[i]->next=malloc(sizeof(snmp_table)))==NULL) {fclose(FICHIER);return 3;}
				tempT[i]=tempT[i]->next;
			}
		}
		pos2=buffer;
		/* read index */
		while ( *pos2!=':' && *pos2!=0 ) pos2++;
		if (*pos2==0) return 1; /* wrong data file -> rewrite */
		memcpy(buffer2,buffer,pos2-buffer);
		buffer2[pos2-buffer]=0;
		tempT[0]->name[0]=strtol(buffer2,&endptr,10); /* TODO test ==LONG_MIN) return 1; */
		tempT[0]->name_length=1;
		for (i=1;i<num;i++) {tempT[i]->name[0]=tempT[0]->name[0];tempT[i]->name_length=1;}
		pos2++;
		pos=pos2;
		/* read descriptions */
		for (i=0;i<num-1;i++) {
			while ( *pos2!=':' && *pos2!=0 ) pos2++;
			if (*pos2==0) return 1; /* wrong data file -> rewrite */
			if ((tempT[i]->value=malloc(sizeof(char)*strlen(buffer)))==NULL) {fclose(FICHIER);return 3;}
			memcpy(tempT[i]->value,pos,pos2-pos);
			tempT[i]->value[pos2-pos]=0;			
			pos2++;
			pos=pos2;
		}		
		if ((tempT[num-1]->value=malloc(sizeof(char)*strlen(buffer)))==NULL) {fclose(FICHIER);return 3;}
		memcpy(tempT[num-1]->value,pos2,strlen(buffer)-(pos2-buffer));
		tempT[num-1]->value[strlen(buffer)-(pos2-buffer)-1]=0;
		rows++;
	}
	for (i=0;i<num;i++) tempT[i]->next=NULL;
	fclose(FICHIER);
	return 0;	
}
/* write cache file 
INPUT : 
num : numbers of index
ttl : time to live of values in seconds
OUTPUT :
0 : write OK, 1: write problems 2: timeout  for lock , 3 remove lock file impossible
num : number of values
descr : description table
index : index table
*/
int write_cache(snmp_table **descT,int num) {
	FILE * FICHIER;
	char file_name[MAX_FILE_LENGTH]=BASE_CACHE;
	char lock_file_name[MAX_FILE_LENGTH];	
	unsigned long int timenow;
	int i;

	strcat(file_name,"/");
	strncat(file_name,o_host,50);
	strcat(file_name,".");
	strcat(file_name,plugin_name);
	strcpy(lock_file_name,file_name);
	strcat(lock_file_name,".lock");
	if (!access(lock_file_name, F_OK)) { /* file exists -> someone is writting -> exit */
		return 2;
	}
	if ((FICHIER=fopen(lock_file_name,"w")) == NULL) return 1;
	fclose(FICHIER);
	sleep(1); /* allow others to finish reading */
	
	if ((FICHIER=fopen(file_name,"w")) == NULL) {remove (lock_file_name);return 1;}
	timenow=time(NULL);
	fprintf(FICHIER,"%lu\n",timenow);
	while (descT[0] !=NULL) {
		fprintf(FICHIER,"%i",(int) descT[0]->name[descT[0]->name_length-1]);
		for (i=0;i<num;i++) {
			/* TODO : values get by snmp get next, so index should be in the same order, but not sure about this so check just in case */
			fprintf(FICHIER,":%s",descT[i]->value);
			descT[i]=descT[i]->next;
		}
		fprintf(FICHIER,"\n");
	}
	fclose(FICHIER);
	if (remove (lock_file_name)==-1) return 3;
	return 0;

}


/************************************************************
			output utilities 
*************************************************************/

void strcat_output(const char *string) {
	int i=strlen(string);
	
	if (output_overflow == 0) {
		if (i>(MAX_OUTPUT-strlen(output)-20)) {
			output_overflow=1;
			strcat(output,"...[LIMIT OUTPUT]");
		} else {
			strcat(output,string);
		}
	}
}

void strcat_output_notnull(const char *string) {
	if ((output_overflow==0) && (strlen(output)!=0)) {
		strcat(output,string);
	}
}

void print_output() {
	printf("%s",output);
	if (p_output != NULL) printf(" |%s",p_output);
	printf("\n");
}

int print_output_with_status(int status) {
	switch (status) {
		case N_OK: printf("OK : ");break;
		case N_WARNING: printf("WARNING : ");break;
		case N_CRITICAL: printf("CRITICAL : ");break;
		default: printf("UNKOWN : ");break;
	}
	print_output();
	return status;
}

/* put lont int values in performance data 
'label'=value[UOM];[warn];[crit];[min];[max]
*/
/******************************************************************************
 *
 * Print perfdata in a standard format (long int as value)
 *
 ******************************************************************************/

void p_output_int(char *label,
 long int val, const char *uom, const char*extension,
 int warnp, long int warn,
 int critp, long int crit,
 int minp, long int minv,
 int maxp, long int maxv)
{
	char *data = NULL;
	
	if (strlen(label)>MAX_LABEL_SIZE) label[MAX_LABEL_SIZE]=0;
	
	if (strpbrk (label, "'= "))
		asprintf (&data, "'%s%s'=%ld%s;", label, extension, val, uom);
	else
		asprintf (&data, "%s%s=%ld%s;", label, extension, val, uom);

	if (warnp)
		asprintf (&data, "%s%ld;", data, warn);
	else
		asprintf (&data, "%s;", data);

	if (critp)
		asprintf (&data, "%s%ld;", data, crit);
	else
		asprintf (&data, "%s;", data);

	if (minp)
		asprintf (&data, "%s%ld", data, minv);

	if (maxp)
		asprintf (&data, "%s;%ld", data, maxv);

	if (p_output==NULL) asprintf (&p_output," %s",data);
	else asprintf (&p_output,"%s %s",p_output,data);
	free (data);
}

/******************************************************************************
 *
 * Print perfdata in a standard format (double as value)
 *
 ******************************************************************************/

void p_output_float(char *label,
 double val, const char *precision,
 const char * extension,char *uom,
 int warnp, double warn,
 int critp, double crit,
 int minp, double minv,
 int maxp, double maxv)
{
	char *data = NULL;
	char *buffer=NULL;
	
	if (strlen(label)>MAX_LABEL_SIZE) label[MAX_LABEL_SIZE]=0;
	
	asprintf(&buffer,"'%%s%%s'=%s%%s;",precision);
	asprintf (&data, buffer, label,extension, val, uom);

	asprintf(&buffer,"%%s%s;",precision);

	if (warnp) 
		asprintf (&data, buffer, data, warn);
	else
		asprintf (&data, "%s;", data);
		
	if (critp)
		asprintf (&data, buffer, data, crit);
	else
		asprintf (&data, "%s;", data);
		
	if (minp)
		asprintf (&data, buffer, data, minv);
	else
		asprintf (&data, "%s;", data);
		
	if (maxp) {
		asprintf(&buffer,"%%s%s",precision);
		asprintf (&data, buffer, data, maxv);
	}
	
	if (p_output==NULL) asprintf (&p_output," %s",data);
	else asprintf (&p_output,"%s %s",p_output,data);
	free (data);
	free (buffer);
}

/******************************************************************************
 *
 * Print perfdata in a standard format (unsigned long long int as value)
 *
 ******************************************************************************/

void p_output_ull(char *label,
 unsigned long long int val, const char *uom,const char*extension,
 int warnp, unsigned long long int warn,
 int critp, unsigned long long int crit,
 int minp, unsigned long long int minv,
 int maxp, unsigned long long int maxv)
{
	char *data = NULL;
	
	if (strlen(label)>MAX_LABEL_SIZE) label[MAX_LABEL_SIZE]=0;

	asprintf (&data, "'%s%s'=%llu%s;", label, extension, val, uom);

	if (warnp)
		asprintf (&data, "%s%llu;", data, warn);
	else
		asprintf (&data, "%s;", data);

	if (critp)
		asprintf (&data, "%s%llu;", data, crit);
	else
		asprintf (&data, "%s;", data);

	if (minp)
		asprintf (&data, "%s%llu", data, minv);

	if (maxp)
		asprintf (&data, "%s;%llu", data, maxv);

	if (p_output==NULL) asprintf (&p_output," %s",data);
	else asprintf (&p_output,"%s %s",p_output,data);
	free (data);
}

int max_state (int a, int b)
{
	if (a == N_CRITICAL || b == N_CRITICAL)
		return N_CRITICAL;
	else if (a == N_WARNING || b == N_WARNING)
		return N_WARNING;
	else if (a == N_UNKNOWN || b == N_UNKNOWN)
		return N_UNKNOWN;
	else if (a == N_OK || b == N_OK)
		return N_OK;
	else
		return max (a, b);
}

