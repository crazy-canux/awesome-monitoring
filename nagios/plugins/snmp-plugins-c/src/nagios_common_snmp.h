#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <libintl.h>
#include <locale.h>
#include <time.h>
#include <unistd.h>

/* undef variables of net-snmp-config.h */
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_VERSION
#include "config.h"

#define NAGIOS_SNMP_VERSION 0.5

/********************** Nagios specific ****************************/
#define N_OK 0
#define N_WARNING 1
#define N_CRITICAL 2
#define N_UNKNOWN 3
#define N_DEPENDANT 4

#define TIMEOUT 15

/********************** snmp_table structure definition *****************/
#define SNMP_STRING 0
#define SNMP_INT 1
#define SNMP_OID 2
#define SNMP_OTHER 255
typedef struct snmp_table_struct snmp_table;
struct snmp_table_struct {
	snmp_table 		*next; /*  Next  value (NULL for end) */
	char* 			value; /*  string value */
	long			value_l; /*  integer_value */
	unsigned long long 	value_ull; /*  counter64 value (ULL) or any unsigned */
	oid				value_oid[MAX_OID_LEN]; /*  oid as a value */
	size_t   		value_oid_length;
	short			type;
	oid 			name[MAX_OID_LEN]; /*  oid of the value */
	size_t   		name_length;
};

/********************** Gettext variables and commands *********************************/
int init_plugins_gettext();
#define _(String) gettext(String)
#define PACKAGE "nagios-plugins-snmp"
/* define by configure script */
/*#define LOCALEDIR "/usr/local/src/nagios-dev/nagios-snmp-plugins/po"*/

/*********************************** Utilities ****************************************/
/* cache utilities */

char plugin_name[30]; /* plugin name */
char plugin_tmp_prefix[50];

/* defined in configure script */
/*#define BASE_CACHE "/tmp/cache"*/

//int read_cache(snmp_table **descT, long unsigned int ttl);
int read_cache(snmp_table **descT,int num, long unsigned int ttl);
//int write_cache(snmp_table *descT);
int write_cache(snmp_table **descT,int num);

#define ULLI unsigned long long int

/* maximum caracters in ouput */
#define MAX_OUTPUT 500
char output[MAX_OUTPUT];
int output_overflow;

/* maximum caracters for performance output label*/
#define MAX_LABEL_SIZE 50
char *p_output;

/* Put data in output */
void strcat_output(const char *string);
void strcat_output_notnull(const char *string);
/* put lont int values in performance data 
'label'=value[UOM];[warn];[crit];[min];[max]
*/
void p_output_int(char *label, long int val, const char *uom,const char*extension, int warnp, long int warn, int critp, long int crit, int minp, long int minv, int maxp, long int maxv);
void p_output_float(char *label, double val, const char *precision, const char * extension,char *uom, int warnp, double warn, int critp, double crit, int minp, double minv, int maxp, double maxv);
void p_output_ull(char *label, unsigned long long int val, const char *uom,const char*extension, int warnp, unsigned long long int warn, int critp, unsigned long long int crit, int minp, unsigned long long int minv, int maxp, unsigned long long int maxv);

void print_output();
int print_output_with_status(int status);

/* convert special chars in strings to "_" (OK : 0-9A-Za-z_.-, */
void convert_text(char *string); 

/* Replace all chars of the remove string by char replace in string string */
void subst_chars(char* string, const char* remove, char replace);
void remove_chars(char* string, const char* remove);

/* maximum number of rows in file */
#define MAX_ROWS 200
#define MAX_ITEMS 10
/* defined in configure script */
/*#define BASE_DIR "/tmp/"*/
/* max file length : must be > len(BASE_DIR)+30. */
#define MAX_FILE_LENGTH 150
int write_file( char* file_name, int rows, int max_rows, int items, unsigned long long int values[MAX_ROWS][MAX_ITEMS]);
int read_file( char* file_name, int *rows, int items, unsigned long long int values[MAX_ROWS][MAX_ITEMS]);

/* maw state : OK<UNKNOWN<WARNING<CRITICAL */
#define max(a,b) (((a)>(b))?(a):(b))
#define min(a,b) (((a)<(b))?(a):(b))
int max_state (int a, int b);


