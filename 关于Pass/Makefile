APEX=~/dachuang/apex
CFLAGS=-g -O0 -include ${APEX}/llvm/common.h -Wall -Werror=implicit-function-declaration -fno-builtin -std=c99 -Wno-unused-variable -Wno-unused-value -DNDEBUG
CLANG=clang
OPT=~/dachuang/llvm/build/bin/opt -load ~/dachuang/llvm/build/lib/LLVMHello.so
LLC=~/dachuang/llvm/build/bin/llc
LINK=~/dachuang/llvm/build/bin/llvm-link
DIS=~/dachuang/llvm/build/bin/llvm-dis
LD=gcc
CC=gcc
T=$(shell basename $$PWD)

.PHONY: all clean run build

all: build run
run: symb.out 
build: exec cfg.out  
clean:
	rm -f exec exec.org exec.asm src.o src.*.[so] *.bc *.ll *.out dot.[0123] cfg.out cfg.x stdout *.log

exec: src.3.o 
	${CC} $^ -o $@ 

asm.ll: src.3.bc
	${OPT} -stats -disasm $< -o /dev/null

src.1.bc: src.c
	${CLANG} -c -emit-llvm ${CFLAGS} $< -o $@

src.2.bc: src.1.bc
	${OPT} -constantexpr $< -o $@

src.3.bc: src.2.bc ${APEX}/llvm/instrument.bc
	${LINK}  $^ -o $@
	
src.4.bc: src.3.bc
	${DIS}  $<   
	${OPT}  -mystuff1 $< -o $@
	${OPT}  -mystuff2 $< -o $@
	

cfg.out: src.4.bc
	${DIS}  $<
	${APEX}/script/llvm_cfg.py

exec.out: exec input
	-./exec $$(cat input)> stdout

symb.out: exec exec.out cfg.out
	${APEX}/script/symbexec.py -out symb.out -cfg /home/txb/dachuang/apex/models/x01/cfg.out -llvm /home/txb/dachuang/apex/models/x01/llvm.out -exec /home/txb/dachuang/apex/models/x01/exec.out -args /home/txb/dachuang/apex/models/x01/args.out

input: ../input
	ln -fs $< $@

${APEX}/llvm/instrument.bc: ${APEX}/llvm/instrument.c
	${CLANG} -c -emit-llvm -c -o $@ $<

%.s: %.bc
	${LLC} -O0 $< -o $@
%.o: %.s
	${CC} -c $<

.DELETE_ON_ERROR:

