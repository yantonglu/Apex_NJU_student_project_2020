//===- Hello.cpp - Example code from "Writing an LLVM Pass" ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "Hello World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//
#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include <cstring>
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


using namespace llvm;

#define DEBUG_TYPE "hello"
static int id=1;
STATISTIC(HelloCounter, "Counts number of functions greeted");

bool is_array=false,is_uint8=false;

void print_global_variable(Module &M,raw_fd_ostream & out);
void print_function(Function *F,raw_fd_ostream & out);
void print_basicblock(BasicBlock *BB,raw_fd_ostream & out);
void print_instruction(Instruction *Inst,raw_fd_ostream &out);
void print_AllocaInst(Instruction* Inst,raw_fd_ostream &out);
void print_CallInst(Instruction* Inst,raw_fd_ostream &out);
void print_LocalVar(Instruction* Inst,raw_fd_ostream &out);
void print_BranchInst(Instruction* Inst,raw_fd_ostream &out);
void print_CmpInst(Instruction*Inst ,raw_fd_ostream &out);
void print_CastInst(Instruction*Inst ,raw_fd_ostream &out);
void print_BinopInst(Instruction*Inst ,raw_fd_ostream &out);
void print_StoreInst(Instruction*Inst ,raw_fd_ostream &out);

std::string Instname(Instruction * inst);
void PrintValueType(Type* type,raw_fd_ostream &out);



static Value** group;  //ID part
static int cur=0;
static int Num=0;
static int getID(Value* val,bool report=true);
static void check_repeat();
static void init(Module &M);
static bool check_pollute(Value* val);
static int is_arg(Function* F,Value* val);


namespace {
  // Hello - The first implementation, without getAnalysisUsage.
  struct mystuff1 : public ModulePass {
    static char ID; // Pass identification, replacement for typeid
    mystuff1() : ModulePass(ID) {}

    bool runOnModule(Module &M) override {
	
	std::error_code ec;
	StringRef pass="llvm.out";
	enum sys::fs::OpenFlags way=(enum sys::fs::OpenFlags)0;
	raw_fd_ostream out(pass,ec,way);
	//set the output path;
	init(M);
	check_repeat();


	print_global_variable(M,out); //print the globalvar

	for(llvm::Module::iterator func=M.begin();func!=M.end();++func)
	{
		Function *F=&* func;
		if(check_pollute((Value*)F))
			continue;
		print_function(F,out);
		
	for(llvm::Function::iterator bb = F->begin();bb != F->end();++bb) 
	{
		BasicBlock *BB=&*bb;	
		print_basicblock(BB,out);
		for (BasicBlock::InstListType::iterator inst = BB->begin();inst  != BB->end();++inst) 
	{
		Instruction *Inst=&*inst;
		
		print_instruction(Inst,out);
		
	}
	}
	}
}
  };
}

