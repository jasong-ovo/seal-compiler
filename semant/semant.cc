#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"

extern int semant_debug;
extern char *curr_filename;

static ostream& error_stream = cerr;
static int semant_errors = 0;
static Decl curr_decl = 0;

typedef SymbolTable<Symbol, Symbol> ObjectEnvironment; // name, type
ObjectEnvironment objectEnv;

//globle variable table
ObjectEnvironment   globalVarTable;
//global func table
typedef SymbolTable<Symbol,Decl_class> GlobaFuncTable; // name, calldecl
GlobaFuncTable globalFuncTable;
//state table : identify if in forstmt or whilestmt
ObjectEnvironment stateEnv;


///////////////////////////////////////////////
// helper func
///////////////////////////////////////////////


static ostream& semant_error() {
    semant_errors++;
    return error_stream;
}

static ostream& semant_error(tree_node *t) {
    error_stream << t->get_line_number() << ": ";
    return semant_error();
}

static ostream& internal_error(int lineno) {
    error_stream << "FATAL:" << lineno << ": ";
    return error_stream;
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

static Symbol 
    Int,
    Float,
    String,
    Bool,
    Void,
    Main,
    print,
    No_type,
    While,
    For,
    Printf,
    Return
    ;

bool isValidCallName(Symbol type) {
    return type != (Symbol)print;
}

bool isValidTypeName(Symbol type) {
    return type != Void;
}//todo

//
// Initializing the predefined symbols.
//

static void initialize_constants(void) {
    // 4 basic types and Void type
    Bool        = idtable.add_string("Bool");
    Int         = idtable.add_string("Int");
    String      = idtable.add_string("String");
    Float       = idtable.add_string("Float");
    Void        = idtable.add_string("Void");  
    No_type=idtable.add_string("_no_type");

    //state
    While   =idtable.add_string("while");
    For       =idtable.add_string("for");
    Return      = idtable.add_string("return");
    // Main function
    Main        = idtable.add_string("main");
    //system function
    Printf      = idtable.add_string("printf");


    // classical function to print things, so defined here for call.
    print        = idtable.add_string("printf");
}

/*
    TODO :
    you should fill the following function defines, so that semant() can realize a semantic 
    analysis in a recursive way. 
    Of course, you can add any other functions to help.
*/

static bool sameType(Symbol name1, Symbol name2) {
    return strcmp(name1->get_string(), name2->get_string()) == 0;
}

static void install_calls(Decls decls) {
    globalFuncTable.enterscope();
    //load system function
    //create a nonrelational *dat
    Variable temp1=new Variable_class(Printf,Printf);
    VariableDecl_class temp2=VariableDecl_class(temp1); 
    ////////////////
    globalFuncTable.addid(Printf,temp2.copy_Decl());//*dat is useless
    /////////////////////////////////////////////////////////////////
        for(int i = decls->first(); decls->more(i); i = decls->next(i)) {
            if (decls->nth(i)->isCallDecl())    {
                bool existed=false;
                if (globalFuncTable.lookup(decls->nth(i)->getName())!=NULL) {existed=true;}
                else existed=false;
                if (existed) {
                    semant_error(decls->nth(i))<<" function: "<<decls->nth(i)->getName()<<" is multiply defined\n";
                }
                else globalFuncTable.addid(decls->nth(i)->getName(),decls->nth(i)->copy_Decl());
            }
        }
}



static void install_globalVars(Decls decls) {
    globalVarTable.enterscope();
        for(int i = decls->first(); decls->more(i); i = decls->next(i)) {
            if (!decls->nth(i)->isCallDecl())    {
                bool existed=false;
                if (globalVarTable.lookup(decls->nth(i)->getName())!=NULL) existed=true;
                else existed=false;
                if (existed) {
                    semant_error(decls->nth(i))<<decls->nth(i)->getName()<<" is multiply defined\n";
                }
                else globalVarTable.addid(decls->nth(i)->getName(),new Symbol(decls->nth(i)->getType()));
            }
        }
}

static void check_calls(Decls decls) {
        for (int i = decls->first(); decls->more(i); i = decls->next(i)) {
                if(decls->nth(i)->isCallDecl())  {
                    decls->nth(i)->check();
                }
        }


}

static void check_main() {
    if (globalFuncTable.lookup(Main)==NULL){
        semant_error()<<" need main function\n";
    }
    //check return type void

}

void VariableDecl_class::check() {
    if (Void==variable->getType()){
        semant_error(variable)<<"type of variable can't be Void\n";
    }

}

void CallDecl_class::check() {
        //load paras in scope
        objectEnv.enterscope();
        //main func paras must be empty
        if (name==Main) {
            if (paras->len()!=0) {
                semant_error(paras)<<"paras of main must be empty\n";
            }
            if(returnType!=Void) {
                semant_error()<<line_number<<":"<<"func main's return type can only be Void\n";
            }
        }

            for (int i=paras->first();paras->more(i);i=paras->next(i)){

                Symbol tempName=paras->nth(i)->getName();
                Symbol tempType=paras->nth(i)->getType();
                //type can't be Void
                if (tempType==Void) {
                    semant_error(paras)<<"type of formal parameter can't be Void\n";
                }
                bool existed=false ;
                // if(globalVarTable.lookup(tempName)!=NULL) existed=true;
                //formal paras don't conflict with the global ones
                if  (objectEnv.lookup(tempName)!=NULL) existed=true;
                if (existed) {
                    semant_error(paras->nth(i))<<"formal paras redefined: "<<tempName<<endl;
                }
                else objectEnv.addid(tempName,new Symbol(tempType));
            }

        //loaded
        body->check(returnType);
        objectEnv.exitscope();
        //if func is main check return
 
        Symbol* returnStatus=stateEnv.lookup(Return);
        if (returnStatus==NULL) {
                semant_error()<<line_number<<":"<<name<<" function must have a overall return\n";
            }

        while (!stateEnv.is_empty()) {
            stateEnv.exitscope();
        }

}

void StmtBlock_class::check(Symbol type) {
    objectEnv.enterscope();
    //load var x *
        for (int i = vars->first();vars->more(i);i=vars->next(i)){
            Symbol tempName=vars->nth(i)->getName();
            Symbol tempType=vars->nth(i)->getType();
            bool existed=false;
            //check existence
            if(globalVarTable.lookup(tempName)!=NULL) existed=true;
            if (objectEnv.lookup(tempName)!=NULL) existed=true;
            if (existed) {
                semant_error(vars->nth(i))<<" variable redefined: "<<tempName<<endl;
            }
            else {
                objectEnv.addid(tempName,new Symbol(tempType));}
        }
    //check stmts
        for (int i=stmts->first();stmts->more(i);i=stmts->next(i)){
            stmts->nth(i)->check(type);
        }

    objectEnv.exitscope();

}

void IfStmt_class::check(Symbol type) {
    //check condition is null
    stateEnv.enterscope();
    if(condition->is_empty_Expr())  {
        semant_error(condition)<<"if (condition) condition can't be null"<<endl;
    }
    else
    {
        Symbol typeCondition=condition->checkType();
        if(typeCondition!=Bool) {
            semant_error(condition)<<"if (condition) condition must be Bool\n";
        }
    }
    thenexpr->check(type);
    elseexpr->check(type);
    stateEnv.exitscope();    
}

void WhileStmt_class::check(Symbol type) {
    //load while in stateEnv
    stateEnv.enterscope();
    stateEnv.addid(While,new Symbol(While));
    //check condition:null        not nulll
    if (condition->is_empty_Expr()) {
        semant_error(condition)<<" while (expr) : expr can't be empty \n";
    }
    else {
        //check type of condition 
        Symbol typeOfExpr=condition->checkType();
        //todo compare between symbols
        if (typeOfExpr!=Bool) {
            semant_error(condition)<<" while (expr) :expr need to be Bool\n";
        }
    }
    body->check(type);

    stateEnv.exitscope();

}

void ForStmt_class::check(Symbol type) {
    //load for in state
    stateEnv.enterscope();
    stateEnv.addid(For,new Symbol(For));
    ///////////////////////////
    Symbol typeIniExpr=initexpr->checkType();
    Symbol typeCondition=condition->checkType();
    //maybe null pointer
    Symbol typeLoopact=loopact->checkType();
    if(condition->is_empty_Expr()) {
        ;
    }
    else {
        if(typeCondition!=Bool) {
            semant_error(initexpr)<<"condition of forstmt must be bool type not " <<"##"<<typeCondition<<endl;
        }
    }
    body->check(type);
    stateEnv.exitscope();

}

void ReturnStmt_class::check(Symbol type) {
    //main func check return exist?
    //if state Env is null then in overall {}
    if(stateEnv.is_empty()){
    stateEnv.enterscope();
    stateEnv.addid(Return,new Symbol(Return));
    }
    Symbol typeReturn=value->checkType();
    if (typeReturn!=Bool&&typeReturn!=Int&&typeReturn!=Float&&typeReturn!=Void&&typeReturn!=String){
        semant_error(value)<<"return type can only be Bool or Int or Float or String or Void\n";
    }
    if (type!=typeReturn) {
        semant_error(value)<<"return type not macth the func declaratian: "<<"return:"<<typeReturn<<"   "<<"need:"<<type<<endl;
    }

}

void ContinueStmt_class::check(Symbol type) {
    Symbol * inWhile=stateEnv.lookup(While);
    Symbol * inFor=stateEnv.lookup(For);
    bool inLoop=!(inWhile==NULL&&inFor==NULL);
    if (!inLoop) {
        semant_error()<<line_number<<":"<<"continue can only be used in for or while\n";
    }
}

void BreakStmt_class::check(Symbol type) {
    Symbol * inWhile=stateEnv.lookup(While);
    Symbol * inFor=stateEnv.lookup(For);
    bool inLoop=!(inWhile==NULL&&inFor==NULL);
    if (!inLoop) {
        semant_error()<<line_number<<":"<<"break can only be used in for or while\n";
    }

}

Symbol Call_class::checkType(){
    //initial (check type its members)
    //paras need check type
    ///////////////////////////////////////
    Symbol tempName=getName();
    Actuals tempActuals=getActuals();
    bool defined=false;
    Decl_class * info=globalFuncTable.lookup(tempName);
    //check if func defined , only defined the actuals can be check
    if (info!=NULL) defined=true;
    else {
        semant_error(actuals)<<"function : "<<tempName<<" is not defined \n";
        return No_type;
    }
    //if printf check
    //printf func paras 1(string) least
    if (tempName==Printf) {
            if (actuals->len()==0) {
                semant_error(actuals)<<"Printf has 1 string parameters at least\n";
            }
            else {
                if(actuals->nth(0)->checkType()!=String) {
                    semant_error(actuals)<<"Printf's first parameter's type should be string not:"<<actuals->nth(0)->checkType()<<endl;
                }
            }
            return Void;
        }

    //check  if paras are constitent
    //(declared) and match
    //to do check f()
    else if (defined)
        {for (int i = actuals->first();actuals->more(i);i=actuals->next(i)){
        
            Symbol tempActualType=actuals->nth(i)->checkType();    
            Symbol formalType= info->getVariables()->nth(i)->getType();
            if (formalType != tempActualType) {
                semant_error(actuals)<<"paras don't match: "<<"formal:"<<formalType<<"##"<<"actual:"<<tempActualType<<endl;
            }
        }
    //set returntype todo check return type
        setType(info->getType());
        return info->getType();
        }
}

Symbol Actual_class::checkType(){
    setType(expr->checkType());
    return expr->checkType();
}

Symbol Assign_class::checkType(){
    //type of lvalue
    Symbol* lvalueType=objectEnv.lookup(lvalue);
    if(lvalueType==NULL) {
        lvalueType=globalVarTable.lookup(lvalue);
    }
    if (lvalueType==NULL) {
        semant_error(value)<<"lvalue not declared: "<<lvalue->get_string()<<endl;
        return No_type;
    }
    else {
    Symbol ltype=*lvalueType;
    Symbol rtype=value->checkType();
    //check rtype==ltype
    if(ltype!=rtype){
        semant_error(value)<<"lvalue not macth rvalue :"<<"lvalue:"<<ltype<<"##"<<"rvalue:"<<rtype<<endl;
    }
    setType(ltype);
    return ltype;
    }
}

Symbol Add_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalAdd=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalAdd) {
        semant_error(e1)<<"the operands of add must be float or int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        if(isfloat) {setType(Float);
         return Float;}
       else {setType(Int);
        return Int;}
    }
}

