echo compiling
time g++ *.cpp *.h -std=c++11
echo "testing"
mkdir myoutputs
time ./a.out ./deliverables/inputs/testcase0.my ./myoutputs/output0.ll
time ./a.out ./deliverables/inputs/testcase1.my ./myoutputs/output1.ll
time ./a.out ./deliverables/inputs/testcase2.my ./myoutputs/output2.ll
time ./a.out ./deliverables/inputs/testcase3.my ./myoutputs/output3.ll
time ./a.out ./deliverables/inputs/testcase4.my ./myoutputs/output4.ll
time ./a.out ./deliverables/inputs/testcase5.my ./myoutputs/output5.ll
time ./a.out ./deliverables/inputs/testcase6.my ./myoutputs/output6.ll
time ./a.out ./deliverables/inputs/testcase7.my ./myoutputs/output7.ll
time ./a.out ./deliverables/inputs/testcase8.my ./myoutputs/output8.ll
time ./a.out ./deliverables/inputs/testcase9.my ./myoutputs/output9.ll
time ./a.out ./deliverables/inputs/testcase10.my ./myoutputs/output10.ll
time ./a.out ./deliverables/inputs/testcase11.my ./myoutputs/output11.ll
time ./a.out ./deliverables/inputs/testcase12.my ./myoutputs/output12.ll
time ./a.out ./deliverables/inputs/testcase13.my ./myoutputs/output13.ll
time ./a.out ./deliverables/inputs/testcase14.my ./myoutputs/output14.ll
time ./a.out ./deliverables/inputs/testcase15.my ./myoutputs/output15.ll
time ./a.out ./deliverables/inputs/testcase16.my ./myoutputs/output16.ll
time ./a.out ./deliverables/inputs/testcase17.my ./myoutputs/output17.ll
time ./a.out ./deliverables/inputs/testcase18.my ./myoutputs/output18.ll

echo "output 0:"
lli ./myoutputs/output0.ll
echo "output 1:"
lli ./myoutputs/output1.ll
echo "output 2:"
lli ./myoutputs/output2.ll
echo "output 3:"
lli ./myoutputs/output3.ll
echo "output 4:"
lli ./myoutputs/output4.ll
echo "output 5:"
lli ./myoutputs/output5.ll
echo "output 6:"
lli ./myoutputs/output6.ll
echo "output 7:"
lli ./myoutputs/output7.ll
echo "output 8:"
lli ./myoutputs/output8.ll
echo "output 9:"
lli ./myoutputs/output9.ll
echo "output 10:"
lli ./myoutputs/output10.ll
echo "output 11:"
lli ./myoutputs/output11.ll
echo "output 12:"
lli ./myoutputs/output12.ll
echo "output 13:"
lli ./myoutputs/output13.ll
echo "output 14:"
lli ./myoutputs/output14.ll
echo "output 15:"
lli ./myoutputs/output15.ll
echo "output 16:"
lli ./myoutputs/output16.ll
echo "output 17:"
lli ./myoutputs/output17.ll
echo "output 18:"
lli ./myoutputs/output18.ll

