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


# echo output0:
# lli ./myoutputs/testcase0.ll
# echo
# echo output1:
# lli ./myoutputs/testcase1.ll
# echo
# echo output2:
# lli ./myoutputs/testcase2.ll
# echo
# echo output3:
# lli ./myoutputs/testcase3.ll
# echo
# echo output4:
# lli ./myoutputs/testcase4.ll
# echo
# echo output5:
# lli ./myoutputs/testcase5.ll
# echo
# echo output6:
# lli ./myoutputs/testcase6.ll
# echo
# echo output7:
# lli ./myoutputs/testcase7.ll
# echo
# echo output8:
# lli ./myoutputs/testcase8.ll
# echo
# echo output9:
# lli ./myoutputs/testcase9.ll
# echo
# echo output10:
# lli ./myoutputs/testcase10.ll
# echo
# echo output11:
# lli ./myoutputs/testcase11.ll
# echo
# echo output12:
# lli ./myoutputs/testcase12.ll
# echo
# echo output13:
# lli ./myoutputs/testcase13.ll
# echo
# echo output14:
# lli ./myoutputs/testcase14.ll
# echo
# echo output15:
# lli ./myoutputs/testcase15.ll
# echo
# echo output16:
# lli ./myoutputs/testcase16.ll
# echo
# echo output17:
# lli ./myoutputs/testcase17.ll
# echo
# echo output18:
# lli ./myoutputs/testcase18.ll
# echo

# echo testing done.