Symbol Minus_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalMinus=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalMinus) {
        semant_error(e1)<<"the operands of minus must be float or int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        if(isfloat) {setType(Float);
         return Float;}
       else {setType(Int);
        return Int;}
    }

 
}

Symbol Multi_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalMulti=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalMulti) {
        semant_error(e1)<<"the operands of multi must be float or int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        if(isfloat) {setType(Float);
         return Float;}
       else {setType(Int);
        return Int;}
    }
 
}

Symbol Divide_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalDivide=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalDivide) {
        semant_error(e1)<<"the operands of divide must be float or int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        if(isfloat) {setType(Float);
         return Float;}
       else {setType(Int);
        return Int;}
    }

}

Symbol Mod_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalMod=(ltype==Int)&&(rtype==Int);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalMod) {
        semant_error(e1)<<"the operands of % must be int and int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        // if(isfloat) return Float;
        setType(Int);
        return Int;
    }

}

Symbol Neg_class::checkType(){
    Symbol type=e1->checkType();
    bool leagalNeg=type==Float||type==Int;
    if(!leagalNeg) {
        semant_error(e1)<<"the oprand of neg(-) must be int or float : not "<<type<<endl;
        return No_type;
    }
    else {
        setType(type);
        return type;}

}

Symbol Lt_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalLt=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalLt) {
        semant_error(e1)<<"the operands of < must be float or int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        // if(isfloat) return Float;
        // return Int;
        setType(Bool);
        return Bool;
    }

}