void PrintValueType(Type* type,raw_fd_ostream &out)
{   
	bool error=false;
	Type::TypeID type_id = type->getTypeID();    
	std::string data_type = "";    
	switch (type_id) 
	{        
		case Type::VoidTyID: data_type = "TypeVoid"; break;        
		case Type::HalfTyID: data_type = "TypeHalf"; break;        
		case Type::FloatTyID: data_type = "TypeFloat"; break;       
		case Type::DoubleTyID: data_type = "TypeDouble"; break;        
		case Type::FP128TyID: data_type = "TypeFP128";error=true; break;        
		case Type::PPC_FP128TyID: data_type = "TypePPC_FP128";error=true; break;        
		case Type::X86_MMXTyID: data_type = "TypeX86_MMX";error=true; break;        
		case Type::IntegerTyID: data_type = "TypeInt"; break;        
		case Type::FunctionTyID: data_type = "TypeFunction"; break;        
		case Type::StructTyID: data_type = "TypeStruct"; break;        
		case Type::ArrayTyID: data_type = "TypeArray"; break;        
		case Type::PointerTyID: data_type = "TypePointer"; break;         
		case Type::VectorTyID: data_type = "TypeVector"; break;       
		default:      error=true;data_type="unknown_type" ; break;    
	}
	if(error)
	{
		error=false;
		errs()<<"Type error type_name:"<<data_type<<" type_ID:"<<type_id<<"\n";
		
	}
	out<<"(";
	out << data_type;
	if(data_type=="TypePointer")
	{
		Type* sub_type=type->getPointerElementType();
		is_array=false;
		PrintValueType(sub_type,out);
	}else
	if(data_type=="TypeArray")
	{
		Type* sub_type=type->getArrayElementType();
		is_array=true;
		PrintValueType(sub_type,out);
		out<<" "<<type->getArrayNumElements();
	}else
	if(data_type=="TypeVector")
	{
		Type* sub_type=type->getVectorElementType();
		PrintValueType(sub_type,out);
		is_array=false;
		out<<" "<<type->getVectorNumElements();
	}else
	if(data_type=="TypeStruct")
	{
		unsigned num=type->getStructNumElements();
		for(int i=0;i<num;i++)
		{
			Type* sub_type=type->getStructElementType(i);
			PrintValueType(sub_type,out);
		}
		is_array=false;
		out<<" "<<type->getStructNumElements();
	}else   
	{
		if(is_array&&(type->getIntegerBitWidth()==8||type->getIntegerBitWidth()==32))
			is_uint8 =true;
		out <<" "<<type->getIntegerBitWidth();
	}
	
	out<<")";
}



void print_global_variable(Module &M,raw_fd_ostream & out)
{
	for (Module::global_iterator GVI = M.global_begin() ;GVI != M.global_end(); ++GVI ) {
		GlobalVariable *GV = &*GVI;
		if(check_pollute((Value*)GV))
			continue;
		Type *type=GV->getType();
		out<<"(";
		out<<"GlobalVar ";
		out<<getID((Value*)GV)<<" ";
		out<<"() ";


		//errs()<<GV->getValueID()<<" ";
		//errs()<<GV->getRawSubclassOptionalData()<<" ";
		//errs()<<GV<<" ";
		

		out<<"\""<<GV->getName()<<"\" ";
		PrintValueType(type,out);
		if(is_uint8)
		{
			is_uint8=false;is_array=false;
			out<<" ";
			out<<"(list ";
			 ConstantDataSequential *CDS =NULL;
			 CDS=cast<ConstantDataSequential>(GV->getInitializer());
			
			for (unsigned i = 0; i < CDS->getNumElements(); i++) 
			{
				out<<CDS->getElementAsInteger(i)<<" ";
			}
			out<<")";
		}
		else
		{
			Constant * constant=GV->getInitializer();
			if(llvm::ConstantInt *CI =dyn_cast<llvm::ConstantInt>((Value*)constant))
			if(CI->getBitWidth()<=32)
			{
				
				int number=CI->getSExtValue();
				out<<" "<<number<<" ";
			}
			else
			{
				long long int number=CI->getSExtValue();
				out<<" "<<number<<" ";
			}
			
		}
		
		out<<")";
		out<<"\n";
	}

}

void print_function(Function *F,raw_fd_ostream & out)
{
	Type *ret_type=F->getReturnType();
	out<<"(";
	out<<"Function ";
	
	out<<getID((Value*)F)<<" ";
	out<<"() ";
	out<<"\""<<F->getName()<<"\" ";
	PrintValueType(ret_type,out);
	//if(F->isVarArg())
	{
		//AttributeList attr_list=F->getAttributes();
		int counter=0;
		for(llvm::Function::arg_iterator it=F->arg_begin();it!=F->arg_end();it++)
		{
			out<<" (";
			out<<"FuncParam ";
			out<<counter<<" ";			
			Argument * arg=it;
			Type * type=arg->getType();
			out<<"\""<<arg->getName()<<"\" ";
			PrintValueType(type,out);
			out<<")";
			counter++;	
		}	
	}
	out<<")"<<"\n";

}


