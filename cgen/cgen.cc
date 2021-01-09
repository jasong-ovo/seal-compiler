
//**************************************************************
//
// Code generator SKELETON
//
//
//**************************************************************

#include "cgen.h"
#include "cgen_gc.h"
#include <iostream>


using namespace std;

extern void emit_string_constant(ostream& str, char *s);
extern int cgen_debug;
//try!!!


static char *CALL_REGS[] = {RDI, RSI, RDX, RCX, R8, R9};
static char *CALL_XMM[] = {XMM0, XMM1, XMM2, XMM3};

void cgen_helper(Decls decls, ostream& s);
void code(Decls decls, ostream& s);

//////////////////////////////////////////////////////////////////
//
//
//    Helper Functions
//  
//
//////////////////////////////////////////////////////////////////

// you can add any helper functions here
const char* pos(int pos_num) {
  string pos=".POS"+to_string(pos_num);
  return pos.c_str();
}

//将字符串组成的float转换成机器中存储的float
double str2double(char* str) {
  double x=0;
  int flag;//小数点的位置
  int len=0;
  for (int i=0;str[i]!='\0';i++){
    if (str[i]!='.')
          {x=x*10+int(str[i])-48;
          }
    else {flag=i;}
    len+=1;
  }
  int j=len-flag-1;//需要x0.1的次数
  while (j>0) {
    x*=0.1;
    j-=1;
  }
  return x;

}

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print,
    formal0,//rdi
    formal1,//rsi
    formal2,//rdx
    formal3,//rcx
    left_op,//左操作数
    right_op//右操作数
    ;

typedef SymbolTable<Symbol,int> objectEnv;
objectEnv obj_loc_table;//todao global var
objectEnv formal_table;//函数调用时保持的形参地址表
objectEnv op_table;//操作符保持的左右地址表
int position=0;//在整个生成代码的意义下的pos段
int order;//记录当前是第几个形参

int rsp_loc;//始终记录当前rsp的相对偏移

//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    // 4 basic types and Void type
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    // main function
    Main        = idtable.add_string("main");

    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");
    formal0     = idtable.add_string("formal0");
    formal1     = idtable.add_string("formal1");
    formal2     = idtable.add_string("formal2");
    formal3     = idtable.add_string("formal3");
    left_op     = idtable.add_string("left");
    right_op    = idtable.add_string("right");
}


//*********************************************************
//
// Define method for code generation
//
//
//*********************************************************

void Program_class::cgen(ostream &os) 
{
  // spim wants comments to start with '#'
  os << "# start of generated code\n";
  os << "\t.section"<<"\t\t.rodata"<<endl;

  initialize_constants();
  cgen_helper(decls,os);

  os << "\n# end of generated code\n";
}


//////////////////////////////////////////////////////////////////////////////
//
//  emit_* procedures
//
//  emit_X  writes code for operation "X" to the output stream.
//  There is an emit_X for each opcode X, as well as emit_ functions
//  for generating names according to the naming conventions (see emit.h)
//  and calls to support functions defined in the trap handler.
//
//  Register names and addresses are passed as strings.  See `emit.h'
//  for symbolic names you can use to refer to the strings.
//
//////////////////////////////////////////////////////////////////////////////

static void emit_mov(const char *source, const char *dest, ostream& s)
{
  s << MOV << source << COMMA << dest << endl;
}

static void emit_rmmov(const char *source_reg, int offset, const char *base_reg, ostream& s)
{
  s << MOV << source_reg << COMMA << offset << "(" << base_reg << ")"
      << endl;
}

static void emit_mrmov(const char *base_reg, int offset, const char *dest_reg, ostream& s)
{
  s << MOV << offset << "(" << base_reg << ")" << COMMA << dest_reg  
      << endl;
}

static void emit_irmov(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOV << "$" << immidiate << COMMA << dest_reg  
      << endl;
}

static void emit_irmov_num(int num, const char *dest_reg, ostream& s)
{
  s << MOV << "$" << char(num) << COMMA << dest_reg  
      << endl;
}

static void emit_irmov_float(unsigned long num, const char *dest_reg, ostream& s)
{
  s << MOV << "$" << num << COMMA << dest_reg  
      << endl;
}

static void emit_irmovl(const char *immidiate, const char *dest_reg, ostream& s)
{
  s << MOVL << "$" << immidiate << COMMA << dest_reg  
      << endl;
}

static void emit_irmovl_num(int num, const char *dest_reg, ostream& s)
{
  s << MOVL << "$" << num << COMMA << dest_reg  
      << endl;
}

static void emit_immov(const char *immidiate, int offset, const char *base_reg, ostream& s)
{
  s << MOV << "$" << immidiate << COMMA << offset<<"(" << base_reg << ")"  
      << endl;
}

static void emit_immov_str(int index, int offset, const char *base_reg, ostream& s)
{
  s << MOV << "$" << ".LC"<< index << COMMA <<offset << "(" << base_reg   << ")"
      << endl;
}

static void emit_immov_float(long unsigned int num, int offset, const char *base_reg, ostream& s)
{
  s << MOV << "$" << num << COMMA << offset<<"(" << base_reg << ")"  
      << endl;
}


static void emit_add(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADD << source_reg << COMMA << dest_reg << endl;
}

static void emit_sub(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUB << source_reg << COMMA << dest_reg << endl;
}

static void emit_mul(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MUL << source_reg << COMMA << dest_reg << endl;
}

static void emit_div(const char *dest_reg, ostream& s)
{
  s << DIV << dest_reg << endl;
}

static void emit_cqto(ostream &s)
{
  s << CQTO << endl;
}

static void emit_neg(const char *dest_reg, ostream& s)
{
  s << NEG << dest_reg << endl;
}

static void emit_and(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << AND << source_reg << COMMA << dest_reg << endl;
}

