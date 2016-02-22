file=`find ./hawq_r -maxdepth 1 -name "*.result"`
for i in $file
do
sed -i '1,2d'  $i
sed -i '$d' $i
sed -i '$d' $i
sed -i 's/ //g' $i
sed -i "s/\.[0-9]*//g" $i
sed -i "s/|//g" $i
done
