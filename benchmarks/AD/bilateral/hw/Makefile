# KERN=matd2.cpp top.c
# build: $(FILES)
# 	clang++ -Wall -fPIC -fno-rtti matd2.cpp -S -emit-llvm -o input.ll -fno-vectorize -fno-slp-vectorize -fno-unroll-loops 
# 	opt input.ll -load=./LLVMEnzyme-12.so -enzyme -o matd2.ll -S -loop-unroll 
	

# clean:
# 	rm -f *.ll
N?=12
BIN_SIZE?=2
MAIN_FILE=bilateral
KERN=$(MAIN_FILE).c top.c
build: $(FILES)
	clang++ -O2 -S -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN) -target x86_64 -DN=$(N)
	opt $(MAIN_FILE).ll -load=../../LLVMEnzyme-12.so -enzyme -o $(MAIN_FILE).ll -S -loop-unroll -mem2reg -always-inline -enable-bins -bin-size=$(BIN_SIZE)
clean:
	rm -f *.ll