void print_basicblock(BasicBlock *BB,raw_fd_ostream & out)
{
	out<<"(";
	out<<"BasicBlock ";
	out<<getID((Value*)BB)<<" ";
	out<<"()";
	for(llvm::succ_iterator it = llvm::succ_begin(BB);it!=succ_end(BB);it++)
	{
		const BasicBlock *Succ = *it;
		out<<" (iref "<<getID((Value*)Succ)<<")";
	}
	out<<")\n";

}
std::string Instname(Instruction * inst)
{
	bool error=false;
	std::string name;
	switch (inst->getOpcode()) {
   	// Terminators
     case Instruction::Ret:    name="ReturnInst";break;
     case Instruction::Br:     name= "BranchInst";break;
     case Instruction::Switch: name="BranchInst";break;
     case Instruction::IndirectBr: error=true;name="indirectbr";break;
     case Instruction::Invoke: error=true;name="invoke";break;
     case Instruction::Resume: error=true;name="resume";break;
     case Instruction::Unreachable: error=true;name="unreachable";break;
     case Instruction::CleanupRet: error=true;name= "cleanupret";break;
     case Instruction::CatchRet: error=true;name= "catchret";break;
     case Instruction::CatchPad:error=true; name= "catchpad";break;
     case Instruction::CatchSwitch: error=true;name= "catchswitch";break;
     //case Instruction::CallBr: return "callbr";
   
     // Standard unary operators...
     //case Instruction::UnaryOps::FNeg: return "fneg";
   
     // Standard binary operators...
     case Instruction::Add: 
     case Instruction::FAdd: 
     case Instruction::Sub: 
     case Instruction::FSub: 
     case Instruction::Mul: 
     case Instruction::FMul:
     case Instruction::UDiv: 
     case Instruction::SDiv: 
     case Instruction::FDiv: 
     case Instruction::URem: 
     case Instruction::SRem: 
     case Instruction::FRem: 
  
    // Logical operators...
     case Instruction::And: 
    case Instruction::Or : 
     case Instruction::Xor: name= "BinopInst";break;
   
     // Memory instructions...
     case Instruction::Alloca:       name= "AllocaInst";break;
     case Instruction::Load:         name= "LoadInst";break;
     case Instruction::Store:        name= "StoreInst";break;
     case Instruction::AtomicCmpXchg:error=true; name= "cmpxchg";break;
     case Instruction::Instruction::AtomicRMW:error=true;     name= "atomicrmw";break;
     case Instruction::Fence:  error=true;       name= "fence";break;
     case Instruction::GetElementPtr: name= "GetPtrInst";break;
   
     // Convert instructions...
     case Instruction::Trunc:         name= "CastInst";break;
     case Instruction::ZExt:         
     case Instruction::SExt:          
     case Instruction::FPTrunc:      
     case Instruction::FPExt:        
     case Instruction::FPToUI:       
     case Instruction::FPToSI:       
     case Instruction::UIToFP:       
     case Instruction::SIToFP:       
     case Instruction::IntToPtr:     
     case Instruction::PtrToInt:      
     case Instruction::BitCast:       
     case Instruction::AddrSpaceCast: name= "CastInst";break;
   
     // Other instructions...
     case Instruction::ICmp:           name= "CmpInst";break;
     case Instruction::FCmp:  error=true;         name= "fcmp";break;
     case Instruction::PHI:            name= "PhiInst";break;
     case Instruction::Select:        name= "SelectInst";break;
    case Instruction::Call:           name= "CallInst";break;
     case Instruction::Shl:    error=true;        name= "shl";break;
     case Instruction::LShr:   error=true;        name= "lshr";break;
     case Instruction::AShr:   error=true;        name= "ashr";break;
     case Instruction::VAArg:  error=true;        name= "va_arg";break;
    case Instruction::ExtractElement:error=true; name= "extractelement";break;
    case Instruction::InsertElement: error=true; name= "insertelement";break;
    case Instruction::ShuffleVector: error=true; name= "shufflevector";break;
    case Instruction::ExtractValue:  error=true; name= "extractvalue";break;
     case Instruction::InsertValue:  error=true;  name= "insertvalue";break;
    case Instruction::LandingPad:    error=true; name= "landingpad";break;
    case Instruction::CleanupPad:    error=true; name= "cleanuppad";break;
   
   default:error=true; name= "Invalid operator";break;
	}
	if(error)
	{
		error=false;
		errs()<<"instruction error: instruction name is" <<name<<"\n"; 
	}
	return name;

}
void print_instruction(Instruction *Inst,raw_fd_ostream &out)
{
	std::string name=Instname(Inst);
	out<<"(";
	if(name=="CallInst")
	{
		CallInst* callinst=(CallInst*)Inst;
		Function* callfunc=callinst->getCalledFunction();
		if(callfunc->getName()=="llvm.dbg.declare")
		{	
			print_LocalVar(Inst,out);
			return;
		}
	}
	out<<name;
	out<<" "<<getID((Value*)Inst);
	out<<" (";
	if(Inst->hasMetadata())
	{
		out<<"DBG";
		if(MDNode *N=Inst->getMetadata("dbg"))
		{
			DILocation *Loc=(DILocation*)N;
			out<<" \""<<Loc->getFilename()<<"\"";
			out<<" "<<Loc->getLine();
			out<<" "<<Loc->getColumn();
		}
	}
	out<<") ";
	if(name=="AllocaInst")
		print_AllocaInst(Inst,out);
	else
	if(name=="CallInst")
		print_CallInst(Inst,out);
	else
	if(name=="BranchInst")
		print_BranchInst(Inst,out);
	else
	if(name=="CmpInst")
		print_CmpInst(Inst,out);
	else
	if(name=="CastInst")
		print_CastInst(Inst,out);
	else
	if(name=="BinopInst")
		print_BinopInst(Inst,out);
	else
	if(name=="StoreInst")
		print_StoreInst(Inst,out);
	else
	for(int i=0;i<Inst->getNumOperands();i++)
	{
		
		Value * op=Inst->getOperand(i);
		
		
		if(llvm::ConstantInt *CI =dyn_cast<llvm::ConstantInt>(op))
		{
			if(CI->getBitWidth()<=32)
			{
				int number=CI->getSExtValue();
				out<<" "<<number<<" ";
			}
			else
			{
				long long int number=CI->getSExtValue();
				out<<" "<<number<<" ";
			}
			
		}
		else
		{
			out<<"(iref "<< getID((Value*)op)<<")";		
		}
	}
	
	out<<")\n";
	

}


