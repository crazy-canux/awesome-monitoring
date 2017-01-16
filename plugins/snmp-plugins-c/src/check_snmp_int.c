#include "nagios_common_snmp.h"

#define SCRIPT_VERSION 0.5
#define TMP_FILE_NAME_PREFIX "tmp_Nagios_int."
/* default delta for averages : 300s = 5min */
#define DEFAULT_DELTA_AVERAGE 300
/* default TTL for cache 15 minutes */
#define DEFAULT_CACHE_TTL 15
/********************************
 help & usage  
 ********************************/
char usage[]="-n <name in desc_oid> [-i -a -r  -D] [-f[eSyY]] [-k[qBMGu] -g -w<warn levels> -c<crit levels> -d<delta>] [-s --label] -K=<min>";


/********************************
 SNMP data  
 ********************************/
oid inter_table[]= {1,3,6,1,2,1,2,2,1};
oid index_table[] = {1,3,6,1,2,1,2,2,1,1};
oid descr_table[] = {1,3,6,1,2,1,2,2,1,2};
oid oper_table[] = {1,3,6,1,2,1,2,2,1,8};
oid admin_table[] = {1,3,6,1,2,1,2,2,1,7};
oid speed_table[] = {1,3,6,1,2,1,2,2,1,5};
oid speed_table_64[] = {1,3,6,1,2,1,31,1,1,1,15};
oid in_octet_table[] = {1,3,6,1,2,1,2,2,1,10};
oid in_octet_table_64[] = {1,3,6,1,2,1,31,1,1,1,6};
oid in_error_table[] = {1,3,6,1,2,1,2,2,1,14};
oid in_discard_table[] = {1,3,6,1,2,1,2,2,1,13};
oid out_octet_table[] = {1,3,6,1,2,1,2,2,1,16};
oid out_octet_table_64[] = {1,3,6,1,2,1,31,1,1,1,10};
oid out_error_table[] = {1,3,6,1,2,1,2,2,1,20};
oid out_discard_table[] = {1,3,6,1,2,1,2,2,1,19};

char status[8][15]={ "","UP","DOWN","TESTING","UNKNOWN","DORMANT","NotPresent","lowerLayerDown"};

void print_usage() {
	printf("Usage: %s %s %s\n",plugin_name, common_usage, usage);
	exit(N_UNKNOWN);
}
void print_version() {
	printf("%s version %f / Package version %f\n",plugin_name,SCRIPT_VERSION,NAGIOS_SNMP_VERSION);
	exit(N_UNKNOWN);
}

void print_help(){
	printf(_("\nSNMP Network Interface Monitor for Nagios version %.2f\n"),SCRIPT_VERSION);
	printf(_("Nagios SNMP plugins version %.2f, GPL licence, (c)2004-2007 Patrick Proy\n\n"),NAGIOS_SNMP_VERSION);
	printf("Usage: %s %s %s\n",plugin_name, common_usage, usage);
	
	DEFAULT_HELP_PRINT
	printf("-n, --name=NAME\n");
	printf(_("   Name in description OID (eth0, ppp0 ...).\n"));
	printf(_("   This is treated as a regexp : -n eth will match eth0,eth1,...\n"));
	printf(_("   Test it before, because there are known bugs (ex : trailling /)\n"));
	printf("-r, --noregexp\n");
	printf(_("   Do not use regexp to match NAME in description OID\n"));
	printf("-i, --inverse\n");
	printf(_("   Make critical when up\n"));
	printf("-a, --admin\n");
	printf(_("   Use administrative status instead of operational\n"));
	printf("-D, --dormant\n");
	printf(_("   Consider \"dormant\" state is an OK state\n"));	
	printf("-K, --cache[=<min>]\n");
	printf(_("   Use cache file to store index\n"));
	printf(_("   Optionnal validity time of index in minutes (default %i)\n"),DEFAULT_CACHE_TTL);
	printf(_("   No space between K and the value as it is optionnal.\n"));
	printf("-f, --perfout\n");
	printf(_("   Performance output (no output when interface is down).\n"));
	printf("-e, --error\n");
	printf(_("   Add error & discard to performance output\n"));
	printf("-S, --intspeed\n");
	printf(_("   Include speed in performance output in bits/s\n"));
	printf("-y, --perfprct ; -Y, --perfspeed\n");
	printf(_("   -y : output performance data in %% of max speed \n"));
	printf(_("   -Y : output performance data in bits/s or Bytes/s (depending on -B)   \n"));
	printf("-k, --perfcheck ; -q, --extperfcheck \n");
	printf(_("   -k check the input/ouput bandwidth of the interface\n"));
	printf(_("   -q also check the error and discard input/output\n"));
	printf("--label\n");
	printf(_("   Add label before speed in output : in=, out=, errors-out=, etc...\n"));
	printf("-g, --64bits\n");
	printf(_("   Use 64 bits counters instead of the standard counters when checking \n"));
	printf(_("   bandwidth & performance data for interface >= 1Gbps.\n"));
	printf(_("   You must use snmp v2c or v3 to get 64 bits counters.\n"));
	printf("-d, --delta=seconds\n");
	printf(_("   make an average of <delta> seconds (default 300=5min)\n"));
	printf("-B, --kbits\n");
	printf(_("   Make the warning and critical levels in K|M|G Bits/s instead of K|M|G Bytes/s\n"));
	printf("-G, --giga ; -M, --mega ; -u, --prct\n");
	printf(_("   -G : Make the warning and critical levels in Gbps (with -B) or GBps\n"));
	printf(_("   -M : Make the warning and critical levels in Mbps (with -B) or MBps\n"));
	printf(_("   -u : Make the warning and critical levels in %% of reported interface speed.\n"));
	printf("-w, --warning=input,output[,error in,error out,discard in,discard out]\n");
	printf(_("   warning level for input / output bandwidth (0 for no warning)\n"));
	printf(_("     unit depends on B,M,G,u options\n"));
	printf(_("   warning for error & discard input / output in error/min (need -q)\n"));
	printf("-c, --critical=input,output[,error in,error out,discard in,discard out]\n");
	printf(_("   critical level for input / output bandwidth (0 for no critical)\n"));
	printf(_("     unit depends on B,M,G,u options\n"));
	printf(_("   critical for error & discard input / output in error/min (need -q)\n"));
	printf("-s, --short=int\n");
	printf(_("   Make the output shorter : only the first <n> chars of the interface(s)\n"));
	printf(_("   If the number is negative, then get the <n> LAST caracters.\n"));
	printf(_("Note : when multiple interface are selected with regexp, \n"));
	printf(_("       all be must be up (or down with -i) to get an OK result.\n"));
	exit(N_UNKNOWN);
}

