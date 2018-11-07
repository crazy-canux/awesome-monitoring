#include "nagios_common_snmp.h"

#define SCRIPT_VERSION 0.5
/* default TTL for cache 15 minutes */
#define DEFAULT_CACHE_TTL 15

/********************************
help & usage  
********************************/
char usage[]="-m <name in desc_oid> [-q storagetype] -w <warn_level> -c <crit_level> [-T pl|pu|bl|bu ] [-r -i -e -G] [-s] [-S 0|1[,<car>]] [-K=<min>] [-R <% reserved>]";

/********************************
SNMP data  
********************************/

oid storage_table[]= {1,3,6,1,2,1,25,2,3,1};
oid storagetype_table[]= {1,3,6,1,2,1,25,2,3,1,2};
oid index_table[]= {1,3,6,1,2,1,25,2,3,1,1};
oid descr_table[]= {1,3,6,1,2,1,25,2,3,1,3};
oid size_table[]= {1,3,6,1,2,1,25,2,3,1,5,};
oid used_table[]= {1,3,6,1,2,1,25,2,3,1,6,};
oid alloc_units[]= {1,3,6,1,2,1,25,2,3,1,4,};

/* Storage types definition  - from /usr/share/snmp/mibs/HOST-RESOURCES-TYPES.txt */
#define STORAGE_TYPE_MAX_LENGTH 20
struct hrStorage {
	oid		storage_oid[MAX_OID_LEN];
	size_t	storage_oid_len;
	char	storage_type[STORAGE_TYPE_MAX_LENGTH];
};

int hrStorage_list_num=10;
struct hrStorage hrStorage_list[10] = {
{ {1,3,6,1,2,1,25,2,1,1},10,"Other"},
{ {1,3,6,1,2,1,25,2,1,2},10,"Ram"},
{ {1,3,6,1,2,1,25,2,1,3},10,"VirtualMemory"},
{ {1,3,6,1,2,1,25,2,1,4},10,"FixedDisk"},
{ {1,3,6,1,2,1,25,2,1,5},10,"RemovableDisk"},
{ {1,3,6,1,2,1,25,2,1,6},10,"FloppyDisk"},
{ {1,3,6,1,2,1,25,2,1,7},10,"CompactDisk"},
{ {1,3,6,1,2,1,25,2,1,8},10,"RamDisk"},
{ {1,3,6,1,2,1,25,2,1,9},10,"FlashMemory"},
{ {1,3,6,1,2,1,25,2,1,10},10,"NetworkDisk"}
}; 

void print_usage() {
	printf("Usage: %s %s %s\n",plugin_name, common_usage, usage);
	exit(N_UNKNOWN);
}
void print_version() {
	printf("%s version %f / Package version %f\n",plugin_name,SCRIPT_VERSION,NAGIOS_SNMP_VERSION);
	exit(N_UNKNOWN);
}