static void emit_or(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << OR << source_reg << COMMA << dest_reg << endl;
}

static void emit_xor(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XOR << source_reg << COMMA << dest_reg << endl;
}

static void emit_not(const char *dest_reg, ostream& s)
{
  s << NOT << " " << dest_reg << endl;
}

static void emit_movsd(const char *source, const char *dest, ostream& s)
{
  s << MOVSD << source << COMMA << dest << endl;
}

static void emit_movsd_rbp(int num, const char *dest, ostream& s)
{
  s << MOVSD << num <<"(%rbp)" << COMMA << dest << endl;
}


static void emit_movaps(const char *source, const char *dest, ostream& s)
{
  s << MOVAPS << source << COMMA << dest << endl;
}

static void emit_addsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << ADDSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_subsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << SUBSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_mulsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << MULSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_divsd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << DIVSD << source_reg << COMMA << dest_reg << endl;
}

static void emit_cmp(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << CMP << source_reg << COMMA << dest_reg << endl;
}

static void emit_test(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << TEST << source_reg << COMMA << dest_reg << endl;
}

static void emit_ucompisd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << UCOMPISD << source_reg << COMMA << dest_reg << endl;
}

static void emit_xorpd(const char *source_reg, const char *dest_reg, ostream& s)
{
  s << XORPD << source_reg << COMMA << dest_reg << endl;
}
// static void emit_jmp(const char *dest, ostream& s)
// {
//   s << JMP << " " << dest << endl;
// }

static void emit_jmp(int dest, ostream& s)
{
  s << JMP << " " << pos(dest) << endl;
}

// static void emit_jl(const char *dest, ostream& s)
// {
//   s << JL << " " << dest << endl;
// }

static void emit_jl(int dest, ostream& s)
{
  s << JL << " " << pos(dest) << endl;
}

// static void emit_jle(const char *dest, ostream& s)
// {
//   s << JLE << " " << dest << endl;
// }

static void emit_jle(int dest, ostream& s)
{
  s << JLE << " " << pos(dest) << endl;
}

// static void emit_je(const char *dest, ostream& s)
// {
//   s << JE << " " << dest << endl;
// }

static void emit_je(int dest, ostream& s)
{
  s << JE << " " << pos(dest) << endl;
}

// static void emit_jne(const char *dest, ostream& s)
// {
//   s << JNE << " " << dest << endl;
// }

static void emit_jne(int dest, ostream& s)
{
  s << JNE << " " << pos(dest) << endl;
}

// static void emit_jg(const char *dest, ostream& s)
// {
//   s << JG << " " << dest << endl;
// }

static void emit_jg(int dest, ostream& s)
{
  s << JG << " " << pos(dest) << endl;
}

// static void emit_jge(const char *dest, ostream& s)
// {
//   s << JGE << " " << dest << endl;
// }

static void emit_jge(int dest, ostream& s)
{
  s << JGE << " " << pos(dest) << endl;
}

// static void emit_jb(const char *dest, ostream& s)
// {
//   s << JB << " " << dest << endl;
// }

static void emit_jb(int dest, ostream& s)
{
  s << JB << " " << pos(dest) << endl;
}

// static void emit_jbe(const char *dest, ostream& s)
// {
//   s << JBE << " " << dest << endl;
// }

static void emit_jbe(int dest, ostream& s)
{
  s << JBE << " " << pos(dest) << endl;
}

// static void emit_ja(const char *dest, ostream& s)
// {
//   s << JA << " " << dest << endl;
// }

static void emit_ja(int dest, ostream& s)
{
  s << JA << " " << pos(dest) << endl;
}

// static void emit_jae(const char *dest, ostream& s)
// {
//   s << JAE << " " << dest << endl;
// }

static void emit_jae(int dest, ostream& s)
{
  s << JAE << " " << pos(dest) << endl;
}

// static void emit_jp(const char *dest, ostream& s)
// {
//   s << JP << " " << dest << endl;
// }

static void emit_jp(int dest, ostream& s)
{
  s << JP << " " << pos(dest) << endl;
}

// static void emit_jz(const char *dest, ostream& s)
// {
//   s << JZ << " " << dest << endl;
// }

static void emit_jz(int dest, ostream& s)
{
  s << JZ << " " << pos(dest) << endl;
}

// static void emit_jnz(const char *dest, ostream& s)
// {
//   s << JNZ << " " << dest << endl;
// }

static void emit_jnz(int dest, ostream& s)
{
  s << JNZ << " " << pos(dest) << endl;
}

static void emit_call(const char *dest, ostream& s)
{
  s << CALL << " " << dest << endl;
}

static void emit_ret(ostream& s)
{
  s << RET << endl;
}

static void emit_push(const char *reg, ostream& s)
{
  s << PUSH << " " << reg << endl;
}

static void emit_pop(const char *reg, ostream& s)
{
  s << POP << " " << reg << endl;
}

static void emit_leave(ostream& s)
{
  s << LEAVE << endl;
}

// static void emit_position(const char *p, ostream& s)
// {
  
//   s << p << ":" << endl;
// }

static void emit_position(int p, ostream& s)
{
  
  s << pos(p) << ":" << endl;
}



static void emit_float_to_int(const char *float_mmx, const char *int_reg, ostream& s)
{
  s << CVTTSD2SIQ << float_mmx << COMMA << int_reg << endl;
}

static void emit_int_to_float(const char *int_reg, const char *float_mmx, ostream& s)
{
  s << CVTSI2SDQ << int_reg << COMMA << float_mmx << endl;
}
///////////////////////////////////////////////////////////////////////////////
//
// coding strings, ints, and booleans
//
// Seal has four kinds of constants: strings, ints, and booleans.
// This section defines code generation for each type.
//
// If you like, you can add any ***Entry::code_def() and ***Entry::code_ref()
// functions to help.
//
///////////////////////////////////////////////////////////////////////////////

