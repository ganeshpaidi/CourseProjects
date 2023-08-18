

#include <string>
#include <utility>
#include <vector>
enum NodeType{
    POINTER,
    ARRAY,
    BASE_TYPE, 
    ERROR
};


template <typename T>
class myVector{
public:
    std::vector<T> entries;
};

class Type{

public:
    NodeType node_type_;
    std::string error_string_;
    
    Type(NodeType node_type) : node_type_(node_type){};
    Type(NodeType node_type, std::string error_string) : node_type_(node_type), error_string_(error_string){};
    virtual int getSize() {return 0;}; 
    
    virtual std::pair<std::string,std::string> getRepresentationPartial() {return {"",""};};
    std::string getRepresentation() {
        auto s1 = getRepresentationPartial();
        return s1.first+s1.second;
    };
    // std::pair<bool,Type*> ParameterTypeEqual(Type* rhs);
    
};


class ChildType : public Type{
public:
    Type* child_type_;
    ChildType(Type* child_type, NodeType node_type) : child_type_(child_type), Type(node_type) {};
    virtual int getSize() {return 0;}; 
    
};
class PointerTo : public ChildType {
    
public:
    PointerTo(Type* child_type) : ChildType(child_type,POINTER){};
    std::pair<std::string,std::string> getRepresentationPartial(){
    auto s1 = child_type_->getRepresentationPartial();
    if(child_type_->node_type_==ARRAY){
        return std::make_pair(s1.first+"(*)",s1.second);
    }else{
        return std::make_pair(s1.first+"*",s1.second);
    }


};
    virtual int getSize(){
    return 4;
};
};

class ArrayOf :  public  ChildType {
public:
    int size_;
    ArrayOf(Type* child_type, int size) :ChildType(child_type,ARRAY), size_(size) {};
    std::pair<std::string,std::string> getRepresentationPartial(){
    auto s1 = child_type_->getRepresentationPartial();
    return std::make_pair(s1.first,"["+ std::to_string(size_) +"]"+s1.second);
};
    virtual int getSize(){
    return size_ * child_type_->getSize();
};
};

class BaseType :  public Type{
public:
    std::string type_name_;
    int size_;
    virtual int getSize(){
    return size_;
};
    std::pair<std::string,std::string> getRepresentationPartial() {
    return std::make_pair(type_name_,"");
};
    BaseType(std::string type_name) : type_name_(type_name), Type(BASE_TYPE) {};
    BaseType(std::string type_name, int size) : type_name_(type_name), Type(BASE_TYPE), size_(size) {};
};


class dealer{
public:
    Type* floatType = new BaseType("float",4);
    Type* intType = new BaseType("int",4);
    Type* voidType = new BaseType("void",0);
    Type* stringType = new BaseType("string",8);
    std::map <std::string, Type*> default_funs = { {"printf" , voidType }, {"scanf",voidType}};
    Type* Error(std::string message){
        Type* error = new Type(ERROR, message);
        return error;
    }
    bool isAssignable(Type* lhs, Type* rhs){
        Type* newrhs;
        bool deallocate = false;
        bool res = false;
        bool cont = true;
        
        if(rhs->node_type_==ARRAY){
            deallocate = true;
            newrhs = new PointerTo(((ChildType*) rhs)->child_type_);
        }else{
            newrhs = rhs;
        }
        Type* root1 = lhs;
        Type* root2 = newrhs;

        while(cont) {

            if(root1->node_type_==BASE_TYPE){
                if(root2->node_type_==BASE_TYPE){
                    res =  (((BaseType*)root1)->type_name_==((BaseType*)root2)->type_name_);
                }else{
                    res = false;
                }
                cont = false;
            }else{
                if(root2->node_type_==BASE_TYPE){
                    res = false;
                    cont = false;
                }else{
                    root1 = ((ChildType*)root1)->child_type_;
                    root2 = ((ChildType*)root2)->child_type_;
                }
            }
        }

        if(deallocate){
            delete newrhs;
        }
        return res;
    }
    ;

    bool isAssignable1(Type* lhs, Type* rhs){
        Type* newrhs;
        bool deallocate = false;
        bool res = false;
        bool cont = true;
        if(lhs->node_type_==POINTER && rhs->node_type_==POINTER){
            if(((PointerTo*)lhs)->child_type_->node_type_==BASE_TYPE && ((PointerTo*)rhs)->child_type_->node_type_==BASE_TYPE){
                return true;
            }
        }
        if(rhs->node_type_==ARRAY){
            deallocate = true;
            newrhs = new PointerTo(((ChildType*) rhs)->child_type_);
        }else{
            newrhs = rhs;
        }
        Type* root1 = lhs;
        Type* root2 = newrhs;

        while(cont) {

            if(root1->node_type_==BASE_TYPE){
                if(root2->node_type_==BASE_TYPE){
                    res =  (((BaseType*)root1)->type_name_==((BaseType*)root2)->type_name_);
                }else{
                    res = false;
                }
                cont = false;
            }else{
                if(root2->node_type_==BASE_TYPE){
                    res = false;
                    cont = false;
                }else{
                    root1 = ((ChildType*)root1)->child_type_;
                    root2 = ((ChildType*)root2)->child_type_;
                }
            }
        }
        std::cout << lhs->getRepresentation() << " " << newrhs->getRepresentation() << std::endl;
        if(lhs->node_type_==POINTER && newrhs->node_type_==POINTER){
            
            if(((PointerTo*)lhs)->child_type_->node_type_==BASE_TYPE && ((PointerTo*)newrhs)->child_type_->node_type_==BASE_TYPE){
                return true;
            }
        }

        if(deallocate){
            delete newrhs;
        }
        return res;
    }
    ;

    bool isAssignable2(Type* lhs, Type* rhs){
        Type* newrhs;
        bool deallocate = false;
        bool res = false;
        bool cont = true;
        
        if(rhs->node_type_==ARRAY){
            deallocate = true;
            newrhs = new PointerTo(((ChildType*) rhs)->child_type_);
        }else{
            newrhs = rhs;
        }
        Type* root1 = lhs;
        Type* root2 = newrhs;

        while(cont) {

            if(root1->node_type_==BASE_TYPE){
                if(root2->node_type_==BASE_TYPE){
                    res =  (((BaseType*)root1)->type_name_==((BaseType*)root2)->type_name_);
                }else{
                    res = false;
                }
                cont = false;
            }else{
                if(root2->node_type_==BASE_TYPE){
                    res = false;
                    cont = false;
                }else{
                    root1 = ((ChildType*)root1)->child_type_;
                    root2 = ((ChildType*)root2)->child_type_;
                }
            }
        }
        if(lhs->node_type_==POINTER && newrhs->node_type_==POINTER){
            if(((PointerTo*)lhs)->child_type_->node_type_==BASE_TYPE && ((BaseType*)((PointerTo*)lhs)->child_type_)->type_name_=="void"){
                return true;
            }
        }
        if(rhs->node_type_==POINTER && lhs->node_type_==POINTER){
            if(((PointerTo*)rhs)->child_type_->node_type_==BASE_TYPE && ((BaseType*)((PointerTo*)rhs)->child_type_)->type_name_=="void"){
                return true;
            }
        }
        if(deallocate){
            delete newrhs;
        }
        return res;
    }
    ;
};