Symbol Le_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalLe=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalLe) {
        semant_error(e1)<<"the operands of <= must be float or int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        // if(isfloat) return Float;
        // return Int;
        setType(Bool);
        return Bool;
    }

}

Symbol Equ_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalEqu=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool leagalLogicalEqu=(ltype==Bool)||(rtype==Bool);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalEqu && !leagalLogicalEqu) {
        semant_error(e1)<<"the operands of == must be (float/int int/float) or (bool bool) : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        // if(isfloat) return Float;
        // return Int;
        setType(Bool);
        return Bool;
    }

}

Symbol Neq_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalNeq=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool leagalLogicalNeq=(ltype==Bool)&&(rtype==Bool);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(leagalNeq) {
        setType(Bool);
        return Bool;
    }
    else if (leagalLogicalNeq){
        setType(Bool);
        return Bool;
    }
    else {
        semant_error(e1)<<"the operands of != must be (float/int float/int) or (bool bool) : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }

}

Symbol Ge_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalGe=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalGe) {
        semant_error(e1)<<"the operands of >= must be float or int : "<<"ltype:"<<ltype<<"##"<<"rtype"<<rtype<<endl;
        return No_type;
    }
    else {
        // if(isfloat) return Float;
        // return Int;
        setType(Bool);
        return Bool;
    }

}

