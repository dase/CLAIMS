#!/usr/bin/python
# Filename : monitor_loader.py
#
# This script must be running on master

usage = u'''
this scripts main role is to load data into
different data path, eg:
 /home/claims/data/tpc-h/sf1/1partition
 /home/claims/data/tpc-h/sf1/4partition
 /home/claims/data/tpc-h/sf1/8partition
 /home/claims/data/tpc-h/sf10/1partition
 /home/claims/data/tpc-h/sf10/4partition
 /home/claims/data/tpc-h/sf10/8partition
 /home/claims/data/tpc-h/sf100/1partition
 /home/claims/data/tpc-h/sf100/4partition
 /home/claims/data/tpc-h/sf100/8partition
and different disk mode, eg:
 local disk
 hdfs
while we load data into these place, we should 
first modify the config file of claims:
 sbin/2-claims-conf/cluster.config
then deploy it, call script:
 sbin/3-deploy.sh
then restart claimsserver:
 sbin/start-all.sh
then start client and load the data, this step will call script:
 sbin/claims-test/claimstest.sh 1 1 <testcase>
here we do all these automatically in this scripts.
and you can alse add your own load path and ddl into this scripts later.
'''
import os
import sys
import ConfigParser 
import subprocess
import time

'''
 local_disk_mode
   0 : hdfs mode
   1 : local disk mode
 data
   the path for claims to store data
 case
    first: loop times
    second: concurrency
    third: test case in sbin/claims-test/testcase/
    the case tuple can contain numerous object
'''
'''
mode_data_case=( 
                 (0, "/home/claims/data/tpc-h/sf1/1partition/",  ((1, 1, "ddl_tpch_sf1_1partition"),)), 
                 (0, "/home/claims/data/tpc-h/sf1/4partition/",  ((1, 1, "ddl_tpch_sf1_4partition"),)),
                 (0, "/home/claims/data/tpc-h/sf1/8partition/",  ((1, 1, "ddl_tpch_sf1_8partition"),)),
                 (0, "/home/claims/data/tpc-h/sf10/1partition/", ((1, 1, "ddl_tpch_sf10_1partition"),)),
                 (0, "/home/claims/data/tpc-h/sf10/4partition/", ((1, 1, "ddl_tpch_sf10_4partition"),)),
                 (0, "/home/claims/data/tpc-h/sf10/8partition/", ((1, 1, "ddl_tpch_sf10_8partition"),)),
                 (0, "/home/claims/data/tpc-h/sf100/1partition/",((1, 1, "ddl_tpch_sf100_1partition"),)),
                 (0, "/home/claims/data/tpc-h/sf100/4partition/",((1, 1, "ddl_tpch_sf100_4partition"),)),
                 (0, "/home/claims/data/tpc-h/sf100/8partition/",((1, 1, "ddl_tpch_sf100_8partition"),)),
                 (1, "/home/claims/data/tpc-h/sf1/1partition/",  ((1, 1, "ddl_tpch_sf1_1partition"),)),
                 (1, "/home/claims/data/tpc-h/sf1/4partition/",  ((1, 1, "ddl_tpch_sf1_4partition"),)),
                 (1, "/home/claims/data/tpc-h/sf1/8partition/",  ((1, 1, "ddl_tpch_sf1_8partition"),)),
                 (1, "/home/claims/data/tpc-h/sf10/1partition/", ((1, 1, "ddl_tpch_sf10_1partition"),)),
                 (1, "/home/claims/data/tpc-h/sf10/4partition/", ((1, 1, "ddl_tpch_sf10_4partition"),)),
                 (1, "/home/claims/data/tpc-h/sf10/8partition/", ((1, 1, "ddl_tpch_sf10_8partition"),)),
                 (1, "/home/claims/data/tpc-h/sf100/1partition/",((1, 1, "ddl_tpch_sf100_1partition"),)),
                 (1, "/home/claims/data/tpc-h/sf100/4partition/",((1, 1, "ddl_tpch_sf100_4partition"),)),
                 (1, "/home/claims/data/tpc-h/sf100/8partition/",((1, 1, "ddl_tpch_sf100_8partition"),)),
               )
'''