struct interface_struct {
	int index;
	int state;
	unsigned long long in_oct;
	unsigned long long out_oct;
	unsigned long in_err;
	unsigned long out_err;
	unsigned long in_disc;
	unsigned long out_disc;
	unsigned long speed;
	unsigned long high_speed;
};

/********************************
 Main of check_snmp_int
 ********************************/
	
int main(int argc, char ** argv)
{
	/* limits */
	int n_maxOID=500;
	/* OID array */
	oid valOID[n_maxOID][MAX_OID_LEN];
	size_t valOID_len[n_maxOID];
	int n_valOID;
	int index_match;
	snmp_table *interface_index[n_maxOID];
	
	/* Interfaces status & count */
	int num_int,test_int;
	int ok_val, num_ok;
	unsigned long long long_int;
	struct interface_struct *int_state;
	char * short_desc;
	int final_status,num_checkperf;
	
	/* counters, output, etc... */
	int i,j;
	unsigned long long interface_speed;
	unsigned long speed_metric;
	char speed_unit[10];
	char buffer[100];	
	unsigned int warn_factor;
	
	/* file datas & name */
	unsigned long long int file_values[MAX_ROWS][MAX_ITEMS]; /* to get & store data in file */
	int rows=0;int items=0;int file_ret,found_data;
	char base_file_name[MAX_FILE_LENGTH]=BASE_DIR;	
	char file_name[MAX_FILE_LENGTH];	
	long unsigned int timenow,trigger,trigger_low;
	
	/* performance data */
	float checkperf_out[7],checkperf_out_raw[7];
	unsigned long long overfl;
	
	/* result tables */
	snmp_table *descT,*tempT,*valueT;
	regex_t regexp_filter;
	
	/* cache management */
	int cache_read_result=1;
	int cache_failure=0; /*1: write problems 2: timeout  for lock , 3 remove lock file impossible*/
	snmp_table **cacheT;

	/* script specific options */
	char *o_descr;		/* interface description */
	int o_admin=0;		/*  admin status instead of oper */
	int o_inverse=0;	/*  Critical when up */
	int o_noreg=0;		/*  Do not use Regexp for name */
	int o_label=0;		/*  add label before speed (in, out, etc...). */
	/*  Performance data options  */
	int o_perf=0;  		/*  Output performance data */
	int o_perfe=0;		/*  Output discard/error also in perf data */
	int o_perfs=0; 		/*  include speed in performance output (-S) */
	int o_perfp=0; 		/*  output performance data in % of max speed (-y) */
	int o_perfr=0; 		/*  output performance data in bits/s or Bytes/s (-Y) */
	/*  Speed/error checks */
	int o_checkperf=0;	/*  checks in/out/err/disc values */
	int o_delta=DEFAULT_DELTA_AVERAGE;	/*  delta of time of perfcheck (default 5min) */
	int o_ext_checkperf=0;  /*  extended perf checks (+error+discard)  */
	int o_kbits=0;		/*  Warn and critical in Kbits instead of KBytes */
	int o_highperf=0;	/*  Use 64 bits counters */
	int o_meg=0; 		/*  output in MBytes or Mbits (-M) */
	int o_gig=0; 		/*  output in GBytes or Gbits (-G) */
	int o_prct=0; 		/*  output in % of max speed  (-u) */

	/*Readable names for counters (M. Berger contrib) */
	char countername[6][15] = { "in=" , "out=" , "errors-in=" , "errors-out=" , "discard-in=" , "discard-out=" };

	float o_warn[6]; 	/* warning levels of perfcheck */
	float o_crit[6]; 	/* critical levels of perfcheck */
	int o_short=0;		/* set maximum of n chars to be displayed */
	int o_cache=0;		/* Use caching of index */
	int o_dormant=0;	/* Dormant state is OK */

	/***************** 
	options init
	********************/ 
	options_nagsnmp liste[] = { 
		COMMON_OPTIONS
		/*********** Specific for the script ***************/
		/*  { type	,short	,	o_long		,	present,ivalue, 		cvalue,		int_value		n_array} */
		{2,	'n',	"name",			0,	NULL,		&o_descr,	NULL,			0}, /* 12 */
		{0,	'i',	"inverse",		0,	NULL,		NULL,		&o_inverse,		0}, /* 13*/
		{0,	'a',	"admin",		0,	NULL,		NULL,		&o_admin,		0}, /* 14 */
		{0,	'r',	"noregexp",		0,	NULL,		NULL,		&o_noreg,		0}, /* 15*/
		{0,	0,		"label",		0,	NULL,		NULL,		&o_label,		0}, /* 16 */
		{0,	'f',	"perfout",		0,	NULL,		NULL,		&o_perf,		0}, /*  17*/
		{0,	'e',	"error",		0,	NULL,		NULL,		&o_perfe,		0}, /*  18*/
		{0,	'k',	"perfcheck",	0,	NULL,		NULL,		&o_checkperf,	0}, /*  19*/
		{0,	'q',	"extperfcheck",	0,	NULL,		NULL,		&o_ext_checkperf,0}, /* 20 */
		{0,	'B',	"kbits",		0,	NULL,		NULL,		&o_kbits,		0}, /* 21 */
		{0,	'g',	"64bits",		0,	NULL,		NULL,		&o_highperf,	0}, /* 22 */
		{0,	'S',	"intspeed",		0,	NULL,		NULL,		&o_perfs,		0}, /* 23 */
		{0,	'y',	"perfprct",		0,	NULL,		NULL,		&o_perfp,		0}, /* 24 */
		{0,	'Y',	"perfspeed",	0,	NULL,		NULL,		&o_perfr,		0}, /* 25 */
		{0,	'M',	"mega",			0,	NULL,		NULL,		&o_meg,			0}, /* 26 */
		{0,	'G',	"giga",			0,	NULL,		NULL,		&o_gig,			0}, /* 27 */	
		{0,	'u',	"prct",			0,	NULL,		NULL,		&o_prct,		0}, /* 28 */
		{1,	'd',	"delta",		0,	NULL,		NULL,		&o_delta,		0}, /* 29 */
		{1,	's',	"short",		0,	NULL,		NULL,		&o_short,		0}, /* 30 */
		{5,	'w',	"warning",		0,	o_warn,		NULL,		NULL,			6}, /* 31 */
		{5,	'c',	"critical",		0,	o_crit,		NULL,		NULL,			6}, /* 32 */
		{9,	'K',	"cache",		0,	NULL,		NULL,		&o_cache,		0}, /* 33  */	
		{0,	'D',	"dormant",		0,	NULL,		NULL,		&o_dormant,		0}  /* 34  */	
	};
	int n_liste=COMOPTNUM+23; 
	
	/* Initialise gettext and other parameters */
	if (init_plugins_gettext()!=0) { printf("Cannot initialize GETTEXT\n"); return N_UNKNOWN;}
	strcpy(plugin_name,"check_snmp_int"); /* Plugin name for cache */
	strcpy(plugin_tmp_prefix,TMP_FILE_NAME_PREFIX);
	
	/***************** 
	options init
	********************/ 

	init_options();
	if (get_options (argc, argv,liste,n_liste)!=0) print_usage();
	i=check_common_options();
	if (i==1) print_usage(); else if (i==2) print_help(); else if (i==3) print_version() ;

	/***************** 
	check options 
	*****************/
	/* specific options tests */
	if (liste[12].present==0) { printf(_("Put interface name\n")); print_usage();}

	/* Check snmpv2c or v3 with 64 bit counters */
	if ( liste[22].present==1 && o_snmpver==SNMP_V1)
	{ printf(_("Can't get 64 bit counters with snmp version 1\n")); print_usage(); }

	/*  check if -e without -f */
	if ( liste[18].present==1 && liste[17].present==0 )
	{ printf(_("Cannot output error without -f option!\n")); print_usage(); }		
	if ( liste[25].present==1 && liste[24].present==1 )  {
		printf(_("-Y and -y options are exclusives\n")); print_usage(); }
	if ((liste[25].present==1 || liste[24].present==1 ) && liste[19].present==0)  {
		printf(_("Cannot put -Y or -y options without perf check option (-k) \n")); print_usage(); }
	if (liste[30].present==1) {
		/* TODO maybe some basic tests ? caracters return empty string */
	}
	if (liste[23].present==1 && liste[19].present==0) {
		printf(_("-S option need -k\n")); print_usage(); 
	}
	if (liste[19].present==1) {
		if (liste[20].present==1 && (liste[31].n_array != 6 || liste[32].n_array != 6  )) {
			printf(_("6 warning / critical levels for extended checks \n")); print_usage(); 
		} 
		if (liste[20].present==0 &&(liste[31].n_array != 2 || liste[32].n_array != 2)){
			printf(_("2 warning /critical levels for bandwidth checks \n")); print_usage(); 
		}
		for (i=0;i<liste[31].n_array;i++) { 
			if ((o_crit[i]!=0)&&(o_warn[i] > o_crit[i])) {
				printf(_("Warning must be < Critical level \n")); print_usage(); 
			}
		}
		if ((liste[26].present==1 && liste[27].present==1 ) || 
				(liste[26].present==1 && liste[28].present==1 )|| 
				(liste[27].present==1 && liste[28].present==1 )) {
			printf(_("-M -G and -u options are exclusives\n")); print_usage(); 
		}
	}	  
	
	if (liste[33].present==3) o_cache=DEFAULT_CACHE_TTL;
	if ((liste[33].present==1)&& o_cache < 2) {
		printf(_("Validity of cache must be >= 2 minutes\n")); print_usage(); 
	}
	
	/**************** open snmp session ******************/	
	if (init_snmp_session()!=0) {
		printf(_("Error opening SNMP session : %s\n"),error);
		free(error);
		return N_UNKNOWN;
	}	
	
	/*****************  Get interface table *****************/
	descT=NULL;
	if (o_cache>0) {
		if ((cacheT=malloc(sizeof(snmp_table *)))==NULL) {
			printf(_("Memory allocation failure in cache file"));
			return N_UNKNOWN;
		}
		if ((cache_read_result=read_cache(cacheT,1, 60*o_cache))==3) {
			printf(_("Memory allocation failure in cache file"));
			return N_UNKNOWN;
		}
		descT=cacheT[0];
		cache_failure=cache_read_result; /* 1 : timeout -> lock file not removed */
	} else cacheT=NULL;
	if (o_verb==1 && o_cache>0) printf("Cache read result : %i\n",cache_read_result);
	if ((o_cache==0) || (cache_read_result!=0)) {	
		descT=get_snmp_table (ss,descr_table, sizeof (descr_table) / sizeof (oid),snmp_bulk,&error);
		if (descT==NULL) {
			printf(_("Error reading table : %s\n"),error);
			free(error);
			return N_UNKNOWN;
		}
	}

	/*****************  Get matching interface and create an array of requested OIDs  *****************/
	tempT=descT;
	/* Init the regexp filter */
	if ((o_noreg==0)&&(regcomp(&regexp_filter,o_descr,REG_NOSUB | REG_EXTENDED)!=0)) {
		printf(_("Error compiling regexp (invalid regexp or memory prb)\n"));
		return N_UNKNOWN; 
	}
	
	n_valOID=num_int=0;	
	while ((tempT != NULL)&&(n_valOID<(n_maxOID-10))) {
		if (o_verb) {printf ("OID : "); for (i=0;i<tempT->name_length;i++) printf(".%lu",tempT->name[i]); printf(" : %s\n", tempT->value);}
		if (o_noreg==0) {
			test_int=regexec(&regexp_filter,tempT->value,0,NULL,0);
		} else {
			test_int=strcmp(o_descr,tempT->value);
		}
		if (test_int==0) {
			interface_index[num_int++]=tempT;
			/* get index (last in OID) */
			index_match=(int) tempT->name[tempT->name_length-1];
			/* staus OID : copy OID prefix & add index */
			if (o_admin==0) {
				copy_oid(valOID[n_valOID],&valOID_len[n_valOID],oper_table,sizeof (oper_table)/sizeof (oid));
			} else {
				copy_oid(valOID[n_valOID],&valOID_len[n_valOID],admin_table,sizeof (admin_table)/sizeof (oid));
			}
			valOID[n_valOID][valOID_len[n_valOID]++]=index_match;
			/* verbose */
			if (o_verb) {printf (_("Query OID %i :"),n_valOID);for (i=0;i<valOID_len[n_valOID];i++) printf(".%lu",valOID[n_valOID][i]);printf("\n");}
			n_valOID++;
			if (o_perf==1 || o_checkperf==1) {
				/* get in_octet OID */
				if (o_highperf==0) {
					copy_oid(valOID[n_valOID],&valOID_len[n_valOID],in_octet_table,sizeof (in_octet_table)/sizeof (oid));
				} else {
					copy_oid(valOID[n_valOID],&valOID_len[n_valOID],in_octet_table_64,sizeof (in_octet_table_64)/sizeof (oid));
				}
				valOID[n_valOID][valOID_len[n_valOID]++]=index_match;
				n_valOID++;
				/* get out_octet OID */
				if (o_highperf==0) {
					copy_oid(valOID[n_valOID],&valOID_len[n_valOID],out_octet_table,sizeof (out_octet_table)/sizeof (oid));
				} else {
					copy_oid(valOID[n_valOID],&valOID_len[n_valOID],out_octet_table_64,sizeof (out_octet_table_64)/sizeof (oid));
				}
				valOID[n_valOID][valOID_len[n_valOID]++]=index_match;
				n_valOID++;
				/* get interface speed OID */
				copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],speed_table,sizeof (speed_table)/sizeof (oid),index_match);
				n_valOID++;
				/* get interface high speed OID */
				copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],speed_table_64,sizeof (speed_table_64)/sizeof (oid),index_match);
				n_valOID++;
				if (o_ext_checkperf==1 || o_perfe==1) {
					/* get in_discard OID */
					copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],in_discard_table,sizeof (in_discard_table)/sizeof (oid),index_match);
					n_valOID++;
					/* get out_discard OID */
					copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],out_discard_table,sizeof (out_discard_table)/sizeof (oid),index_match);
					n_valOID++;
					/* get in_error OID */
					copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],in_error_table,sizeof (in_error_table)/sizeof (oid),index_match);		
					n_valOID++;
					/* get out_error OID */
					copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],out_error_table,sizeof (out_error_table)/sizeof (oid),index_match);		
					n_valOID++;
				}
			}
		}
		tempT=tempT->next;
	}
	if (o_verb) printf(_("Found : %i match : %i OID queries\n"),num_int,n_valOID);
	if (n_valOID>=(n_maxOID-10)) { printf(_("ERROR : Maximum number of OID queries reached : %i\n"),n_maxOID); return N_UNKNOWN;}
	/* No interface found -> error */
	if (num_int==0) { printf(_("ERROR : Unknown interface %s\n"),o_descr); return N_UNKNOWN;}
	
	/******************************** 
	Get operating status & other if needed 
	********************************/
	valueT=get_snmp_oids(ss,valOID,valOID_len,n_valOID,&error);
	if (valueT==NULL) {
		printf(_("Error reading table : %s\n"),error);
		free(error);
		return N_UNKNOWN;
	}
	snmp_close(ss);
	/*******************************
	write to cache file if needed
	*******************************/
	
	if ((o_cache>0) && (cache_read_result!=0) && cache_failure != 2) {
		cacheT[0]=descT;
		cache_read_result = write_cache(cacheT,1);
		cache_failure=cache_read_result;
		if (o_verb) printf("Write cache returned : %i\n",cache_read_result);
	}
	
	if ((int_state = malloc(sizeof(struct interface_struct)*num_int)) == NULL) { printf(_("ERROR : memory allocation failure\n")); return N_UNKNOWN;}
	
	/*********************************
	Put all the info in interface structure, return UNKNOWN if values not found (except speed of interface) 
	*********************************/
	for (i=0;i<num_int;i++) {
		tempT=interface_index[i]; /* Get interface first query (description & OID) */
		
		/* get index (last in OID) */
		index_match=(int) tempT->name[tempT->name_length-1];
		
		/* get status */
		if (o_admin==0) {
			if (get_value_int(oper_table,sizeof (oper_table)/sizeof (oid),index_match,valueT,&(int_state[i].state))!=0) { /* cannot get operating value */
				printf(_("Cannot get operationnal status of interface %s\n"),tempT->value);return N_UNKNOWN;
			}
		} else {
			if (get_value_int(admin_table,sizeof (admin_table)/sizeof (oid),index_match,valueT,&(int_state[i].state))!=0) { /* cannot get operating value */
				printf(_("Cannot get admin status of interface %s\n"),tempT->value);return N_UNKNOWN;
			}
		}
		if (o_perf==1 || o_checkperf==1) {
			/* get in_octet OID */
			if (o_highperf==0) {
				if (get_value_long(in_octet_table,sizeof (in_octet_table)/sizeof (oid),index_match,valueT,&(int_state[i].in_oct))!=0) { 
					printf(_("Cannot get input octet of interface %s\n"),tempT->value);return N_UNKNOWN;
				}
			} else {
				if (get_value_long(in_octet_table_64,sizeof (in_octet_table_64)/sizeof (oid),index_match,valueT,&(int_state[i].in_oct))!=0) { 
					printf(_("Cannot get input octet (64 bits) of interface %s\n"),tempT->value);return N_UNKNOWN;
				}
			}
			/* get out_octet OID */
			if (o_highperf==0) {
				if (get_value_long(out_octet_table,sizeof (out_octet_table)/sizeof (oid),index_match,valueT,&(int_state[i].out_oct))!=0) { 
					printf(_("Cannot get output octet of interface %s\n"),tempT->value);return N_UNKNOWN;
				}
			} else {
				if (get_value_long(out_octet_table_64,sizeof (out_octet_table_64)/sizeof (oid),index_match,valueT,&(int_state[i].out_oct))!=0) { 
					printf(_("Cannot get output octet (64 bits) of interface %s\n"),tempT->value);return N_UNKNOWN;
				}
			}
			/* get interface speed OID */
			if (get_value_long(speed_table,sizeof (speed_table)/sizeof (oid),index_match,valueT,&long_int)!=0) { 
				long_int=0;
			}
			int_state[i].speed=(unsigned long) long_int;
			/* get interface high speed OID */
			if (get_value_long(speed_table_64,sizeof (speed_table_64)/sizeof (oid),index_match,valueT,&long_int)!=0) { 
				long_int=0;
			}
			int_state[i].high_speed=(unsigned long) long_int;
			if (o_ext_checkperf==1 || o_perfe==1) {
				/* get in_discard OID */
				if (get_value_long(in_discard_table,sizeof (in_discard_table)/sizeof (oid),index_match,valueT,&long_int)!=0) { 
					printf(_("Cannot get input discard of interface %s\n"),tempT->value);return N_UNKNOWN;
				}
				int_state[i].in_disc=(unsigned long) long_int;
				/* get out_discard OID */
				if (get_value_long(out_discard_table,sizeof (out_discard_table)/sizeof (oid),index_match,valueT,&long_int)!=0) { 
					printf(_("Cannot get output discard of interface %s\n"),tempT->value);return N_UNKNOWN;
				}
				int_state[i].out_disc=(unsigned long) long_int;
				/* get in_error OID */
				if (get_value_long(in_error_table,sizeof (in_error_table)/sizeof (oid),index_match,valueT,&long_int)!=0) { 
					printf(_("Cannot get input error of interface %s\n"),tempT->value);return N_UNKNOWN;
				}
				int_state[i].in_err=(unsigned long) long_int;
				/* get out_error OID */
				if (get_value_long(out_error_table,sizeof (out_error_table)/sizeof (oid),index_match,valueT,&long_int)!=0) { 
					printf(_("Cannot get output error of interface %s\n"),tempT->value);return N_UNKNOWN;
				}
				int_state[i].out_err=(unsigned long) long_int;
			}
		}		
	}
	


	/*********************************
	Now check : state, speed (if requested) & perf data (if requested).
	*********************************/	
	/* get time and triggers : now-3*delta < good value < now - 0,9 * delta     */
	timenow=time(NULL);
	trigger=timenow - (int) (o_delta * 0.9);
	trigger_low=timenow - 3*o_delta;
	final_status=0;	
	ok_val= (o_inverse==1) ? 2 : 1;
	num_ok=0;
	strcat(base_file_name,"/");
	strcat(base_file_name,TMP_FILE_NAME_PREFIX);
	interface_speed=0;
	checkperf_out_raw[0]=checkperf_out_raw[1]=0;
	found_data=0;
	
	for (i=0;i<num_int;i++) {
		if (o_verb) printf(_("Interface %s : state %i\n"),interface_index[i]->value,int_state[i].state);
		/* output basic status of interface */
		strcat_output_notnull(",");
	    if (o_short!=0) {
	      if ((o_short < 0) && ((-o_short) < strlen(interface_index[i]->value))) {
			short_desc = interface_index[i]->value + strlen(interface_index[i]->value)+o_short;}
		  else if ((o_short > 0) && (o_short < strlen(interface_index[i]->value))) {
		    short_desc=malloc(sizeof(char)*(o_short+1));
			strncpy(short_desc,interface_index[i]->value,o_short);
			short_desc[o_short]=0;
		  } else {
			short_desc = interface_index[i]->value;
		  }
		  strcat_output(short_desc);
	    } else {
	      strcat_output(interface_index[i]->value);
	    }
		strcat_output(":");
		strcat_output(status[int_state[i].state]);
		/********************************** 
		Add OK int if state is as requested 
		**********************************/
		if (( int_state[i].state == ok_val) || (o_dormant == 1 && int_state[i].state == 5)) num_ok++;

		convert_text(interface_index[i]->value); /* get rid of special caracters */

		/**********************************
		Check speed, in/out errors and discard 
		*********************************/
		if ((o_checkperf ==1) && (int_state[i].state == 1)) { /* performance data is needed & interface is up */
			
			/* copy to get full file name : BASE_DIR/TMP_FILE_NAME_PREFIX<host/ip>.<int_name> */
			strncpy(file_name,base_file_name,MAX_FILE_LENGTH);
			strncat(file_name,o_host,MAX_FILE_LENGTH-strlen(file_name)-20); /* put host name (leave 20 cars for interface */	
			strcat(file_name,".");
			strncat(file_name,interface_index[i]->value,MAX_FILE_LENGTH-strlen(file_name)-1); /* put interface name */
			/* read file */
			items=(o_ext_checkperf==1) ? 7 : 3;
			file_ret=read_file( file_name, &rows, items, file_values);
			if (file_ret!=0) rows=0;
			if (o_verb) printf(_("File read returns : %i with %i rows\n"),file_ret,rows);
			
		    /* Get the speed in normal or highperf speed counters */
		    if (int_state[i].speed == 4294967295UL) { /*  Too high for this counter (cf IF-MIB) */
				if (o_highperf==0 && (o_prct==1 || o_perfs==1 || o_perfp==1 )) {
					printf(_("Cannot get interface speed with standard MIB, use highperf mib (-g) : UNKNOWN\n"));
					return N_UNKNOWN;
				}
				if (int_state[i].high_speed != 0) {
					interface_speed=int_state[i].high_speed * 1000000;
				} else {
					printf(_("Cannot get interface speed using highperf mib : UNKNOWN\n"));
					return N_UNKNOWN;
				}
		    } else {
		      interface_speed=int_state[i].speed;
		    }
		    if (o_verb) printf(_("Interface speed : %llu\n"),interface_speed);			
			/* make the checks if the file is OK */		
			found_data=0; /* set to 1 if correct data is found (between trigger_low and trigger) */
			if (file_ret ==0) {
				j=rows-1;
				do {
					if ((file_values[j][0] < trigger) && (file_values[j][0] > trigger_low)) {

						/* Define the speed metric ( K | M | G ) (Bits|Bytes) or % */
						if (o_prct==1) {  /* in % of speed */
							/* Speed is in bits/s, calculated speed is in Bytes/s */
							speed_metric=interface_speed/800;
							strcpy(speed_unit,"%");
						} else {
							if (o_kbits==1) { /*  metric in bits */
								if (o_meg==1) { /*  in Mbit/s = 1000000 bit/s */
									  speed_metric=125000; /*   (1000/8) * 1000 */
									  strcpy(speed_unit,"Mbps");
							    } else {
									if (o_gig==1) { /*  in Gbit/s = 1000000000 bit/s */
										  speed_metric=125000000; /*   (1000/8) * 1000 * 1000 */
										  strcpy(speed_unit,"Gbps");
									} else { /*  in Kbits */
										  speed_metric=125; /*   ( 1000/8 ) */
										  strcpy(speed_unit,"Kbps");
									}
								}
							} else { /*  metric in byte */
							    if (o_meg==1) { /*  in Mbits */
									  speed_metric=1048576; /*  1024^2 */
									  strcpy(speed_unit,"MBps");
							    } else {
									if (o_gig==1) { /*  in Mbits */
										  speed_metric=1073741824; /*  1024^3 */
										  strcpy(speed_unit,"GBps");
									} else {
										  speed_metric=1024; /*  1024^3 */
										  strcpy(speed_unit,"KBps");
									}
								}
							}
						}
						/*  check if the counter is back to 0 after 2^32 (highperf is using another OID ) */
						/*  Check counter (s) */
						if (o_highperf==0) {
							overfl = (int_state[i].in_oct >= file_values[j][1] ) ? 0 : 4294967296ULL;
						} else { /* test if reset of counter has occured with highperf counters : TODO*/
							overfl = 0;
						}
						checkperf_out_raw[0] = ( (overfl + int_state[i].in_oct - file_values[j][1])/
			      			      (timenow - file_values[j][0] ));
						checkperf_out[0] = checkperf_out_raw[0] / speed_metric;
						
						if (o_highperf==0) {
							overfl = (int_state[i].in_oct >= file_values[j][1] ) ? 0 : 4294967296ULL;
						} else { /* test if reset of counter has occured with highperf counters : TODO*/
							overfl = 0;
						}
						checkperf_out_raw[1] = ( (overfl + int_state[i].out_oct - file_values[j][2])/
						      (timenow - file_values[j][0] ));
						checkperf_out[1] = checkperf_out_raw[1] / speed_metric;
			    
					    if (o_ext_checkperf==1) {
					      checkperf_out[2] = ( (int_state[i].in_err - file_values[j][3])/
								(timenow - file_values[j][0] ))*60;
					      checkperf_out[3] = ( (int_state[i].out_err - file_values[j][4])/
								(timenow - file_values[j][0] ))*60;
					      checkperf_out[4] = ( (int_state[i].in_disc - file_values[j][5])/
								(timenow - file_values[j][0] ))*60;
					      checkperf_out[5] = ( (int_state[i].out_disc - file_values[j][6])/
								(timenow - file_values[j][0] ))*60;
					    }
						found_data=1;
					}
					j--;
				} while ( (j>=0) && (found_data==0) );
		    }			
			
		    /* Put the new values in the array and write the file */
			file_values[rows][0]=timenow;
		    file_values[rows][1]=int_state[i].in_oct;
		    file_values[rows][2]=int_state[i].out_oct;
		    if (o_ext_checkperf==1) { /* Add other values (error & disc) */
		      file_values[rows][3]=int_state[i].in_err;
		      file_values[rows][4]=int_state[i].out_err;
		      file_values[rows][5]=int_state[i].in_disc;
		      file_values[rows][6]=int_state[i].out_disc;
		    } 
		    rows++;
		    file_ret=write_file( file_name, rows, MAX_ROWS, items, file_values);
		    if (o_verb) printf(_("Write file returned : %i\n"),file_ret);	

			if (file_ret !=0) { /* On error writing, return Unknown status */
				final_status=max_state(final_status,N_UNKNOWN);
				strcat_output(_(" !!Unable to write file "));
				strcat_output(file_name);
			}
		    /*  print the other checks if it was calculated */
		    if (found_data==1) {
				strcat_output(" (");
		 
				/*  check 2 or 6 values depending on ext_check_perf */
				num_checkperf=(o_ext_checkperf==1)?6:2;
				for (j=0;j < num_checkperf;j++) {
					if (o_verb) printf(_("Interface %i, check %i : %f\n"),i,j,checkperf_out[j]);
					if (j!=0) strcat_output("/");
					sprintf(buffer,"%.1f",checkperf_out[j]); /* put  value in string buffer */
					if ((o_crit[j]!=0) && (checkperf_out[j]>o_crit[j])) { 
						final_status=N_CRITICAL;
						strcat_output("CRIT ");
						if (o_label==1) strcat_output(countername[j]);
						strcat_output(buffer);
					} else if ((o_warn[j]!=0) && (checkperf_out[j]>o_warn[j])) { 
						final_status=max_state(final_status,N_WARNING);
						strcat_output("WARN ");
						if (o_label==1) strcat_output(countername[j]);		  
						strcat_output(buffer);
					} else {
						if (o_label==1) strcat_output(countername[j]);
						strcat_output(buffer);
					}
					if ( j==0 || j == 1) { strcat_output(speed_unit); }
				}
				strcat_output(")");
			} else { /*  Return unknown when no data */
			    strcat_output(_(" No usable data on file ("));
				sprintf(buffer,"%i ",rows);
				strcat_output(buffer);
				strcat_output(_("rows) "));
				final_status=max_state(final_status,N_UNKNOWN);
		    }			
			
		}
		
		/********************************** 
		Process performance data 
		**********************************/
		if (( int_state[i].state == 1 ) && (o_perf==1)) {
			if (o_perfp==1) { /* output in % of speed */
				if (found_data==1) {
					p_output_float(interface_index[i]->value,
					checkperf_out_raw[0] * 800 / interface_speed,
						"%.0f","_in_prct","%",
						(o_warn[0]!=0)?1:0,o_warn[0],
						(o_crit[0]!=0)?1:0,o_crit[0],
						1,(double) 0.0,
						1,(double) 100.0);
					p_output_float(interface_index[i]->value,
						checkperf_out_raw[1] * 800 / interface_speed,
						"%.0f","_out_prct","%",
						(o_warn[1]!=0)?1:0,o_warn[1],
						(o_crit[1]!=0)?1:0,o_crit[1],
						1,(double) 0.0,
						1,(double) 100.0);
				}
			} else if (o_perfr==1) {  /* output in bites or Bytes /s */
				if (found_data==1) {
					if (o_kbits == 1) { /* bps */
						/* put warning and critical levels into bps or Bps */
						warn_factor = (o_meg == 1) ? 1000000 : (o_gig == 1) ? 1000000000 : 1000;
						p_output_float(interface_index[i]->value,
							checkperf_out_raw[0] * 8,
							"%.0f","_in_bps","bps",
							(o_warn[0]!=0)?1:0,o_warn[0]*warn_factor,
							(o_crit[0]!=0)?1:0,o_crit[0]*warn_factor,
							1,0.0,
							1,interface_speed);
						p_output_float(interface_index[i]->value,
							checkperf_out_raw[1] * 8,
							"%.0f","_in_bps","bps",
							(o_warn[1]!=0)?1:0,o_warn[1]*warn_factor,
							(o_crit[1]!=0)?1:0,o_crit[1]*warn_factor,
							1,0.0,
							1,interface_speed);	
					} else { /* Bps */
						warn_factor = (o_meg == 1) ? 1048576 : (o_gig == 1) ? 1073741824 : 1024;
						p_output_float(interface_index[i]->value,
							checkperf_out_raw[0],
							"%.0f","_in_Bps","Bps",
							(o_warn[0]!=0)?1:0,o_warn[0]*warn_factor,
							(o_crit[0]!=0)?1:0,o_crit[0]*warn_factor,
							1,0.0,
							1,(int) (interface_speed/8));
						p_output_float(interface_index[i]->value,
							checkperf_out_raw[1],
							"%.0f","_in_Bps","Bps",
							(o_warn[1]!=0)?1:0,o_warn[1]*warn_factor,
							(o_crit[1]!=0)?1:0,o_crit[1]*warn_factor,
							1,0.0,
							1,(int) (interface_speed));	
					}
	  
				}
			} else { /* output in octet counter */
				p_output_ull(interface_index[i]->value,int_state[i].in_oct,"c","_in_octet",0,0,0,0,0,0,0,0);
				p_output_ull(interface_index[i]->value,int_state[i].out_oct,"c","_out_octet",0,0,0,0,0,0,0,0);
			}
		    if (o_perfe==1) {
				p_output_ull(interface_index[i]->value,int_state[i].in_err  ,"c","_in_error"   ,0,0,0,0,0,0,0,0);
				p_output_ull(interface_index[i]->value,int_state[i].out_err ,"c","_out_error"  ,0,0,0,0,0,0,0,0);
				p_output_ull(interface_index[i]->value,int_state[i].in_disc ,"c","_in_discard" ,0,0,0,0,0,0,0,0);
				p_output_ull(interface_index[i]->value,int_state[i].out_disc,"c","_out_discard",0,0,0,0,0,0,0,0);		
		    }
			if (o_perfs==1) {
			  p_output_ull(interface_index[i]->value,interface_speed,"","_speed_bps",0,0,0,0,0,0,0,0);
			}
		}

	}

	/******************************* Output and return ****************************/
	SOCK_CLEANUP;	/* for windows */
	
	/* Check if all interface are OK  */
    sprintf(buffer,_(": %i %s"),num_ok,status[ok_val]);
	strcat_output(buffer);
	if (o_cache!=0 && cache_failure!=0) {
		switch (cache_failure) {
			case 1: strcat_output(_("(Write problems in cache file)"));break;
			case 2: strcat_output(_("(Cache lock file remaining)"));break;
			case 3: strcat_output(_("(Cache lock file cannot be erased)"));break;
		}
	}
	if (num_ok == num_int) {
	  if (final_status==N_OK) {
		return print_output_with_status(N_OK);
	  } else if (final_status==N_WARNING) {
		return print_output_with_status(N_WARNING);
	  } else if (final_status==N_CRITICAL) {
		return print_output_with_status(N_CRITICAL);
	  } else {
		return print_output_with_status(N_UNKNOWN);
	  }
	}

	/* else print the not OK interface number and exit (return is always critical if at least one int is down). */

	sprintf(buffer,_(": %i int not %s"),num_int-num_ok,status[ok_val]);
	strcat_output(buffer);
	return print_output_with_status(N_CRITICAL);

} 

