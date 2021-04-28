echo compiling...
time make
echo compilation done.
echo testing...
mkdir myoutputs
time ./mylang2ir ./deliverables/inputs/testcase0.my ./myoutputs/testcase0.ll
time ./mylang2ir ./deliverables/inputs/testcase1.my ./myoutputs/testcase1.ll
time ./mylang2ir ./deliverables/inputs/testcase2.my ./myoutputs/testcase2.ll
time ./mylang2ir ./deliverables/inputs/testcase3.my ./myoutputs/testcase3.ll
time ./mylang2ir ./deliverables/inputs/testcase4.my ./myoutputs/testcase4.ll
time ./mylang2ir ./deliverables/inputs/testcase5.my ./myoutputs/testcase5.ll
time ./mylang2ir ./deliverables/inputs/testcase6.my ./myoutputs/testcase6.ll
time ./mylang2ir ./deliverables/inputs/testcase7.my ./myoutputs/testcase7.ll
time ./mylang2ir ./deliverables/inputs/testcase8.my ./myoutputs/testcase8.ll
time ./mylang2ir ./deliverables/inputs/testcase9.my ./myoutputs/testcase9.ll
time ./mylang2ir ./deliverables/inputs/testcase10.my ./myoutputs/testcase10.ll
time ./mylang2ir ./deliverables/inputs/testcase11.my ./myoutputs/testcase11.ll
time ./mylang2ir ./deliverables/inputs/testcase12.my ./myoutputs/testcase12.ll
time ./mylang2ir ./deliverables/inputs/testcase13.my ./myoutputs/testcase13.ll
time ./mylang2ir ./deliverables/inputs/testcase14.my ./myoutputs/testcase14.ll
time ./mylang2ir ./deliverables/inputs/testcase15.my ./myoutputs/testcase15.ll
time ./mylang2ir ./deliverables/inputs/testcase16.my ./myoutputs/testcase16.ll
time ./mylang2ir ./deliverables/inputs/testcase17.my ./myoutputs/testcase17.ll
time ./mylang2ir ./deliverables/inputs/testcase18.my ./myoutputs/testcase18.ll

echo output0:
lli ./myoutputs/testcase0.ll
echo
echo output1:
lli ./myoutputs/testcase1.ll
echo
echo output2:
lli ./myoutputs/testcase2.ll
echo
echo output3:
lli ./myoutputs/testcase3.ll
echo
echo output4:
lli ./myoutputs/testcase4.ll
echo
echo output5:
lli ./myoutputs/testcase5.ll
echo
echo output6:
lli ./myoutputs/testcase6.ll
echo
echo output7:
lli ./myoutputs/testcase7.ll
echo
echo output8:
lli ./myoutputs/testcase8.ll
echo
echo output9:
lli ./myoutputs/testcase9.ll
echo
echo output10:
lli ./myoutputs/testcase10.ll
echo
echo output11:
lli ./myoutputs/testcase11.ll
echo
echo output12:
lli ./myoutputs/testcase12.ll
echo
echo output13:
lli ./myoutputs/testcase13.ll
echo
echo output14:
lli ./myoutputs/testcase14.ll
echo
echo output15:
lli ./myoutputs/testcase15.ll
echo
echo output16:
lli ./myoutputs/testcase16.ll
echo
echo output17:
lli ./myoutputs/testcase17.ll
echo
echo output18:
lli ./myoutputs/testcase18.ll
echo

echo testing done.