void print_StoreInst(Instruction*Inst ,raw_fd_ostream &out)
{
	StoreInst* inst=(StoreInst*) Inst;
	Value * ptr=inst->getPointerOperand();
	Value * val=inst->getValueOperand();
	out<<"(iref "<< getID(ptr)<<") ";
	
	if(getID(val,false)==-1 )
		if(llvm::ConstantInt *CI =dyn_cast<llvm::ConstantInt>(val))
		{	
			int number=CI->getSExtValue();
			out<<" "<<number<<" ";
		}
		else
		{
			Function* F=Inst->getFunction();
			int arg_id=is_arg(F,val);
			if(arg_id!=-1)
			{
				out<<"(arg "<< arg_id<<")";
			}
			else
			{
				getID(val);  
			}

		}
	else
	{
			out<<"(iref "<< getID(val)<<")";
	}	
}
void print_AllocaInst(Instruction* Inst,raw_fd_ostream &out)
{

	out<<"\""<<Inst->getName()<<"\"";
	out<<" ";
	
	PrintValueType(Inst->getType(),out);
	
	
	
}  


void print_LocalVar(Instruction* Inst,raw_fd_ostream &out)
{
	out<<"LocalVar";
	out<<" ";

	Metadata *Meta2 = cast<MetadataAsValue>(Inst->getOperand(1))->getMetadata();
	DIVariable * info = (DIVariable *)Meta2;

	Metadata *Meta = cast<MetadataAsValue>(Inst->getOperand(0))->getMetadata();
	Value *V;
	if (isa<ValueAsMetadata>(Meta)) 
	{
  		V = cast <ValueAsMetadata>(Meta)->getValue();
	}
	out<<getID(V)<<" ";
	out<<" \""<<info->getName()<<"\" ";
	
	Type* type=V->getType();
	if(Type::PointerTyID==type->getTypeID())
		PrintValueType(type->getPointerElementType(),out);
	else
		PrintValueType(V->getType(),out);
	out<<")";
	out<<"\n";
}


