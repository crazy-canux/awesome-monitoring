# Awesome-Monitoring [![Awesome](https://cdn.rawgit.com/sindresorhus/awesome/d7305f38d29fed78fa85652e3a63e154dd8e8829/media/badge.svg)](https://github.com/sindresorhus/awesome)

> A curated list of awesome resources for monitoring tools.

* [awesome](https://github.com/sindresorhus/awesome)
* [awesome-awesomeness](https://github.com/bayandin/awesome-awesomeness)

***

## Contents

* [DC Monitoring](#dc-monitoring)
    * [Nagios](#nagios)
    * [Zabbix](#zabbix)
    * [Ganglia](#ganglia)
    * [Zenoss](#zenoss)
    * [Opserver](#opserver)
    * [Netdata](#netdata)
* [Nagios Monitoring](#nagios-monitoring)
    * [Icinga](#icinga)
    * [Naemon](#naemon)
    * [Shinken](#shinken)
    * [Centreon](#centreon)
    * [Opsview](#opsview)
    * [Check_mk](#check_mk)
    * [Sensu](#sensu)
    * [Cabot](#cabot)
    * [Server Density](#server-density)
    * [PagerDuty](#pagerduty)
    * [Nagios Monitoring Plugins](#nagios-monitoring-plugins)
    * [Nagios Monitoring Addons](#nagios-monitoring-addons)
* [Zabbix Monitoring](#zabbix-monitoring)
    * [Zabbix Monitoring Plugins](#zabbix-monitoring-plugins)
    * [Zabbix Monitoring Addons](#zabbix-monitoring-addons)
* [APM Monitoring](#apm-monitoring)
    * [collector](#collector)
    * [backend](#backend)
    * [alerting](#alerting)
    * [dashboard](#dashboard)
* [Distributed Tracing](#distributed-tracing)
    * [OpenTelementry](#opentelementry)
    * [Zipkin](#zipkin)
    * [Sentry](#sentry)
    * [Jaeger](#jaeger)
    * [Pinpoint](#pinpoint)
    * [Skywalking](#skywalking)
* [API](#api)
    * [super-devops](#super-devops)
    * [go-devops](#go-devops)
* [Contributing](#contributing)
* [License](#license)

***

## DC Monitoring

Tranditional Data Center Monitoring tools.

### Nagios

* [Nagios](https://www.nagios.org/) - Nagios Is The Industry Standard In IT Infrastructure Monitoring.
* [Nagios enterprise](https://www.nagios.com/) - IT Infrastructure Monitoring World Class Network, Server and Log Monitoring Software.
* [Nagios exchange](https://exchange.nagios.org/) - Nagios official exchange website.
* [Nagios core github](https://github.com/NagiosEnterprises/nagioscore) - Nagios core written in C.
* [Nagios chinese](https://sourceforge.net/projects/nagios-cn/files/?source=navbar) - Nagios chinese document.
* [Nagios](http://canuxcheng.com/devops/Monitoring_Nagios.html) - Blog.

### Zabbix

> Zabbix is the ultimate enterprise-level software designed for real-time monitoring of millions of metrics collected from tens of thousands of servers, virtual machines and network devices. Zabbix is Open Source and comes at no cost.

* [Zabbix](https://www.zabbix.org) - The Ultimate Enterprise-class Monitoring Platform.
* [Zabbix enterprise](http://www.zabbix.com/) - Zabbix enterprise website.
* [Zabbix core](https://zabbix.org/wiki/Get_Zabbix) - Zabbix core written in C.
* [Zabbix github](https://github.com/zabbix) - Zabbix github.

### Ganglia

> Ganglia is a scalable distributed monitoring system for high-performance computing systems such as clusters and Grids.

* [Ganglia](http://ganglia.info/) - Ganglia is a scalable distributed monitoring system for high-performance computing systems.
* [Ganglia core github](https://github.com/ganglia/monitor-core) - Ganglia written in C.
* [Ganglia github](https://github.com/ganglia) - Related source code.

### Zenoss

> Zenoss works with the world's largest companies to ensure their IT services and applications are always on. As the global leader in hybrid IT monitoring and analytics software, Zenoss provides complete visibility for cloud, virtual and physical IT environments.

* [Zenoss](https://www.zenoss.org/) - The Hybrid IT Monitoring Platform.
* [Zenoss enterprise](https://www.zenoss.com/) - Zenoss enterprise website.
* [Zenoss core sf](https://sourceforge.net/projects/zenoss/) - Zenoss core written in C.
* [Zenoss github](https://github.com/zenoss) - Zenoss github.

### Opserver

* [Opserver](http://opserver.org/) - Stack Exchange's monitoring system.
* [Opserver github](https://github.com/opserver/Opserver) - Opserver written in C#.

### Netdata

> netdata is a system for distributed real-time performance and health monitoring. It provides unparalleled insights, in real-time, of everything happening on the system it runs (including applications such as web and database servers), using modern interactive web dashboards.

* [netdata](http://my-netdata.io/) - Get control of your servers. Simple. Effective. Awesome.
* [netdata github](https://github.com/firehol/netdata) - Netdata written in C.

***

## Nagios Monitoring

Monitoring tools based on nagios.

### Icinga

* [Icinga](https://www.icinga.org/) - Monitoring core with features and REST API.
* [Icinga exchange](https://exchange.icinga.org/) - Icinga2 official exchange website.
* [Icinga2 core github](https://github.com/Icinga/icinga2) - Icinga2 core written in C++.
* [Icinga chinese](https://sourceforge.net/projects/icinga-cn/files/) - Icinga2 chinese document.

### Naemon

> Naemon is the new monitoring suite that aims to be fast, stable and innovative while giving you a clear view of the state of your network and applications.

* [Naemon](http://www.naemon.org/) - Networks, Applications and Event Monitor.
* [Naemon core github](https://github.com/naemon/naemon-core) - Next generation of nagios core written in C.

### Shinken

> Shinken is a monitoring framework. It's a Python Nagios® Core total rewrite enhancing flexibility and large environment management.

* [Shinken](http://www.shinken-monitoring.org/) - Flexible and scalable monitoring framework.
* [Shinken core github](https://github.com/naparuba/shinken) - Shinken core written in Python.
* [Shinken exchange github](https://github.com/shinken-monitoring) - Shinken exchange.

### Centreon

> Centreon is a network, system, applicative supervision and monitoring tool.

* [Centreon](https://www.centreon.com) - An industry reference for open source monitoring.
* [Centreon core github](https://github.com/centreon/centreon-engine) - Centreon core written in C++.
* [Centreon plugins github](https://github.com/centreon/centreon-plugins) - Centreon plugins.

### Opsview

> Powerful monitoring for IT networks large and small, visualize all aspects of your IT at a glance.

* [Opsview](https://www.opsview.com/) - Comprehensive coverage, rapid resolution, elegant visualization.
* [Opsview core](https://www.openhub.net/p/opsview) - Opsview core written in Perl.

### check_mk

> Check_MK is comprehensive IT monitoring solution in the tradition of Nagios.

* [Check_mk](http://mathias-kettner.de/check_mk.html) - Check_mk is open source and pure python code.
* [check_mk exchange](http://mathias-kettner.com/check_mk_exchange.php?HTML=yes) - Check_mk plugins.
* [check_mk core git](http://git.mathias-kettner.de/git/?p=check_mk.git;a=tree) - Check_mk core written in Python.

### Sensu

* [Sensu](https://sensuapp.org/) - Monitoring for today's infrastructure.
* [Sensu github](https://github.com/sensu/sensu) - Sensu core written in Ruby.
* [Sensu-plugins](http://sensu-plugins.io/) - Sensu plugins.
* [Sensu-plugins github](https://github.com/sensu-plugins) - Sensu plugins source code.

### Cabot

* [cabot github](https://github.com/arachnys/cabot) - Self-hosted, easily-deployable monitoring and alerts service.

### Server Density

* [server density](https://www.serverdensity.com/) - Server monitoring.
* [server density github](https://github.com/serverdensity) - Related source code.

### PagerDuty

* [pagerduty](https://www.pagerduty.com/) - Digital Operations Management.
* [pagerduty github](https://github.com/PagerDuty) - Related source code.

### Nagios Monitoring Plugins

> The Monitoring Plugins Development Team, maintain a bundle of more than fifty standard plugins for Icinga, Naemon, Nagios, Shinken, Sensu, Centreon and Opsview.

* [monitoring-plugins](https://www.monitoring-plugins.org/) - Monitoring plugins official website.
* [monitoring-plugins github](https://github.com/monitoring-plugins) - Monitoring plugins source code.

#### Develop Plugins

> How to develop plugins for your own service.

* [monitoring plugin development guidelines](https://www.monitoring-plugins.org/doc/guidelines.html) - Plugins development document official website.

#### Plugins API

* [monitoring-plugins perl](https://github.com/monitoring-plugins/monitoring-plugin-perl) - Official Perl module for plugins.
* [arguspy](https://github.com/crazy-canux/arguspy) - A set of Python API for write your monitoring plugins.
* [arguspy docs](http://arguspy.readthedocs.io/en/latest/) - Document for arguspy.

#### Monitoring Windows server

* [check_wmi_plus.pl](http://www.edcint.co.nz/checkwmiplus/) - Plugin for windows monitoring use WMI.

#### Monitoring OS X

* [OSX-Monitoring-Tools](https://github.com/jedda/OSX-Monitoring-Tools) - Plugins used to monitoring Mac system.

#### Monitoring Linux/Unix

* [check_fujitsu_health.pl](https://github.com/lausser/check_fujitsu_health) - For fujitsu.
* [check_hpasm.pl](https://github.com/lausser/check_hpasm) - For Hpasm.

#### Monitoring Network

* [check_nwc_health.pl](https://github.com/lausser/check_nwc_health) - For nwc.

#### Monitoring Storage

* [check_tl_health.pl](https://github.com/lausser/check_tl_health) - For tl.
* [check_raid.pl](https://github.com/glensc/nagios-plugin-check_raid) - For raid.
* [check_sstcam.pl](https://github.com/lausser/check_sstcam) - For sstcam.

#### Monitoring Virtualization

* [check_wmware_api.pl](https://github.com/op5/check_vmware_api) - For vmware.
* [check_vmware_esx.pl](https://github.com/BaldMansMojo/check_vmware_esx) - For vmware.

#### Monitoring Cloud Computing

* [openstack nagios plugins](https://github.com/cirrax/openstack-nagios-plugins) - Monitoring openstack.

#### Monitoring BigData

#### Monitoring Database

* [check_mysql_health.pl](https://github.com/lausser/check_mysql_health) - For mysql.
* [check_oracle_health.pl](https://github.com/lausser/check_oracle_health) - For oracle.
* [check_db2_health.pl](https://github.com/lausser/check_db2_health) - For db2.
* [check_mssql_health.pl](https://github.com/lausser/check_mssql_health) - For mssql.
* [check_mongodb.py](https://github.com/mzupan/nagios-plugin-mongodb) - For mongodb.
* [check_postgres.pl](https://github.com/bucardo/check_postgres) - For postgres.

#### Monitoring Log

* [check_logfiles.pl](https://github.com/lausser/check_logfiles) - For logfile.
* [check events](https://github.com/pewo/nagios-plugins)
* [check_generic](https://github.com/flackem/check_generic)
* [check_multi](https://github.com/flackem/check_multi)

#### Monitoring Application

* [check_sap_health.pl](https://github.com/lausser/check_sap_health) - For sap.
* [check_mailbox_health.pl](https://github.com/lausser/check_mailbox_health) - For mailbox.
* [check_elasticsearch.py](https://github.com/anchor/nagios-plugin-elasticsearch) - For ES.
* [check_graphite_data.py](https://github.com/etsy/nagios_tools) - For graphite.
* [nagios-plugins-rabbitmq](https://github.com/nagios-plugins-rabbitmq/nagios-plugins-rabbitmq) - For rabbitmq.
* [Celery-Nagios-Plugin](https://github.com/wilhelm-murdoch/Celery-Nagios-Plugin) - For celery.
* [nagios-jenkins-plugin](https://github.com/jonlives/nagios-jenkins-plugin) - For jenkins.
* [ceph-nagios-plugins](https://github.com/valerytschopp/ceph-nagios-plugins) - For ceph.
* [check_ssl_cert](https://github.com/matteocorti/check_ssl_cert) - For CA.

#### Monitoring Languages

* [nagios_erlang](https://github.com/lethain/nagios_erlang) - Monitoring erlang

#### Monitoring SNMP

* [nagios snmp plugins](http://nagios.manubulon.com/) - For snmp.
* [nagios snmp plugins SF](https://sourceforge.net/projects/nagios-snmp/) - For snmp.

#### Misc

* [zplugin](https://github.com/crazy-canux/zplugin) - Tons of plugins monitoring your business.
* [nagios plugins](https://github.com/HariSekhon/nagios-plugins) - HariSekhon's plugins.
* [nagios plugins](https://github.com/willixix/naglio-plugins) - Willixix's plugins.
* [nagios plugins](https://github.com/MonitoringPlug/monitoringplug) - MonitoringPlug's plugins.
* [nagios plugins](https://github.com/AppliedTrust/nagios-plugins) - AppliedTrust's plugins.
* [nagios plugins](https://github.com/opinkerfi/nagios-plugins) - opinkerfi's plugins.
* [nagios plugins](https://github.com/kumina/nagios-plugins-kumina) - kumina's plugins.
* [nagios plugins](https://github.com/Inuits/monitoring-plugins) - Inuits's plugins.
* [nagios plugins](https://github.com/appfirst/nagios-plugins) - appfirst's plugins.
* [nagios plugins](https://github.com/Voxer/nagios-plugins) - Voxer's plugins.
* [nagios plugins](https://github.com/innogames/igmonplugins) - innogames's plugins.

### Nagios Monitoring Addons

> This is the addon can be used for Nagios, Naemon, Icinga, Shinken, Centreon and Opsview.

* [nagios-addons github](https://github.com/NagiosEnterprises) - Nagios addons.
* [naemon-addons github](https://github.com/naemon) - Naemon addons.
* [icinga-addons github](https://github.com/Icinga) - Icinga addons.
* [shinken-addons github](https://github.com/shinken-monitoring) - Shinken addons.

#### Agent

* [NRPE github](https://github.com/NagiosEnterprises/nrpe) - Nagios Remote Plugin Executor for Linux.
* [NRPE_NT SF](https://sourceforge.net/projects/nrpent/) - Nagios Remote Plugin Executor for Windows.
* [NSCP](http://nsclient.org/) - NSClient++.
* [NSCP github](https://github.com/mickem/nscp) - NSCP source code.
* [NCPA github](https://github.com/NagiosEnterprises/ncpa) - Nagios Cross Platform Agent.

#### Event broker

* [NDOUtils github](https://github.com/NagiosEnterprises/ndoutils) - Nagios Data Output Utils, for Nagios.
* [IDOUtils github](https://github.com/Icinga) - Icinga Data Output Utils, for Icinga.
* [naemon-livestatus github](https://github.com/naemon/naemon-livestatus) - For Naemon.
* [mod-livestaus github](https://github.com/shinken-monitoring/mod-livestatus) - For Shinken.
* [MK Livestatus](http://mathias-kettner.com/checkmk_livestatus.html) - For nagios/check_mk.
* [MK Livestatus git](http://git.mathias-kettner.de/git/?p=check_mk.git;a=tree;f=livestatus;hb=HEAD) - Livesttus source code.

#### Distributions

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

#### Visualization

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

#### Metric Storage

* [rrdtool](http://oss.oetiker.ch/rrdtool/) - Round Robin Database Tool, store perfomance data.
* [rrdtool github](https://github.com/oetiker/rrdtool-1.x) - Rrdtool source code.
* [graphite-whisper github](https://github.com/graphite-project/whisper) - Graphite whisper.
* [statsd github](https://github.com/etsy/statsd) - Daemon for easy but powerful stats aggregation.

#### Metric Forwarding

* [graphios github](https://github.com/shawn-sterling/graphios) - A program to send nagios perf data to graphite(carbon)/statsd/librato/influxdb.
* [ledbetter github](https://github.com/github/ledbetter) - Script that scrapes alert statistics from Nagios and reports them to Graphite.
* [nagflux github](https://github.com/Griesbacher/nagflux) - A connector which copies performancedata from Nagios / Icinga(2) / Naemon to InfluxDB

#### Metric Graphing

* [nagiosgraph](http://nagiosgraph.sourceforge.net/) - Create graphs and store data in rrdtool.
* [nagiosgraph SF](https://sourceforge.net/projects/nagiosgraph/) - Nagiosgraph source code.
* [pnp4nagios](http://docs.pnp4nagios.org/) - Create graphs and store rrdtool.
* [pnp4nagios github](https://github.com/lingej/pnp4nagios) - Php4nagios source code.
* [Graphite carbon github ](https://github.com/graphite-project/carbon) - Graphite carbon.
* [Graphite web github](https://github.com/graphite-project/graphite-web) - Graphite web.

#### Configuration

* [nagios.vim](https://github.com/crazy-canux/nagios.vim) - Vim plugin for Nagios.
* [icinga2.vim](https://github.com/crazy-canux/icinga2.vim) - Vim plugin for Icinga2.
* [nconf](http://www.nconf.org/dokuwiki/doku.php) - Enterprise Nagios configurator.
* [nconf github](https://github.com/nconf/nconf) - Nconf source code.
* [adagios](http://adagios.org/) - Web based nagios configuration.
* [adagios github](https://github.com/opinkerfi/adagios) - Adagios source code.

#### Procedure

* [dokuwiki](https://www.dokuwiki.org/dokuwiki/) - Dokuwiki.
* [dokuwiki github](https://github.com/splitbrain/dokuwiki) - Dokuwiki source code.

#### Business Process Intelligence

* [nagiosBPI github](https://github.com/NagiosEnterprises/nagiosbpi) - NagiosBPI.
* [BP](http://bp-addon.monitoringexchange.org/) - BP.
* [BP github](https://github.com/booboo-at-gluga-de/bp-addon) - BP source code.

#### SNMP

* [NSTI github](https://github.com/NagiosEnterprises/nsti) - NSTI.
* [nagios-mib github](https://github.com/nagios-plugins/nagios-mib) - This is the initial set of MIBs for Nagios to allow traps sent from Nagios to be recognized by other NMSes.

#### Addons API

* [nagiosvshell github](https://github.com/NagiosEnterprises/nagiosvshell) - Nagios v-shell PHP interface.
* [pynag](http://pynag.org/) - API for managin nagios configuration and write plugins.
* [pynag github](https://github.com/pynag/pynag) - Pynag source code.
* [nagios-api github](https://github.com/zorkian/nagios-api) - A REST-like, JSON interface to nagios.
* [nagiosharder github](https://github.com/railsmachine/nagiosharder) - Ruby API for scraping nagios.
* [nagrestconf](http://nagrestconf.smorg.co.uk/) - A REST interface and configuration GUI for nagios.
* [nagrestconf github](https://github.com/mclarkson/nagrestconf) - Nagrestconf.

#### Misc

* [chef-nagios-cookbook github](https://github.com/schubergphilis/nagios) - Chef nagios cookbook.
* [cucumber](http://cucumber-nagios.org/) - Cucumber.
* [cucumber github](https://github.com/auxesis/cucumber-nagios) - Cucumber source code.
* [nagios-herald](https://codeascraft.com/2014/06/06/introducing-nagios-herald/) - Customizing nagios alerts.
* [nagios-herald github](https://github.com/etsy/nagios-herald) - Nagios-herald source code.

***

## Zabbix Monitoring

> Monitoring tools based on Zabbix.

### Zabbix Monitoring Plugins

### Zabbix Monitoring Addons

***

## APM Monitoring

Application Performance Monitoring.

Modern Monitoring tools for devops, container(kubernetes), microservice and serverless.

### Collector

Traces, Metrics, Logs.

Metrics

* [telegraf github](https://github.com/influxdata/telegraf) - TICK stack, The plugin-driven server agent for collecting & reporting metrics.
* [node-exporter github](https://github.com/prometheus/node_exporter) - Prometheus stack, Exporter for machine metrics.
* [collectd](http://collectd.org/) - The system statistics collection daemon.
* [collectd github](https://github.com/collectd/collectd) - collectd written in C.
* [tcollector github](https://github.com/OpenTSDB/tcollector) - Data collection framework for OpenTSDB
* [falcon-plus github](https://github.com/open-falcon/falcon-plus) - An open-source and enterprise-level monitoring system.

Logs

* [promtail github](https://github.com/grafana/loki) - log agent for loki.
* [beats github](https://github.com/elastic/beats) - Lightweight shippers for Elasticsearch & Logstash, Elastic stack.
* [logstash github](https://github.com/elastic/logstash) - Transport and process your logs, events, or other data, Elastic stack.
* [fluent-bit](https://github.com/fluent/fluent-bit) - Fast and Lightweight Logs and Metrics processor for Linux, BSD, OSX and Windows.
* [fluent github](https://github.com/fluent/fluentd) - Fluentd is an open-source logging solution to unify data collection and consumption.

Traces

* [new relic github](https://github.com/newrelic) - New relic written in Ruby.
* [javamelody github](https://github.com/javamelody/javamelody) - The source code.
* [kamon github](https://github.com/kamon-io/Kamon) - The source code.

### Backend

Some all-in-one APM service provide backend service.

Metrics

* [influxdata](https://influxdata.com) - influxdb, written in go.
* [influxdata github](https://github.com/influxdata/influxdb) - TICK stack.
* [prometheus](https://prometheus.io/) - The Prometheus monitoring system and time series database.
* [prometheus github](https://github.com/prometheus/prometheus) - Prometheus stack.
* [VictoriaMetrics](https://github.com/VictoriaMetrics/VictoriaMetrics/) - VictoriaMetrics stack.
* [OpenTSDB](http://opentsdb.net/) - OpenTSDB, written in java.
* [OpenTSDB github](https://github.com/OpenTSDB/opentsdb) - OpenTSDB source code.
* [kairosDB](http://kairosdb.github.io/) - KairosDB.
* [kairosDB github](https://github.com/kairosdb/kairosdb) - KairosDB source code.

Logging

* [loki github](https://github.com/grafana/loki) - Loki: like Prometheus, but for logs.
* [elasticsearch](https://www.elastic.co/cn/products/elasticsearch) - Open Source, Distributed, RESTful Search Engine, written in java.
* [elasticsearch github](https://github.com/elastic/elasticsearch) - Elastic stack.

Tracing

* [tempo github](https://github.com/grafana/tempo) - Grafana Tempo is a high volume, minimal dependency distributed tracing backend.
* [new relic github](https://github.com/newrelic) - New relic written in Ruby.
* [javamelody github](https://github.com/javamelody/javamelody) - The source code.
* [kamon github](https://github.com/kamon-io/Kamon) - The source code.

### Alerting

* [kapacitor github](https://github.com/influxdata/kapacitor) - TICK stack, written in go.
* [alertmanager github](https://github.com/prometheus/alertmanager) - Prometheus stack, Prometheus Alertmanager, written in go.
* [x-pack](https://www.elastic.co/cn/products/x-pack) - Elastic stack.
* [Bosun](http://bosun.org/) - Time Series Alerting Framework.
* [Bosun github](https://github.com/bosun-monitor) - Bosun written in Go.
* [grafana github](https://github.com/grafana/grafana) - Grafana alerting.

### Dashboard

* [grafana github](https://github.com/grafana/grafana) - Grafana stack.
* [chronograf github](https://github.com/influxdata/chronograf) - TICK stack.
* [kibana github](https://github.com/elastic/kibana) - Elastic stack.

***

## Distributed Tracing

### OpenTelementry

all-in-one.

OpenCensus and OpenTracing have merged to form OpenTelemetry.

* [opentelementry](https://opentelemetry.io/) - High-quality, ubiquitous, and portable telemetry to enable effective observability.
* [opentelementry github](https://github.com/open-telemetry)

### Zipkin

* [zipkin github](https://github.com/openzipkin/zipkin) - A distributed tracing system.

### Jaeger

* [Jaeger](https://www.jaegertracing.io/) 
* [Jaeger github](https://github.com/jaegertracing) - Distributed Tracing System

### Sentry

* [Sentry](https://sentry.io/welcome/) - Sentry provides open-source and hosted error monitoring that helps all software
teams discover, triage, and prioritize errors in real-time.
* [Sentry github](https://github.com/getsentry/sentry) - Sentry is cross-platform application monitoring, with a focus on error reporting.

### Pinpoint

* [pinpoint github](https://github.com/pinpoint-apm/pinpoint) - APM, (Application Performance Management) tool for large-scale distributed systems.

### SkyWalking

all-in-one

* [skywalking](https://skywalking.apache.org/) - Application performance monitor tool for distributed systems, especially designed for microservices, cloud native and container-based (Kubernetes) architectures.
* [skywalking github](https://github.com/apache/skywalking) 

***

## API

### super-devops

* [super-devops](http://super-devops.readthedocs.io/en/latest/) - Tons of devops tools for devops related project.
* [super-devops github](https://github.com/crazy-canux/super-devops)

### go-devops

* [go-devops](https://godoc.org/github.com/crazy-canux/go-devops) - Tons of devops tools for devops related project.
* [go-devops github](https://github.com/crazy-canux/go-devops) 

***

## Contributing

[Contribution guide](CONTRIBUTING.md)

***

## Authors

[Authors](AUTHORS.md)

***

## License

[![CC0](https://licensebuttons.net/p/zero/1.0/88x31.png)](http://creativecommons.org/publicdomain/zero/1.0/)
