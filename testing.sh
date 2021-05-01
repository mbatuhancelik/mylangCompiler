echo compiling...
time make
echo compilation done.
echo testing...

n=19
for((counter=0; counter<$n; counter++))
do
echo " executing $counter"
time ./mylang2ir ./deliverables/inputs/testcase$counter.my
done

mkdir llvmoutputs

for((counter=0; counter<$n; counter++))
do
echo "\n executing ll codes $counter\n"
rm ./llvmoutputs/output$counter
touch ./llvmoutputs/output$counter
lli ./deliverables/inputs/testcase$counter.ll >> ./llvmoutputs/output$counter
done

for((counter=0; counter<$n; counter++))
do
echo " "
echo " checking $counter"
echo " "
diff ./deliverables/outputs/testcase$counter.txt ./llvmoutputs/output$counter
done



