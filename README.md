# Awesome-Monitoring [![Awesome](https://cdn.rawgit.com/sindresorhus/awesome/d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg)](https://github.com/sindresorhus/awesome)

> A curated list of awesome resources for monitoring tools.

* [awesome](https://github.com/sindresorhus/awesome)
* [awesome-awesomeness](https://github.com/bayandin/awesome-awesomeness)

***

## Contents

* [Monitoring](#server-monitoring)
    * [Nagios](#nagios)
    * [Naemon](#naemon)
    * [Icinga](#icinga)
    * [Shinken](#shinken)
    * [Centreon](#centreon)
    * [Opsview](#opsview)
* [Yet Another Nagios](#yet-another-nagios)
    * [Check_mk](#check_mk)
    * [Sensu](#sensu)
    * [cabot](#cabot)
    * [Server Density](#server-density)
    * [PagerDuty](#pagerduty)
* [Monitroing Plugins](#monitoring-plugins)
* [Monitoring Addons](#monitoring-addons)
* [Ganglia](#ganglia)
* [Zabbix](#zabbix)
* [Zenoss](#zenoss)
* [Bosun](#bosun)
* [Opserver](#opserver)
* [Netdata](#netdata)
* [Application Monitoring](#application-monitoring)
    * [NewRelic](#newrelic)
    * [AppDynamics](#appdynamics)
    * [javamelody](#javamelody)
    * [kamon](#kamon)
    * [SPM](#spm)
    * [OverOps](#overops)
* [Website Monitoring](#website-monitoring)
    * [pingdom](#pingdom)
* [Contributing](#contributing)
* [License](#license)

***

## Monitoring

* [Monitoring](http://crazy-canux.github.io/2016/05/18/Monitoring/) - Blog.

### Nagios

* [Nagios](https://www.nagios.org/) - Nagios official website.
* [Nagios enterprise](https://www.nagios.com/) - Nagios enterprise website.
* [Nagios exchange](https://exchange.nagios.org/) - Nagios official exchange website.
* [Nagios core github](https://github.com/NagiosEnterprises/nagioscore) - Nagios core written in C.
* [Nagios chinese](https://sourceforge.net/projects/nagios-cn/files/?source=navbar) - Nagios chinese document.
* [Nagios](http://crazy-canux.github.io/2016/03/24/Monitoring%E4%B9%8BNagios/) - Blog.

### Naemon

* [Naemon](http://www.naemon.org/) - Naemon official website.
* [Naemon core github](https://github.com/naemon/naemon-core) - Next generation of nagios core written in C.

### Icinga

* [Icinga](https://www.icinga.org/) - Icinga official website.
* [Icinga exchange](https://exchange.icinga.org/) - Icinga2 official exchange website.
* [Icinga2 core github](https://github.com/Icinga/icinga2) - Icinga2 core written in C++.
* [Icinga chinese](https://sourceforge.net/projects/icinga-cn/files/) - Icinga2 chinese document.

### Shinken

* [Shinken](http://www.shinken-monitoring.org/) - Shinken official website.
* [Shinken core github](https://github.com/naparuba/shinken) - Shinken core written in Python.
* [Shinken exchange github](https://github.com/shinken-monitoring) - Shinken exchange.

### Centreon

* [Centreon](https://www.centreon.com) - Shinken offcial website.
* [Centreon core github](https://github.com/centreon/centreon-engine) - Centreon core written in C++.
* [Centreon plugins github](https://github.com/centreon/centreon-plugins) - Centreon plugins.

### Opsview

* [Opsview](https://www.opsview.com/) - Opsview official website.
* [Opsview core](https://www.openhub.net/p/opsview) - Opsview core written in Perl.

***

## Yet Another Nagios

Lots of monitoring tools are based on nagios.

### check_mk

* [Check_mk](http://mathias-kettner.de/check_mk.html) - Check_mk official website.
* [check_mk exchange](http://mathias-kettner.com/check_mk_exchange.php?HTML=yes) - Check_mk plugins.
* [check_mk core git](http://git.mathias-kettner.de/git/?p=check_mk.git;a=tree) - Check_mk core written in Python.
* [Check_mk](http://crazy-canux.github.io/2016/05/11/Monitoring%E4%B9%8Bcheck_MK/) - Blog.

### Sensu

* [Sensu](https://sensuapp.org/) - Monitoring for today's infrastructure.
* [Sensu github](https://github.com/sensu/sensu) - Sensu core written in Ruby.
* [Sensu-plugins](http://sensu-plugins.io/) - Sensu plugins.
* [Sensu-plugins github](https://github.com/sensu-plugins) - Sensu plugins source code.

### Cabot

* [arachnys](https://www.arachnys.com/)
* [cabot github](https://github.com/arachnys/cabot) - Self-hosted, easily-deployable monitoring and alerts service.

### Server Density

* [server density](https://www.serverdensity.com/) - Server monitoring.
* [server density github](https://github.com/serverdensity) - Related source code.

### PagerDuty

* [pagerduty](https://www.pagerduty.com/) - Digital Operations Management.
* [pagerduty github](https://github.com/PagerDuty) - Related source code.

***

## Plugins

* [Plugins](http://crazy-canux.github.io/2016/04/08/Monitoring%E4%B9%8BPlugins/) - Blog.

### Monitoring Plugins

> The Monitoring Plugins Development Team, maintain a bundle of more than fifty standard plugins for Icinga, Naemon, Nagios, Shinken, Sensu, Centreon and Opsview.

* [monitoring-plugins](https://www.monitoring-plugins.org/) - Monitoring plugins official website.
* [monitoring-plugins github](https://github.com/monitoring-plugins) - Monitoring plugins source code.

### Develop Plugins

> How to develop plugins for your own service.

* [monitoring plugin development guidelines](https://www.monitoring-plugins.org/doc/guidelines.html) - Plugins development document official website.

### Plugins API

* [monitoring-plugins perl](https://github.com/monitoring-plugins/monitoring-plugin-perl) - Official Perl module for plugins.
* [pymonitoringplugins](https://github.com/crazy-canux/pymonitoringplugins) - A set of Python API for write your monitoring plugins.
* [pymonitoringplugins docs](http://pymonitoringplugins.readthedocs.io/en/latest/) - Document for pymonitoringplugins.

### Monitoring Windows server

* [check_wmi_plus.pl](http://www.edcint.co.nz/checkwmiplus/) - Plugin for windows monitoring use WMI.

### Monitoring OS X

* [OSX-Monitoring-Tools](https://github.com/jedda/OSX-Monitoring-Tools) - Plugins used to monitoring Mac system.

### Monitoring Linux/Unix

* [check_fujitsu_health.pl](https://github.com/lausser/check_fujitsu_health) - For fujitsu.
* [check_hpasm.pl](https://github.com/lausser/check_hpasm) - For Hpasm.

### Monitoring Network

* [check_nwc_health.pl](https://github.com/lausser/check_nwc_health) - For nwc.

### Monitoring Storage

* [check_tl_health.pl](https://github.com/lausser/check_tl_health) - For tl.
* [check_raid.pl](https://github.com/glensc/nagios-plugin-check_raid) - For raid.
* [check_sstcam.pl](https://github.com/lausser/check_sstcam) - For sstcam.

### Monitoring Vitual Machine

* [check_wmware_api.pl](https://github.com/op5/check_vmware_api) - For vmware.
* [check_vmware_esx.pl](https://github.com/BaldMansMojo/check_vmware_esx) - For vmware.

### Monitoring Database

* [check_mysql_health.pl](https://github.com/lausser/check_mysql_health) - For mysql.
* [check_oracle_health.pl](https://github.com/lausser/check_oracle_health) - For oracle.
* [check_db2_health.pl](https://github.com/lausser/check_db2_health) - For db2.
* [check_mssql_health.pl](https://github.com/lausser/check_mssql_health) - For mssql.
* [check_mongodb.py](https://github.com/mzupan/nagios-plugin-mongodb) - For mongodb.
* [check_postgres.pl](https://github.com/bucardo/check_postgres) - For postgres.

### Monitoring Log

* [check_logfiles.pl](https://github.com/lausser/check_logfiles) - For logfile.
* [check_events.pl](https://github.com/pewo/nagios-plugins/tree/master/check_events) - For events.

### Monitoring Application

* [check_sap_health.pl](https://github.com/lausser/check_sap_health) - For sap.
* [check_mailbox_health.pl](https://github.com/lausser/check_mailbox_health) - For mailbox.
* [check_elasticsearch.py](https://github.com/anchor/nagios-plugin-elasticsearch) - For ES.
* [check_graphite_data.py](https://github.com/etsy/nagios_tools) - For graphite.
* [nagios-plugins-rabbitmq](https://github.com/nagios-plugins-rabbitmq/nagios-plugins-rabbitmq) - For rabbitmq.
* [Celery-Nagios-Plugin](https://github.com/wilhelm-murdoch/Celery-Nagios-Plugin) - For celery.

### Misc

* [zplugin](https://github.com/crazy-canux/zplugin) - Tons of plugins monitoring your business.
* [nagios snmp plugins](http://nagios.manubulon.com/) - For snmp.
* [nagios snmp plugins SF](https://sourceforge.net/projects/nagios-snmp/) - For snmp.
* [nagios plugins](https://github.com/HariSekhon/nagios-plugins) - Misc.
* [nagios plugins](https://github.com/pewo/nagios-plugins) - Misc.

***

## Addons

* [Addons](http://crazy-canux.github.io/2016/06/07/Monitoring%E4%B9%8BAddons/) - Blog

### Monitoring Addons

> This is the addon can be used for Nagios, Naemon, Icinga, Shinken, Centreon and Opsview.

* [nagios-addons github](https://github.com/NagiosEnterprises) - Nagios addons.
* [naemon-addons github](https://github.com/naemon) - Naemon addons.
* [icinga-addons github](https://github.com/Icinga) - Icinga addons.
* [shinken-addons github](https://github.com/shinken-monitoring) - Shinken addons.

### Agent

* [NRPE github](https://github.com/NagiosEnterprises/nrpe) - Nagios Remote Plugin Executor for Linux.
* [NRPE_NT SF](https://sourceforge.net/projects/nrpent/) - Nagios Remote Plugin Executor for Windows.
* [NSCP](http://nsclient.org/) - NSClient++.
* [NSCP github](https://github.com/mickem/nscp) - NSCP source code.
* [NCPA github](https://github.com/NagiosEnterprises/ncpa) - Nagios Cross Platform Agent.

### Event broker

* [NDOUtils github](https://github.com/NagiosEnterprises/ndoutils) - Nagios Data Output Utils, for Nagios.
* [IDOUtils github](https://github.com/Icinga) - Icinga Data Output Utils, for Icinga.
* [naemon-livestatus github](https://github.com/naemon/naemon-livestatus) - For Naemon.
* [mod-livestaus github](https://github.com/shinken-monitoring/mod-livestatus) - For Shinken.
* [MK Livestatus](http://mathias-kettner.com/checkmk_livestatus.html) - For nagios/check_mk.
* [MK Livestatus git](http://git.mathias-kettner.de/git/?p=check_mk.git;a=tree;f=livestatus;hb=HEAD) - Livesttus source code.

### Distributions

* [MK OMD](http://omdistro.org/) - The Open Monitoring Distribution, for Nagios/Naemon/Icinga/Shinken/check_mk.
* [MK OMD git](http://git.mathias-kettner.de/git/?p=omd.git;a=tree) - OMD source code.
* [NSCA github](https://github.com/NagiosEnterprises/nsca) - Nagios Service Check Acceptor, for nagios/naemon/icinga.
* [NSCA-ng](http://www.nsca-ng.org/) - Next Generation of NSCA.
* [NSCA-ng github](https://github.com/weiss/nsca-ng) - NSCA-ng source code.
* [NRDP github](https://github.com/NagiosEnterprises/nrdp) - Nagios Remote Data Processor, as a replacement for NSCA.
* [DNX](http://dnx.sourceforge.net/) - Distributed Nagios eXecutor, for Nagios/Naemon.
* [DNX github](https://github.com/DNX-Project/DNX) - DNX source code.
* [gearman](http://gearman.org/) - Gearman official website.
* [gearman github](https://github.com/gearman/gearmand) - Gearmand source code. A gearman job server written in C++.
* [consol mod-gearman](http://www.mod-gearman.org/) - Gearman worker for Nagios/Naemon.
* [consol mod-gearman github](https://github.com/sni/mod_gearman) - Mod-gearman source code. A gearman wroker written in C.
* [op5 merlin](https://kb.op5.com/display/MERLIN/Distributed+%28Merlin%29+Home) - For Nagios/Naemon.
* [op5 merlin github](https://github.com/op5/merlin) - Merlin source code.

### Visualization

* [op5 Ninja](https://kb.op5.com/display/GUI/GUI+%28Ninja%29+Home) - For Nagios/Naemon.
* [op5 Ninja github](https://github.com/op5/ninja) - Ninja source code.
* [mod-webui github](https://github.com/shinken-monitoring/mod-webui) - For Shinken.
* [icingaweb2 github](https://github.com/Icinga/icingaweb2) - For Icinga.
* [consol thruk](http://www.thruk.org/) - Webinterface for Nagios/Naemon/Icinga/Shinken.
* [consol thruk github](https://github.com/sni/Thruk) - Thruk source code.
* [nagdash github](https://github.com/lozzd/Nagdash) - Dashboard interface for nagios.
* [nagstamon](https://nagstamon.ifw-dresden.de/) - Nagios status viewer on windows.
* [nagstamon github](https://github.com/HenriWahl/Nagstamon) - Nagstamon source code.
* [coffeesaint](https://www.vanheusden.com/java/CoffeeSaint/) - Java based nagios status viewer.
* [coffeesaint github](https://github.com/flok99/CoffeeSaint) - Coffeesaint source code.
* [nagmap github](https://github.com/hecko/nagmap/) - Integration of nagios/icinga into google maps.
* [nagvis](http://www.nagvis.org/) - Draw map from event brokers.
* [nagvis github](https://github.com/NagVis/nagvis) - Nagvis source code.
* [realopinsight](http://realopinsight.com/) - Realopinsight.
* [realopinsight github](https://github.com/RealOpInsightLabs/realopinsight-workstation) - Realopinsight source code.

### Metric Storage

* [rrdtool](http://oss.oetiker.ch/rrdtool/) - Round Robin Database Tool, store perfomance data.
* [rrdtool github](https://github.com/oetiker/rrdtool-1.x) - Rrdtool source code.
* [graphite-whisper github](https://github.com/graphite-project/whisper) - Graphite whisper.
* [graphite-ceres github](https://github.com/graphite-project/ceres) - Graphite ceres to replace whisper.
* [influxdata](https://influxdata.com) - Influxdata.
* [influxdata github](https://github.com/influxdata/influxdb) - Influxdata source code.
* [OpenTSDB](http://opentsdb.net/) - OpenTSDB.
* [OpenTSDB github](https://github.com/OpenTSDB/opentsdb) - OpenTSDB source code.
* [kairosDB](http://kairosdb.github.io/) - KairosDB.
* [kairosDB github](https://github.com/kairosdb/kairosdb) - KairosDB source code.
* [Kenshin github](https://github.com/douban/Kenshin) - A time-series database alternative to Graphite Whisper with 40x improvement in IOPS.

### Metric Forwarding

* [graphios github](https://github.com/shawn-sterling/graphios) - A program to send nagios perf data to graphite(carbon)/statsd/librato/influxdb.
* [ledbetter github](https://github.com/github/ledbetter) - Script that scrapes alert statistics from Nagios and reports them to Graphite.

### Graphing

* [nagiosgraph](http://nagiosgraph.sourceforge.net/) - Create graphs and store data in rrdtool.
* [nagiosgraph SF](https://sourceforge.net/projects/nagiosgraph/) - Nagiosgraph source code.
* [pnp4nagios](http://docs.pnp4nagios.org/) - Create graphs and store rrdtool.
* [pnp4nagios github](https://github.com/lingej/pnp4nagios) - Php4nagios source code.
* [Graphite](http://graphite.readthedocs.io/en/latest/) - Graphite.
* [Graphite carbon github ](https://github.com/graphite-project/carbon) - Graphite carbon.
* [Graphite web github](https://github.com/graphite-project/graphite-web) - Graphite web.
* [grafana](http://grafana.org/) - Grafana.
* [grafana github](https://github.com/grafana/grafana) - Grafana source code.

### Configuration

* [nagios.vim](https://github.com/crazy-canux/nagios.vim) - Vim plugin for Nagios.
* [icinga2.vim](https://github.com/crazy-canux/icinga2.vim) - Vim plugin for Icinga2.
* [nconf](http://www.nconf.org/dokuwiki/doku.php) - Enterprise Nagios configurator.
* [nconf github](https://github.com/nconf/nconf) - Nconf source code.
* [adagios](http://adagios.org/) - Web based nagios configuration.
* [adagios github](https://github.com/opinkerfi/adagios) - Adagios source code.

### Procedure

* [dokuwiki](https://www.dokuwiki.org/dokuwiki/) - Dokuwiki.
* [dokuwiki github](https://github.com/splitbrain/dokuwiki) - Dokuwiki source code.

### Business Process Intelligence

* [nagiosBPI github](https://github.com/NagiosEnterprises/nagiosbpi) - NagiosBPI.
* [BP](http://bp-addon.monitoringexchange.org/) - BP.
* [BP github](https://github.com/booboo-at-gluga-de/bp-addon) - BP source code.

### SNMP

* [NSTI github](https://github.com/NagiosEnterprises/nsti) - NSTI.
* [nagios-mib github](https://github.com/nagios-plugins/nagios-mib) - This is the initial set of MIBs for Nagios to allow traps sent from Nagios to be recognized by other NMSes.

### Addons API

* [nagiosvshell github](https://github.com/NagiosEnterprises/nagiosvshell) - Nagios v-shell PHP interface.
* [pynag](http://pynag.org/) - API for managin nagios configuration and write plugins.
* [pynag github](https://github.com/pynag/pynag) - Pynag source code.
* [nagios-api github](https://github.com/zorkian/nagios-api) - A REST-like, JSON interface to nagios.
* [nagiosharder github](https://github.com/railsmachine/nagiosharder) - Ruby API for scraping nagios.
* [nagrestconf](http://nagrestconf.smorg.co.uk/) - A REST interface and configuration GUI for nagios.
* [nagrestconf github](https://github.com/mclarkson/nagrestconf) - Nagrestconf.

### Misc

* [chef-nagios-cookbook github](https://github.com/schubergphilis/nagios) - Chef nagios cookbook.
* [cucumber](http://cucumber-nagios.org/) - Cucumber.
* [cucumber github](https://github.com/auxesis/cucumber-nagios) - Cucumber source code.
* [nagios-herald](https://codeascraft.com/2014/06/06/introducing-nagios-herald/) - Customizing nagios alerts.
* [nagios-herald github](https://github.com/etsy/nagios-herald) - Nagios-herald source code.

***

## Ganglia

> Ganglia is a scalable distributed monitoring system for high-performance computing systems such as clusters and Grids.

* [Ganglia](http://ganglia.info/) - Ganglia is a scalable distributed monitoring system for high-performance computing systems.
* [Ganglia core github](https://github.com/ganglia/monitor-core) - Ganglia written in C.
* [Ganglia github](https://github.com/ganglia) - Related source code.

***

## Zabbix

> Zabbix is the ultimate enterprise-level software designed for real-time monitoring of millions of metrics collected from tens of thousands of servers, virtual machines and network devices. Zabbix is Open Source and comes at no cost.

* [Zabbix](https://www.zabbix.org) - The Ultimate Enterprise-class Monitoring Platform.
* [Zabbix enterprise](http://www.zabbix.com/) - Zabbix enterprise website.
* [Zabbix core](https://zabbix.org/wiki/Get_Zabbix) - Zabbix core written in C.
* [Zabbix github](https://github.com/zabbix) - Zabbix github.

***

## Zenoss

> Zenoss works with the world's largest companies to ensure their IT services and applications are always on. As the global leader in hybrid IT monitoring and analytics software, Zenoss provides complete visibility for cloud, virtual and physical IT environments.

* [Zenoss](https://www.zenoss.org/) - The Hybrid IT Monitoring Platform.
* [Zenoss enterprise](https://www.zenoss.com/) - Zenoss enterprise website.
* [Zenoss core sf](https://sourceforge.net/projects/zenoss/) - Zenoss core written in C.
* [Zenoss github](https://github.com/zenoss) - Zenoss github.

***

## Opserver

* [Opserver](http://opserver.org/) - Stack Exchange's monitoring system.
* [Opserver github](https://github.com/opserver/Opserver) - Opserver written in C#.

## Bosun

> Bosun is an open-source, MIT licensed, monitoring and alerting system by Stack Exchange. It has an expressive domain specific language for evaluating alerts and creating detailed notifications. It also lets you test your alerts against history for a faster development experience.

* [Bosun](http://bosun.org/) - Time Series Alerting Framework.
* [Bosun github](https://github.com/bosun-monitor) - Bosun written in Go.

***

## Netdata

> netdata is a system for distributed real-time performance and health monitoring. It provides unparalleled insights, in real-time, of everything happening on the system it runs (including applications such as web and database servers), using modern interactive web dashboards.

* [netdata](http://my-netdata.io/) - Get control of your servers. Simple. Effective. Awesome.
* [netdata github](https://github.com/firehol/netdata) - Netdata written in C.

***

## Application Monitoring

Monitoring the Application.

### NewRelic

* [new relic](https://newrelic.com/) - Performance management system.
* [new relic github](https://github.com/newrelic) - New relic written in Ruby.

### AppDynamics

* [appdynamics](https://www.appdynamics.com/) - Business and application performance monitoring.
* [appdynamics github](https://github.com/Appdynamics) - Related source code.

### javamelody

* [javamelody](https://github.com/javamelody/javamelody/wiki) - Monitoring of JavaEE applications.
* [javamelody github](https://github.com/javamelody/javamelody) - The source code.

### Kamon

* [kamon](http://www.kamon.io/) - Monitoring applications running on the JVM.
* [kamon github](https://github.com/kamon-io/Kamon) - The source code.

### SPM

* [SPM](https://sematext.com/spm/) - solutions for performance monitoring.
* [SPM github](https://github.com/sematext) - Related source code.

### OverOps

* [OverOps](https://www.overops.com/) - Know when and why code breaks in production.
* [OverOps github](https://github.com/overops) - Related source code.

***

## Website monitoring

Monitoring the Website.

### pingdom

* [pingdom](https://www.pingdom.com/) - Website monitoring for everyone.
* [pingdom github](https://github.com/Pingdom) - Related source code.

***

## Contributing

[Contribution guide](CONTRIBUTING.md)

***

## Authors

[Authors](AUTHORS.md)

***

## License

[![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png)](http://creativecommons.org/publicdomain/zero/1.0/)
