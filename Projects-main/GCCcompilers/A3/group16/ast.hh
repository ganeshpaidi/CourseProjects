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
    STRINGCONST,
    PRINTF
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
    virtual int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {return z;};
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
    virtual int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {return z;};
    statement_astnode(bool isInit, enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : abstract_astnode(isInit,astnode_ty,astnode_superty) {};
    statement_astnode(enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : abstract_astnode(astnode_ty,astnode_superty) {};
};

class exp_astnode: public abstract_astnode{
public:
    Type* type_ = nullptr;
    bool isAddress = false;
    exp_astnode() {};
    virtual void print(int blanks) {};
    virtual int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {return z;};
    exp_astnode(bool isInit, enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : abstract_astnode(isInit,astnode_ty,astnode_superty) {};
    exp_astnode(enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : abstract_astnode(astnode_ty,astnode_superty) {};
};

class ref_astnode : public exp_astnode{
public:
    ref_astnode() {};
    virtual void print(int blanks) {};
    virtual int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {return z;};
    
    ref_astnode(bool isInit, enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : exp_astnode(isInit,astnode_ty,astnode_superty) {};
    ref_astnode(enum typeExp astnode_ty,  enum SupertypeExp astnode_superty) : exp_astnode(astnode_ty,astnode_superty) {};
};

// {
// "identifier": "fib"
// }
class identifier_astnode : public ref_astnode{
public:
    std::string id_;
    std::string access_;
    void print(int blanks){
    std::cout<< "{\n\"identifier\": \"" + id_ + "\"\n}\n";
};

    identifier_astnode(std::string id) : id_(id), ref_astnode(true,IDENTIFIER,REF) {};
    identifier_astnode() : ref_astnode(IDENTIFIER,REF){};


    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        isAddress = true;
        if(fentry->var_map_[id_]->scope_ == "param" && type_->node_type_==ARRAY) {
            std::cout << "	movl " << fentry->var_map_[id_]->offset_ << "(%ebp), %eax" << "\n";
            std::cout << "	pushl %eax" << "\n";
        }else{
            std::cout << "	leal " << fentry->var_map_[id_]->offset_ << "(%ebp), %eax" << "\n";
            std::cout << "	pushl %eax" << "\n";
        }
            
        return z;
    };


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
    
    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        isAddress = false;
        std::cout << "	pushl $" << number_ << "\n";
        return z;
    };
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
    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        isAddress = false;
        std::cout << "	pushl $" << the_string_ << "\n";
        return z;
    };
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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        isAddress = true;
        
        the_array_->gencode(sMap,fentry,z,fmap);
        the_index_->gencode(sMap,fentry,z,fmap);

        std::cout << "	popl %eax\n";
        std::cout << "	popl %ebx\n";

        if(the_index_->isAddress){
            std::cout << "	movl (%eax), %eax\n";
        }

        if(the_array_->type_->node_type_ == POINTER && the_array_->isAddress){
            std::cout << "	movl (%ebx), %ebx\n";
        }

        int s = ((ChildType*)the_array_->type_)->child_type_->getSize();

        std::cout << "  imull $" << s << ", %eax\n";
        std::cout << "  addl %ebx, %eax\n";

        std::cout << "	pushl %eax\n";

        return z;
    };
    
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

    // src and dest are the end pointers, bottom-most highest address
    void copy(std::string reg, int src,int dest, int s) {
        int i = s/4;
        while(i--){
            src -= 4;
            dest -= 4;
            std::cout << "	movl " << src << "(%" << reg << "), %eax";
            std::cout << "  movl %eax, " << dest << "(%" << reg << ")\n";
        }
    };
    
    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        the_struct_variable_->gencode(sMap,fentry,z,fmap);
        isAddress = the_struct_variable_->isAddress;
        entry* e = sMap[the_struct_variable_->type_->getRepresentation()]->var_map_[the_member_->id_];
        
        if(isAddress) {
            std::cout << "	popl %eax\n";
            std::cout << "	addl $" << e->offset_ <<", %eax\n";
            std::cout << "  pushl %eax\n";
        }else{
            int s = sMap[the_struct_variable_->type_->getRepresentation()]->size_;
            int s1 = e->size_;
            copy("esp",e->offset_+s1,s,s1);
            std::cout << "  addl $" << (s-s1) << ", %esp\n";
        }
        return z;
    };
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
    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        the_struct_variable_->gencode(sMap,fentry,z,fmap);
        isAddress = true;
        entry* e = sMap[((ChildType*)the_struct_variable_->type_)->child_type_->getRepresentation()]->var_map_[the_member_->id_];
        if(the_struct_variable_->isAddress) {
            std::cout << "	popl %eax\n";
            std::cout << "	movl (%eax), %eax\n";
            std::cout << "	addl $" << e->offset_ <<", %eax\n";
            std::cout << "  pushl %eax\n";
        }else{
            std::cout << "	popl %eax\n";
            std::cout << "	addl $" << e->offset_ <<", %eax\n";
            std::cout << "  pushl %eax\n";
        }
        return z;
    };
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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        isAddress = false;
        int s = type_->getSize();
        std::cout << "  subl $" << s << ", %esp\n";
        int totalsize = 4;
        for(auto it : function_call_) {
            it->gencode(sMap,fentry,z,fmap);
            if(it->isAddress){
                if(it->type_->node_type_==POINTER){
                    std::cout << "	popl %eax\n";
                    std::cout << "	movl (%eax), %eax\n";
                    std::cout << "	pushl %eax\n";
                }else if(it->type_->node_type_==BASE_TYPE){
                    std::cout << "	popl %eax\n";
                    int s = it->type_->getSize();
                    std::cout << "	subl $" << s <<",%esp\n";
                    int i = s/4;
                    int src = s;
                    while(i--){
                        src-=4;
                        std::cout << "	movl "<< src <<"(%eax), %ebx\n";
                        std::cout << "	movl %ebx,"<< src <<"(%esp)\n";
                    }
                }

            }

            if(it->type_->node_type_==BASE_TYPE){
                totalsize += it->type_->getSize();
            }else{
                totalsize += 4;
            }
            // std::cout << totalsize << std::endl;
        }

        std::cout << "  subl $4, %esp\n";
        std::cout << "  call " << fname_->id_ << "\n";
        std::cout << "  addl $" << totalsize << ", %esp\n";       
        return z;
    };

};


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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        isAddress = false;
        int s = lhs_->type_->getSize();
        rhs_->gencode(sMap,fentry,z,fmap);
        lhs_->gencode(sMap,fentry,z,fmap);
        std::cout << "  popl %eax\n";
        if(rhs_->isAddress){
            std::cout << "  popl %ebx\n";
            int i = s/4;
            while(i--){
                s -= 4;
                std::cout << "  movl " << s << "(%ebx), %ecx\n";
                std::cout << "  movl %ecx, " << s << "(%eax)\n";                
            }
        }else{
            int i = s/4;
            while(i--){
                s -= 4;
                std::cout << "  movl " << s << "(%esp), %ecx\n";
                std::cout << "  movl %ecx, " << s << "(%eax)\n";                
            }
            std::cout << "  addl $" << lhs_->type_->getSize() << ", %esp\n";
        }
        return z;
    };
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
    
    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        isAddress = false;
        operand_->gencode(sMap,fentry,z,fmap);
        if(operator_=="UMINUS"){
            if(operand_->isAddress){
                std::cout << "  popl %eax\n";
                std::cout << "  movl (%eax), %eax\n";
                std::cout << "  negl %eax\n";
                std::cout << "  pushl %eax\n";
            }else{
                std::cout << "  popl %eax\n";
                std::cout << "  negl %eax\n";
                std::cout << "  pushl %eax\n";
            }
        }else if(operator_=="NOT"){
            if(operand_->isAddress){
                std::cout << "  popl %eax\n";
                std::cout << "  movl (%eax), %eax\n";
                std::cout << "  cmpl $0, %eax\n";
                std::cout << "  sete %al\n";
                std::cout << "  movzbl %al, %eax\n"; 
                std::cout << "  pushl %eax\n";
            }else{
                std::cout << "  popl %eax\n";
                std::cout << "  cmpl $0, %eax\n";
                std::cout << "  sete %al\n";
                std::cout << "  movzbl %al, %eax\n"; 
                std::cout << "  pushl %eax\n";
            }
        }else if(operator_=="ADDRESS") {
        }else if(operator_=="DEREF"){
            isAddress = true;
            if(operand_->isAddress){
                if(operand_->type_->node_type_==POINTER){
                    std::cout << "  popl %eax\n";
                    std::cout << "  movl (%eax), %eax\n";
                    std::cout << "  pushl %eax\n";
                }
            }
        }else{
            std::cout << "  popl %ebx\n";
            std::cout << "  movl (%ebx), %eax\n";
            std::cout << "  pushl %eax\n";
            std::cout << "  addl $1, %eax\n";
            std::cout << "  movl %eax, (%ebx)\n";
            

        }
        return z;
    };

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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        isAddress = false;
        right_operand_->gencode(sMap,fentry,z,fmap);
        left_operand_->gencode(sMap,fentry,z,fmap);
        
        
        std::cout << "  popl %eax\n";
        std::cout << "  popl %ebx\n";

        if(left_operand_->isAddress && left_operand_->type_->node_type_!=ARRAY){
            std::cout << "  movl (%eax), %eax\n";
        }

        if(right_operand_->isAddress && right_operand_->type_->node_type_!=ARRAY) {
            std::cout << "  movl (%ebx), %ebx\n";
        }
        
        if(left_operand_->type_->node_type_!=BASE_TYPE && right_operand_->type_->node_type_==BASE_TYPE){
            std::cout << "\t imull $" << left_operand_->type_->getSize() << ", %ebx\n";
        }else if(left_operand_->type_->node_type_==BASE_TYPE && right_operand_->type_->node_type_!=BASE_TYPE){
            std::cout << "\t imull $" << left_operand_->type_->getSize() << ", %eax\n";
        }

        if(operator_=="OR_OP"){
            std::cout << "  orl %ebx, %eax\n";
            std::cout << "  cmpl $0, %eax\n";
            std::cout << "  setne %al\n";
            std::cout << "  movzbl %al, %eax\n";
        }else if(operator_=="AND_OP"){
            std::cout << "  andl %ebx, %eax\n";
            std::cout << "  cmpl $0, %eax\n";
            std::cout << "  setne %al\n";
            std::cout << "  movzbl %al, %eax\n";
        }else if(operator_=="EQ_OP_INT") {
            std::cout << "  cmpl %ebx, %eax\n";
            std::cout << "  sete %al\n";
            std::cout << "  movzbl %al, %eax\n";
        }else if(operator_=="NE_OP_INT"){
            std::cout << "  cmpl %ebx, %eax\n";
            std::cout << "  setne %al\n";
            std::cout << "  movzbl %al, %eax\n";
        }else if(operator_=="LT_OP_INT"){
            std::cout << "  cmpl %ebx, %eax\n";
            std::cout << "  setl %al\n";
            std::cout << "  movzbl %al, %eax\n";
        }else if(operator_=="GT_OP_INT"){
            std::cout << "  cmpl %ebx, %eax\n";
            std::cout << "  setg %al\n";
            std::cout << "  movzbl %al, %eax\n";
        }else if(operator_=="LE_OP_INT"){
            std::cout << "  cmpl %ebx, %eax\n";
            std::cout << "  setle %al\n";
            std::cout << "  movzbl %al, %eax\n";
        }else if(operator_=="GE_OP_INT"){
            std::cout << "  cmpl %ebx, %eax\n";
            std::cout << "  setge %al\n";
            std::cout << "  movzbl %al, %eax\n";
        }else if(operator_ == "PLUS_INT"){
            std::cout << "  addl %ebx, %eax\n";
        }else if(operator_ == "MINUS_INT"){
            std::cout << "  subl %ebx, %eax\n";
        }else if(operator_ == "MULT_INT"){
            std::cout << "  imull %ebx, %eax\n";
        }else if(operator_ == "DIV_INT"){
            std::cout << "  movl $0, %edx\n";
            std::cout << "\tcltd\n";
            std::cout << "  idivl %ebx\n";
        }

        if(left_operand_->type_->node_type_!=BASE_TYPE && right_operand_->type_->node_type_!=BASE_TYPE){
            std::cout << "\tmovl $0, %edx\n";
            std::cout << "\tcltd\n";
            std::cout << "\t idivl $" << left_operand_->type_->getSize() << "\n";
        }


        std::cout << "  pushl %eax\n";
        return z;
    };

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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        for(auto u: statements_) {
            z = u->gencode(sMap,fentry,z,fmap);
        }
        return z;
    };
};

