---
layout: document
title: "Quick Start"
date: 2015-08-12 13:42:21
categories: claims
---

CLAIMS runs on clusters of shared-nothing servers, where each server hosts a single CLAIMS instance. One instance actors as the mater, while others are slavers. On each of the server, follow the instructions below to install and execute CLAIMS. Without otherwise specification, the operators on the master node and the slave nodes are the same (This allow you to use tools like cssh to facilitate your work).

Requirement
--

Operating system: CentOS 6.3 (x86_64) or above.

0.Create users
==

	    [root@node1 ~]$useradd claims
	    [root@node1 ~]$passwd claims

1.Install hadoop
==



CLAIMS leverages Hadoop Distributed File System (HDFS) for persistent data storage. To install Hadoop, refer to [Installation manual](wiki.apache.org/hadoop/#Setting_up_a_Hadoop_Cluster). Note that you must use claims user.

2.Install third party library
==

2.0 install numa library
--

The codes in CLAIMS are NUMA-aware, which requires numa libraries. Installing numa libraries is a must for CLAIMS compiling, even if the hardware is not under UNMA architectures. To install numa libraries, make sure your machines are connected to the Internet and use the yum command.

	    sudo yum install -y numa*

2.1 install boost library
--

We use several boost libraries to for serialization of execution query plan, catalog persistence, date_time, data type, etc. Download boost1.53.0 from [here](http://sourceforge.net/projects/boost/files/boost/1.53.0/). Move the downloaded file into ~/supports/ dir, and then use the following commands to install the boost components and set the corresponding environment for CLAIMS:

	    [claims@node1 supports]$ export BOOST_HOME=/home/claims/supports/boost_1_53_0
	    [claims@node1 supports]$ source ~/.bashrc
	    [claims@node1 supports]$ cd boost_1_53_0
	    [claims@node1 boost_1_53_0]$ ./bootstrap.sh --with libraries=serialization,system,date_time
	    [claims@node1 boost_1_53_0]$ ./bjam

or you can use yum to install the boost lib like:

	    [root@node1 supports]$yum install boost

2.2 install xs library
--

XS is a socket library which will be used by [Theron](https://github.com/dase/Claims/wiki), a C++ actor library. Please move the libxs-1.2.0.tar.gz from thirdparty into ~/supports/, and do the following steps:

	    [claims@node1 supports]$ tar zxvf libxs-1.2.0.tar.gz
	    [claims@node1 supports]$ cd libxs-1.2.0
	    [claims@node1 supports]$ ./configure
	    [claims@node1 libxs-1.2.0]$ make check
	    [claims@node1 libxs-1.2.0]$ su
	    [root@node1 libxs-1.2.0]$ make install
	    [root@node1 libxs-1.2.0]$ ldconfig

2.3 install theron library
--

[Theron](https://github.com/dase/Claims/wiki) is a high performance C++ actor library, which is widely used in CLAIMS for execution query plan shipping and communications across CLAIMS instances. Please move `Theron-5.01.01.zip` into supports/ dir, The following commands install Theron and set the environment.

	    [claims@node1 supports]$ export THERON_HOME=/home/claims/supports/Theron-5.01.01
	    [claims@node1 supports]$ source ~/.bashrc
	    [claims@node1 supports]$ cd Theron-5.01.01
	    [claims@node1 Theron-5.01.01]$ make xs=on mode=debug (if the make complaints "cannot find -ld_xs", don't worry and ignore this error)

2.4 install libconfig library
--

CLAIMS use libconfig to parse CLAIMS configuration file. Please move the libconfig-1.4.9 from thirdparty into ~/supports/, and do the following steps:

	    [claims@node1 supports]$ cd libconfig-1.4.9
	    [claims@node1 libconfig-1.4.9]$ ./configure
	    [claims@node1 libconfig-1.4.9]$ make
	    [claims@node1 libconfig-1.4.9]$ sudo make install

or you can use yum to install the libconfig lib like:

	    [root@node1 supports]$yum install libconfig

2.5 install gtest library
--

We use the gtest framework to test CLAIMS. Please move the gtest into supports/ dir, , and set it in ~/.bashrc as follows:

	    [claims@node1 supports]$ export GTEST_HOME=/home/claims/supports/gtest-1.7.0
	    [claims@node1 supports]$ source ~/.bashrc
	    [claims@node1~]$ cd gtest-1.7.0
	    [claims@node1 gtest-1.7.0]$ g++ -isystem include -I./ -pthread -c src/gtest-all.cc
	    [claims@node1 gtest-1.7.0]$ ar -rv libgtest.a gtest-all.o

3.Install CLAIMS
==

CLAIMS uses llvm, which requires the gcc 4.7.1 or above. To update gcc without effecting the entire system, follow [this](https://github.com/dase/Claims/wiki/Updating-gcc) to install devtoolset.

3.0 install claims
--

We use automake build system to build claims, if you have PR to submit, please refer to the following steps. But before you must successfully run the all tests. Enter the main dir of claims, and do the following steps:

	    [claims@node1 Claims]$ .build.sh init
	    [claims@node1 Claims]$ .configure --prefix=/home/claims/claims/Claims/install/
	    [claims@node1 Claims]$ make
	    [claims@node1 Claims]$ sudo make install

3.1 set environment
--

You must set up all relevant environment parameters, and set it in ~/.bashrc as follows:

	    [claims@node1 Claims]$ export CLAIMS_HOME=/home/claims/claims/Claims
	    [claims@node1 Claims]$ export PATH=$CLAIMS_HOME/install/bin:$PATH
	    [claims@node1 Claims]$ export LD_LIBRARY_PATH=$HADOOP_HOME/c++/Linux-amd64-64/lib:$BOOST_HOME/stage/lib:$JAVA_HOME/jre/lib/amd64/server:/usr/local/lib

3.2 configure the config file
--

copy config.template to config. and configure the config file according to your machine.

4.Run the first query
==

5.Appendix
==

5.0 ~/.bashrc file under claims user

you can refer to this file.

	    # .bashrc
	    # Source global definitions
	    if [ -f /etc/bashrc ]; then
	        . /etc/bashrc
	    Fi
	    
	    # User specific aliases and functions
	    #set java
	    export JAVA_HOME=/home/claims/java/jdk1.7.0_45
	    export PATH=$JAVA_HOME/bin:$JRE_HOME/bin:$PATH
	    export JRE_HOME=/home/claims/java/jdk1.7.0_45/jre
	    export CLASSPATH=.:$JAVA_HOME/lib:$JRE_HOME/lib:$CLASSPATH
	    #set hadoop
	    export HADOOP_HOME=/home/claims/hadoop/hadoop-1.0.3
	    export PATH=$HADOOP_HOME/bin:$PATH
	    export CLASSPATH=$HADOOP_HOME/lib/:$CLASSPATH
	    
	    #set THERON
	    export THERON_HOME=/home/claims/supports/Theron-5.01.01
	    
	    #set gtest
	    export GTEST_HOME=/home/claims/supports/gtest-1.7.0
	    
	    #set BOOST
	    export BOOST_HOME=/home/claims/supports/boost_1_53_0
	    
	    #set so
	    export LD_LIBRARY_PATH= $HADOOP_HOME/c++/Linux-amd64-64/lib: $BOOST_HOME/stage/lib:$JAVA_HOME/jre/lib/amd64/server:/usr/local/lib
	    
	    #set claims
	    export CLAIMS_HOME=/home/claims/claims/Claims/
	    export PATH=$CLAIMS_HOME/install/bin/:$PATH
	    
	    #set classpath
	    for i in $HADOOP_HOME/*.jar
	    do
		      CLASSPATH=$CLASSPATH:$i
	    done
	    for i in $HADOOP_HOME/lib/*.jar
	    do
		      CLASSPATH=$CLASSPATH:$i
	    done

5.1 config file
--

	    #local IP address
	    ip = "10.11.1.195";
	    
	    #port range
	    PortManager:
	    {
		      start = 19000;
		      end   = 19500;
	    }
	    
	    #IP address and port in master node
	    coordinator:
	    {
		      ip="10.11.1.195"
		      port="11001"
	    }
	    
	    #data dir in hdfs
	    data="/home/claims/data/"
	    
	    #master node of hdfs
	    hdfs_master_ip="10.11.1.190"
	    
	    #master node port of hdfs
	    hdfs_master_port=9000
	    
	    #max parallelism degree of single machine, set the value as many as the number of physical cores on the machine.
	    max_degree_of_parallelism=1
	    
	    #initial parallelism degree of single machine (optional, default = 1 )
	    initial_degree_of_parallelism=1

	    expander_adaptivity_check_frequency=1000 (optional, default = 1000 )

	    #whether enable the adaptivity of intra-node parallelism. 
	    enable_expander_adaptivity=0 (optional, default =0)

Notation:
==

If you have any questions, welcome to start a issue about your problem on Claims github homepage, we are very appreciate that.

	    If you have any question, do not hesitate to contract us. Email: wangli1426@gmail.com
