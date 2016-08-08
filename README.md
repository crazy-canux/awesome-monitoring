# Awesome-Monitoring [![Awesome](https://cdn.rawgit.com/sindresorhus/awesome/d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg)](https://github.com/sindresorhus/awesome)

> Collect monitoring tools which is the industry standard in IT infrastructure monitoring and have open source version.

* [The mother repository](https://github.com/sindresorhus/awesome)
* [What is an awesome list?](https://github.com/sindresorhus/awesome/blob/master/awesome.md)
* [How to creating a list?](https://github.com/sindresorhus/awesome/blob/master/create-list.md)
* [How to pull request?](https://github.com/sindresorhus/awesome/blob/master/pull_request_template.md)

***

## Table of Contents

* [Monitoring](#monitoring)
    * [Nagios](#nagios)
    * [Naemon](#naemon)
    * [Icinga](#icinga)
    * [Shinken](#shinken)
    * [Plugins](#pluings)
    * [Addons](#addons)
* [Check_mk](#check_mk)
* [Sensu](#sensu)
* [Centreon](#centreon)
* [Zabbix](#zabbix)
* [Ganglia](#ganglia)
* [Zenoss](#zenoss)
* [Opserver](#opserver)
* [Opsview](#opsview)
* [Contributing](#contributing)
* [License](#license)

***

## Monitoring

> All these are open source tools to monitor your servers、network devices、applications and services etc....

* [Monitoring-Blog](http://crazy-canux.github.io/2016/05/18/Monitoring/)

### Nagios

> Nagios and some other monitoring tools based on nagios.

* [Nagios-Blog](http://crazy-canux.github.io/2016/03/25/Monitoring%E4%B9%8BNagios/)
* [Nagios-official](https://www.nagios.com/)
* [Nagios-opensource](https://www.nagios.org/)
* [Nagioscore-github](https://github.com/NagiosEnterprises/nagioscore)
* [Nagios-cn](https://sourceforge.net/projects/nagios-cn/files/?source=navbar)
* [Nagios-exchange](https://exchange.nagios.org/)

### Naemon

* [Naemon-official](http://www.naemon.org/)
* [Naemoncore-github](https://github.com/naemon/naemon-core) - Next generation of nagioscore

### Icinga

* [Icinga-official](https://www.icinga.org/)
* [Icinga2-github](https://github.com/Icinga/icinga2) - Icinga2 core
* [Icinga-cn](https://sourceforge.net/projects/icinga-cn/files/)
* [Icinga-exchange](https://exchange.icinga.org/)

### Shinken

* [Shinken-official](http://www.shinken-monitoring.org/)
* [Shinken-github](https://github.com/naparuba/shinken) - Shinken core

***

### Plugins

> Collect the monitoring plugins on github which star more than 1 hundred. And some exchange plugins.

* [Plugins-Blog](http://crazy-canux.github.io/2016/04/08/Monitoring%E4%B9%8BPlugins/)

#### Monitoring Plugins

> The Monitoring Plugins Development Team, maintain a bundle of more than
fifty standard plugins for Icinga, Naemon, Nagios, Shinken, Sensu, and other
monitoring applications.

* [monitoring-plugins-official](https://www.monitoring-plugins.org/)
* [monitoring-plugins-github](https://github.com/monitoring-plugins)
* [nagios-plugins-official](http://nagios-plugins.org/) - Replaced by monitoring plugins
* [nagios-plugins-github](https://github.com/nagios-plugins) - Replaced by monitoring plugins

#### Develop Plugins

> How to develop plugins for your own service.

* [monitoring plugin development guidelines](https://www.monitoring-plugins.org/doc/guidelines.html)
* [nagios plugin development guidelines](https://nagios-plugins.org/doc/guidelines.html) - Replaced by monitoring plugin development guidelines

#### Monitoring Windows server

* [check_wmi_plus.pl](http://www.edcint.co.nz/checkwmiplus/)

#### Monitoring OS X

* [OSX-Monitoring-Tools](https://github.com/jedda/OSX-Monitoring-Tools)

#### Monitoring Linux/Unix

* [check_fujitsu_health.pl](https://github.com/lausser/check_fujitsu_health)
* [check_hpasm.pl](https://github.com/lausser/check_hpasm)

#### Monitoring Network

* [check_nwc_health.pl](https://github.com/lausser/check_nwc_health)

#### Monitoring Storage

* [check_tl_health.pl](https://github.com/lausser/check_tl_health)
* [check_raid.pl](https://github.com/glensc/nagios-plugin-check_raid)
* [check_sstcam.pl](https://github.com/lausser/check_sstcam)

#### Monitoring Vitual Machine

* [check_wmware_api.pl](https://github.com/op5/check_vmware_api)
* [check_vmware_esx.pl](https://github.com/BaldMansMojo/check_vmware_esx)
* [check_docker.go](https://github.com/newrelic/check_docker)

#### Monitoring Database

* [check_mysql_health.pl](https://github.com/lausser/check_mysql_health)
* [check_oracle_health.pl](https://github.com/lausser/check_oracle_health)
* [check_db2_health.pl](https://github.com/lausser/check_db2_health)
* [check_mssql_health.pl](https://github.com/lausser/check_mssql_health)
* [check_mongodb.py](https://github.com/mzupan/nagios-plugin-mongodb)
* [check_postgres.pl](https://github.com/bucardo/check_postgres)

#### Monitoring Log

* [check_logfiles.pl](https://github.com/lausser/check_logfiles)
* [check_events.pl](https://github.com/pewo/nagios-plugins/tree/master/check_events)

#### Monitoring Application

* [check_sap_health.pl](https://github.com/lausser/check_sap_health)
* [check_mailbox_health.pl](https://github.com/lausser/check_mailbox_health)
* [check_elasticsearch.py](https://github.com/anchor/nagios-plugin-elasticsearch)
* [check_graphite_data.py](https://github.com/etsy/nagios_tools)

#### Misc

* [xPlugin_Monitoring](https://github.com/crazy-canux/xPlugin_Monitoring)
* [nagios-plugins](https://github.com/HariSekhon/nagios-plugins)
* [nagios-plugins](https://github.com/pewo/nagios-plugins)
* [nagios-plugins-rabbitmq](https://github.com/nagios-plugins-rabbitmq/nagios-plugins-rabbitmq)
* [nagios-snmp-plugins](http://nagios.manubulon.com/)
* [nagios-snmp-plugins-SF](https://sourceforge.net/projects/nagios-snmp/)

***

### Addons

> Collect the monitoring addons on github which star more than 1 hundred. And others addons.

* [Addons-Blog](http://crazy-canux.github.io/2016/06/08/Monitoring%E4%B9%8BAddons/)
* [nagios-addons-github](https://github.com/NagiosEnterprises)
* [naemon-addons-github](https://github.com/naemon)
* [icinga-addons-github](https://github.com/Icinga)
* [shinken-addons-github](https://github.com/shinken-monitoring)

#### Agent

* [NRPE-github](https://github.com/NagiosEnterprises/nrpe) - Nagios Remote Plugin Executor for Linux.
* [NRPE_NT-SF](https://sourceforge.net/projects/nrpent/) - Nagios Remote Plugin Executor for Windows.
* [NSCP](http://nsclient.org/) - NSClient++
* [NSCP-github](https://github.com/mickem/nscp)
* [NCPA-github](https://github.com/NagiosEnterprises/ncpa) - Nagios Cross Platform Agent

#### Event broker

* [NDOUtils-github](https://github.com/NagiosEnterprises/ndoutils) - Nagios Data Output Utils, for Nagios
* [IDOUtils-github](https://github.com/Icinga) - Icinga Data Output Utils, for Icinga
* [naemon-livestatus-github](https://github.com/naemon/naemon-livestatus) - for Naemon
* [mod-livestaus-github](https://github.com/shinken-monitoring/mod-livestatus) - for Shinken
* [MK Livestatus](http://mathias-kettner.com/checkmk_livestatus.html) - for Nagios/check_mk
* [MK Livestatus-repo](http://git.mathias-kettner.de/git/?p=check_mk.git;a=tree;f=livestatus;hb=HEAD)

#### Distributions

* [MK OMD](http://omdistro.org/) - The Open Monitoring Distribution, for Nagios/Naemon/Icinga/Shinken/check_mk
* [MK OMD-repo](http://git.mathias-kettner.de/git/?p=omd.git;a=tree)
* [NSCA](https://github.com/NagiosEnterprises/nsca) - Nagios Service Check Acceptor, for nagios/naemon/icinga
* [NSCA-ng](http://www.nsca-ng.org/) - Next Generation of NSCA
* [NSCA-ng-github](https://github.com/weiss/nsca-ng)
* [NRDP](https://github.com/NagiosEnterprises/nrdp) - Nagios Remote Data Processor, as a replacement for NSCA
* [DNX-official](http://dnx.sourceforge.net/) - Distributed Nagios eXecutor, for Nagios/Naemon
* [DNX-github](https://github.com/DNX-Project/DNX)
* [consol mod-gearman](http://www.mod-gearman.org/) - for Nagios/Naemon
* [consol mod-gearman-github](https://github.com/sni/mod_gearman)
* [op5 merlin](https://kb.op5.com/display/MERLIN/Distributed+%28Merlin%29+Home) - for Nagios/Naemon
* [op5 merlin-github](https://github.com/op5/merlin)

#### Visualization

* [op5 Ninja](https://kb.op5.com/display/GUI/GUI+%28Ninja%29+Home) - for Nagios/Naemon
* [op5 Ninja-github](https://github.com/op5/ninja)
* [mod-webui-github](https://github.com/shinken-monitoring/mod-webui) - for Shinken
* [icingaweb2-github](https://github.com/Icinga/icingaweb2) - for Icinga
* [consol thruk](http://www.thruk.org/) - webinterface for Nagios/Naemon/Icinga/Shinken
* [consol thruk-github](https://github.com/sni/Thruk)
* [nagdash-github](https://github.com/lozzd/Nagdash) - Dashboard interface for nagios
* [nagstamon](https://nagstamon.ifw-dresden.de/) - nagios status viewer on windows
* [nagstamon-github](https://github.com/HenriWahl/Nagstamon)
* [coffeesaint](https://www.vanheusden.com/java/CoffeeSaint/) - java based nagios status viewer
* [coffeesaint-github](https://github.com/flok99/CoffeeSaint)
* [nagmap-github](https://github.com/hecko/nagmap/) - Integration of nagios/icinga into google maps
* [nagvis](http://www.nagvis.org/) - Draw map from event brokers.
* [nagvis-github](https://github.com/NagVis/nagvis)
* [realopinsight](http://realopinsight.com/)
* [realopinsight-github](https://github.com/RealOpInsightLabs/realopinsight-workstation)

#### Metric Storage

* [rrdtool](http://oss.oetiker.ch/rrdtool/) - Round Robin Database Tool, store perfomance data.
* [rrdtool-github](https://github.com/oetiker/rrdtool-1.x)
* [graphite-whisper-github](https://github.com/graphite-project/whisper)
* [graphite-ceres-github](https://github.com/graphite-project/ceres)
* [influxdata](https://influxdata.com)
* [influxdata-github](https://github.com/influxdata/influxdb)
* [OpenTSDB](http://opentsdb.net/)
* [OpenTSDB-github](https://github.com/OpenTSDB/opentsdb)
* [kairosDB](http://kairosdb.github.io/)
* [kairosDB-github](https://github.com/kairosdb/kairosdb)
* [Kenshin-github](https://github.com/douban/Kenshin) - A time-series database alternative to Graphite Whisper with 40x improvement in IOPS

#### Metric Forwarding

* [graphios-github](https://github.com/shawn-sterling/graphios) - A program to send nagios perf data to graphite(carbon)/statsd/librato/influxdb
* [ledbetter-github](https://github.com/github/ledbetter) - Script that scrapes alert statistics from Nagios and reports them to Graphite

#### Graphing

* [nagiosgraph-official](http://nagiosgraph.sourceforge.net/) - Create graphs and store data in rrdtool.
* [nagiosgraph-SF](https://sourceforge.net/projects/nagiosgraph/)
* [pnp4nagios](http://docs.pnp4nagios.org/) - Create graphs and store rrdtool.
* [pnp4nagios-github](https://github.com/lingej/pnp4nagios)
* [Graphite](http://graphite.readthedocs.io/en/latest/)
* [Graphite-github](https://github.com/graphite-project/graphite-web)
* [grafana](http://grafana.org/)
* [grafana-github](https://github.com/grafana/grafana)

#### Congiruration

* [nagios.vim](https://github.com/crazy-canux/nagios.vim) - Vim plugin for Nagios
* [icinga2.vim](https://github.com/crazy-canux/icinga2.vim) - Vim plugin for Icinga
* [nconf-official](http://www.nconf.org/dokuwiki/doku.php) - Enterprise Nagios configurator
* [nconf-github](https://github.com/nconf/nconf)
* [adagios](http://adagios.org/) - web based nagios configuration
* [adagios-github](https://github.com/opinkerfi/adagios)

#### Procedure

* [dokuwiki](https://www.dokuwiki.org/dokuwiki/)
* [dokuwiki-github](https://github.com/splitbrain/dokuwiki)

#### Business Process Intelligence

* [nagiosBPI-github](https://github.com/NagiosEnterprises/nagiosbpi)
* [BP](http://bp-addon.monitoringexchange.org/)
* [BP-github](https://github.com/booboo-at-gluga-de/bp-addon)

#### SNMP

* [NSTI-github](https://github.com/NagiosEnterprises/nsti)
* [nagios-mib-github](https://github.com/nagios-plugins/nagios-mib) - This is the initial set of MIBs for Nagios to allow traps sent from Nagios to be recognized by other NMSes.

#### API

* [nagios-plugin-perl-github](https://github.com/nagios-plugins/nagios-plugin-perl) - Perl module Nagios::Monitoring::Plugin
* [nagiosvshell-github](https://github.com/NagiosEnterprises/nagiosvshell) - nagios v-shell PHP interface
* [pynag](http://pynag.org/) - API for managin nagios configuration and write plugins
* [pynag-github](https://github.com/pynag/pynag)
* [nagios-api-github](https://github.com/zorkian/nagios-api) - A REST-like, JSON interface to nagios
* [nagiosharder-github](https://github.com/railsmachine/nagiosharder) - ruby API for scraping nagios
* [nagrestconf](http://nagrestconf.smorg.co.uk/) - a REST interface and configuration GUI for nagios
* [nagrestconf-github](https://github.com/mclarkson/nagrestconf)

#### Misc

* [chef-nagios-cookbook-github](https://github.com/schubergphilis/nagios)
* [cucumber](http://cucumber-nagios.org/)
* [cucumber-github](https://github.com/auxesis/cucumber-nagios)
* [nagios-herald](https://codeascraft.com/2014/06/06/introducing-nagios-herald/) - Customizing nagios alerts
* [nagios-herald-github](https://github.com/etsy/nagios-herald)

***

## check_mk

* [Check_mk-Blog](http://crazy-canux.github.io/2016/05/12/Monitoring%E4%B9%8Bcheck_MK/) - Monitoring tools written in python
* [Check_mk-official](http://mathias-kettner.de/check_mk.html)
* [check_mk-repo](http://git.mathias-kettner.de/git/?p=check_mk.git;a=tree)
* [check_mk-exchange](http://mathias-kettner.com/check_mk_exchange.php?HTML=yes) - check_mk plugins

***

### Sensu

> Sensu is an infrastructure and application monitoring and telemetry solution.

* [Sensu-official](https://sensuapp.org/) - Monitoring tools written in ruby
* [Sensu-github](https://github.com/sensu)
* [sensu-plugins-official](http://sensu-plugins.io/) - sensu plugin wirtten in ruby
* [sensu-plugins-github](https://github.com/sensu-plugins)

***

## Centreon

* [Centreon-official](https://www.centreon.com) - Monitoring tools written in C++
* [Centreon-github](https://github.com/centreon)
* [Centreon-plugins-github](https://github.com/centreon/centreon-plugins) - centreon plugin writen in perl

***

## Zabbix

* [Zabbix-official](http://www.zabbix.com/)
* [Zabbix-Opensource](https://www.zabbix.org/wiki/Main_Page)
* [Zabbix-github](https://github.com/zabbix)

***

## Ganglia

* [Ganglia-official](http://ganglia.info/)
* [Ganglia-github](https://github.com/ganglia)

***

## Zenoss

* [Zenoss-official](https://www.zenoss.com/)
* [Zenoss-Opensource](https://www.zenoss.org/)
* [Zenoss-github](https://github.com/zenoss)

***

## Opserver

* [Opserver-official](http://opserver.org/)
* [Opserver-github](https://github.com/opserver)

***

## Opsview

* [Opsview-official](https://www.opsview.com/)

***

## Bosun

* [Bosun-official](http://bosun.org/)
* [Bosun-github](https://github.com/bosun-monitor)

***

## Authors

[Authors](AUTHORS.md)

***

## Contributing

[Contribution guide](CONTRIBUTING.md)

***

## License

[![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png)](http://creativecommons.org/publicdomain/zero/1.0/)

See [License](LICENSE)
