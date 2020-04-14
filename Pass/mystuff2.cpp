#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/IR/LLVMContext.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/DerivedTypes.h"
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>

using namespace llvm;

#define DEBUG_TYPE "hello"
static int id = 1;
STATISTIC(HelloCounter, "Counts number of functions greeted");

void insert_globalvar(Module &M);
//void insert_func(Module &M, Function *F);
void insert_basicblock(Module &M, BasicBlock *BB);
void insert_ret(Module &M, BasicBlock *BB, Value *v, BasicBlock::iterator BI);
void insert_main_args1(Module &M, Function *F);
void insert_main_args2(Module &M, Function *F);
void insert_localvar(Module &M, BasicBlock *BB, Value *v, BasicBlock::iterator BI);
void insert_ret_and_store_inst(Module &M, BasicBlock *BB, Instruction *v, BasicBlock::iterator BI);
void insert_before_call(Module &M, BasicBlock *BB, Instruction *v, BasicBlock::iterator BI);


//ID part
extern int getID(Value* val,bool report=true);
extern void init(Module &M);
extern bool check_pollute(Value* val);
extern int is_arg(Function* F,Value* val);


namespace
{
struct mystuff2 : public ModulePass
{
	static char ID; // Pass identification, replacement for typeid
	mystuff2() : ModulePass(ID) {}

	bool runOnModule(Module &M) override
	{
		init(M);

		//errs() << "初始化完成\n";
		//errs() << "开始遍历函数\n";
		for (llvm::Module::iterator func = M.begin(); func != M.end(); ++func)
		{
			Function *F = &*func;
			//errs() << "进入 " << F->getName() << "\n";

			if (F->getName() == "__instrument1" 
				|| F->getName() == "__instrument2" 
				|| F->getName() == "__main_args"
				|| F->getName() == "pow"
				|| F->getName() == "INPUT_CHAR_ARRAY"
				|| F->getName() == "INPUT_VARIABLE"
				|| F->getName() == "INPUT_ARRAY"
				|| F->getName() == "INPUT_MATRIX"
				|| F->getName() == "INTERNAL_ARRAY"
				|| F->getName() == "INTERNAL_VARIABLE"
				|| F->getName() == "DESCRIPTION"
				|| F->getName() == "END_DESCRIPTION"
				|| F->getName() == "__my_malloc"
				) //函数排除表
			{
				//errs() << "引入的函数，取消插入\n";
				continue;
			}

			if (F->begin() == F->end())
			{
				//errs() << "找不到指令的外部函数，取消插入\n";
				continue;
			}
		
			

			if (F->getName() == "main")
			{
				if(F->arg_begin()!=F->arg_end())
				{
				//errs() << "插入函数参数\n";
				insert_main_args1(M, F);
				insert_main_args2(M, F);
				//errs() << "函数参数插入完成\n";
				}
				//errs() << "开始插入全局变量\n";
				insert_globalvar(M);
				//errs() << "全局变量插入完成\n";
			}

			//errs() << "开始遍历基本块\n";

			for (llvm::Function::iterator bb = F->begin(); bb != F->end(); ++bb)
			{
				BasicBlock *BB = &*bb;
				insert_basicblock(M, BB);
				//errs() << "开始遍历指令\n";

				for (BasicBlock::InstListType::iterator inst = BB->begin(); inst != BB->end(); ++inst)
				{
					Instruction *Inst = &*inst;

					if (Inst->getOpcode() == Instruction::Ret)
					{
						//errs() << "插入中间变量\n";
						insert_ret_and_store_inst(M, BB, Inst, inst);
						//errs() << "插入中间变量成功\n";
					}
					else if(Inst->getOpcode() == Instruction::Store)
					{
						//errs() << "插入中间变量\n";
						insert_ret_and_store_inst(M, BB, Inst, inst);
						//errs() << "插入中间变量成功\n";
					}
					else if (Inst->getOpcode() != Instruction::Call 
					&& getID(Inst, false) != -1)//指令排除表
					{
						//errs() << "插入中间变量\n";
						insert_localvar(M, BB, Inst, ++inst);
						--inst;
						//errs() << "插入中间变量成功\n";
					}
				}

				for (BasicBlock::InstListType::iterator inst = BB->begin(); inst != BB->end(); ++inst)
				{
					Instruction *Inst = &*inst;

					if (Inst->getOpcode() == Instruction::Call)
					{
						//errs() << "Call指令\n";
						CallInst *callinst = (CallInst *)Inst;
						Function *callfunc = callinst->getCalledFunction();
						if (callfunc == NULL)
							continue;
						if (callfunc->getName() == "__instrument1" 
							|| callfunc->getName() == "__instrument2" 
							|| callfunc->getName() == "__main_args"
							|| callfunc->getName() == "pow"
							|| callfunc->getName() == "INPUT_CHAR_ARRAY"
							|| callfunc->getName() == "INPUT_VARIABLE"
							|| callfunc->getName() == "INPUT_ARRAY"
							|| callfunc->getName() == "INPUT_MATRIX"
							|| callfunc->getName() == "INTERNAL_ARRAY"
							|| callfunc->getName() == "INTERNAL_VARIABLE"
							|| callfunc->getName() == "DESCRIPTION"
							|| callfunc->getName() == "END_DESCRIPTION"
							|| callfunc->getName() == "__my_malloc"
							|| callfunc->getName() == "llvm.dbg.declare") //函数排除表
						{
							//errs() << "引入的函数调用，取消插入返回值\n";
							continue;
						}
						//errs() << "在Call前插入\n";
						insert_before_call(M, BB, callinst, inst);
						//errs() << "Call前成功\n";

						//errs() << "插入返回值\n";
						insert_ret(M, BB, callinst, ++inst);
						--inst;
						//errs() << "插入返回值成功\n";
					}
				}
			}
		}
		return 1;
	}
};
} // namespace

