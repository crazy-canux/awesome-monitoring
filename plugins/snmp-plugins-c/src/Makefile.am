datadir = @datadir@
localedir = $(datadir)/locale
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

libexec_PROGRAMS = check_snmp_int check_snmp_process check_snmp_storage
check_snmp_int_SOURCES = check_snmp_int.c nagios_common_snmp.c
check_snmp_process_SOURCES = check_snmp_process.c nagios_common_snmp.c
check_snmp_storage_SOURCES = check_snmp_storage.c nagios_common_snmp.c