//
// Strings
//
void StringEntry::code_ref(ostream& s)
{
  s << "$" << STRINGCONST_PREFIX << index;
}

//
// Emit code for a constant String.
//

void StringEntry::code_def(ostream& s)
{
  s << STRINGCONST_PREFIX << index << ":" << endl;
  s  << STRINGTAG ; emit_string_constant(s,str);                                                // align to word
}

//
// StrTable::code_string
// Generate a string object definition for every string constant in the 
// stringtable.
//
void StrTable::code_string_table(ostream& s)
{  
  for (List<StringEntry> *l = tbl; l; l = l->tl())
    l->hd()->code_def(s);
}

// the following 2 functions are useless, please DO NOT care about them
void FloatEntry::code_ref(ostream &s)
{
  s << FLOATTAG << index;
}

void IntEntry::code_def(ostream &s)
{
  s << GLOBAL;
}

//***************************************************
//
//  Emit global var and functions.
//
//***************************************************

static void emit_global_int(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  INTTAG << 0 << endl;
}

static void emit_global_float(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl <<
  FLOATTAG << 0 << endl <<
  FLOATTAG << 0 << endl;
}

static void emit_global_bool(Symbol name, ostream& s) {
  s << GLOBAL << name << endl << 
  ALIGN << 8 << endl << 
  SYMBOL_TYPE << name << COMMA << OBJECT << endl <<
  SIZE << name << COMMA << 8 << endl << 
  name << ":" << endl << 
  BOOLTAG << 0 << endl;
}

static void emit_global_func(Symbol name, ostream& s){
  s << GLOBAL << name << endl << 
  SYMBOL_TYPE << name << COMMA << FUNCTION << endl<<
  name<<":"<<endl;
  

}

void code_global_data(Decls decls, ostream &str)
{
    for(int i=decls->first();decls->more(i);i=decls->next(i)){
      if (!decls->nth(i)->isCallDecl()){
        if (decls->nth(i)->getType()==Int) emit_global_int(decls->nth(i)->getName(),str);
        if (decls->nth(i)->getType()==Float) emit_global_float(decls->nth(i)->getName(),str);
        if (decls->nth(i)->getType()==Bool) emit_global_bool(decls->nth(i)->getName(),str);
      }
    }
    //load stringtab
    stringtable.code_string_table(str);
    
}

void code_calls(Decls decls, ostream &str) {
  str<<"\t.text"<<endl;
  for(int i=decls->first();decls->more(i);i=decls->next(i)) {
    if (decls->nth(i)->isCallDecl()){
      //emit_global_func
      Symbol name = decls->nth(i)->getName();
      emit_global_func(name,str);
      //generate code
      decls->nth(i)->code(str);
    }
  }

}

//***************************************************
//
//  Emit code to start the .text segment and to
//  declare the global names.
//
//***************************************************



//********************************************************
//
// Cgen helper helps to initialize and call code() function.
// You can do any initializing operations here
//
//********************************************************

void cgen_helper(Decls decls, ostream& s)
{

  code(decls, s);
}


void code(Decls decls, ostream& s)
{
  if (cgen_debug) cout << "Coding global data" << endl;
  code_global_data(decls, s);

  if (cgen_debug) cout << "Coding calls" << endl;
  code_calls(decls, s);
}

//******************************************************************
//
//   Fill in the following methods to produce code for the
//   appropriate expression.  You may add or remove parameters
//   as you wish, but if you do, remember to change the parameters
//   of the declarations in `seal-decl.h', `seal-expr.h' and `seal-stmt.h'
//   Sample code for constant integers, strings, and booleans are provided.
//   
//*****************************************************************

void CallDecl_class::code(ostream &s) {
  obj_loc_table.enterscope();
  emit_push(RBP,s);
  emit_mov(RSP,RBP,s);
  rsp_loc=0;
  emit_push(RBX,s);
  rsp_loc-=8;
  emit_push(R10,s);
  rsp_loc-=8;
  emit_push(R11,s);
  rsp_loc-=8;
  emit_push(R12,s);
  rsp_loc-=8;
  emit_push(R13,s);
  rsp_loc-=8;
  emit_push(R14,s);
  rsp_loc-=8;
  emit_push(R15,s);
  rsp_loc-=8;

  Variables formals=getVariables();
  if (formals->len()>0){
    //判断List不为空后可循环
      for(int i=formals->first();formals->more(i);i=formals->next(i)){
        Variable var = formals->nth(i);
        rsp_loc-=8;
        obj_loc_table.addid(var->getName(),new int(rsp_loc));
        emit_sub("$8",RSP,s);
        if (i==0) {
          if (var->getType()==Float){
            emit_rmmov(XMM0,rsp_loc,RBP,s);
          }
          else {
          emit_rmmov(RDI,rsp_loc,RBP,s);
          }
          }
        if (i==1) {
          if (var->getType()==Float){
            emit_rmmov(XMM1,rsp_loc,RBP,s);
          }
          else {
          emit_rmmov(RSI,rsp_loc,RBP,s);
          }
          }
        if (i==2) {
          if (var->getType()==Float){
            emit_rmmov(XMM2,rsp_loc,RBP,s);
          }
          else {
          emit_rmmov(RDX,rsp_loc,RBP,s);
          }
          }
        if (i==3) {
          if (var->getType()==Float){
            emit_rmmov(XMM3,rsp_loc,RBP,s);
          }
          else {
          emit_rmmov(RCX,rsp_loc,RBP,s);
          }
          }
        }
  }
  body->code(s);

// //保证恢复现场 (恢复现场交给return完成)
//   emit_irmov("-56",RBX,s);
//   emit_add(RBP,RBX,s);
//   emit_mov(RBX,RSP,s);

//   emit_pop(R15,s);
//   emit_pop(R14,s);
//   emit_pop(R13,s);
//   emit_pop(R12,s);
//   emit_pop(R11,s);
//   emit_pop(R10,s);
//   emit_pop(RBX,s);
//   //emit_pop(RBP,s);为什么？
//   emit_leave(s);
//   emit_ret(s);
  s<<"\t.size\t"<<name->get_string()<<COMMA<<"."<<"-"<<name->get_string()<<endl;
  //todo .size
  rsp_loc=0;

  obj_loc_table.exitscope();

  

}