int value = 0;

void insert_globalvar(Module &M)
{
	Function *func_instrument = M.getFunction("__instrument1");
	Function *func_main = M.getFunction("main");
	Function::iterator BB = func_main->begin();
	BasicBlock::iterator BI = BB->begin();
	IRBuilder<> Builder((BasicBlock *)&*BB);
	for (Module::global_iterator GVI = M.global_begin(); GVI != M.global_end(); ++GVI)
	{
		GlobalVariable *GV = &*GVI;
		if (GV->getName() == "fpexec_avoid_crash" 
		 || GV->getName() == "fpargs_avoid_crash" 
		 || GV->getName() == "fpvars_avoid_crash" 
		 || GV->getName() == "fpinput_avoid_crash" 
		 || GV->getName() == "args_avoid_crash" 
		 || GV->getName() == "argc_avoid_crash" 
		 || GV->getName() == "argv_avoid_crash"
		 || GV->getName() == "memory_avoid_crash"
		 || GV->getName() == "__PRETTY_FUNCTION__.__my_malloc"
		 || GV->getName() == "errno") //全局变量排除表
			continue;

		//errs() << "正在插入全局变量 " << GV->getName() << "\n";
		Instruction *ptrtointInst = PtrToIntInst::Create(Instruction::PtrToInt, (Value *)GV, Builder.getInt64Ty());
		Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(GV)), (Value *)ptrtointInst}, "");
		BB->getInstList().insert(BI, ptrtointInst);
		BB->getInstList().insert(BI, callInst);
	}
}

void insert_basicblock(Module &M, BasicBlock *BB)
{
	Function *func_instrument = M.getFunction("__instrument1");
	BasicBlock::iterator BI = BB->begin();
	if(BB->begin()->getOpcode() == Instruction::PHI)
		BI++;
	IRBuilder<> Builder((BasicBlock *)&*BB);
	Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(BB)), Builder.getInt64(0)}, "");
	BB->getInstList().insert(BI, callInst);
}

void insert_ret(Module &M, BasicBlock *BB, Value *v, BasicBlock::iterator BI)
{
	Function *func_instrument = M.getFunction("__instrument2");
	IRBuilder<> Builder((BasicBlock *)&*BB);
	if (v->getType() == Builder.getInt64Ty())
	{
		Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), v}, "");
		BB->getInstList().insert(BI, callInst);
	}
	else if (v->getType()->isIntegerTy())
	{
		Instruction *zextInst = ZExtInst::Create(Instruction::ZExt, v, Builder.getInt64Ty());
		Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), (Value *)zextInst}, "");
		BB->getInstList().insert(BI, zextInst);
		BB->getInstList().insert(BI, callInst);
	}
	else
	{
		Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), Builder.getInt64(0)}, "");
		BB->getInstList().insert(BI, callInst);
	}
}