Symbol Gt_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalGe=(ltype==Float||ltype==Int)&&(rtype==Float||rtype==Int);
    bool isfloat=(ltype==Float)||(rtype==Float);
    if(!leagalGe) {
        semant_error(e1)<<"the operands of > must be float or int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        // if(isfloat) return Float;
        // return Int;
        setType(Bool);
        return Bool;
    }

}

Symbol And_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalAnd=(ltype==Bool)&&(rtype==Bool);
    if (!leagalAnd) {
        semant_error(e1)<<"the oprands of && must be bool and bool : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        setType(Bool);
        return Bool;}

}

Symbol Or_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalOr=(ltype==Bool)&&(rtype==Bool);
    if (!leagalOr) {
        semant_error(e1)<<"the oprands of || must be bool and bool : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        setType(Bool);
        return Bool;}

}

Symbol Xor_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalXor=ltype==Bool&&rtype==Bool;
    bool leagalBitxor=ltype==Int&&rtype==Int;
//bool xor
    if (leagalXor) {
        setType(Bool);
        return Bool;
    }
    else if (leagalBitxor) {
        setType(Int);
        return Int;
    }
    else {
        semant_error(e1)<<"the oprands of ^ must be (int,int) or (bool,bool) :"<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }



}

Symbol Not_class::checkType(){
    Symbol type=e1->checkType();
    bool leagalNot=type==Bool;
    if (!leagalNot) {
        semant_error(e1)<<"the oprands of ! must be bool : not" <<type<<endl;
        return No_type;
    }
    else {
        setType(Bool);
        return Bool;}
}

Symbol Bitand_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalBitand=(ltype==Int)&&(rtype==Int);
    if (!leagalBitand) {
        semant_error(e1)<<"the oprands of & must be int and int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        setType(Int);
        return Int;}
}

Symbol Bitor_class::checkType(){
    Symbol ltype=e1->checkType();
    Symbol rtype=e2->checkType();
    bool leagalBitor=(ltype==Int)&&(rtype==Int);
    if (!leagalBitor) {
        semant_error(e1)<<"the oprands of | must be int and int : "<<"ltype:"<<ltype<<"##"<<"rtype:"<<rtype<<endl;
        return No_type;
    }
    else {
        setType(Int);
        return Int;}

}

Symbol Bitnot_class::checkType(){
    Symbol type=e1->checkType();
    bool leagalBitnot=(type==Int);
    if (!leagalBitnot) {
        semant_error(e1)<<"the oprands of ~ must be int : not "<<type<<endl;
        return No_type;
    }
    else {
        setType(Int);
        return Int;}

}

Symbol Const_int_class::checkType(){
    setType(Int);
    return type;
}

Symbol Const_string_class::checkType(){
    setType(String);
    return type;
}

Symbol Const_float_class::checkType(){
    setType(Float);
    return type;
}

Symbol Const_bool_class::checkType(){
    setType(Bool);
    return type;
}

Symbol Object_class::checkType(){
    //find data in scope
    Symbol* tempType=objectEnv.lookup(var);
    if (tempType==NULL){
        tempType=globalVarTable.lookup(var);
    }
    //if not found
    if (tempType==NULL) {
        semant_error()<<line_number<<":"<<"variable not declared : "<<var->get_string()<<endl;
        return No_type;
    }
    else {
        setType(*tempType);
        return getType();
    }


}

Symbol No_expr_class::checkType(){
    setType(Void);
    return getType();
}

void Program_class::semant() {
    initialize_constants();
    install_calls(decls);
    check_main();
    install_globalVars(decls);
    check_calls(decls);

    
    if (semant_errors > 0) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }
}