void StmtBlock_class::code(ostream &s){
  if (vars->len()>0){
    for(int i=vars->first();vars->more(i);i=vars->next(i)){
      VariableDecl x=vars->nth(i);
      rsp_loc-=8;
      obj_loc_table.addid(x->getName(),new int(rsp_loc));
      emit_sub("$8",RSP,s);
    }
  }

  if (stmts->len()>0) {
    for (int i=stmts->first();stmts->more(i);i=stmts->next(i)) {
      stmts->nth(i)->code(s);
    }
  }
 
}

void IfStmt_class::code(ostream &s) {
  condition->code(s);
  emit_mrmov(RBP,rsp_loc,RAX,s);

  int pos_true=position;//if{}
  position+=1;
  int pos_false=position;//else{}
  position+=1;
  int pos_follow=position;
  position+=1;

  emit_test(RAX,RAX,s);
  emit_jz(pos_false,s);//condition为0转到else{}

  //condition为1的代码直接If{}
  thenexpr->code(s);
  emit_jmp(pos_follow,s);//转到ifstmt接着的stmt代码

  emit_position(pos_false,s);//else部分
  elseexpr->code(s);

  emit_position(pos_follow,s);
}

void WhileStmt_class::code(ostream &s) {
  int pos_loop_body=position;
  position+=1;
  int pos_loop_test=position;
  position+=1;
  int pos_out=position;
  position+=1;
  emit_jmp(pos_loop_test,s);

  emit_position(pos_loop_body,s);
  body->code(s);

  emit_position(pos_loop_test,s);
  condition->code(s);
  emit_mrmov(RBP,rsp_loc,RAX,s);//初始判断
  emit_test(RAX,RAX,s);
  emit_jnz(pos_loop_body,s); 

  emit_position(pos_out,s);
}

void ForStmt_class::code(ostream &s) {
  //Expr initexpr, condition, loopact;
	//StmtBlock body;
  int pos_loop_body=position;
  position+=1;
  int pos_loop_test=position;
  position+=1;
  int pos_out=position;
  position+=1;
  initexpr->code(s);
  emit_jmp(pos_loop_test,s);

  emit_position(pos_loop_body,s);
  body->code(s);
  loopact->code(s);

  emit_position(pos_loop_test,s);
  condition->code(s);
  emit_mrmov(RBP,rsp_loc,RAX,s);
  emit_test(RAX,RAX,s);
  emit_jnz(pos_loop_body,s);

  emit_position(pos_out,s);

}

void ReturnStmt_class::code(ostream &s) {
  //return 到RAX上
  value->code(s);
  emit_mrmov(RBP,rsp_loc,RAX,s);
  //保证恢复现场 (恢复现场交给returnstmt完成)
  emit_irmov("-56",RBX,s);
  emit_add(RBP,RBX,s);
  emit_mov(RBX,RSP,s);

  emit_pop(R15,s);
  emit_pop(R14,s);
  emit_pop(R13,s);
  emit_pop(R12,s);
  emit_pop(R11,s);
  emit_pop(R10,s);
  emit_pop(RBX,s);
  //emit_pop(RBP,s);为什么？
  emit_leave(s);
  emit_ret(s);  
}

void ContinueStmt_class::code(ostream &s) {
  emit_jmp(position-2,s); 
}

void BreakStmt_class::code(ostream &s) {
  //position-1是out的位置,position-2是test的位置
  emit_jmp(position-1,s);
}