void insert_main_args1(Module &M, Function *F)
{
	Function *func_main_args = M.getFunction("__main_args");
	Function::iterator BB = F->begin();
	BasicBlock::iterator BI = BB->begin();
	IRBuilder<> Builder((BasicBlock *)&*BB);
	Instruction *bitcastInst = BitCastInst::Create(Instruction::BitCast, (Value *)(F->arg_begin()+1), Builder.getInt8PtrTy());
	Instruction *callInst = CallInst::Create(func_main_args, ArrayRef<Value *>{(Value *)bitcastInst}, "");
	BB->getInstList().insert(BI, bitcastInst);
	BB->getInstList().insert(BI, callInst);
}

void insert_main_args2(Module &M, Function *F)
{
	Function *func_main_args = M.getFunction("__main_args");
	Function::iterator BB = F->begin();
	BasicBlock::iterator BI = BB->begin();
	IRBuilder<> Builder((BasicBlock *)&*BB);
	Instruction *inttoptrInst = IntToPtrInst::Create(Instruction::IntToPtr, (Value *)F->arg_begin(), Builder.getInt8PtrTy());
	Instruction *callInst = CallInst::Create(func_main_args, ArrayRef<Value *>{(Value *)inttoptrInst}, "");
	BB->getInstList().insert(BI, inttoptrInst);
	BB->getInstList().insert(BI, callInst);
}

void insert_localvar(Module &M, BasicBlock *BB, Value *v, BasicBlock::iterator BI)
{
	Function *func_instrument = M.getFunction("__instrument1");
	IRBuilder<> Builder((BasicBlock *)&*BB);
	if (v->getType()->isIntegerTy())
	{
		if (v->getType() == Builder.getInt64Ty())
		{
			Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), v}, "");
			BB->getInstList().insert(BI, callInst);
		}
		else
		{
			Instruction *zextInst = ZExtInst::Create(Instruction::ZExt, v, Builder.getInt64Ty());
			Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), zextInst}, "");
			BB->getInstList().insert(BI, zextInst);
			BB->getInstList().insert(BI, callInst);
		}
	}
	else if (v->getType()->isPointerTy())
	{
		Instruction *ptrtointInst = PtrToIntInst::Create(Instruction::PtrToInt, v, Builder.getInt64Ty());
		Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), (Value *)ptrtointInst}, "");
		BB->getInstList().insert(BI, ptrtointInst);
		BB->getInstList().insert(BI, callInst);
	}
}

void insert_ret_and_store_inst(Module &M, BasicBlock *BB, Instruction *v, BasicBlock::iterator BI)
{
	Function *func_instrument = M.getFunction("__instrument1");
	IRBuilder<> Builder((BasicBlock *)&*BB);

	if (v->getOpcode() == Instruction::Ret && ((ReturnInst*)v)->getReturnValue() == NULL)
	{
		Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), Builder.getInt64(0)}, "");
		BB->getInstList().insert(BI, callInst);
	}
	else if (v->getOperand(0)->getType()->isIntegerTy())
	{
		if (v->getOperand(0)->getType() == Builder.getInt64Ty())
		{
			Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), v->getOperand(0)}, "");
			BB->getInstList().insert(BI, callInst);
		}
		else
		{
			Instruction *zextInst = ZExtInst::Create(Instruction::ZExt, v->getOperand(0), Builder.getInt64Ty());
			Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), zextInst}, "");
			BB->getInstList().insert(BI, zextInst);
			BB->getInstList().insert(BI, callInst);
		}
	}
	else if (v->getOperand(0)->getType()->isPointerTy())
	{
		//这里可能有问题
		Instruction *ptrtointInst = PtrToIntInst::Create(Instruction::PtrToInt, v->getOperand(0), Builder.getInt64Ty());
		Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), (Value *)ptrtointInst}, "");
		BB->getInstList().insert(BI, ptrtointInst);
		BB->getInstList().insert(BI, callInst);
	}
	
}

void insert_before_call(Module &M, BasicBlock *BB, Instruction *v, BasicBlock::iterator BI)
{
	Function *func_instrument = M.getFunction("__instrument1");
	IRBuilder<> Builder((BasicBlock *)&*BB);
	Instruction *callInst = CallInst::Create(func_instrument, ArrayRef<Value *>{Builder.getInt64(getID(v)), Builder.getInt64(0)}, "");
	BB->getInstList().insert(BI, callInst);
}


char mystuff2::ID = 0;
static RegisterPass<mystuff2> X("mystuff2", "Hello World Pass");

