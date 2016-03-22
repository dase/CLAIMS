#!/bin/sh
set -e
cd $CLAIMS_HOME/sbin/2-claims-conf/
source ./load-config.sh

# for debug begin #####
cd $CLAIMS_HOME
cd install
ulimit -c unlimited
# for debug end #######

$CLAIMS_HOME/install/client $master $client_listener_port 