// assignS" : {"left" : {"identifier": "y"}, "right":{"floatconst":12.56}}
class assignS_astnode : public statement_astnode {
public:
    exp_astnode* lvalue_ = nullptr;
    exp_astnode* rvalue_ = nullptr;
    assignS_astnode(exp_astnode* lvalue, exp_astnode* rvalue) : lvalue_(lvalue), rvalue_(rvalue), statement_astnode(true,ASSIGNS,STATEMENT) {};
    assignS_astnode() : statement_astnode(ASSIGNS,STATEMENT) {};

    void print(int blanks){
        std::cout << "{ \"assignS\": {\n\"left\":\n";
        lvalue_->print(0);
        std::cout << ",\n\"right\":\n";
        rvalue_->print(0);
        std::cout << "}\n}\n";
    };

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        int s = lvalue_->type_->getSize();
        rvalue_->gencode(sMap,fentry,z,fmap);
        lvalue_->gencode(sMap,fentry,z,fmap);
        std::cout << "  popl %eax\n";
        if(rvalue_->isAddress){
            std::cout << "  popl %ebx\n";
            int i = s/4;
            while(i--){
                s -= 4;
                std::cout << "  movl " << s << "(%ebx), %ecx\n";
                std::cout << "  movl %ecx, " << s << "(%eax)\n";                
            }
        }else{
            int i = s/4;
            while(i--){
                s -= 4;
                std::cout << "  movl " << s << "(%esp), %ecx\n";
                std::cout << "  movl %ecx, " << s << "(%eax)\n";                
            }
            std::cout << "  addl $" << lvalue_->type_->getSize() << ", %esp\n";
        }
        return z;
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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {

        return_exp_->gencode(sMap,fentry,z,fmap);
        int s = fentry->data_or_return_type_->getSize();
        if(fentry->name_ == "main") {
            std::cout << "  movl (%esp), %eax\n";
            std::cout << "  leave\n ret\n";
            return z;
        }
        if(return_exp_->isAddress) {
            std::cout << "  popl %eax\n";
            int i = s/4;
            while(i--){
                s -= 4;
                std::cout << "  movl " << s << "(%eax), %ebx\n";
                std::cout << "  movl %ebx, " << s << "(%esi)\n";
            }            
        }else{
            int i = s/4;
            while(i--){
                s -= 4;
                std::cout << "  movl " << s << "(%esp), %ebx\n";
                std::cout << "  movl %ebx, " << s << "(%esi)\n";
            }
            i = s/4;
            std::cout << "\taddl $" << fentry->data_or_return_type_->getSize() << ", %esp\n";
        }
        std::cout << "	popl %esi\n";
        std::cout << "  leave\n ret\n";
        return z;
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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        int s = fmap[fname_->id_]->data_or_return_type_->getSize();
        std::cout << "  subl $" << s << ", %esp\n";
        int totalsize = 4;
        for(auto it : proccall_) {
            it->gencode(sMap,fentry,z,fmap);
            if(it->isAddress){
                if(it->type_->node_type_==POINTER){
                    std::cout << "	popl %eax\n";
                    std::cout << "	movl (%eax), %eax\n";
                    std::cout << "	pushl %eax\n";
                }else if(it->type_->node_type_==BASE_TYPE){
                    std::cout << "	popl %eax\n";
                    int s = it->type_->getSize();
                    totalsize += 4;
                    std::cout << "	subl $" << s <<",%esp\n";
                    int i = s/4;
                    int src = s;
                    while(i--){
                        src-=4;
                        std::cout << "	pushl "<< src <<"(%eax)\n";
                    }
                }

            }
            if(it->type_->node_type_==BASE_TYPE){
                totalsize += it->type_->getSize();
            }else{
                totalsize += 4;
            }
        }

        std::cout << "  subl $4, %esp\n";
        std::cout << "  call " << fname_->id_ << "\n";
        std::cout << "  addl $" << totalsize << ", %esp\n";
        std::cout << "  addl $" << s << ", %esp\n";  
        return z;
    };


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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        int z1 = z++;   // False part
        int z2 = z++;   // After if statement

