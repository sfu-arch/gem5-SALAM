cd benchmarks/sys_validation/matd2_rev/hw
make -j;
cd ../../../..
# scons build/ARM/gem5.opt -j`nproc`; 
./systemValidation.sh -b matd2_rev 2> 1.txt