void print_CallInst(Instruction* Inst,raw_fd_ostream &out)
{
	CallInst* callinst=(CallInst*)Inst;
	Function* callfunc=callinst->getCalledFunction();
	out<<"(iref ";
	out<<getID((Value*)callfunc)<<")";

	for(int i=0;i<Inst->getNumOperands()-1;i++)
	{
		out<<"(CallParam";
		Value * op=Inst->getOperand(i);

		if(llvm::ConstantInt *CI =dyn_cast<llvm::ConstantInt>(op))
		{
			if(CI->getBitWidth()<=32)
			{
				int number=CI->getSExtValue();
				out<<" "<<number<<" ";
			}
			else
			{
				long long int number=CI->getSExtValue();
				out<<" "<<number<<" ";
			}
			PrintValueType(op->getType(),out);
		}
		else
		{
			out<<" (iref "<<getID(op)<<")";
			PrintValueType(op->getType(),out);		
		}
		out<<") ";
	}
	

}
void print_BranchInst(Instruction* Inst,raw_fd_ostream &out)
{  
	if(Inst->getNumOperands()==1)
		out<<" nil ";
	for(int i=0;i<Inst->getNumOperands();i++)
	{
		
		Value * op=Inst->getOperand(i);
		out<<"(iref "<<getID(op)<<")";
}
}


void print_CmpInst(Instruction*Inst ,raw_fd_ostream &out)
{
	CmpInst* cmp=(CmpInst*)Inst;
	int predicate=cmp->getPredicate();
	out<<" "<<predicate;

	for(int i=0;i<Inst->getNumOperands();i++)
	{
		
		Value * op=Inst->getOperand(i);
		
		
		if(llvm::ConstantInt *CI =dyn_cast<llvm::ConstantInt>(op))
		{
			if(CI->getBitWidth()<=32)
			{
				int number=CI->getSExtValue();
				out<<" "<<number;
			}
			else
			{
				long long int number=CI->getSExtValue();
				out<<" "<<number;
			}
		}
		else
		{
			out<<"(iref "<<getID(op)<<")";		
		}
	}

}

void print_CastInst(Instruction*Inst ,raw_fd_ostream &out)
{
		Value * op=Inst->getOperand(0);
		out<<" ";
		
		PrintValueType(op->getType(),out);
		PrintValueType(Inst->getType(),out);
		out<<"(iref "<<getID(op)<<")";		
	
}
void print_BinopInst(Instruction*Inst ,raw_fd_ostream &out)
{
	BinaryOperator* biop=(BinaryOperator*)Inst;
	out<<" "<<(int)(biop->getOpcode())-3;
	for(int i=0;i<Inst->getNumOperands();i++)
	{
		
		Value * op=Inst->getOperand(i);
		
		
		if(llvm::ConstantInt *CI =dyn_cast<llvm::ConstantInt>(op))
		{
			if(CI->getBitWidth()<=32)
		  	{
				int number=CI->getSExtValue();
				out<<" "<<number;
			}
			else
		  	{
				long long int number=CI->getSExtValue();
				out<<" "<<number;
			}
		}
		else
		{
			out<<"(iref "<< getID((Value*)op)<<")";		
		}
	}
}






