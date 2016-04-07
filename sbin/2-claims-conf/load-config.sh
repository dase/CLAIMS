#!/bin/sh

#------------------------------------------------------------------------------  
# model: cfg_get  
# args: [1] => IN:<configure-file>  
#       [2] => IN:<key>  
#       [3] => OUT:<value-as-env-var>  
# describe: get configure value by key from a configure file  
# example:  
#   > cfg_get "cluster.config" "master" "master"  
#   > echo $master
#   file: 
#------------------------------------------------------------------------------  
# Usage: getcfg <in:file> <in:key> <out:valueENV>  
getcfg() {  
    export $3="`sed '/^\s*'$2'\s*=/!d;s/.*=\s*//' $1`"
}

function load_config()
{
    config=cluster.config
    getcfg $config master master
    getcfg $config slaves slaves
    getcfg $config claimshome claimshome 
    getcfg $config user user
    getcfg $config logpath logpath 
    getcfg $config runclaimsprocid runclaimsprocid
    getcfg $config data data
    getcfg $config hdfs_master_ip hdfs_master_ip
    getcfg $config hdfs_master_port hdfs_master_port
    getcfg $config max_degree_of_parallelism max_degree_of_parallelism 
    getcfg $config initial_degree_of_parallelism initial_degree_of_parallelism
    getcfg $config expander_adaptivity_check_frequency expander_adaptivity_check_frequency
    getcfg $config enable_expander_adaptivity enable_expander_adaptivity
    getcfg $config local_disk_mode local_disk_mode
    getcfg $config client_listener_port client_listener_port
    getcfg $config enable_codegen enable_codegen 
    getcfg $config load_thread_num load_thread_num
    getcfg $config memory_utilization memory_utilization

}

function currdir()
{
    CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
    cd $CURRDIR
}
 

function print_config()
{
echo "=========cluster config start========"
echo "config :           [$config]"
echo "master :           [$master]"
echo "slaves :           [$slaves]"
echo "claimshome :       [$claimshome]"
echo "user :             [$user]"
echo "logpath :          [$logpath]"
echo "runclaimsprocid :  [$runclaimsprocid]"
echo "data :             [$data]"
echo "hdfs_master_ip :   [$hdfs_master_ip]"
echo "hdfs_master_port : [$hdfs_master_port]"
echo "max_degree_of_parallelism :           [$max_degree_of_parallelism]"
echo "initial_degree_of_parallelism :       [$initial_degree_of_parallelism]"
echo "expander_adaptivity_check_frequency : [$expander_adaptivity_check_frequency]"
echo "enable_expander_adaptivity :          [$enable_expander_adaptivity]"
echo "local_disk_mode :                     [$local_disk_mode]"
echo "client_listener_port :                [$client_listener_port]"
echo "enable_codegen :                      [$enable_codegen]"
echo "load_thread_num :                     [$load_thread_num]"
echo "memory_utilization :                  [$memory_utilization]"
echo "=========cluster config end=========="
}

currdir
load_config
#print_config
