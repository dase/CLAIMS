#!/bin/sh
set -e
CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR

cd ../2-claims-conf/
source ./load-config.sh
cd ../../
# now in CLAIMS_HOME

# for debug begin #####
cd install
ulimit -c unlimited
cd ../
# for debug end #######
./install/claimsserver -c ./sbin/2-claims-conf/config-$master > /dev/null 2>&1 &
echo "[$(date '+%Y-%m-%d %H:%M:%S')] claimsserver=$!" >> guizhoutest.log
