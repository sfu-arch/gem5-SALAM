# KERN=matd2.cpp top.c
# build: $(FILES)
# 	clang++ -Wall -fPIC -fno-rtti matd2.cpp -S -emit-llvm -o input.ll -fno-vectorize -fno-slp-vectorize -fno-unroll-loops 
# 	opt input.ll -load=./LLVMEnzyme-12.so -enzyme -o matd2.ll -S -loop-unroll 
	

# clean:
# 	rm -f *.ll
MAIN_FILE=alexnet
KERN=$(MAIN_FILE).c top.c
build: $(FILES)
	clang++ -O3 -S -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN) -target x86_64
	opt $(MAIN_FILE).ll -load=../../LLVMEnzyme-12.so -enzyme -o $(MAIN_FILE).ll -S -loop-unroll -mem2reg -always-inline -enable-bins -bin-size=10000
clean:
	rm -f *.ll
