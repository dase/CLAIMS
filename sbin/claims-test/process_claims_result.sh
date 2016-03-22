result=$1
file=`find ./testresult/${result} -maxdepth 1 -name "*.result"`
for i in $file
do
sed -i '1,10d'  $i
sed -i '$d' $i
sed -i '$d' $i
sed -i '$d' $i
sed -i '$d' $i
sed -i "/[>]/d " $i
sed -i "1,4d"  $i
sed -i 's/ //g' $i
sed -i "s/\.[0-9]*//g" $i
sed -i "s/|//g" $i
done
