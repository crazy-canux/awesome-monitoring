#!/bin/bash
############################## install.sh  #####################
# Version : 1.4
# Date :  Jan 13 2007
# Author  : Patrick Proy ( nagios at proy.org)
# Help : http://www.manubulon.com/nagios/
# Licence : GPL - http://www.fsf.org/licenses/gpl.txt
# TODO : 
# Contribs : 
#################################################################
#
# USAGE : ./install [<perl script name> | AUTO <dir> <tmp_dir> <perl_dir> [<install_location>] ]
# USAGE : by default all scripts will be installed
#
# REQUIREMENTS : /bin/bash and sed 
#
# This script will : 
# - Check perl binary (and asks for path)
# - Ask for nagios plugin path and checks for file "utils.pm" in it (default /usr/local/nagios/libexec)
# - Ask for temporary file location (default /tmp)
# - Check Net::SNMP version 
# - Install plugins in the plugins directory and modify paths if necessary.

############################ script list
PLUGINS="check_snmp_boostedge.pl check_snmp_css.pl check_snmp_linkproof_nhr.pl check_snmp_nsbox.pl check_snmp_vrrp.pl check_snmp_cpfw.pl check_snmp_env.pl check_snmp_load.pl check_snmp_process.pl check_snmp_win.pl check_snmp_css_main.pl check_snmp_int.pl check_snmp_mem.pl check_snmp_storage.pl"
############################ get script to install or install type