void Call_class::code(ostream &s) {
  order=0;
  formal_table.enterscope();
  Symbol func_name=getName();
  if (func_name!=print) {
    int isfloat[4]={0,0,0,0};
    if (actuals->len()>0){
      for (int i=actuals->first();actuals->more(i);i=actuals->next(i)){
          actuals->nth(i)->code(s);
          if(actuals->nth(i)->getType()==Float) {isfloat[i]=1;}
          if (i==0) {formal_table.addid(formal0,new int(rsp_loc));}
          else if (i==1) {formal_table.addid(formal1,new int(rsp_loc));}
          else if (i==2) {formal_table.addid(formal2,new int(rsp_loc));}
          else if (i==3) {formal_table.addid(formal3,new int(rsp_loc));}
      }
    }
    int * formal_loc;
    formal_loc=formal_table.probe(formal0);
    if (formal_loc!=NULL) {
      if (isfloat[0]==0) emit_mrmov(RBP,*formal_loc,RDI,s);
      else {emit_movsd_rbp(*formal_loc,XMM0,s);}
    }
    formal_loc=formal_table.probe(formal1);
    if (formal_loc!=NULL) {
      if (isfloat[1]==0) emit_mrmov(RBP,*formal_loc,RSI,s);
      else{emit_movsd_rbp(*formal_loc,XMM1,s);}
    }
    formal_loc=formal_table.probe(formal2);
    if (formal_loc!=NULL) {
      if (isfloat[2]==0) emit_mrmov(RBP,*formal_loc,RDX,s);
      else {emit_movsd_rbp(*formal_loc,XMM2,s);}
    }
    formal_loc=formal_table.probe(formal3);
    if (formal_loc!=NULL) {
      if (isfloat[3]==0) emit_mrmov(RBP,*formal_loc,RCX,s);
      else {emit_movsd_rbp(*formal_loc,XMM3,s);}
    }

    emit_call(name->get_string(),s);
    //rax里面的还要取出来
    rsp_loc-=8;
    emit_rmmov(RAX,rsp_loc,RBP,s);
    emit_sub("$8",RSP,s);
    
  }
  

  if (func_name ==print){

    // please set %eax to the number of Float parameters, num.
    //  把%eax赋值为Float类型的参数个数, num
    int num=0;
    int isfloat[4]={0,0,0,0};//是浮点数标志位置1
    for (int i=actuals->first();actuals->more(i);i=actuals->next(i)){
      
      actuals->nth(i)->code(s);
      if(actuals->nth(i)->getType()==Float) {num+=1;isfloat[i]=1;}
      if (i==0) formal_table.addid(formal0,new int(rsp_loc));
      if (i==1) formal_table.addid(formal1,new int(rsp_loc));
      if (i==2) formal_table.addid(formal2,new int(rsp_loc));
      if (i==3) formal_table.addid(formal3,new int(rsp_loc));

    }
    int * formal_loc;
    //要讨论参数是不是浮点
    formal_loc=formal_table.probe(formal0);
    if (formal_loc!=NULL) {
      if (isfloat[0]==0) emit_mrmov(RBP,*formal_loc,RDI,s);
      else {emit_movsd_rbp(*formal_loc,XMM0,s);}
    }
    formal_loc=formal_table.probe(formal1);
    if (formal_loc!=NULL) {
      if (isfloat[1]==0) emit_mrmov(RBP,*formal_loc,RSI,s);
      else{emit_movsd_rbp(*formal_loc,XMM0,s);}
    }
    formal_loc=formal_table.probe(formal2);
    if (formal_loc!=NULL) {
      if (isfloat[2]==0) emit_mrmov(RBP,*formal_loc,RDX,s);
      else {emit_movsd_rbp(*formal_loc,XMM1,s);}
    }
    formal_loc=formal_table.probe(formal3);
    if (formal_loc!=NULL) {
      if (isfloat[3]==0) emit_mrmov(RBP,*formal_loc,RCX,s);
      else {emit_movsd_rbp(*formal_loc,XMM2,s);}
    }
    emit_sub("$8", RSP, s);
    emit_irmovl_num(num, EAX, s);
    emit_call("printf", s);
      
    return;
  }

  formal_table.exitscope();
}

void Actual_class::code(ostream &s) {
    expr->code(s);
    // // actual的结果记录在此时的rsp_loc对应的栈中
    // if (order ==0 ) {emit_mrmov(RBP,rsp_loc,RDI,s);}
    // if (order ==1 ) {emit_mrmov(RBP,rsp_loc,RSI,s);}
    // if (order ==2 ) {emit_mrmov(RBP,rsp_loc,RDX,s);}
    // if (order ==3 ) {emit_mrmov(RBP,rsp_loc,RAX,s);}
    // order++;  
}
//todo 浮点数
void Assign_class::code(ostream &s) {
  int obj_loc;
  obj_loc=*obj_loc_table.probe(lvalue);
  value->code(s);
  if (getType()==Float) {
    emit_mrmov(RBP,rsp_loc,XMM0,s);
    emit_rmmov(XMM0,obj_loc,RBP,s);
  }
  else {
  emit_mrmov(RBP,rsp_loc,RBX,s);
  emit_rmmov(RBX,obj_loc,RBP,s);
  }
}

void Add_class::code(ostream &s) {
  //左操作数rax,右操作数rbx,结果放入rbx中再存到栈中
  op_table.enterscope();
  e1->code(s);
  op_table.addid(left_op,new int(rsp_loc));
  //emit_mrmov(RBP,rsp_loc,RAX,s);
  e2->code(s);
  op_table.addid(right_op,new int(rsp_loc));
  // emit_mrmov(RBP,rsp_loc,RBX,s);
  int left_addr=*op_table.probe(left_op);
  int right_addr=*op_table.probe(right_op);
  bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
  if (isfloat){//左操作数xmm0,右操作数xmm1
  //todo先将int转换成float
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left_addr,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left_addr,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right_addr,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right_addr,XMM1,s);}
    emit_addsd(XMM0,XMM1,s);
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(XMM1,rsp_loc,RBP,s);
  }
  else {
  emit_mrmov(RBP,left_addr,RAX,s);
  emit_mrmov(RBP,right_addr,RBX,s);
  emit_add(RAX,RBX,s);//float 用addsd
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RBX,rsp_loc,RBP,s);
  }
  op_table.exitscope();  
}

void Minus_class::code(ostream &s) {
  op_table.enterscope();
  e1->code(s);
  op_table.addid(left_op,new int(rsp_loc));
  //emit_mrmov(RBP,rsp_loc,RAX,s);
  e2->code(s);
  op_table.addid(right_op,new int(rsp_loc));
  // emit_mrmov(RBP,rsp_loc,RBX,s);
  int left_addr=*op_table.probe(left_op);
  int right_addr=*op_table.probe(right_op);
  bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
  if (isfloat) {
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left_addr,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left_addr,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right_addr,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right_addr,XMM1,s);}    
    // emit_mrmov(RBP,left_addr,XMM0,s);
    // emit_mrmov(RBP,right_addr,XMM1,s);
    emit_subsd(XMM1,XMM0,s);
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(XMM0,rsp_loc,RBP,s);

  }
  else {
  emit_mrmov(RBP,left_addr,RAX,s);
  emit_mrmov(RBP,right_addr,RBX,s);
  emit_sub(RBX,RAX,s);
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s);
  }
  op_table.exitscope();
}

