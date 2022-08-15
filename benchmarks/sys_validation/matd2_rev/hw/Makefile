# KERN=matd2.cpp top.c
# build: $(FILES)
# 	clang++ -Wall -fPIC -fno-rtti matd2.cpp -S -emit-llvm -o input.ll -fno-vectorize -fno-slp-vectorize -fno-unroll-loops 
# 	opt input.ll -load=./LLVMEnzyme-12.so -enzyme -o matd2.ll -S -loop-unroll 
	

# clean:
# 	rm -f *.ll
KERN=matd2.c top.c
build: $(FILES)
	clang++ -O1 -S -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN) -target x86_64
	opt matd2.ll -load=./LLVMEnzyme-12.so -enzyme -o matd2.ll -S -loop-unroll -mem2reg -always-inline
clean:
	rm -f *.ll
