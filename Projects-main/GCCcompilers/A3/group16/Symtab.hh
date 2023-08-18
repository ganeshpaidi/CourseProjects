


#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>
#include "type.hh"


class entry {
public:
    std::string name_;
    std::string type_;       // var/fun/struct
    std::string scope_;     
    int size_;
    int offset_;
    Type* data_or_return_type_;
    std::vector<entry*> localST_;
    std::map<std::string, entry*> var_map_;
    bool isFun_ = false;
    bool isstruct_ = false;
    entry(std::string name, std::string type, std::string scope, int size, int offset, Type* data_or_return_type, bool isFun) : name_(name), type_(type), scope_(scope), size_(size), offset_(offset), data_or_return_type_(data_or_return_type), isFun_(isFun) {};
    entry() {};
    void addSTEntry(entry *e){localST_.push_back(e);};
    void printentry() {
        std::cout << "[         \"" << name_ << "\",         \"" << type_ <<"\",         \"" << scope_ << "\",";
        std::string size_string  = std::to_string(size_);
        while(size_string.size() < 8){
            size_string = " "+size_string;
        }
        std::cout << size_string << ",";
        std::string offset_string;
        if(isstruct_){
            offset_string = "\"-\"";
        }else{
            offset_string = std::to_string(offset_);
        }
        while(offset_string.size() < 8){
            offset_string = " " + offset_string;
        }
        std::cout << offset_string <<",";
        if(isstruct_){
            std::cout << "\"-\"";
        }else{
            std::cout << "\"" << data_or_return_type_->getRepresentation() << "\"";
        }
        std::cout<<"\n]\n";
    }
    void printentry1() {
        std::cout << "[         \"" << name_ << "\",         \"" << type_ <<"\",         \"" << scope_ << "\",";
        std::string size_string  = std::to_string(size_);
        while(size_string.size() < 8){
            size_string = " "+size_string;
        }
        std::cout << size_string << ",";
        std::string offset_string;
        if(isstruct_){
            offset_string = "\"-\"";
        }else{
            offset_string = std::to_string(offset_);
        }
        while(offset_string.size() < 8){
            offset_string = " " + offset_string;
        }
        std::cout << offset_string <<",";
        if(isstruct_){
            std::cout << "\"-\"";
        }else{
            std::cout << "\"" << data_or_return_type_->getRepresentation() << "\"";
        }
        std::cout<<"\n],\n";
    }
};

class funEntry: public entry {
public:
    std::vector<Type*> parameter_types_;
    int astNum;
    int return_offset;
    int totalLocalSize;
    funEntry(std::string name, Type* data_or_return_type, int ast) : entry(name, "fun", "global", 0,0,data_or_return_type,true), astNum(ast) {};
    funEntry() {};
    void addParametertType(Type* t) { parameter_types_.push_back(t);};
};


class SymbTab{
public:
    std::vector<entry*> entries;
    void printgst(){ 
    std::cout <<"[\n";
    std::map<std::string, entry*> entr;
    for(auto u: entries){
        entr[u->name_] = u;
    }
    for(auto it = entr.begin(); it != entr.end(); ++it){
        it->second->printentry();
        if (next(it,1) != entr.end()) 
        std::cout << ",\n";
        
    }
    std::cout <<"]\n";
}
;
    
    
};