void Multi_class::code(ostream &s) {
  op_table.enterscope();
  e1->code(s);
  op_table.addid(left_op,new int(rsp_loc));
  // emit_mrmov(RBP,rsp_loc,RAX,s);
  e2->code(s);
  op_table.addid(right_op,new int(rsp_loc));
  // emit_mrmov(RBP,rsp_loc,RBX,s);
  int left_addr=*op_table.probe(left_op);
  int right_addr=*op_table.probe(right_op);
  bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
  if (isfloat) {
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left_addr,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left_addr,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right_addr,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right_addr,XMM1,s);}
    // emit_mrmov(RBP,left_addr,XMM0,s);
    // emit_mrmov(RBP,right_addr,XMM1,s);
    emit_mulsd(XMM0,XMM1,s);
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(XMM1,rsp_loc,RBP,s);

  }
  else {
  emit_mrmov(RBP,left_addr,RAX,s);
  emit_mrmov(RBP,right_addr,RBX,s);
  emit_mul(RAX,RBX,s);
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RBX,rsp_loc,RBP,s);
  }
  op_table.exitscope();
 
}

void Divide_class::code(ostream &s) {
 op_table.enterscope();
 e1->code(s);
 op_table.addid(left_op,new int(rsp_loc));
//  emit_mrmov(RBP,rsp_loc,RAX,s);
//  emit_cqto(s);
 e2->code(s);
 op_table.addid(right_op,new int(rsp_loc));
//  emit_mrmov(RBP,rsp_loc,RCX,s);
 int left_addr=*op_table.probe(left_op);
 int right_addr=*op_table.probe(right_op);
 bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
 if (isfloat) {
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left_addr,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left_addr,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right_addr,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right_addr,XMM1,s);}
    // emit_mrmov(RBP,left_addr,XMM0,s);
    // emit_mrmov(RBP,right_addr,XMM1,s);
    emit_divsd(XMM1,XMM0,s);
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(XMM0,rsp_loc,RBP,s);
 }
 else {
 emit_mrmov(RBP,left_addr,RAX,s);
 emit_cqto(s);
 emit_mrmov(RBP,right_addr,RCX,s);
 emit_div(RCX,s);
 rsp_loc-=8;
 emit_sub("$8",RSP,s);
 emit_rmmov(RAX,rsp_loc,RBP,s);
 }
 op_table.exitscope();
 
}

void Mod_class::code(ostream &s) {
 op_table.enterscope();
 e1->code(s);
 op_table.addid(left_op,new int(rsp_loc));
//  emit_mrmov(RBP,rsp_loc,RAX,s);
//  emit_cqto(s);
 e2->code(s);
 op_table.addid(right_op,new int(rsp_loc));
//  emit_mrmov(RBP,rsp_loc,RCX,s);
 int left_addr=*op_table.probe(left_op);
 int right_addr=*op_table.probe(right_op);
 emit_mrmov(RBP,left_addr,RAX,s);
 emit_cqto(s);
 emit_mrmov(RBP,right_addr,RCX,s);
 emit_div(RCX,s);
 rsp_loc-=8;
 emit_sub("$8",RSP,s);
 emit_rmmov(RDX,rsp_loc,RBP,s);
 op_table.exitscope();
 
}

void Neg_class::code(ostream &s) {
  //单操作数，用rax
  op_table.enterscope();
  e1->code(s);
  bool isfloat=(e1->getType()==Float);
  if (isfloat) {
    //0-x
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_immov("0",rsp_loc,RBP,s);
    emit_mrmov(RBP,rsp_loc,XMM0,s);
    emit_mrmov(RBP,rsp_loc+8,XMM1,s);
    emit_subsd(XMM1,XMM0,s);
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(XMM0,rsp_loc,RBP,s);
  }
  else {
  emit_mrmov(RBP,rsp_loc,RAX,s);
  emit_neg(RAX,s);
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s);
  }
  op_table.exitscope();

 
}

void Lt_class::code(ostream &s) {
  //<,<=,==,!=,>=,>
  e1->code(s);
  int left=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RAX,s);
  e2->code(s);
  int right=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RBX,s);
  // emit_mrmov(RBP,left,RAX,s);
  // emit_mrmov(RBP,right,RBX,s);
  // emit_cmp(RAX,RBX,s);//基于后一个减前一个
  int pos_true;
  int pos_follow;
  pos_true=position;
  position+=1;
  pos_follow=position;
  position+=1;
  //pos_true//判断结果为真跳转的位置
  //pos_follow//执行完比较紧跟的代码的位置
  bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
  if (isfloat) {
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right,XMM1,s);}
    // emit_mrmov(RBP,left,XMM0,s);
    // emit_mrmov(RBP,right,XMM1,s);
    emit_ucompisd(XMM0,XMM1,s);

    emit_ja(pos_true,s);

    emit_irmov("0",RAX,s);//else
    emit_jmp(pos_follow,s);
    
    emit_position(pos_true,s);
    emit_irmov("1",RAX,s);

    emit_position(pos_follow,s);//保存比较的值
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(RAX,rsp_loc,RBP,s); 


  }
  else {
  emit_mrmov(RBP,left,RAX,s);
  emit_mrmov(RBP,right,RBX,s);
  emit_cmp(RAX,RBX,s);//基于后一个减前一个

  emit_jg(pos_true,s);//if x<y;

  emit_irmov("0",RAX,s);//else
  emit_jmp(pos_follow,s);

  emit_position(pos_true,s);
  emit_irmov("1",RAX,s);

  emit_position(pos_follow,s);//保存比较的值
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s); 
  }
}