/****************************************************************************************
				Options structure && common values for all scripts 
****************************************************************************************/
typedef struct options_struct options_nagsnmp;
struct options_struct {
	int			type; 			/*  0 : no value, 1 : number, 2 : string, 4 : array, 8 : optionnal, i.e can be no options (13=optionnal array of int) */
	char		o_short; 		/*  short option label(0 for no short label) */
	char		*o_long; 		/*  long option label (NULL for no long label). */
	/* address of variable to return */
	int			present;		/*  0 if not present , 1 if present, 2 present without options */
	float		*ivalue; 		/*  returned float (type=1) */
	char		**cvalue; 		/*  returned string (type=2) */
	int			*intvalue;		/*  returned int (type=1) if *ivalue=NULL, set to 1 if type=0 & not NULL */
	int			n_array; 		/*  input : max number of elements, output : number of elements */
};

/* function to query options and put them in a options_nagsnmp structure */
int get_options (int narg, char* const valarg[],options_nagsnmp *liste, int n_list);

/* Common options variables */

#define SNMP_V1 1
#define SNMP_V2C 2
#define SNMP_V3_NOPRIV 3
#define SNMP_V3_PRIV 4

#define TIMEOUT_DEFAULT 3

/* standard options */
char *o_host;
int o_verb;
char* error; 
/* snmp options */
int o_port,o_timeout;
int o_help, o_verbose,o_version,o_v2c; 
int retries;
int o_snmpver,snmp_bulk;
char *o_community,*o_login,*o_passwd,*o_privpasswd;
char *o_protocols[2],*o_authproto,*o_privproto;
/* snmp sessions */
struct snmp_session session, *ss;

/* Common options definition and check */
#define common_usage "[-v -V] -H <host> -C <snmp_community> [-2] | (-l login -x passwd [-X pass -L <authp>,<privp>)  [-p <port>] [-t <timeout>]"
/*  { type,short	,o_long		,present,	ivalue, 		cvalue,			var_present	n_array} */
#define COMMON_OPTIONS \
{2,	'H',	"hostname",		0,	NULL,		&o_host,		NULL,		0}, \
{0,	'v',	"verbose",		0,	NULL,		NULL,			&o_verbose,	0}, \
{0,	'h',	"help",			0,	NULL,		NULL,			&o_help,	0}, \
{2,	'C',	"community",	0,	NULL,		&o_community,	NULL,		0}, \
{0,	'2',	"v2c",			0,	NULL,		NULL,			&o_v2c,		0}, \
{2,	'l',	"login",		0,	NULL,		&o_login,		NULL,		0}, \
{2,	'x',	"passwd",		0,	NULL,		&o_passwd,		NULL,		0}, \
{2,	'X',	"privpass",		0,	NULL,		&o_privpasswd,	NULL,		0}, \
{6,	'L',	"protocols",	0,	NULL,		o_protocols,	NULL,		2}, \
{0,	'V',	"version",		0,	NULL,		NULL,			&o_version,	0}, \
{1,	'p',	"port",			0,	NULL,		NULL,			&o_port,	0}, \
{1,	't',	"timeout",		0,	NULL,		NULL,			&o_timeout,	0},

#define COMOPTNUM 12

/* Put defaults in common options */
void init_options();
/* Check common options values */
int check_common_options();

/********************************* SNMP functions *************************************/
#define MAX_NUM_OID 1000
/* maximum number of OID requests per PDU */
#define MAX_NUM_OID_PDU 50
void copy_oid( oid* dst,size_t *dst_len, oid*src, size_t src_len);

/* copy OID and add index at the end */
void copy_oid_index( oid* dst,size_t *dst_len, oid*src, size_t src_len, int index);

/* get value in snmp table chain by OID : int / unsigned long long */
int get_value_int(oid* base_oid,size_t base_oid_len, int index_oid,snmp_table * val_table,int *returned_val);
int get_value_ulint(oid* base_oid,size_t base_oid_len, int index_oid,snmp_table * val_table,unsigned long *returned_val);
int get_value_long(oid* base_oid,size_t base_oid_len, int index_oid,snmp_table * val_table,unsigned long long *returned_val);

int init_snmp_session();

netsnmp_session * init_session_v1 (
	netsnmp_session * session,
	char* host, char* community,
	int port, long int timeout,
	int retries,
	char** error);
	
netsnmp_session * init_session_v2 (
	netsnmp_session * session,
	char* host, char* community,
	int port, long int timeout,
	int retries,
	char** error);