void print_help(){

	printf(_("\nSNMP storage Monitor for Nagios version %.2f\n"),SCRIPT_VERSION);
	printf(_("Nagios SNMP plugins version %.2f, GPL licence, (c)2004-2007 Patrick Proy\n\n"),NAGIOS_SNMP_VERSION);
	printf("Usage: %s %s %s\n",plugin_name, common_usage, usage);

	DEFAULT_HELP_PRINT

	printf("-m, --name=NAME\n");
	printf(_("   Name in description OID (can be mounpoints '/home' or 'Swap Space'...)\n"));
	printf(_("   This is treated as a regexp : -m /var will match /var , /var/log, /opt/var ...\n"));
	printf(_("   Test it before, because there are known bugs (ex : trailling /)\n"));
	printf(_("   No trailing slash for mountpoints !\n"));
	printf("-q, --storagetype=[Other|Ram|VirtualMemory|FixedDisk|RemovableDisk|FloppyDisk\n");
	printf("                    CompactDisk|RamDisk|FlashMemory|NetworkDisk]\n");
	printf(_("   Also check the storage type in addition of the name\n"));
	printf(_("   It is possible to use regular expressions ( \"FixedDisk|FloppyDisk\" )\n"));
	printf("-r, --noregexp\n");
	printf(_("   Do not use regexp to match NAME in description OID\n"));
	printf("-s, --sum\n");
	printf(_("   Add all storages that match NAME (used space and total space)\n"));
	printf(_("   THEN make the tests.\n"));
	printf("-i, --index\n");
	printf(_("   Get storage from index instead of description table\n"));
	printf(_("   No cache, storage type and NAME must be an integer\n"));
	printf("-e, --exclude\n");
	printf(_("   Select all storages except the one(s) selected by -m\n"));
	printf(_("   No action on storage type selection\n"));
	printf("-T, --type=TYPE\n");
	printf(_("   pl : calculate percent left\n"));
	printf(_("   pu : calculate percent used (Default)\n"));
	printf(_("   bl : calculate MegaBytes left\n"));
	printf(_("   bu : calculate MegaBytes used\n"));
	printf("-w, --warn=INTEGER\n");
	printf(_("   percent / MB of disk used to generate WARNING state\n"));
	printf(_("   you can add the %% sign \n"));
	printf("-c, --critical=INTEGER\n");
	printf(_("   percent / MB of disk used to generate CRITICAL state\n"));
	printf(_("   you can add the %% sign \n"));
	printf("-K, --cache[=<min>]\n");
	printf(_("   Use cache file to store index\n"));
	printf(_("   Optionnal validity time of index in minutes (default %i)\n"),DEFAULT_CACHE_TTL);	
	printf("-f, --perfparse\n");
	printf(_("   Perfparse compatible output (default)\n"));
	printf("-R, --reserved=INTEGER\n");
	printf(_("   %% reserved blocks for superuser\n"));
	printf(_("   For ext2/3 filesystems, it is 5%% by default\n"));
	printf("-G, --gigabyte\n");
	printf(_("   output, warning & critical levels in gigabytes\n"));
	printf("-S, --short=<type>[,<cut>]\n");
	printf(_("   <type>: Make the output shorter :\n"));
	printf(_("     0 : only print the global result except the disk in warning or critical\n"));
	printf(_("         ex: \"OK : < 80%%\"\n"));
	printf(_("     1 : Don't print all info for every disk \n"));
	printf(_("         ex : \"OK : / : 66 %%used  (<  80)\"\n"));
	printf(_("   <cut>: take the <n> first caracters or <n> last if n<0\n"));
	printf(_("Note : \n"));
	printf(_("  By default, plugin will monitor %%used on drives :\n"));
	printf(_("  warn if %%used > warn and critical if %%used > crit\n"));
	printf(_("  with T=pu or T=bu : OK < warn < crit\n"));
	printf(_("  with T=pl ot T=bl : crit < warn < OK\n\n"));
	printf(_("  If multiple storage are selected, the worse condition will be returned\n"));
	printf(_("  i.e if one disk is critical, the return is critical\n\n"));
	printf(_("  example : \n"));
	printf(_("  Browse storage list : <script> -C <community> -H <host> -m <anything> -w 1 -c 2 -v \n"));
	printf(_("  the -m option allows regexp in perl format : \n"));
	printf(_("  Test drive C,F,G,H,I on Windows 	: -m ^[CFGHI]:    \n"));
	printf(_("  Test all mounts containing /var      	: -m /var\n"));
	printf(_("  Test all mounts under /var      	: -m ^/var\n"));
	printf(_("  Test only /var                 	: -m /var -r\n"));
	printf(_("  Test all swap spaces			: -m ^Swap\n"));
	printf(_("  Test all but swap spaces		: -m ^Swap -e\n"));

	exit(N_UNKNOWN);
}

/* process struct to put snmp data for each process */
struct storage_struct {
	int state;
	unsigned long int used;
	unsigned long int size;
	unsigned long int alloc_unit;
	
};

