

#include "Symtab.hh"
#include <map>
#include <iostream>

enum typeExp{
    ABSTRACT,
    EMPTY,
    SEQ,
    ASSIGNS,
    RETURN,
    IF,
    WHILE,
    FOR,
    PROCCALL,
    IDENTIFIER,
    ARRAYREF,
    MEMBER,
    ARROW,
    NONREF,
    OP_BINARY,
    OP_UNARY,
    ASSIGNE,
    FUNCALL,
    INTCONST,
    FLOATCONST,
    STRINGCONST
};

enum SupertypeExp{
    STATEMENT,
    EXP,
    REF
};


class abstract_astnode
{
public:
    virtual void print(int blanks) {};
    enum typeExp astnode_type;
    enum SupertypeExp astnode_supertypen;
    bool isInitialised = false;
    abstract_astnode(bool isInit, enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : isInitialised(isInit), astnode_type(astnode_ty), astnode_supertypen(astnode_superty) {};
    abstract_astnode(enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : astnode_type(astnode_ty), astnode_supertypen(astnode_superty) {};
    abstract_astnode(){};
};



class statement_astnode : public abstract_astnode{

public:
    statement_astnode() {};
    virtual void print(int blanks) {}; 
    statement_astnode(bool isInit, enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : abstract_astnode(isInit,astnode_ty,astnode_superty) {};
    statement_astnode(enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : abstract_astnode(astnode_ty,astnode_superty) {};
};

class exp_astnode: public abstract_astnode{
public:
    Type* type_ = nullptr;
    exp_astnode() {};
    virtual void print(int blanks) {};
    exp_astnode(bool isInit, enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : abstract_astnode(isInit,astnode_ty,astnode_superty) {};
    exp_astnode(enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : abstract_astnode(astnode_ty,astnode_superty) {};
};

class ref_astnode : public exp_astnode{
public:
    ref_astnode() {};
    virtual void print(int blanks) {};
    ref_astnode(bool isInit, enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : exp_astnode(isInit,astnode_ty,astnode_superty) {};
    ref_astnode(enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : exp_astnode(astnode_ty,astnode_superty) {};
};

// {
// "identifier": "fib"
// }
class identifier_astnode : public ref_astnode{
public:
    std::string id_;
    void print(int blanks){
    std::cout<< "{\n\"identifier\": \"" + id_ + "\"\n}\n";
};
    identifier_astnode(std::string id) : id_(id), ref_astnode(true,IDENTIFIER,REF) {};
    identifier_astnode() : ref_astnode(IDENTIFIER,REF){};
};
// {
// "intconst": 10}
class intconstant_astnode : public exp_astnode{
public:
    int number_;
    std::string number_string_;    
    void print(int blanks){
    std::cout << "{\n\"intconst\": "+number_string_+"}\n";
};
    intconstant_astnode(std::string number) : exp_astnode(INTCONST,EXP) {
    number_string_ = number;
    type_ = new BaseType("int");
    number_ = stoi(number);
};
    intconstant_astnode() : exp_astnode(INTCONST,EXP) {};
};

// {
// "intconst": 54.56}
class floatconstant_astnode : public exp_astnode{
public:
    double number_;
    std::string number_string_;    
    void print(int blanks){
    std::cout << "{\n\"floatconst\": " << number_ << "}\n";
};
    floatconstant_astnode(std::string number_string) : exp_astnode(FLOATCONST,EXP) {
    number_string_ = number_string;
    type_ = new BaseType("float");
    number_ = std::stod(number_string);
};
    floatconstant_astnode() : exp_astnode(FLOATCONST,EXP) {};
};

// {
// "stringconst": "dsdsdsd\n"
// }
class stringconstant_astnode : public exp_astnode{
public:
    std::string the_string_;    
    void print(int blanks){
    std::cout << "{\n\"stringconst\": " << the_string_ << "\n}\n";
};
    stringconstant_astnode(std::string the_string) : the_string_(the_string) , exp_astnode(true, STRINGCONST,EXP) {type_ = new BaseType("string");};
    stringconstant_astnode() : exp_astnode(STRINGCONST,EXP) {};
};

// { "arrayref": {
// "array": 
// {
// "identifier": "x"
// }
// ,
// "index": 
// {
// "intconst": 3}
// }
// }

class arrayref_astnode : public ref_astnode{
public:
    exp_astnode* the_array_ = nullptr;
    exp_astnode* the_index_ = nullptr;
    arrayref_astnode(exp_astnode* the_array, exp_astnode* the_index) :the_array_(the_array), the_index_(the_index),ref_astnode(true, ARRAYREF,REF) {};
    arrayref_astnode() : ref_astnode(ARRAYREF,REF) {};
    void print(int blanks){
    std::cout << "{ \"arrayref\": {\n\"array\":\n";
    the_array_->print(0);
    std::cout << ",\n\"index\":\n";
    the_index_->print(0);
    std::cout << "}\n}\n";
}
;
    // bool addArray(exp_astnode* the_array);
    // bool addIndex(exp_astnode* the_index);
};

// { "member": {
// "struct": 
// { "arrayref": {
// "array": 
// {
// "identifier": "x"
// }
// ,
// "index": 
// {
// "intconst": 3}
// }
// }
// ,
// "field": 
// {
// "identifier": "x"
// }
// }
// }


class member_astnode : public ref_astnode{
public:
    exp_astnode* the_struct_variable_ = nullptr;
    identifier_astnode* the_member_ = nullptr;
    member_astnode(exp_astnode* the_struct_variable, identifier_astnode* the_member) : the_struct_variable_(the_struct_variable), the_member_(the_member), ref_astnode(true, MEMBER,REF) {};
    member_astnode() : ref_astnode(MEMBER,REF) {};
    void print(int blanks){
    std::cout << "{ \"member\": {\n\"struct\":\n";
    the_struct_variable_->print(0);
    std::cout << ",\n\"field\":\n";
    the_member_->print(0);
    std::cout << "}\n}\n";
}
;
    // bool addStructVariable(exp_astnode* the_struct_variable);
    // bool addMember(identifier_astnode* the_member);
};

// { "arrow": {
// "pointer": 
// {
// "identifier": "x"
// }
// ,
// "field": 
// {
// "identifier": "y"
// }
// }
// }

class arrow_astnode : public ref_astnode{
public:
    exp_astnode* the_struct_variable_ = nullptr;
    identifier_astnode* the_member_ = nullptr;
    arrow_astnode(exp_astnode* the_struct_variable, identifier_astnode* the_member) : the_struct_variable_(the_struct_variable), the_member_(the_member), ref_astnode(true, ARROW,REF) {};
    arrow_astnode() : ref_astnode(ARROW,REF) {};
    void print(int blanks){
    std::cout << "{ \"arrow\": {\n\"pointer\":\n";
    the_struct_variable_->print(0);
    std::cout << ",\n\"field\":\n";
    the_member_->print(0);
    std::cout << "}\n}\n";
};
    // bool addStructVariable(exp_astnode* the_struct_variable);
    // bool addMember(identifier_astnode* the_member);
};

// { "funcall": {
// "fname": 
// {
// "identifier": "fib"
// }
// ,
// "params": [
// {
// "identifier": "i"
// }


// ]
// }
// }

class funcall_astnode : public exp_astnode {
public:
    identifier_astnode* fname_;
    std::vector<exp_astnode*> function_call_; 
    // The first member is the identifier for the function, the others are what is passed.

    funcall_astnode(identifier_astnode* fname ,std::vector<exp_astnode*> function_call) : fname_(fname), function_call_(function_call), exp_astnode(true, FUNCALL,EXP) {};
    funcall_astnode()  : exp_astnode(FUNCALL,EXP) {};
    void print(int blanks){
    std::cout << "{ \"funcall\": {\n\"fname\":\n";
    fname_->print(0);
    std::cout << ",\n\"params\": [\n";
    for(size_t i = 0;i<function_call_.size();i++){
        if(i==(function_call_.size()-1)){
            function_call_[i]->print(0);
            std::cout<<"\n\n";
        }else{
            function_call_[i]->print(0);
            std::cout<<",\n";
        }
    }
    std::cout << "]\n}\n}\n";
};
    // void addExpNode(exp_astnode* expnode);
    // void addfname(identifier_astnode* name);
};

// { "assignE": {
// "left": 
// {
// "identifier": "i"
// }
// ,
// "right": 
// { "op_binary": {
// "op": "PLUS_INT"
// ,
// "left": 
// {
// "identifier": "i"
// }
// ,
// "right": 
// {
// "intconst": 1}
// }
// }
// }

class assignE_exp : public exp_astnode{
public:
    exp_astnode* lhs_ = nullptr;
    exp_astnode* rhs_ = nullptr;
    assignE_exp(exp_astnode* lhs, exp_astnode* rhs) : lhs_(lhs), rhs_(rhs), exp_astnode(true,ASSIGNE,EXP){};
    assignE_exp() : exp_astnode(ASSIGNE,EXP) {};
    void print(int blanks){
    std::cout << "{ \"assignE\": {\n\"left\":\n";
    lhs_->print(0);
    std::cout << ",\n\"right\": \n";
    rhs_->print(0);
    std::cout << "}\n}\n";
};
    // bool addLHS(exp_astnode* lhs);
    // bool addRHS(exp_astnode* rhs);
};

// { "op_unary": {
// "op": "PP"
// ,
// "child": 
// {
// "identifier": "i"
// }
// }
// }
class op_unary_astnode : public exp_astnode{
public:
    std::string operator_;
    exp_astnode* operand_ = nullptr;
    op_unary_astnode(std::string operator__, exp_astnode* operand) : operator_(operator__), operand_(operand), exp_astnode(true, OP_UNARY,EXP) {};
    op_unary_astnode() : exp_astnode(OP_UNARY,EXP) {};
    void print(int blanks){
    std::cout << "{ \"op_unary\": {\n\"op\": \"" << operator_ << "\"\n,\n\"child\":\n";
    operand_->print(0);
    std::cout << "}\n}\n";
};
    // bool addOperator(std::string operator_);
    // bool addOperand(exp_astnode* operand);
};

// { "op_binary": {
// "op": "LT_OP_INT"
// ,
// "left": 
// {
// "identifier": "i"
// }
// ,
// "right": 
// {
// "intconst": 10}
// }
// }

class op_binary_astnode : public exp_astnode{
public:
    std::string operator_;
    exp_astnode* left_operand_ = nullptr;
    exp_astnode* right_operand_ = nullptr;
    op_binary_astnode(std::string operator__, exp_astnode* left_operand, exp_astnode* right_operand)  : operator_(operator__), left_operand_(left_operand), right_operand_(right_operand), exp_astnode(true,OP_BINARY,EXP) {};
    op_binary_astnode() : exp_astnode( OP_BINARY,EXP) {};
    void print(int blanks){
    std::cout << "{ \"op_binary\": {\n\"op\": \"" << operator_ << "\"\n,\n\"left\":\n";
    left_operand_->print(0);
    std::cout << ",\n\"right\":\n";
    right_operand_->print(0);
    std::cout << "}\n}\n";
};
    // bool addOperator(std::string operator__);
    // bool addleftOperand(exp_astnode* operand);
    // bool addrightOperand(exp_astnode* operand);
};

// Just the string "empty is used to print it"
class empty_astnode : public statement_astnode {
public:
    empty_astnode() : statement_astnode(true,EMPTY,STATEMENT) {};
    void print(int blanks){
    std::cout << "\"empty\"\n";
};
};

// "seq" : Followed by list 
class seq_astnode : public statement_astnode {
public:
    std::vector<statement_astnode*> statements_;
    seq_astnode(std::vector<statement_astnode *> statements) : statements_(statements), statement_astnode(true, SEQ, STATEMENT) {};
    seq_astnode() : statement_astnode(SEQ, STATEMENT) {};
    void addStatement(statement_astnode* statement){
    statements_.push_back(statement);
};
    void print(int blanks){
    std::cout << "{\n\"seq\": [\n";
    for(size_t i = 0;i<statements_.size();i++){
        if(i==(statements_.size()-1)){
            statements_[i]->print(0);
            std::cout<<"\n\n";
        }else{
            statements_[i]->print(0);
            std::cout<<",\n";
        }
    }
    std::cout << "]\n}\n";
};
};

// assignS" : {"left" : {"identifier": "y"}, "right":{"floatconst":12.56}}
class assignS_astnode : public statement_astnode {
public:
    exp_astnode* lvalue_ = nullptr;
    exp_astnode* rvalue_ = nullptr;
    assignS_astnode(exp_astnode* lvalue, exp_astnode* rvalue) : lvalue_(lvalue), rvalue_(rvalue), statement_astnode(true,ASSIGNS,STATEMENT) {};
    assignS_astnode() : statement_astnode(ASSIGNS,STATEMENT) {};
    // bool addlValue(exp_astnode* val);
    // bool addrValue(exp_astnode* val);
    void print(int blanks){
    std::cout << "{ \"assignS\": {\n\"left\":\n";
    lvalue_->print(0);
    std::cout << ",\n\"right\":\n";
    rvalue_->print(0);
    std::cout << "}\n}\n";
};
};


// "return": {exp_astnode enclosed}
class return_astnode : public statement_astnode {
public:
    exp_astnode* return_exp_ = nullptr;
    return_astnode(exp_astnode* return_exp) : return_exp_(return_exp), statement_astnode(true, RETURN,STATEMENT) {};
    return_astnode() : statement_astnode(RETURN,STATEMENT) {};
    void print(int blanks){
    std::cout << "{\n\"return\":\n";
    return_exp_->print(0);
    std::cout << "}\n";
};
};

// "proccall": {
// "fname": 
// {
// "identifier": "fib"
// }
// ,
// "params": [
// {
// "identifier": "ar"
// }


// ]
// }

class proccall_astnode : public statement_astnode {
public:
    identifier_astnode* fname_;
    std::vector<exp_astnode*> proccall_;
    // The first member is the identifier for the function, the others are what is passed.
    proccall_astnode(identifier_astnode* fname ,std::vector<exp_astnode*> proccall) : fname_(fname), proccall_(proccall), statement_astnode(true,PROCCALL,STATEMENT) {};
    proccall_astnode()  : statement_astnode(PROCCALL,STATEMENT) {};
    // void addExpNode(exp_astnode* expnode);
    void print(int blanks){
    std::cout << "{ \"proccall\": {\n\"fname\":\n";
    fname_->print(0);
    std::cout << ",\n\"params\": [\n";
    for(size_t i = 0;i<proccall_.size();i++){
        if(i==(proccall_.size()-1)){
            proccall_[i]->print(0);
            std::cout<<"\n\n";
        }else{
            proccall_[i]->print(0);
            std::cout<<",\n";
        }
    }
    std::cout << "]\n}\n}\n";
}
;
    // void addfname(identifier_astnode* name);
};


// { "if": {
// "cond": 
// { "op_binary": {
// "op": "GT_OP_INT"
// ,
// "left": 
// {
// "intconst": 1}
// ,
// "right": 
// {
// "intconst": 0}
// }
// }
// ,
// "then": 
// {
// "seq": [
// "empty"


// ]
// }
// ,
// "else": 
// {
// "seq": [
// "empty"


// ]
// }
// }
// }

class if_astnode : public statement_astnode {
public:
    exp_astnode* conditional_ = nullptr;
    statement_astnode* if_part_ = nullptr;
    statement_astnode* else_part_ = nullptr;
    if_astnode(exp_astnode* conditional, statement_astnode* if_part, statement_astnode* else_part) : conditional_(conditional), if_part_(if_part), else_part_(else_part),statement_astnode(true,IF, STATEMENT) {};
    if_astnode() : statement_astnode(IF, STATEMENT) {};
    // bool addConditional(exp_astnode* conditional);
    // bool addifPart(statement_astnode* statement);
    // bool addelsePart(statement_astnode* statement);
    void print(int blanks){
    std::cout << "{ \"if\": {\n\"cond\":\n";
    conditional_->print(0);
    std::cout << ",\n\"then\":\n";
    if_part_->print(0);
    std::cout << ",\n\"else\":\n";
    else_part_->print(0);
    std::cout << "}\n}\n";
}
;
};


// { "while": {
// "cond": 
// { "op_binary": {
// "op": "GT_OP_INT"
// ,
// "left": 
// {
// "intconst": 1}
// ,
// "right": 
// {
// "intconst": 0}
// }
// }
// ,
// "stmt": 
// {
// "seq": [
// "empty"


// ]
// }
// }
// }

class while_astnode : public statement_astnode {
public:
    exp_astnode* conditional_ = nullptr;
    statement_astnode* loop_part_ = nullptr;
    while_astnode(exp_astnode* conditional, statement_astnode* loop_part) : conditional_(conditional), loop_part_(loop_part), statement_astnode(true,WHILE,STATEMENT) {};
    while_astnode() : statement_astnode(WHILE,STATEMENT) {};
    // bool addConditional(exp_astnode* conditional);
    // bool addloopPart(statement_astnode* statement);
    void print(int blanks){
    std::cout << "{ \"while\": {\n\"cond\":\n";
    conditional_->print(0);
    std::cout << ",\n\"stmt\":\n";
    loop_part_->print(0);
    std::cout << "}\n}\n";
}
;
};

// { "for": {
// "init": 
// { "assignE": {
// "left": 
// {
// "identifier": "i"
// }
// ,
// "right": 
// {
// "intconst": 0}
// }
// }
// ,
// "guard": 
// { "op_binary": {
// "op": "LT_OP_INT"
// ,
// "left": 
// {
// "identifier": "i"
// }
// ,
// "right": 
// {
// "intconst": 10}
// }
// }
// ,
// "step": 
// { "assignE": {
// "left": 
// {
// "identifier": "i"
// }
// ,
// "right": 
// { "op_binary": {
// "op": "PLUS_INT"
// ,
// "left": 
// {
// "identifier": "i"
// }
// ,
// "right": 
// {
// "intconst": 1}
// }
// }
// }
// }
// ,
// "body": 
// {
// "seq": [
// "empty"


// ]
// }
// }
// }

class for_astnode : public statement_astnode {
public:
    exp_astnode* initializer_ = nullptr;
    exp_astnode* iterative_ = nullptr;
    exp_astnode* conditional_ = nullptr;
    statement_astnode* loop_part_ = nullptr;
    for_astnode(exp_astnode* initializer, exp_astnode* iterative, exp_astnode* conditional, statement_astnode* loop_part) : initializer_(initializer), iterative_(iterative), conditional_(conditional), loop_part_(loop_part), statement_astnode(true,FOR,STATEMENT) {};
    for_astnode() :statement_astnode(FOR,STATEMENT) {};
    // bool addInitializer(exp_astnode* initializer);
    // bool addIterative(exp_astnode* iterative);
    // bool addConditional(exp_astnode* conditional);
    // bool addloopPart(statement_astnode* statement);
    void print(int blanks){
    std::cout << "{ \"for\": {\n\"init\":\n";
    initializer_->print(0);
    std::cout << ",\n\"guard\":\n";
    conditional_->print(0);
    std::cout << ",\n\"step\":\n";
    iterative_->print(0);
    std::cout << ",\n\"body\":\n";
    loop_part_->print(0);
    std::cout << "}\n}\n";
}

;
};