mode_data_case=( 
            (1, "/home/claims/data/tpc-h/sf1/1partition/",  ((1, 1, "tpc_sql_1"),(1, 1, "tpc_sql_3"),(1, 1, "tpc_sql_5"),(1, 1, "tpc_sql_6"),(1, 1, "tpc_sql_10"),(1, 1, "tpc_sql_12"),(1, 1, "tpc_sql_13"),(1, 1, "tpc_sql_17"),)),
            (1, "/home/claims/data/tpc-h/sf1/4partition/",  ((1, 1, "tpc_sql_1"),(1, 1, "tpc_sql_3"),(1, 1, "tpc_sql_5"),(1, 1, "tpc_sql_6"),(1, 1, "tpc_sql_10"),(1, 1, "tpc_sql_12"),(1, 1, "tpc_sql_13"),(1, 1, "tpc_sql_17"),)),
            (1, "/home/claims/data/tpc-h/sf1/8partition/",  ((1, 1, "tpc_sql_1"),(1, 1, "tpc_sql_3"),(1, 1, "tpc_sql_5"),(1, 1, "tpc_sql_6"),(1, 1, "tpc_sql_10"),(1, 1, "tpc_sql_12"),(1, 1, "tpc_sql_13"),(1, 1, "tpc_sql_17"),)),
            (1, "/home/claims/data/tpc-h/sf10/1partition/", ((1, 1, "tpc_sql_1"),(1, 1, "tpc_sql_3"),(1, 1, "tpc_sql_5"),(1, 1, "tpc_sql_6"),(1, 1, "tpc_sql_10"),(1, 1, "tpc_sql_12"),(1, 1, "tpc_sql_13"),(1, 1, "tpc_sql_17"),)),
            (1, "/home/claims/data/tpc-h/sf10/4partition/", ((1, 1, "tpc_sql_1"),(1, 1, "tpc_sql_3"),(1, 1, "tpc_sql_5"),(1, 1, "tpc_sql_6"),(1, 1, "tpc_sql_10"),(1, 1, "tpc_sql_12"),(1, 1, "tpc_sql_13"),(1, 1, "tpc_sql_17"),)),
            (1, "/home/claims/data/tpc-h/sf10/8partition/", ((1, 1, "tpc_sql_1"),(1, 1, "tpc_sql_3"),(1, 1, "tpc_sql_5"),(1, 1, "tpc_sql_6"),(1, 1, "tpc_sql_10"),(1, 1, "tpc_sql_12"),(1, 1, "tpc_sql_13"),(1, 1, "tpc_sql_17"),)),
            (1, "/home/claims/data/tpc-h/sf100/1partition/",((1, 1, "tpc_sql_1"),(1, 1, "tpc_sql_3"),(1, 1, "tpc_sql_5"),(1, 1, "tpc_sql_6"),(1, 1, "tpc_sql_10"),(1, 1, "tpc_sql_12"),(1, 1, "tpc_sql_13"),(1, 1, "tpc_sql_17"),)),
            (1, "/home/claims/data/tpc-h/sf100/4partition/",((1, 1, "tpc_sql_1"),(1, 1, "tpc_sql_3"),(1, 1, "tpc_sql_5"),(1, 1, "tpc_sql_6"),(1, 1, "tpc_sql_10"),(1, 1, "tpc_sql_12"),(1, 1, "tpc_sql_13"),(1, 1, "tpc_sql_17"),)),
            (1, "/home/claims/data/tpc-h/sf100/8partition/",((1, 1, "tpc_sql_1"),(1, 1, "tpc_sql_3"),(1, 1, "tpc_sql_5"),(1, 1, "tpc_sql_6"),(1, 1, "tpc_sql_10"),(1, 1, "tpc_sql_12"),(1, 1, "tpc_sql_13"),(1, 1, "tpc_sql_17"),)),
               )


claimshome=os.getenv("CLAIMS_HOME")
cf = ConfigParser.ConfigParser()
cf.read("%s/sbin/2-claims-conf/cluster.config"%(claimshome))
hostname = cf.get("cluster", "master") 
user = cf.get("cluster", "user")
config_disk_mode="ssh -f -n -l %s %s \"cd $CLAIMS_HOME/sbin/2-claims-conf; sed -i 's:^local_disk_mode = .*$:local_disk_mode = %s:g' ./cluster.config; exit;\";sleep 1;"
config_data="ssh -f -n -l %s %s \"cd $CLAIMS_HOME/sbin/2-claims-conf; sed -i 's:^data = .*$:data = %s:g' ./cluster.config; exit;\";sleep 1;"
deploy="ssh -f -n -l %s %s \"$CLAIMS_HOME/sbin/3-deploy.sh config; exit;\";"
startall="%s/sbin/start-all.sh;"
stopall="%s/sbin/stop-all.sh;"
runtest="cd %s/sbin/claims-test; ./claimstest.sh %d %d %s;"

def autotest():
    for i in mode_data_case:
        command1=config_disk_mode%(user, hostname, i[0])+config_data%(user, hostname, i[1])+deploy%(user, hostname)
        print command1
        os.system(command1)
        time.sleep(5)
        for j in i[2]:
            os.system(stopall%claimshome)
            time.sleep(25)
            os.system(startall%claimshome)
            time.sleep(30)
            print "test:[%s],loop:[%d],currencuy:[%d]" % (j[2], j[0], j[1])
            os.system(runtest % (claimshome, j[0], j[1], j[2]))
            time.sleep(5)
        os.system(stopall%claimshome)
        time.sleep(5)

def main():
    if len(sys.argv)>1:
        if sys.argv[1]=="--help":
            print usage
        else:
            print "unrecognized option '%s'" % sys.argv[1]
            print "use the --help option for usage information"
    else:
	#print "time sleep 3600s"
	#time.sleep(3600)
	#print "start auto test"
        autotest()

if __name__=="__main__":
    main()