void Le_class::code(ostream &s) {
  //<=
  e1->code(s);
  int left=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RAX,s);
  e2->code(s);
  int right=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RBX,s);
  int pos_true;
  int pos_follow;
  pos_true=position;
  position+=1;
  pos_follow=position;
  position+=1;


  bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
  if (isfloat) {
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right,XMM1,s);}
    // emit_mrmov(RBP,left,XMM0,s);
    // emit_mrmov(RBP,right,XMM1,s);
    emit_ucompisd(XMM0,XMM1,s);

    emit_jae(pos_true,s);

    emit_irmov("0",RAX,s);//else
    emit_jmp(pos_follow,s);
    
    emit_position(pos_true,s);
    emit_irmov("1",RAX,s);

    emit_position(pos_follow,s);//保存比较的值
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(RAX,rsp_loc,RBP,s); 
  }
  else {
  emit_mrmov(RBP,left,RAX,s);
  emit_mrmov(RBP,right,RBX,s);
  emit_cmp(RAX,RBX,s);//基于后一个减前一个
  emit_jge(pos_true,s);//x<=y

  emit_irmov("0",RAX,s);//else
  emit_jmp(pos_follow,s);

  emit_position(pos_true,s);
  emit_irmov("1",RAX,s);

  emit_position(pos_follow,s);//保存比较的值
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s); 
  }
}

void Equ_class::code(ostream &s) {
  //==
  e1->code(s);
  int left=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RAX,s);
  e2->code(s);
  int right=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RBX,s);
  bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
  int pos_true;
  int pos_follow;
  pos_true=position;
  position+=1;
  pos_follow=position;
  position+=1;

  if (isfloat) {
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right,XMM1,s);}
    // emit_mrmov(RBP,left,XMM0,s);
    // emit_mrmov(RBP,right,XMM1,s);
    emit_ucompisd(XMM0,XMM1,s);

    emit_je(pos_true,s);

    emit_irmov("0",RAX,s);//else
    emit_jmp(pos_follow,s);

    emit_position(pos_true,s);
    emit_irmov("1",RAX,s);

    emit_position(pos_follow,s);//保存比较的值
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(RAX,rsp_loc,RBP,s);
  }
  else {
  emit_mrmov(RBP,left,RAX,s);
  emit_mrmov(RBP,right,RBX,s);
  emit_cmp(RAX,RBX,s);//基于后一个减前一个

  emit_je(pos_true,s);//x==y

  emit_irmov("0",RAX,s);//else
  emit_jmp(pos_follow,s);

  emit_position(pos_true,s);
  emit_irmov("1",RAX,s);

  emit_position(pos_follow,s);//保存比较的值
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s);  
  }
}

void Neq_class::code(ostream &s) {
  //!=
  e1->code(s);
  int left=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RAX,s);
  e2->code(s);
  int right=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RBX,s);
  int pos_true;
  int pos_follow;
  pos_true=position;
  position+=1;
  pos_follow=position;
  position+=1;

  bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
  if (isfloat) {
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right,XMM1,s);}
    // emit_mrmov(RBP,left,XMM0,s);
    // emit_mrmov(RBP,right,XMM1,s);
    emit_ucompisd(XMM0,XMM1,s);

    emit_jne(pos_true,s);

    emit_irmov("0",RAX,s);//else
    emit_jmp(pos_follow,s);
    
    emit_position(pos_true,s);
    emit_irmov("1",RAX,s);

    emit_position(pos_follow,s);//保存比较的值
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(RAX,rsp_loc,RBP,s); 
  }
  else {
  emit_mrmov(RBP,left,RAX,s);
  emit_mrmov(RBP,right,RBX,s);
  emit_cmp(RAX,RBX,s);//基于后一个减前一个

  emit_jne(pos_true,s);//x!=y

  emit_irmov("0",RAX,s);//else
  emit_jmp(pos_follow,s);

  emit_position(pos_true,s);
  emit_irmov("1",RAX,s);

  emit_position(pos_follow,s);//保存比较的值
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s);
  }  
}

void Ge_class::code(ostream &s) {
    //>=
  e1->code(s);
  int left=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RAX,s);
  e2->code(s);
  int right=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RBX,s);
  int pos_true;
  int pos_follow;
  pos_true=position;
  position+=1;
  pos_follow=position;
  position+=1;

  bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
  if (isfloat) {
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right,XMM1,s);}
    // emit_mrmov(RBP,left,XMM0,s);
    // emit_mrmov(RBP,right,XMM1,s);
    emit_ucompisd(XMM0,XMM1,s);

    emit_jbe(pos_true,s);

    emit_irmov("0",RAX,s);//else
    emit_jmp(pos_follow,s);
    
    emit_position(pos_true,s);
    emit_irmov("1",RAX,s);

    emit_position(pos_follow,s);//保存比较的值
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(RAX,rsp_loc,RBP,s); 
  }

  emit_mrmov(RBP,left,RAX,s);
  emit_mrmov(RBP,right,RBX,s);
  emit_cmp(RAX,RBX,s);//基于后一个减前一个
  emit_jle(pos_true,s);//x>=y

  emit_irmov("0",RAX,s);//else
  emit_jmp(pos_follow,s);

  emit_position(pos_true,s);
  emit_irmov("1",RAX,s);

  emit_position(pos_follow,s);//保存比较的值
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s); 
 
}