        conditional_->gencode(sMap,fentry,z,fmap);
        std::cout << "  popl %eax\n";
        if(conditional_->isAddress) {
            std::cout << "  movl (%eax), %eax\n";
        }

        std::cout << "  cmpl $0, %eax\n";
        std::cout << "  je .L" << z1 << "\n";
        z = if_part_->gencode(sMap,fentry,z,fmap);
        std::cout << "  jmp .L" << z2 << "\n";
        std::cout << ".L" << z1 << ":\n";
        z = else_part_->gencode(sMap,fentry,z,fmap);
        std::cout << ".L" << z2 << ":\n";
        return z;
    };
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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        int z1 = z++;   // Before condition
        int z2 = z++;   // False part


        std::cout << ".L" << z1 << ":\n";
        conditional_->gencode(sMap,fentry,z,fmap);
        if(conditional_->isAddress) {
            std::cout << "  popl %eax\n";
            std::cout << "  movl (%eax), %eax\n";
        }
        std::cout << "  cmpl $0, %eax\n";
        std::cout << "  je .L" << z2 << "\n";
        z = loop_part_->gencode(sMap,fentry,z,fmap);
        std::cout << "  jmp .L" << z1 << "\n";
        std::cout << ".L" << z2 << ":\n";
        return z;
    };
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

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        int z1 = z++;   // Before condition
        int z2 = z++;   // False part

        initializer_->gencode(sMap,fentry,z,fmap);
        std::cout << ".L" << z1 << ":\n";
        conditional_->gencode(sMap,fentry,z,fmap);
        if(conditional_->isAddress) {
            std::cout << "  popl %eax\n";
            std::cout << "  movl (%eax), %eax\n";
        }
        std::cout << "  cmpl $0, %eax\n";
        std::cout << "  je .L" << z2 << "\n";
        z = loop_part_->gencode(sMap,fentry,z,fmap);
        iterative_->gencode(sMap,fentry,z,fmap);
        std::cout << "  jmp .L" << z1 << "\n";
        std::cout << ".L" << z2 << ":\n";
        return z;
    };

};

class printf_astnode : public statement_astnode {
public:
    stringconstant_astnode* stringast;
    std::vector<exp_astnode*> expressions;
    printf_astnode(stringconstant_astnode* sast, std::vector<exp_astnode*>  exps) : stringast(sast), expressions(exps),statement_astnode(true,PRINTF,STATEMENT) {};
    printf_astnode() :statement_astnode(PRINTF,STATEMENT) {};

    int gencode(std::map<std::string,entry*> &sMap, funEntry* fentry, int z, std::map<std::string, funEntry*> fmap) {
        int totalsize = 4;
        for(int i = (expressions.size()-1); i >=0 ; i--) {
            exp_astnode* it = expressions[i];
            it->gencode(sMap,fentry,z,fmap);
            if(it->isAddress){
                std::cout << "	popl %eax\n";
                std::cout << "	movl (%eax), %eax\n";
                std::cout << "	pushl %eax\n";
                
            }
            totalsize += 4;
        }

        std::cout << "	pushl $" << stringast->the_string_ << "\n";
        std::cout << "	call printf\n";
        std::cout << "	addl $" << totalsize << ", %esp\n";  
        return z;
    };
};