netsnmp_session * init_session_v3 (
        netsnmp_session * session,
        char* host,
        char* login, char* passwd, char* authproto, /*  Login/pass & proto */
        char* privpass, char* privproto,           /*  encryption key & proto */
        int priv,                                  /*  == 1 : use AuthPriv */
        int port, long int timeout,
        int retries,
        char** error);
	
snmp_table * get_snmp_oids (   
        struct snmp_session * session,  
        oid table_oid[200][MAX_OID_LEN], size_t table_oid_len[200],
        int num_oid,char** error);

snmp_table * get_snmp_table (
	struct snmp_session * session,
	oid * table_oid, size_t table_oid_len,
	int bulk_read,char** error);

/************ Default help print *****************/
#define DEFAULT_HELP_PRINT \
  printf("-v, --verbose\n");\
  printf(_("   print extra debugging information (including list of elements found on the system)\n"));\
  printf("-h, --help\n");\
  printf(_("   print this help message\n"));\
  printf("-H, --hostname=HOST\n");\
  printf(_("   name or IP address of host to check\n"));\
  printf("-C, --community=COMMUNITY NAME\n");\
  printf(_("   community name for the host's SNMP agent (implies v1 protocol)\n"));\
  printf("-l, --login=LOGIN ; -x, --passwd=PASSWD, -2, --v2c\n");\
  printf(_("   Login and auth password for snmpv3 authentication \n"));\
  printf(_("   If no priv password exists, implies AuthNoPriv \n"));\
  printf(_("   -2 : use snmp v2c\n"));\
  printf("-X, --privpass=PASSWD\n");\
  printf(_("   Priv password for snmpv3 (AuthPriv protocol)\n"));\
  printf("-L, --protocols=<authproto>,<privproto>\n");\
  printf(_("   <authproto> : Authentication protocol (md5|sha : default md5)\n"));\
  printf(_("   <privproto> : Priv protocole (des|aes : default des) \n"));\
  printf("-P, --port=PORT\n");\
  printf(_("   SNMP port (Default 161)\n"));\
  printf("-t, --timeout=INTEGER\n");\
  printf(_("   timeout for SNMP in seconds (Default: 5)   \n"));\
  printf("-V, --version\n");\
  printf(_("   prints version number\n"));

	
/*
    struct counter64 {
        u_long          high;
        u_long          low;
    };
typedef union {
   long           *integer;
   u_char         *string;
   oid            *objid;
   u_char         *bitstring;
   struct counter64 *counter64;
} netsnmp_vardata;

#define SNMP_VERSION_1     0
#define SNMP_VERSION_2c    1
#define SNMP_VERSION_3     3

#define ASN_BOOLEAN         ((u_char)0x01)
#define ASN_INTEGER         ((u_char)0x02)
#define ASN_BIT_STR         ((u_char)0x03)
#define ASN_OCTET_STR       ((u_char)0x04)
#define ASN_NULL            ((u_char)0x05)
#define ASN_OBJECT_ID       ((u_char)0x06)
#define ASN_SEQUENCE        ((u_char)0x10)
#define ASN_SET             ((u_char)0x11)

#define ASN_UNIVERSAL       ((u_char)0x00)
#define ASN_APPLICATION     ((u_char)0x40)
#define ASN_CONTEXT         ((u_char)0x80)
#define ASN_PRIVATE         ((u_char)0xC0)

#define ASN_PRIMITIVE       ((u_char)0x00)
#define ASN_CONSTRUCTOR     ((u_char)0x20)

#define ASN_LONG_LEN        (0x80)
#define ASN_EXTENSION_ID    (0x1F)
#define ASN_BIT8            (0x80)

#define ASN_IPADDRESS   (ASN_APPLICATION | 0)
#define ASN_COUNTER     (ASN_APPLICATION | 1)
#define ASN_GAUGE       (ASN_APPLICATION | 2)
#define ASN_UNSIGNED    (ASN_APPLICATION | 2)    RFC 1902 - same as GAUGE 
#define ASN_TIMETICKS   (ASN_APPLICATION | 3)
#define ASN_OPAQUE      (ASN_APPLICATION | 4)  changed so no conflict with other includes 

  
     * defined types (from the SMI, RFC 1442)
     
#define ASN_NSAP        (ASN_APPLICATION | 5)  historic - don't use 
#define ASN_COUNTER64   (ASN_APPLICATION | 6)
#define ASN_UINTEGER    (ASN_APPLICATION | 7)   historic - don't use 

	
*/


