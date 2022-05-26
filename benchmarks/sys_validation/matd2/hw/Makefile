KERN=matd2.c top.c
build: $(FILES)
	clang++ -O1 -S -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN) -target x86_64
	opt matd2.ll -o matd2.ll -S -loop-unroll -mem2reg -always-inline

clean:
	rm -f *.ll
