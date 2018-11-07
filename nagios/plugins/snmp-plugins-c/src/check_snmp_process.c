#include "nagios_common_snmp.h"

#define SCRIPT_VERSION 0.5
#define TMP_FILE_NAME_PREFIX "tmp_Nagios_proc."
/* default delta for averages : 300s = 5min */
#define DEFAULT_DELTA_AVERAGE 300

/********************************
help & usage  
********************************/
char usage[]=
"-n <name> [-w <min_proc>[,<max_proc>] -c <min_proc>[,max_proc] ] [-m<warn Mb>,<crit Mb> -a -u<warn %>,<crit%> -d<delta>] [-f -A -F] [-r]"; 

/********************************
SNMP data  
********************************/
oid process_table[]= {1,3,6,1,2,1,25,4,2,1};
oid index_table[] = {1,3,6,1,2,1,25,4,2,1,1};
oid run_name_table[] = {1,3,6,1,2,1,25,4,2,1,2};
oid run_path_table[] = {1,3,6,1,2,1,25,4,2,1,4};
oid run_param_table[] = {1,3,6,1,2,1,25,4,2,1,5};
oid proc_mem_table[] = {1,3,6,1,2,1,25,5,1,1,2}; /* Kbytes */
oid proc_cpu_table[] = {1,3,6,1,2,1,25,5,1,1,1}; /* Centi sec of CPU */
oid proc_run_state[] = {1,3,6,1,2,1,25,4,2,1,7};

void print_usage() {
	printf("Usage: %s %s %s\n",plugin_name, common_usage, usage);
	exit(N_UNKNOWN);
}
void print_version() {
	printf("%s version %.2f / Package version %.2f\n",plugin_name,SCRIPT_VERSION,NAGIOS_SNMP_VERSION);
	exit(N_UNKNOWN);
}

void print_help(){

	printf(_("\nSNMP Process Monitor for Nagios version %.2f\n"),SCRIPT_VERSION);
	printf(_("Nagios SNMP plugins version %.2f, GPL licence, (c)2004-2007 Patrick Proy\n\n"),NAGIOS_SNMP_VERSION);
	printf("Usage: %s %s %s\n",plugin_name, common_usage, usage);

	DEFAULT_HELP_PRINT

	printf("-n, --name=NAME\n");
	printf(_("   Name of the process (regexp)\n"));
	printf(_("   No trailing slash !\n"));
	printf("-r, --noregexp\n");
	printf(_("   Do not use regexp to match NAME in description OID\n"));
	printf("-f, --fullpath\n");
	printf(_("   Use full path name instead of process name \n"));
	printf(_("   (Windows doesn't provide full path name)\n"));
	printf("-w, --warning=MIN[,MAX]\n");
	printf(_("   Number of process that will cause a warning\n"));
	printf(_("   -1 for no warning, MAX must be >0. Ex : -w-1,50\n"));
	printf("-c, --critical=MIN[,MAX]\n");
	printf(_("   number of process that will cause an error\n"));
	printf(_("   -1 for no critical, MAX must be >0. Ex : -c-1,50\n"));
	printf(_("Notes on warning and critical : \n"));
	printf(_("   with the following options : -w m1,x1 -c m2,x2\n"));
	printf(_("   you must have : m2 <= m1 < x1 <= x2\n"));
	printf(_("   you can omit x1 or x2 or both\n"));
	printf("-m, --memory=WARN,CRIT\n");
	printf(_("   checks memory usage (default max of all process)\n"));
	printf(_("   values are warning and critical values in Mb\n"));
	printf("-a, --average\n");
	printf(_("   makes an average of memory used by process instead of max\n"));
	printf("-u, --cpu=WARN,CRIT\n");
	printf(_("   checks cpu usage of all process\n"));
	printf(_("   values are warning and critical values in %% of CPU usage\n"));
	printf(_("   if more than one CPU, value can be > 100%% : 100%%=1 CPU\n"));
	printf(" -A, --param\n");
	printf(_("   Add parameters to select processes.\n"));
	printf(_("   ex : \"named.*-t /var/named/chroot\" will only select named process with this parameter\n"));
	printf("-F, --perfout\n");
	printf(_("   Performance output : number of processes\n"));
	printf(_("   mem & cpu with -a and -u options)\n"));
	printf("-d, --delta=seconds\n");
	printf(_("   make an average of <delta> seconds for cpu (default 300)\n"));	
/*	printf("-g, --getall\n");   dropped this option, maybe get it back if really needed */
	printf(_("Note :   \n"));
	printf(_("  CPU usage is in %% of one cpu, so maximum can be 100%% * number of CPU \n"));
	printf(_("  example : \n"));
	printf(_("  Browse process list : <script> -C <community> -H <host> -n <anything> -v \n"));
	printf(_("  the -n option allows regexp in perl format : \n"));
	printf(_("  All process of /opt/soft/bin  	: -n /opt/soft/bin/ -f\n"));
	printf(_("  All 'named' process			: -n named\n"));
	exit(N_UNKNOWN);
}