void Gt_class::code(ostream &s) {
    //>
  e1->code(s);
  int left=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RAX,s);
  e2->code(s);
  int right=rsp_loc;
  // emit_mrmov(RBP,rsp_loc,RBX,s);
  int pos_true;
  int pos_follow;
  pos_true=position;
  position+=1;
  pos_follow=position;
  position+=1;

  bool isfloat=(e1->getType()==Float)||(e2->getType()==Float);
  if (isfloat) {
    if (e1->getType()==Int) {
      emit_mrmov(RBP,left,RAX,s);
      emit_int_to_float(RAX,XMM0,s);
      }
      else {emit_mrmov(RBP,left,XMM0,s);}
    if (e2->getType()==Int) {
      emit_mrmov(RBP,right,RAX,s);
      emit_int_to_float(RAX,XMM1,s);
    }
      else {emit_mrmov(RBP,right,XMM1,s);}
    // emit_mrmov(RBP,left,XMM0,s);
    // emit_mrmov(RBP,right,XMM1,s);
    emit_ucompisd(XMM0,XMM1,s);

    emit_jb(pos_true,s);

    emit_irmov("0",RAX,s);//else
    emit_jmp(pos_follow,s);
    
    emit_position(pos_true,s);
    emit_irmov("1",RAX,s);

    emit_position(pos_follow,s);//保存比较的值
    rsp_loc-=8;
    emit_sub("$8",RSP,s);
    emit_rmmov(RAX,rsp_loc,RBP,s); 
  }
  else {
  emit_mrmov(RBP,left,RAX,s);
  emit_mrmov(RBP,right,RBX,s);
  emit_cmp(RAX,RBX,s);//基于后一个减前一个

  emit_jl(pos_true,s);//x>y

  emit_irmov("0",RAX,s);//else
  emit_jmp(pos_follow,s);

  emit_position(pos_true,s);
  emit_irmov("1",RAX,s);

  emit_position(pos_follow,s);//保存比较的值
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s);  
  }
}

void And_class::code(ostream &s) {
 e1->code(s);
 int left=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RAX,s);
 e2->code(s);
 int right=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RBX,s);

 emit_mrmov(RBP,left,RAX,s);
 emit_mrmov(RBP,right,RBX,s);

 emit_and(RAX,RBX,s);

 rsp_loc-=8;
 emit_sub("$8",RSP,s);
 emit_rmmov(RBX,rsp_loc,RBP,s);
}

void Or_class::code(ostream &s) {
 e1->code(s);
 int left=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RAX,s);
 e2->code(s);
 int right=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RBX,s);

 emit_mrmov(RBP,left,RAX,s);
 emit_mrmov(RBP,right,RBX,s);

 emit_or(RAX,RBX,s);

 rsp_loc-=8;
 emit_sub("$8",RSP,s);
 emit_rmmov(RBX,rsp_loc,RBP,s);
 
}

void Xor_class::code(ostream &s) {
 e1->code(s);
 int left=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RAX,s);
 e2->code(s);
 int right=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RBX,s);

 emit_mrmov(RBP,left,RAX,s);
 emit_mrmov(RBP,right,RBX,s);
 emit_xor(RAX,RBX,s);

 rsp_loc-=8;
 emit_sub("$8",RSP,s);
 emit_rmmov(RBX,rsp_loc,RBP,s);
 
}

void Not_class::code(ostream &s) {
  e1->code(s);
  emit_mrmov(RBP,rsp_loc,RAX,s);

  emit_not(RAX,s);

  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s);
 
}

void Bitnot_class::code(ostream &s) {
  e1->code(s);
  emit_mrmov(RBP,rsp_loc,RAX,s);

  emit_not(RAX,s);

  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_rmmov(RAX,rsp_loc,RBP,s);

}

void Bitand_class::code(ostream &s) {
 e1->code(s);
 int left=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RAX,s);
 e2->code(s);
 int right=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RBX,s);

 emit_mrmov(RBP,left,RAX,s);
 emit_mrmov(RBP,right,RBX,s);

 emit_and(RAX,RBX,s);

 rsp_loc-=8;
 emit_sub("$8",RSP,s);
 emit_rmmov(RBX,rsp_loc,RBP,s);


}

void Bitor_class::code(ostream &s) {
 e1->code(s);
 int left=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RAX,s);
 e2->code(s);
 int right=rsp_loc;
//  emit_mrmov(RBP,rsp_loc,RBX,s);

 emit_mrmov(RBP,left,RAX,s);
 emit_mrmov(RBP,right,RBX,s);
 emit_or(RAX,RBX,s);

 rsp_loc-=8;
 emit_sub("$8",RSP,s);
 emit_rmmov(RBX,rsp_loc,RBP,s);
 
}

void Const_int_class::code(ostream &s) {
  rsp_loc-=8;
  emit_immov(value->get_string(),rsp_loc,RBP,s);
  emit_sub("$8",RSP,s);
 
}

void Const_string_class::code(ostream &s) {
  rsp_loc-=8;
  //求出前缀 .LC1
  int index;
  index=value->get_index();
  emit_immov_str(index,rsp_loc,RBP,s);
  emit_sub("$8",RSP,s);
 
}

void Const_float_class::code(ostream &s) {
  // rsp_loc-=8;
  // //求出前缀 .LC1
  // int index;
  // index=value->get_index();
  // emit_immov_str(index,rsp_loc,RBP,s);
  // emit_sub("$8",RSP,s);
  unsigned long * p;
  double x=str2double(value->get_string());
  p=(long unsigned int *)&x;
  long unsigned int num_in_mem=*p;
  rsp_loc-=8;
  emit_sub("$8",RSP,s);
  emit_irmov_float(num_in_mem,RAX,s);
  emit_rmmov(RAX,rsp_loc,RBP,s);
}

void Const_bool_class::code(ostream &s) {
  rsp_loc-=8;
  if (value==1) emit_immov("$1",rsp_loc,RBP,s);
  if (value==0) emit_immov("$0",rsp_loc,RBP,s);
  emit_sub("$8",RSP,s);
 
}

void Object_class::code(ostream &s) {
  int origin_loc;
  origin_loc=*obj_loc_table.probe(var);
  rsp_loc-=8;
  if(getType()==Float) {
    emit_mrmov (RBP,origin_loc,XMM0,s);
    emit_rmmov (XMM0,rsp_loc,RBP,s);
    emit_sub("$8",RSP,s);
  }
  else {
  emit_mrmov (RBP,origin_loc,RBX,s);
  emit_rmmov (RBX,rsp_loc,RBP,s);
  emit_sub("$8",RSP,s);
  }
}

void No_expr_class::code(ostream &s) {

}