void init(Module &M)
{
	int num=0;
	for (Module::global_iterator GVI = M.global_begin() ;GVI != M.global_end(); ++GVI )
		num++;
	for(llvm::Module::iterator func=M.begin();func!=M.end();++func)
	{

		Function *F=&* func;
		num++;
		
		for(llvm::Function::iterator bb = F->begin();bb != F->end();++bb) 
		{
		BasicBlock *BB=&*bb;	
		num++;
		for (BasicBlock::InstListType::iterator inst = BB->begin();inst  != BB->end();++inst) 
		{
		Instruction *Inst=&*inst;
		
		num++;
		
		}
		}
	}
	Num=num+200;
	group=new Value*[Num];
	Value* tmp;
	for (Module::global_iterator GVI = M.global_begin() ;GVI != M.global_end(); ++GVI )
	{
		GlobalVariable *GV = &*GVI;
		
		tmp=(Value*)GV;
		if(check_pollute(tmp))
			continue;		
		group[cur]=tmp;
		cur++;
	}


	for(llvm::Module::iterator func=M.begin();func!=M.end();++func)
	{

		Function *F=&* func;
		tmp=(Value*)F;
		if(check_pollute(tmp))
			continue;	
		group[cur]=tmp;
		cur++;
		
		for(llvm::Function::iterator bb = F->begin();bb != F->end();++bb) 
		{
		BasicBlock *BB=&*bb;	
		tmp=(Value*)BB;
		group[cur]=tmp;
		cur++;
		for (BasicBlock::InstListType::iterator inst = BB->begin();inst  != BB->end();++inst) 
		{
		Instruction *Inst=&*inst;
		tmp=(Value*)Inst;
		group[cur]=tmp;
		cur++;
		num++;
		
		}
		}
	}
}
int getID(Value* val,bool report)
{
	for(int i=0;i<Num;i++)
	{
		if(val==group[i])
			return i;
	}
	if(report)
		errs()<<"id error: "<<*val<<"\n";
	return -1;
}

void check_repeat()
{
	
	for(int i=0;i<cur;i++)
	{
		for(int j=i+1;j<cur;j++)
		{
			if(group[i]==group[j])
			{
				errs()<<"ID repeat error *******\n";
				return;
			}
		}
	}

	
}


int is_arg(Function* F,Value* val)
{
	int order=0;
	for(llvm::Function::arg_iterator it=F->arg_begin();it!=F->arg_end();it++)
		{	
			Argument * arg=it;
			if((Value*)arg==val)
				return order;
			order++;	
		}	
	return -1;
}
bool check_pollute(Value* val)
{
	if (val->getName() == "__instrument1"
		|| val->getName() == "__instrument2"
		|| val->getName() == "__main_args"
		|| val->getName() == "args_avoid_crash"
		|| val->getName() == "argc_avoid_crash"
		|| val->getName() == "argv_avoid_crash"
		|| val->getName() == "fpexec_avoid_crash"
		|| val->getName() == "fpargs_avoid_crash"
		|| val->getName() == "struct._IO_FILE"
		|| val->getName() == "struct._IO_marker"
		|| val->getName() == "fopen"
		|| val->getName() == "fprintf"
		|| val->getName() == "fflush"
		|| val->getName() == "llvm.dbg.declare"
		|| val->getName() == "llvm.dbg.value"
		|| val->getName() == "fpvars_avoid_crash"
		|| val->getName() == "fpinput_avoid_crash"
		|| val->getName() == "memory_avoid_crash"
		|| val->getName() == "INPUT_CHAR_ARRAY"
		|| val->getName() == "INPUT_VARIABLE"
		|| val->getName() == "INPUT_ARRAY"
		|| val->getName() == "INPUT_MATRIX"
		|| val->getName() == "__my_malloc"
		|| val->getName() == "INTERNAL_ARRAY"
		|| val->getName() == "INTERNAL_VARIABLE"
		|| val->getName() == "fgetc"
		|| val->getName() == "__isoc99_fscanf"
		|| val->getName() == "errno"
		|| val->getName() == "pow"
		|| val->getName() == "DESCRIPTION"
		|| val->getName() == "END_DESCRIPTION"
		
  
		)
		return true;
	else
		return false;
}

char mystuff1::ID = 0;
static RegisterPass<mystuff1> X("mystuff1", "Hello World Pass");