/* process struct to put snmp data for each process */
struct process_struct {
	int index;
	int state;
	int mem_used;
	int cpu_count;
};

/********************************
Main of check_snmp_process
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
	snmp_table *descT,*tempT,*valueT,*paramT,*temp2T;
	regex_t regexp_filter;
	struct process_struct *proc_state;
	
	/* counters, output, etc... */
	int i,j;
	char buffer[100]="",mem_buffer[100]="",cpu_buffer[100]="";
	char *buff_select;
	int num_proc,test_int,num_proc_ok;
	int final_status;
	float res_memory;
	int res_cpu;
	
	/* file datas & name */
	unsigned long long int file_values[MAX_ROWS][MAX_ITEMS]; /* to get & store data in file */
	int rows=0;int items=0;int file_ret,found_data;
	char base_file_name[MAX_FILE_LENGTH]=BASE_DIR;	
	char file_name[MAX_FILE_LENGTH];
	long unsigned int timenow,trigger,trigger_low;
	float found_value;
	
	/************ Options ******************/
	char *o_descr;			/* description filter */
	int o_warn[2]={0,-1}; 	/*  warning limit  (min,max) */
	int o_crit[2]={0,-1};	/*  critical limits (min,max) */
	int o_noreg=0;			/*  Do not use Regexp for name */
	int o_path=0;			/*  check path instead of name */
	/* int o_inverse=0;	 checks max instead of min number of process */ 
	int o_get_all=0;		/*  get all tables at once */
	/*  Memory & CPU */
	int o_mem[2]={-1,-1};	/*  warn and crit level for mem */
	int o_mem_avg=0;		/*  cheks memory average */
	int o_cpu[2]={-1,-1};	/*  checks CPU usage */
	int o_delta=DEFAULT_DELTA_AVERAGE;		/*  delta time for CPU check */
	int o_perf=0;			/* output performance data */
	int o_param=0;			/* check parameter too */

	/***************** 
	options init
	********************/ 
	options_nagsnmp liste[] = { 
		COMMON_OPTIONS
		/*********** Specific for the script ***************/
		/*  { type	,short	,	o_long		,	present,ivalue, 		cvalue,		int_value		n_array} */
		{2,	'n',	"name",			0,	NULL,		&o_descr,	NULL,			0}, /* 12 */		
		{0,	'r',	"noregexp",		0,	NULL,		NULL,		&o_noreg,		0}, /* 13 */
		{5,	'w',	"warning",		0,	NULL,		NULL,		o_warn,			2}, /* 14 */
		{5,	'c',	"critical",		0,	NULL,		NULL,		o_crit,			2}, /* 15 */		
		{0,	'f',	"fullpath",		0,	NULL,		NULL,		&o_path,		0}, /* 16 */
		{5,	'm',	"memory",		0,	NULL,		NULL,		o_mem,			2}, /* 17 */			
		{0,	'a',	"average",		0,	NULL,		NULL,		&o_mem_avg,		0}, /* 18*/
		{0,	'g',	"getall",		0,	NULL,		NULL,		&o_get_all,		0}, /* 19 */
		{5,	'u',	"cpu",			0,	NULL,		NULL,		o_cpu,			2}, /* 20 */
		{0,	'F',	"perfout",		0,	NULL,		NULL,		&o_perf,		0}, /* 21 */
		{1,	'd',	"delta",		0,	NULL,		NULL,		&o_delta,		0}, /* 22 */
		{0,	'A',	"param",		0,	NULL,		NULL,		&o_param,		0}  /* 23 */		
	};
	int n_liste=COMOPTNUM+12; 
	
	/* Initialise gettext and other parameters */
	if (init_plugins_gettext()!=0) { printf("Cannot initialize GETTEXT\n"); return N_UNKNOWN;}
	strcpy(plugin_name,"check_snmp_process"); /* Plugin name for cache */
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
	if (liste[12].present==0) { printf(_("Put process name\n")); print_usage();}
	
	/* Check for positive numbers on maximum number of processes */
	if ((liste[14].present==1) && (liste[14].n_array==2) && (o_warn[1] < 0)) {
		printf(_("Maximum process warninig and critical must be > 0 \n\n")); print_usage();}
	if ((liste[15].present==1) && (liste[15].n_array==2) && (o_crit[1] < 0)) {
		printf(_("Maximum process warninig and critical must be > 0 \n\n")); print_usage();}

	/* Check min_crit < min warn < max warn < crit warn */
	if ((liste[14].present==1) && (liste[15].present==1) && (o_warn[0] < o_crit[0])) {
		printf(_("Warning minimum must be >= critical minimum\n"));print_usage();}
	if ((liste[15].present==1) && (liste[15].n_array==2) && (o_crit[1] <= o_crit[0])) {
		printf(_("critical minimum must be < critical maximum\n")); print_usage();}
	
	if ((liste[14].present==1) && (liste[14].n_array==2)) {
		if (o_warn[1] <= o_warn[0])
		{ printf(_("warn minimum must be < warn maximum\n"));print_usage();};
	} else if ( (liste[15].present==1) && (liste[15].n_array==2) && (o_crit[1] <= o_warn[0])) 
	{ printf(_("warn minimum must be < crit maximum when no crit warning defined\n"));print_usage();} 

	if ( (liste[15].present==1) && (liste[15].n_array==2) && (liste[14].present==1) && (liste[14].n_array==2) && (o_crit[1]<o_warn[1])) 
	{ printf(_("warn max must be <= crit maximum\n"));print_usage();}  
	/* Memory checks */
	if (liste[17].present==1) {
		if (liste[17].n_array != 2) 
		{printf(_("2 values (warning,critical) for memory\n"));print_usage();};
		if (o_mem[0]>o_mem[1])
		{printf(_("Warning must be <= Critical for memory!\n"));print_usage();};
	}
	/* CPU checks */
	if (liste[20].present==1) {
		if (liste[20].n_array != 2) 
		{printf(_("2 values (warning,critical) for cpu\n"));print_usage();}
		if (o_cpu[0]>o_cpu[1]) 
		{printf(_("Warning must be <= Critical for cpu!\n"));print_usage();}
	}	
	/*printf ("Warn, crit : %i,%i,%i,%i",o_warn[0],o_warn[1] , o_crit[0], o_crit[1]);*/
	
	/**************** open snmp session ******************/	
	if (init_snmp_session()!=0) {
		printf(_("Error opening SNMP session : %s\n"),error);
		free(error);
		return N_UNKNOWN;
	}

	/*****************  Get name or path table *****************/
	if (o_path==0) 
		descT=get_snmp_table (ss,run_name_table, sizeof (run_name_table) / sizeof (oid),snmp_bulk,&error);
	else 
		descT=get_snmp_table (ss,run_path_table, sizeof (run_path_table) / sizeof (oid),snmp_bulk,&error);
	
	if (descT==NULL) {
		printf(_("Error reading table : %s\n"),error);
		free(error);
		return N_UNKNOWN;
	}	
	
	/*****************  Get param table if needed*****************/
	if (o_param==1) {
		paramT=get_snmp_table (ss,run_param_table, sizeof (run_param_table) / sizeof (oid),snmp_bulk,&error);	
		if (paramT==NULL) {
			printf(_("Error reading param table : %s\n"),error);
			free(error);
			return N_UNKNOWN;
		}
	} else paramT=NULL;
	/*****************  Get matching process and create an array of requested OIDs  *****************/
	tempT=descT;
	/* Init the regexp filter */
	if ((o_noreg==0)&&(regcomp(&regexp_filter,o_descr,REG_NOSUB | REG_EXTENDED)!=0)) {
		printf(_("Error compiling regexp (invalid regexp or memory prb)\n"));
		return N_UNKNOWN; 
	}
	
	n_valOID=num_proc=0;	
	while ((tempT != NULL)&&(n_valOID<(n_maxOID-5))) {
		if (o_param==1) { 
			temp2T=paramT;
			index_match=(int) tempT->name[tempT->name_length-1];
			while ((temp2T!=NULL) && (temp2T->name[temp2T->name_length-1]!=index_match))
				temp2T=temp2T->next;
			if (temp2T!=NULL) asprintf(&buff_select,"%s %s",tempT->value,temp2T->value);
			if (o_verb) {printf ("OID : "); for (i=0;i<tempT->name_length;i++) printf(".%lu",tempT->name[i]); printf(" : %s\n", buff_select);}
			if (o_noreg==0) {
				test_int=regexec(&regexp_filter,buff_select,0,NULL,0);
			} else {
				test_int=strcmp(o_descr,buff_select);
			}
			free(buff_select);
		} else {
			if (o_verb) {printf ("OID : "); for (i=0;i<tempT->name_length;i++) printf(".%lu",tempT->name[i]); printf(" : %s\n", tempT->value);}
			if (o_noreg==0) {
				test_int=regexec(&regexp_filter,tempT->value,0,NULL,0);
			} else {
				test_int=strcmp(o_descr,tempT->value);
			}
		}
		if (test_int==0) {
			proc_index[num_proc++]=tempT;
			/* get index (last in OID) */
			index_match=(int) tempT->name[tempT->name_length-1];

			/* Get process state */
			copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],
			proc_run_state,sizeof (proc_run_state)/sizeof (oid),index_match);
			n_valOID++;
			/* get  memory if needed */
			if (o_mem[0]>0) {
				copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],
				proc_mem_table,sizeof (proc_mem_table)/sizeof (oid),index_match);
				n_valOID++;
			}
			/* get  cpu if needed */
			if (o_cpu[0]>0) {
				copy_oid_index(valOID[n_valOID],&valOID_len[n_valOID],
				proc_cpu_table,sizeof (proc_cpu_table)/sizeof (oid),index_match);
				n_valOID++;
			}
		}
		tempT=tempT->next;
	}

	if (o_verb) printf(_("Found : %i match : %i OID queries\n"),num_proc,n_valOID);
	if (n_valOID>=(n_maxOID-5)) { printf(_("ERROR : Maximum number of OID queries reached : %i\n"),n_maxOID); return N_UNKNOWN;}

	/* If number of selected process is 0, return value depending on warning/critical options( by default o_crit[0]=0) */
	if ( num_proc == 0) {
		printf (_("No process %s %s found :"),(o_noreg==1) ? _("named") : _("matching"), o_descr);
		if (o_crit[0]>=0) {
			printf(_("CRITICAL\n"));
			return N_CRITICAL;
		} else if (o_warn[0]>=0) {
			printf(_("WARNING\n"));
			return N_WARNING;
		}
		printf(_("YOU told me it was : OK\n"));
		return N_OK;
	}
	
	/******************************** 
	Get status & other if needed 
	********************************/
	valueT=get_snmp_oids(ss,valOID,valOID_len,n_valOID,&error);
	if (valueT==NULL) {
		printf(_("Error reading table : %s\n"),error);
		free(error);
		return N_UNKNOWN;
	}
	snmp_close(ss);

	/*************************************************************************
	Put all the info in process_table structure, put state to -1 if requested values not found 
	**************************************************************************/	
	/* alloc the proc_state table */
	if ((proc_state = malloc(sizeof(struct process_struct)*num_proc)) == NULL) { printf(_("ERROR : memory allocation failure\n")); return N_UNKNOWN;}
	num_proc_ok=0;
	for (i=0;i<num_proc;i++) {
		tempT=proc_index[i]; /* Get process first query (description & OID) */
		
		/* get index (last in OID) */
		index_match=(int) tempT->name[tempT->name_length-1];
		
		/* get status */
		if (get_value_int(proc_run_state,sizeof (proc_run_state)/sizeof (oid),index_match,valueT,&(proc_state[i].state))!=0) { 
			/* cannot get running state value */
			proc_state[i].state=-1; /* to mark process has died */
			if (o_verb) printf(_("Process %i died between SNMP queries\n"),index_match);
			break; /* go to next process */
		}
		if (proc_state[i].state == 1 || proc_state[i].state == 2) num_proc_ok++;
		/* Get memory data */
		if (o_mem[0]>0) {
			if (get_value_int(proc_mem_table,sizeof (proc_mem_table)/sizeof (oid),index_match,valueT,&(proc_state[i].mem_used))!=0) { 
				/* cannot get memory state */
				proc_state[i].state=-1; /* to mark process has died */
				if (o_verb) printf(_("Cannot read mem usage for process %i\n"),index_match);
				break; /* go to next process */
			}			
		} else proc_state[i].mem_used=-1;
		if (o_cpu[0]>0) {
			if (get_value_int(proc_cpu_table,sizeof (proc_cpu_table)/sizeof (oid),index_match,valueT,&(proc_state[i].cpu_count))!=0) { 
				/* cannot get cpu counter */
				proc_state[i].state=-1; /* to mark process has died */
				if (o_verb) printf(_("Cannot read proc counter for process %i\n"),index_match);
				break; /* go to next process */
			}	
		} else proc_state[i].cpu_count=-1;
		if (o_verb) printf(_("Process %i in state %i using %i mem and %i cpu\n"),index_match,proc_state[i].state,proc_state[i].mem_used,proc_state[i].cpu_count);
	}
	
	final_status=N_OK;
	res_memory=0;
	/*************************************************************************
	Checks memory usage 
	**************************************************************************/
	
	if (o_mem[0] > 0 ) {
		if (o_mem_avg == 1) {
			for (i=0; i< num_proc; i++) { res_memory += (proc_state[i].mem_used==-1) ? 0 : proc_state[i].mem_used; }
			if (res_memory != -1) res_memory /= (num_proc_ok*1024);
			if (o_verb) printf(_("Memory average : %0.f\n"),res_memory); 
		} else {
			for (i=0; i< num_proc; i++) 
				if (proc_state[i].mem_used > res_memory) res_memory=proc_state[i].mem_used;
			
			if (res_memory != -1) res_memory /=1024;
			if (o_verb) printf(_("Memory max : %0.f\n"),res_memory);
		}
		if (res_memory == -1) {
			final_status = max_state (final_status, N_UNKNOWN);
			sprintf(mem_buffer,_(", Mem UNKNOWN"));
		} else {
			/* output performance data */
			if (o_perf==1) p_output_int("memory_usage",(long int) res_memory,"MB","",
				1, (long int) o_mem[0],
				1, (long int) o_mem[1],
				0,0,0,0);
			if (o_mem[1]!= -1 && res_memory > o_mem[1]) {
				final_status = max_state (final_status, N_CRITICAL);
				sprintf(mem_buffer,_(", Mem : %.1fMB > %i CRITICAL"),res_memory,o_mem[1]);
			} else if ( res_memory > o_mem[0]) {
				final_status = max_state (final_status, N_WARNING);
				sprintf(mem_buffer,_(", Mem %.1fMB > %i WARNING"),res_memory,o_mem[0]);
			} else {
				sprintf(mem_buffer,_(", Mem %.1fMB OK"),res_memory);
			}
		}
	}

	/*************************************************************************
	Checks cpu usage 
	**************************************************************************/

	if (o_cpu[0] > 0) {
		timenow=time(NULL);
		trigger=timenow - (int) (o_delta * 0.9);
		trigger_low=timenow - 3*o_delta;
		strcat(base_file_name,"/");
		strcat(base_file_name,TMP_FILE_NAME_PREFIX);
		found_data=0;
		found_value=0;
		res_cpu=0;
		
		/* Get the current values */
		for (i=0; i< num_proc; i++) { res_cpu += (proc_state[i].cpu_count==-1) ? 0 : proc_state[i].cpu_count; } 
		if (o_verb) printf(_("Time: %lu , cpu (centiseconds) : %i\n"),timenow,res_cpu);
		
		if (res_cpu==0) { /* no data found */
			final_status = max_state (final_status, N_UNKNOWN);
			sprintf(mem_buffer,_(", CPU UNKNOWN"));
		} else {
			/* Read file */
			/* copy to get full file name : BASE_DIR/TMP_FILE_NAME_PREFIX<host/ip>.<description> */		
			strncpy(file_name,base_file_name,MAX_FILE_LENGTH);
			strncat(base_file_name,o_host,MAX_FILE_LENGTH-strlen(file_name)-20); /* put host name (leave at least 20 cars for name) */	
			strcat(base_file_name,".");
			strncpy(file_name,o_descr,MAX_FILE_LENGTH-strlen(base_file_name)-1); /* put process name */
			if (strlen(o_descr)>(MAX_FILE_LENGTH-strlen(base_file_name)-1)) file_name[MAX_FILE_LENGTH-strlen(base_file_name)-1]=0;
			convert_text(file_name); /* get rid of special caracters */
			strncat(base_file_name,file_name,MAX_FILE_LENGTH-strlen(base_file_name)-1);
			/* read file */
			items=2;
			file_ret=read_file( base_file_name, &rows, items, file_values);
			if (file_ret!=0) rows=0;
			if (o_verb) printf(_("File read returns : %i with %i rows\n"),file_ret,rows);
			
			/* make the checks if the file is OK   */
			if (file_ret ==0) {
				j=rows-1;
				do {
					if ((file_values[j][0] < trigger) && (file_values[j][0] > trigger_low)) {
						/*  found value = centiseconds / seconds = %cpu */
						found_value= (res_cpu-file_values[j][1]) / (timenow - file_values[j][0] );
						if (found_value <0){
							j=1;found_data=0; /* don't look for more values */
							rows=0; /* reset file  */						
						} else 
							found_data=1;
					}				
					j--;
				} while ( (j>=0) && (found_data==0) );;
			}

			/* ##### Write file */
			file_values[rows][0]=timenow;
			file_values[rows][1]=res_cpu;
			rows++;
			file_ret=write_file( base_file_name, rows, MAX_ROWS, items, file_values);
			if (o_verb) printf(_("Write file returned : %i\n"),file_ret);	

			if (file_ret !=0) { /* On error writing, return Unknown status */
				final_status=max_state(final_status,N_UNKNOWN);
				sprintf(cpu_buffer,_(",CPU !!Unable to write file %s"),base_file_name);
			}	

			/* Check values (if something to check...) */
			if (found_data == 1) {
				if (o_perf==1) p_output_int("cpu_usage",(long int) found_value,"%","",
					1, (long int) o_cpu[0],
					1, (long int) o_cpu[1],
					0,0,0,0);			
				if (found_value > o_cpu[1]) {
					final_status = max_state (final_status, N_CRITICAL);
					sprintf(cpu_buffer,_(", Cpu %.0f%% > %i CRITICAL"),found_value,o_cpu[1]);
				} else if ( found_value > o_cpu[0]) {
					final_status = max_state (final_status, N_WARNING);
					sprintf(cpu_buffer,_(", Cpu %.0f%% > %i WARNING"),found_value,o_cpu[0]);
				} else {
					sprintf(cpu_buffer,_(", Cpu %.0f%% OK"),found_value);
				}
			} else if (file_ret == 0) {
				final_status = max_state (final_status, N_UNKNOWN);
				printf ("final:%i\n",N_UNKNOWN);
				sprintf(cpu_buffer,_(", No data for CPU (%i) rows : UNKNOWN"),rows);
			}
		}
	}	

	sprintf(buffer,"%i process %s %s",num_proc_ok, (o_noreg==1) ? "named" : "matching", o_descr);
	if (o_perf==1) p_output_int("num_process",(long int) num_proc_ok,"","",
		1, (long int) o_warn[0],
		1, (long int) o_crit[0],
		0,0,0,0);			
	strcat_output(buffer);
	/* Check for min and max number of process */
	if ( num_proc_ok <= o_crit[0] ) {
		sprintf(buffer,"(<= %i : CRITICAL",o_crit[0]);
		final_status = max_state (final_status, N_CRITICAL);
	} else if ( num_proc_ok <= o_warn[0] ) {
		sprintf(buffer,"(<= %i : WARNING",o_warn[0]);
		final_status = max_state (final_status, N_WARNING);
	} else {
		sprintf(buffer,"(> %i",o_warn[0]);
	}
	strcat_output(buffer);
	if (o_crit[1]!=-1 && (num_proc_ok > o_crit[1])) {
		sprintf(buffer,", > %i : CRITICAL)",o_crit[1]);
		final_status = max_state (final_status, N_CRITICAL);
	} else if (o_warn[1]!=-1 && (num_proc_ok > o_warn[1])) {
		sprintf(buffer,", > %i : WARNING)",o_warn[1]);
		final_status = max_state (final_status, N_WARNING);
	} else if (o_warn[1]!=-1) {
		sprintf(buffer,", <= %i)",o_warn[1]);
	} else sprintf(buffer,")");
	
	strcat_output(buffer);
	strcat_output(mem_buffer);
	strcat_output(cpu_buffer);

	if (final_status==N_CRITICAL) {
		print_output_with_status(N_CRITICAL);
		return N_CRITICAL;
	}
	if (final_status==N_WARNING) {
		print_output_with_status(N_WARNING);
		return N_WARNING;
	}
	if (final_status==N_UNKNOWN) {
		print_output_with_status(N_UNKNOWN);
		return N_UNKNOWN;
	}
	
	print_output_with_status(N_OK);
	return N_OK;
}
