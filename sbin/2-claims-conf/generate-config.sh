#!/bin/sh

cd $CLAIMS_HOME/sbin/2-claims-conf
source ./load-config.sh

rm -f config-*

for slave in $slaves $master
do
if [ "$slave" = "$master" ]; then
 ismaster=1
else
 ismaster=0
fi

cat >> config-$slave << EOF
#本机IP地址
ip = "$slave";

#端口范围（调试用）
PortManager:
{
    start = 19000;
    end   = 19500;
}

#master的IP地址和端口
coordinator:
{
    ip="$master"
    port="11001"
}

# whether this instance is the master.
master = $ismaster

#hadoop上的数据目录
data = "$data"

#hdfs主节点
hdfs_master_ip = "10.11.1.192"

#hdfs主节点端口
hdfs_master_port = 9000

#最大单机算子并行度
max_degree_of_parallelism=10

#初始单机算子并行度
initial_degree_of_parallelism=2

expander_adaptivity_check_frequency=1000

enable_expander_adaptivity=0

# 0: hdfs
# 1: local
local_disk_mode = $local_disk_mode

client_listener_port = $client_listener_port

enable_codegen = 0

load_thread_num = 12

EOF
done