if [ $# -gt 0 ] ; then INSTSCRIPT=$1 ; else INSTSCRIPT="all" ; fi

if [ $INSTSCRIPT != "AUTO" ] ; then 
    ############################ Manual installation
    echo 
    echo "###### Nagios snmp scripts installer ######"
    echo 
    echo "Will install $INSTSCRIPT script(s)"
    echo

    ############################ default values

    SRCDIR=$PWD
    PERLHOME=`which perl 2>&1`
    if [ $? -ne 0 ]; then PERLHOME="" ; fi

    PLUGHOME=/usr/local/nagios/libexec
    TMPDATA=/tmp
    ############################ Checking Perl

    echo -n "What is your perl location ? [$PERLHOME] "
    read USERPERL
    if [ "ZZ$USERPERL" != "ZZ" ]; then  PERLHOME=$USERPERL ; fi

    if [ "z$PERLHOME" == "z" ]; then 
      echo "Can't find perl binary... exiting"
      echo "######### ERROR ########"
      exit 1
    fi

    NETSNMP=`$PERLHOME -e 'if (eval "require Net::SNMP") { print "Yes" ;}'`
    if [ $? -ne 0 ] ; then 
      echo "Error while checking Net::SNMP module"
      echo "######### ERROR ########"
      exit 1;  
    fi

    if [ "zz$NETSNMP" != "zzYes" ]; then
      echo "Module Net::SNMP not found!"
      echo "Install it with CPAN or manually : http://www.manubulon.com/nagios/faq.html#FAQ2"
      echo "######### ERROR ########"
      exit 1; 
    fi

    SNMPVER=`$PERLHOME -e 'require Net::SNMP;print Net::SNMP->VERSION'`
    echo "Net::SNMP module version is $SNMPVER [OK]"

    GETOPT=`$PERLHOME -e 'if (eval "require Getopt::Long") { print "Yes" ;}'`
    if [ "zz$GETOPT" != "zzYes" ]; then
      echo "Module Getopt::Long not found!"
      echo "Install it with CPAN or manually"
      echo "######### ERROR ########"
      exit 1; 
    fi
    echo "Module Getopt::Long found [OK]"

    ############################ Check nagios plugin directory and utils.pm

    echo
    echo "What is your nagios plugin location ? "
    echo -n "Note : file utils.pm must be present in it [$PLUGHOME] "
    read USERPLUG

    if [ "z$USERPLUG" != "z" ]; then PLUGHOME=$USERPLUG ; fi
    if [ ! -d $PLUGHOME ] ; then 
      echo "Directory $PLUGHOME does not exist !"
      echo "######### ERROR ########"
      exit 1
    fi
    if [ ! -f $PLUGHOME/utils.pm ] ; then 
      echo "File $PLUGHOME/utils.pm does not exist !"
      echo "Install it from nagios plugins"
      echo "######### ERROR ########"
      exit 1
    fi

    ############################ Asking for temp directory

    echo
    echo "Where do you want the plugins to put temporary data (only used by some plugins) ? "
    echo -n "Nagios user must be able to write files in it [$TMPDATA] "
    read USERTMP

    if [ "z$USERTMP" != "z" ]; then TMPDATA=$USERTMP ; fi

    if [ ! -d $TMPDATA ] ; then
      echo "Directory $TMPDATA does not exist !"
      echo "######### ERROR ########"
      exit 1
    fi

    ############################ Looks OK, copying with changes if necessary

    TRANS=""
    # Change '#!/usr/bin/perl -w' 
    if [ $PERLHOME != "/usr/bin/perl" ] ; then
      TRANS="-r -e s#/usr/bin/perl#$PERLHOME#"
    fi

    # Change 'use lib "/usr/local/nagios/libexec";'
    if [ $PLUGHOME != "/usr/local/nagios/libexec" ] ; then
      if [ "z$TRANS" == "z" ]; then TRANS="-r -e s#/usr/local/nagios/libexec#$PLUGHOME#"
      else TRANS="$TRANS -e s#/usr/local/nagios/libexec#$PLUGHOME#";fi
    fi

    # Change 'my $o_base_dir="/tmp/tmp_Nagios_'
    if [ $TMPDATA != "/tmp" ] ; then
      if [ "z$TRANS" == "z" ]; then TRANS="-r -e s#/tmp/tmp_Nagios#$TMPDATA/tmp_Nagios#"
      else TRANS="$TRANS -e s#/tmp/tmp_Nagios#$TMPDATA/tmp_Nagios#";fi
    fi

    ######################### script install

    echo
    echo "Will now install $INSTSCRIPT script(s) : "
    echo "in directory    : $PLUGHOME"
    echo "perl            : $PERLHOME"
    echo "temp directory  : $TMPDATA"
    echo 
    echo -n "OK ? [Y/n]"
    read INSTOK

    if [ "$INSTOK" == "n" ]; then 
      echo "Aborting....."
      echo "######### ERROR ########"
      exit 1
    fi

    ERROR=0

    if [ $INSTSCRIPT == "all" ] ; then 
      for i in $PLUGINS ; do
        echo 
    	if [ ! -f $i ] ; then 
    	  echo "Can't find source file $i : ##### ERROR #####"
    	else
    		echo -n "Installing $i : "
    		if [ "z$TRANS" == "z" ] ; then 
     	      cp $i $PLUGHOME/$i 2>&1
    		else
    		  sed $TRANS $i > $PLUGHOME/$i 2>&1
    		fi
    		if [ $? -ne 0 ] ; then 
    		   echo "##### ERROR #####";	   
    	       rm -f $PLUGHOME/$i  
    		   ERROR=1
    		else 
    		  echo "OK" 
    		  chmod 755 $PLUGHOME/$i 2>&1
    		fi
    	fi
      done
    else
        echo
        if [ ! -f $INSTSCRIPT ] ; then
    	  echo "Can't find source file $INSTSCRIPT : ##### ERROR #####"
    	else
    		echo -n "Installing $INSTSCRIPT : "
    		if [ "z$TRANS" == "z" ] ; then 
    		  cp $INSTSCRIPT >  $PLUGHOME/$INSTSCRIPT
    		else
    		  sed $TRANS $INSTSCRIPT > $PLUGHOME/$INSTSCRIPT 2>&1
    		fi
    		if [ $? -ne 0 ] ; then
    		  echo "##### ERROR #####";
    		  rm -f $PLUGHOME/$INSTSCRIPT
    		  ERROR=1
    		  exit 1;
    		else 
    		  echo "OK" 
    		  chmod 755 $PLUGHOME/$INSTSCRIPT 2>&1
    		fi		
    	fi
    fi

    echo
    if [ $ERROR -eq 1 ] ; then
      echo "Installation ended with errors. Check output above"
      exit 1;
    fi

    echo "Installation completed OK"
    echo "You can delete all the source files and directory"
    echo "Remember to look for informtation at http://www.manubulon.com/nagios/" 
    exit 0;
	
else
####################### Silent install with parameters ############
# PARAM AUTO <dir> <tmp_dir> <perl_dir> [<install_location>]
    if [ $# -ne 4 ] && [ $# -ne 5 ]  ; then exit 1; fi

    SRCDIR=$PWD
    PERLHOME=$4
    PLUGHOME=$2
    TMPDATA=$3
    INSTALLDIR=$5
      
    TRANS=""
    # Change '#!/usr/bin/perl -w' 
    if [ $PERLHOME != "/usr/bin/perl" ] ; then
      TRANS="-r -e s#/usr/bin/perl#$PERLHOME#"
    fi

    # Change 'use lib "/usr/local/nagios/libexec";'
    if [ $PLUGHOME != "/usr/local/nagios/libexec" ] ; then
      if [ "z$TRANS" == "z" ]; then TRANS="-r -e s#/usr/local/nagios/libexec#$PLUGHOME#"
      else TRANS="$TRANS -e s#/usr/local/nagios/libexec#$PLUGHOME#";fi
    fi

    # Change 'my $o_base_dir="/tmp/tmp_Nagios_'
    if [ $TMPDATA != "/tmp" ] ; then
      if [ "z$TRANS" == "z" ]; then TRANS="-r -e s#/tmp/tmp_Nagios#$TMPDATA/tmp_Nagios#"
      else TRANS="$TRANS -e s#/tmp/tmp_Nagios#$TMPDATA/tmp_Nagios#";fi
    fi

    ######################### script install
    ERROR=0
    if [ "z$INSTALLDIR" != "z" ] ; then
      PLUGHOME=$INSTALLDIR
    fi
    for i in $PLUGINS ; do
      if [ ! -f $i ] ; then 
	    ERROR=1
      else
    	if [ "z$TRANS" == "z" ] ; then 
     	    cp $i $PLUGHOME/$i 2>&1
    	else
     	    sed $TRANS $i > $PLUGHOME/$i 2>&1
    	fi
    	if [ $? -ne 0 ] ; then 
    	   rm -f $PLUGHOME/$i  
    	   ERROR=1
    	else 
    	  chmod 755 $PLUGHOME/$i 2>&1
     	  fi
    	fi
      done
    if [ $ERROR -eq 1 ] ; then
      exit 1;
    fi
    exit 0;
fi

