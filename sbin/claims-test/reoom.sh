#!/bin/sh  

CURRDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $CURRDIR
cd ../2-claims-conf/
source ./load-config.sh
source ./generate-config.sh
cd ../../
# now in CLAIMS_HOME

#allcluster="90s190 90s191 90s192 90s193 90s194 90s195 90s196 90s197 90s198 90s199"

for node in $master $slaves #$allcluster
do
{
  ssh -f -n -l $user $node "pgrep -f ./install/claimsserver | while read PID;do sudo echo -17 > /proc/\$PID/oom_adj;done"
  echo -e "$node claimsserver echo -17 to oom_adj [\033[32mOK\033[0m]"
}
done