/********************************
Main of check_snmp_storage
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
	snmp_table *proc_index[n_maxOID];

	/* result tables */
	snmp_table *descT,*tempT,*storT,*temp_storT,*valueT;
	regex_t regexp_filter, regexp_storage_filter;
	struct storage_struct *storage_state; 

	/* cache management */
	int cache_read_result=1;
	int cache_failure=0; /*1: write problems 2: timeout  for lock , 3 remove lock file impossible*/
	snmp_table **cacheT;
	
	/* counters, output, etc... */
	int i;
	int num_storage,test_int,test_stor_type;
	double to,pu,bu,pl,bl;
	double p_warn,p_crit;
	int local_state, warn_state,crit_state;
	char *temp_desc,buffer[200];
	char comp_oper;
	char comp_unit[5];
	char output_metric='M'; /* output metric default MB */
	double output_metric_val=1048576; /* default output in MB (1024*1024) */
	/*	int j;
	char buffer[100]="";
	int num_proc_ok;
	int final_status;
	float res_memory;
	int res_cpu;
*/	
	/************ Options ******************/
	char *o_descr;				/* description filter */
	int o_warn=0;		/*  warning limit  */
	int o_crit=0;		/*  critical limit */
	char* o_type;				/*  pl, pu, mbl, mbu  */
	int o_type_num=-1;			/* 0 : pu, 1 : pl, 2 : bu, 3 : bl */
	int num_typeok=4;
	char o_typeok[4][3]={"pu","pl","bu","bl"}; /*  valid values for o_type */
	int o_noreg=0;				/*  Do not use Regexp for name */
	int o_sum=0;				/*  add all storage before testing */
	int o_index=0;				/*  Put index instead of description */
	int o_index_num=-1;			/* description as number for index */
	int o_negate=0;				/*  Negate the regexp if set */
	int o_perf=0;				/*  Output performance data */
	int o_short[2]={-1,0};		/*  Short output parameters  : default print all & all characters */
	char *o_storagetype; 		/* Storage type */
	int o_cache=0;		/* Use caching of index */
	int o_gig=0;				/* output & warning in GB */
	int o_reserve=0;		/* prct of reverved space fo rsuperuser for ext2/3 FS */

	/***************** 
	options init
	********************/ 
	options_nagsnmp liste[] = { 
		COMMON_OPTIONS
		/*********** Specific for the script ***************/
		/*  { type	,short	,	o_long		,	present,ivalue, 		cvalue,		int_value		n_array} */
		{2,	'm',	"name",			0,	NULL,		&o_descr,	NULL,			0}, /* 12 */
		{0,	'r',	"noregexp",		0,	NULL,		NULL,		&o_noreg,		0}, /* 13 */
		{1,	'w',	"warning",		0,	NULL,		NULL,		&o_warn,		0}, /* 14 */
		{1,	'c',	"critical",		0,	NULL,		NULL,		&o_crit,		0}, /* 15 */
		{2,	'T',	"type",			0,	NULL,		&o_type,	NULL,			0}, /* 16 */
		{2,	'q',	"storagetype",	0,	NULL,		&o_storagetype,	NULL,		0}, /* 17 */
		{0,	'f',	"perfparse",	0,	NULL,		NULL,		&o_perf,		0}, /* 18 */
		{0,	's',	"sum",			0,	NULL,		NULL,		&o_sum,			0}, /* 19 */
		{0,	'i',	"index",		0,	NULL,		NULL,		&o_index,		0}, /* 20 */
		{0,	'e',	"exclude",		0,	NULL,		NULL,		&o_negate,		0}, /* 21 */
		{5,	'S',	"short",		0,	NULL,		NULL,		o_short,		2}, /* 22 */
		{9,	'K',	"cache",		0,	NULL,		NULL,		&o_cache,		0}, /* 23 */
		{0,	'G',	"gigabyte",		0,	NULL,		NULL,		&o_gig,			0}, /* 24 */
		{1,	'R',	"reserved",		0,	NULL,		NULL,		&o_reserve,		0}  /* 25 */	
	};
	int n_liste=COMOPTNUM+14; 
	
	/* Initialise gettext and other parameters */
	if (init_plugins_gettext()!=0) { printf("Cannot initialize GETTEXT\n"); return N_UNKNOWN;}
	strcpy(plugin_name,"check_snmp_storage"); /* Plugin name for cache */
	
	/***************** 
	options init
	********************/ 

	init_options();
	if (get_options (argc, argv,liste,n_liste)!=0) print_usage();
	i=check_common_options();
	if (i==1) print_usage(); else if (i==2) print_help(); else if (i==3) print_version() ;
	/***************************************************** TODO TODO TODO TODO ***************************************************/
	/***************** 
	check options 
	*****************/
	/* specific options tests */
	if (liste[12].present==0) { printf(_("Put storage name\n")); print_usage();}

	/*  Check types */
	if (liste[16].present==0) o_type_num=0;
	else {
		for (i=0;i<num_typeok;i++) {
			if (strcmp(o_typeok[i],o_type)==0) {o_type_num=i;break;}
		}
		if (i==num_typeok) { printf(_("Invalid check type : %s\n"),o_type); print_usage();}
	}
	if (liste[14].present==0) { printf(_("Put warning level\n")); print_usage();}
	if (liste[15].present==0) { printf(_("Put critical level\n")); print_usage();}
	/*  Check for positive numbers */
	if ((o_warn < 0) || (o_crit < 0)) { printf(_("Warning and critical must be > 0 \n"));print_usage(); }
	/*  Check warning and critical values */
	if (( o_type_num==0 || o_type_num==2 ) && ( o_warn >= o_crit ))  {
		printf(_("warn must be < crit if type=%s\n"),o_type);print_usage(); 
	}
	if (( o_type_num==1 || o_type_num==3 ) && ( o_warn <= o_crit ))  {
		printf(_("warn must be > crit if type=%s\n"),o_type);print_usage(); 
	}
	if ( ( o_type_num==0 || o_type_num==1 ) && (o_warn > 100  || o_crit > 100 )) {
		printf(_("percent values must be < 100\n"));print_usage(); 
	} 
	/*  Check short values */
	if ( liste[22].present==1) {
		if (o_short[0] !=0 && o_short[0]!=1) {
			printf(_("-S first option must be 0 or 1\n"));print_usage();
		}
	}
		
	/* check index description */
	if (o_index==1) {
		if (sscanf(o_descr,"%i",&o_index_num) == 0) {
			printf(_("Put integer as storage name with index selection\n"));print_usage();
		}
	}
	if (o_index==1 && (liste[23].present!=0 || liste[17].present!=0)) {
		printf(_("No cache or storage type with index selection\n"));print_usage();
	}
	/* Check cache value */
	if (liste[23].present==3) o_cache=DEFAULT_CACHE_TTL;
	if ((liste[23].present==1)&& o_cache < 2) {
		printf(_("Validity of cache must be >= 2 minutes\n")); print_usage(); 
	}
	
    /* reserved blocks checks (A. Greiner-Bär patch). */
    if ((liste[25].present==1) && (o_reserve > 99 || o_reserve < 0 )) {
		printf(_("Reserved blocks must be < 100 and >= 0\n")); print_usage(); 
    }

	/***********************************************************************
	**************** open snmp session ******************
	***********************************************************************/
	if (init_snmp_session()!=0) {
		printf(_("Error opening SNMP session : %s\n"),error);
		free(error);
		return N_UNKNOWN;
	}

	/***********************************************************************
	*****************  Get name or index, and storage type table *****************
	***********************************************************************/
	storT=descT=NULL; /* init to prevent compile time warnings */
	cacheT=NULL; 
	if (o_cache>0) {	
		if ((cacheT=malloc(sizeof(snmp_table *)*2))==NULL) {
			printf(_("Memory allocation failure in cache file"));
			return N_UNKNOWN;
		}
		if ((cache_read_result=read_cache(cacheT,2, 60*o_cache))==3) {
			printf(_("Memory allocation failure in cache file"));
			return N_UNKNOWN;
		}
		descT=cacheT[1];storT=cacheT[0];
		cache_failure=cache_read_result; /* 1 : timeout -> lock file not removed */
	}
	if (o_verb==1 && o_cache>0) printf("Cache read result : %i\n",cache_read_result);
	if ((o_index==0) && ((o_cache==0) || (cache_read_result!=0))) {
		descT=get_snmp_table (ss,descr_table, sizeof (descr_table) / sizeof (oid),snmp_bulk,&error);
		if (descT==NULL) {
			printf(_("Error reading table : %s\n"),error);
			free(error);
			return N_UNKNOWN;
		}	
		/*****************  Get storage type table *****************/
		if ((liste[17].present==1)||(o_cache>0)) { /* get storage type table : always get when using cache */
			storT=get_snmp_table (ss,storagetype_table, sizeof (storagetype_table) / sizeof (oid),snmp_bulk,&error);
			if (storT==NULL) {
				printf(_("Error reading table : %s\n"),error);
				free(error);
				return N_UNKNOWN;
			}
		}		
	}

	
	/*****************  Get matching interface and create an array of requested OIDs  *****************/
	tempT=descT;
	/* Init the regexp filter */
	if ((o_noreg==0)&&(regcomp(&regexp_filter,o_descr,REG_NOSUB | REG_EXTENDED)!=0)) {
		printf(_("Error compiling regexp (invalid regexp or memory prb)\n"));
		return N_UNKNOWN; 
	}
	if (liste[17].present==1) {
		if (regcomp(&regexp_storage_filter,o_storagetype,REG_NOSUB | REG_EXTENDED)!=0) {
			printf(_("Error compiling regexp (invalid regexp or memory prb)\n"));
			return N_UNKNOWN; 	
		}
	} 
	if (((o_cache>0) && (cache_read_result!=0))) { /* Caching all storage value types with cache on, so must get them */
		temp_storT=storT;
		while (temp_storT !=NULL) {
			i=0;
			while ((i<hrStorage_list_num) && snmp_oid_compare(
					hrStorage_list[i].storage_oid,hrStorage_list[i].storage_oid_len,
					temp_storT->value_oid	  ,temp_storT->value_oid_length)!=0) 
				i++;
			if ( i == hrStorage_list_num ) {
				printf (_("UNKNOWN : Unknown storage type corresponding to oid : "));
				for (i=0;i<temp_storT->value_oid_length;i++) printf(".%lu",temp_storT->value_oid[i]);
				printf("\n");
				return N_UNKNOWN;
			}
			temp_storT->value=malloc(sizeof(char)*STORAGE_TYPE_MAX_LENGTH);
			sprintf(temp_storT->value,"%s",hrStorage_list[i].storage_type);			
			temp_storT=temp_storT->next;
		}
	}		
	
	
	n_valOID=num_storage=0;	
	if (o_index==1) {
		/* Get storage size */
		copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],
		size_table,sizeof (size_table)/sizeof (oid),o_index_num);
		n_valOID++;
		/* Get storage used */
		copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],
		used_table,sizeof (used_table)/sizeof (oid),o_index_num);
		n_valOID++;
		/* Get storage allocation unit  */
		copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],
		alloc_units,sizeof (alloc_units)/sizeof (oid),o_index_num);
		n_valOID++;
		tempT=malloc(sizeof(snmp_table));
		tempT->name_length=1;
		tempT->name[0]=o_index_num;
		tempT->value=malloc(sizeof(char)*20);
		sprintf(tempT->value,"Index %i",o_index_num);
		proc_index[num_storage++]=tempT;
	} else {
		while ((tempT != NULL)&&(n_valOID<(n_maxOID-5))) {
			if (o_verb) {printf ("OID : "); for (i=0;i<tempT->name_length;i++) printf(".%lu",tempT->name[i]); printf(" : %s\n", tempT->value);}

			if (o_noreg==0) {
				test_int=regexec(&regexp_filter,tempT->value,0,NULL,0);
			} else {
				test_int=strcmp(o_descr,tempT->value);
			}
			if (o_negate==1) test_int=(test_int==0)?1:0; /* negate result of test if needed */
			if (test_int==0) {
				/* get index (last in OID) */
				index_match=(int) tempT->name[tempT->name_length-1];
				test_stor_type=0;
			   /* Check if storage type matches option  */
				if (liste[17].present==1) {				
					temp_storT=storT;
					while ((temp_storT !=NULL) && (temp_storT->name[temp_storT->name_length-1] != index_match)) 
						temp_storT=temp_storT->next;
					if (temp_storT ==NULL) {
						printf (_("Can't find storage type OID for storage %s : UNKNOWN"),tempT->value);return N_UNKNOWN;
					}
					if (o_cache==0) { /* without caching */
						i=0;
						while ((i<hrStorage_list_num) && snmp_oid_compare(
								hrStorage_list[i].storage_oid,hrStorage_list[i].storage_oid_len,
								temp_storT->value_oid	  ,temp_storT->value_oid_length)!=0) 
							i++;
						if ( i == hrStorage_list_num ) {
							printf (_("UNKNOWN : Unknown storage type corresponding to oid : "));
							for (i=0;i<temp_storT->value_oid_length;i++) printf(".%lu",temp_storT->value_oid[i]);
							printf("\n");
							return N_UNKNOWN;
						}
						test_stor_type=regexec(&regexp_storage_filter,hrStorage_list[i].storage_type,0,NULL,0);
						if (o_verb) printf(_("Storage type : %s (match = %c)\n"),hrStorage_list[i].storage_type,(test_stor_type==0)?'Y':'N');
					} else { /* read from cache (or calculated just before) : just need to compare the value */
						test_stor_type=regexec(&regexp_storage_filter,temp_storT->value,0,NULL,0);
						if (o_verb) printf(_("Storage type : %s (match = %c)\n"),temp_storT->value,(test_stor_type==0)?'Y':'N');		
					}
				}
				/* get OIDs to query */
				if (test_stor_type==0) {
					proc_index[num_storage++]=tempT;
					/* Get storage size */
					copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],
					size_table,sizeof (size_table)/sizeof (oid),index_match);
					n_valOID++;
					/* Get storage used */
					copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],
					used_table,sizeof (used_table)/sizeof (oid),index_match);
					n_valOID++;
					/* Get storage allocation unit  */
					copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],
					alloc_units,sizeof (alloc_units)/sizeof (oid),index_match);
					n_valOID++;
				}
				
			} 
			tempT=tempT->next;
		} 
	} /* end index if */

	if (o_verb) printf(_("Found : %i match : %i OID queries\n"),num_storage,n_valOID);
	if (n_valOID>=(n_maxOID-5)) { printf(_("ERROR : Maximum number of OID queries reached : %i\n"),n_maxOID); return N_UNKNOWN;}
	
	/* No storage found -> error */
	if (num_storage==0) { printf(_("ERROR : Unknown storage %s\n"),o_descr); return N_UNKNOWN;}
	
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
		cacheT[1]=descT;
		cacheT[0]=storT;
		cache_read_result = write_cache(cacheT,2);
		cache_failure=cache_read_result;
		if (o_verb) printf(_("Write cache returned : %i\n"),cache_read_result);	
	}
	

	if ((storage_state = malloc(sizeof(struct storage_struct)*num_storage)) == NULL) { printf(_("ERROR : memory allocation failure\n")); return N_UNKNOWN;}
	
	/*********************************
	Put all the info in storage structure, return UNKNOWN if values not found (TODO : maybe invalidate cache and retry ? )
	*********************************/	
	for (i=0;i<num_storage;i++) {
		tempT=proc_index[i]; /* Get storage first query (description & OID) */
		
		/* get index (last in OID) */
		index_match=(int) tempT->name[tempT->name_length-1];
		
		/* get used value */
		if (get_value_ulint(used_table,sizeof (used_table)/sizeof (oid),index_match,valueT,&(storage_state[i].used))!=0) { 
			printf(_("Cannot get used value for %s\n"),tempT->value);return N_UNKNOWN;
		}
		if (get_value_ulint(size_table,sizeof (size_table)/sizeof (oid),index_match,valueT,&(storage_state[i].size))!=0) { 
			printf(_("Cannot get size value for %s\n"),tempT->value);return N_UNKNOWN;
		}
		if (get_value_ulint(alloc_units,sizeof (alloc_units)/sizeof (oid),index_match,valueT,&(storage_state[i].alloc_unit))!=0) { 
			printf(_("Cannot get alloc unit value for %s\n"),tempT->value);return N_UNKNOWN;
		}		
		if (o_verb) printf("Storage %s : %lu used, %lu size, %lu alloc unit\n",tempT->value,storage_state[i].used,storage_state[i].size,storage_state[i].alloc_unit);
	}
	
	/* Set the metric */
	if (o_gig==1) {
		output_metric_val *= 1024;
		output_metric='G';
	}
	/* treat sum option */
	to=bu=pl=pu=bl=0.0;
	if (o_sum==1) {
		for (i=0;i<num_storage;i++) {
			to += (double)storage_state[i].size * storage_state[i].alloc_unit;
			bu += (double)storage_state[i].used * storage_state[i].alloc_unit;
		}
		to = to * ( ( 100 - o_reserve ) / 100 ) / (output_metric_val);
		bu = bu / (output_metric_val);
		pu = bu/to;
		pl = (to-bu)/to;
		num_storage=1;
	}
	warn_state=crit_state=0;
	p_warn=p_crit=0;
	/*make the checks */
	for (i=0;i<num_storage;i++) {
		tempT=proc_index[i]; /* Get storage first query (description & OID) */
		if (o_sum!=1) {
			to=(double) storage_state[i].size * ( ( 100.0 - o_reserve ) / 100.0 ) * storage_state[i].alloc_unit / output_metric_val;
			if (storage_state[i].size != 0) {
				pu=storage_state[i].used * 100 / (storage_state[i].size* ( 100.0 - o_reserve ) / 100.0 );
			} else 
				pu=0;
			bu=(double)storage_state[i].used * storage_state[i].alloc_unit / output_metric_val;
			pl=100-pu;
			bl = ((double) storage_state[i].size * ( ( 100.0 - o_reserve ) / 100.0 ) - storage_state[i].used ) * storage_state[i].alloc_unit / output_metric_val;
		}
		/* Ouputs and checks */
		/* Keep complete description for performance output (in MB) */
		
		
		subst_chars(tempT->value, "'|\"", '_');
		if (o_sum==1) {
			asprintf(&temp_desc,"Sum of all %s",o_descr);
		}else {
			asprintf(&temp_desc,"%s",tempT->value);
		}
		if (o_short[1]!=0) {
			if (o_short[1]>0) { /* print n first char only */
				if (o_short[1] <strlen(temp_desc)) 
					temp_desc[o_short[1]]=0;
			} else {  /* print n first char only */
				if (( - o_short[1]) < strlen(temp_desc)) 
					temp_desc+=strlen(temp_desc)+o_short[1];
			}
		}
		local_state=0;
		switch (o_type_num) { /* 0 : pu, 1 : pl, 2 : bu, 3 : bl */
			case 0: /* % used */
				p_warn=o_warn*to/100;p_crit=o_crit*to/100;
				if (pu>=o_crit) (local_state=crit_state=1);
				if (pu>=o_warn) (local_state=warn_state=1);
				if ((local_state==1)|| (o_short[0]==-1)) {
					sprintf(buffer,"%s: %.0f%%used(%.0f%cB/%.0f%cB) ",temp_desc,pu,bu,output_metric,to,output_metric);
					strcat_output(buffer);
				} else if (o_short[0]==1) {
					sprintf(buffer,"%s: %.0f%% ",temp_desc,pu);
					strcat_output(buffer);				
				}
				comp_oper='<';comp_unit[0]='%';comp_unit[1]=0;
			break;
			case 1: /* % left */
				p_warn=(100-o_warn)*to/100;p_crit=(100-o_crit)*to/100;
				if (pl<=o_crit) (local_state=crit_state=1);
				if (pl<=o_warn) (local_state=warn_state=1);
				if ((local_state==1)|| (o_short[0]==-1)) {
					sprintf(buffer,"%s: %.0f%%left(%.0f%cB/%.0f%cB) ",temp_desc,pl,bu,output_metric,to,output_metric);
					strcat_output(buffer);
				} else if (o_short[0]==1) {
					sprintf(buffer,"%s: %.0f%% ",temp_desc,pl);
					strcat_output(buffer);			
				}
				comp_oper='>';comp_unit[0]='%';comp_unit[1]=0;
			break;
			case 2: /* MB used */
				p_warn=o_warn;p_crit=o_crit;
				if (bu>=o_crit) (local_state=crit_state=1);
				if (bu>=o_warn) (local_state=warn_state=1);
				if ((local_state==1)|| (o_short[0]==-1)) {
					sprintf(buffer,"%s: %.0f%cBused/%.0f%cB (%.0f%%) ",temp_desc,bu,output_metric,to,output_metric,pu);
					strcat_output(buffer);
				} else if (o_short[0]==1) {
					sprintf(buffer,"%s: %.0f%cB ",temp_desc,bu,output_metric);
					strcat_output(buffer);					
				}
				comp_oper='<';comp_unit[0]=output_metric;comp_unit[1]='B';comp_unit[2]=0;
			break;		
			case 3: /* MB left */
				p_warn=to-o_warn;p_crit=to-o_crit;
				if (bl<=o_crit) (local_state=crit_state=1);
				if (bl<=o_warn) (local_state=warn_state=1);
				if ((local_state==1)|| (o_short[0]==-1)) {
					sprintf(buffer,"%s: %.0f%cBleft/%.0f%cB (%.0f%%) ",temp_desc,bl,output_metric,to,output_metric,pl);
					strcat_output(buffer);
				} else if (o_short[0]==1){
					sprintf(buffer,"%s: %.0f%cB ",temp_desc,bl,output_metric);
					strcat_output(buffer);				
				}
				comp_oper='>';comp_unit[0]=output_metric;comp_unit[1]='B';comp_unit[2]=0;
			break;				
		}
		
		if (o_gig==1) {
			p_output_int(tempT->value,(long int) bu,"GB","",1,(long int)p_warn,1,(long int)p_crit,1,0,1,(long int)to);
		} else {
			p_output_int(tempT->value,(long int) bu,"MB","",1,(long int)p_warn,1,(long int)p_crit,1,0,1,(long int)to);
		}
	}

	//if (!defined ($output)) { $output="All selected storages "; }
		
	if ( crit_state == 1) {
		comp_oper = (comp_oper == '<') ? '>' : '<';  /* Inverse comp operator */
		sprintf(buffer," : %c %i %s ",comp_oper,o_crit,comp_unit);
		strcat_output(buffer);
		return print_output_with_status(N_CRITICAL);
	}
	if ( warn_state == 1) {
		comp_oper = (comp_oper == '<') ? '>' : '<';  /* Inverse comp operator */
		sprintf(buffer," : %c %i %s ",comp_oper,o_warn,comp_unit);
		strcat_output(buffer);
		return print_output_with_status(N_WARNING);
	}
	sprintf(buffer," : %c %i %s ",comp_oper,o_warn,comp_unit);
	strcat_output(buffer);
	return print_output_with_status(N_OK);
}
