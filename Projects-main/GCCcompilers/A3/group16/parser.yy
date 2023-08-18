%skeleton "lalr1.cc"
%require  "3.0.1"

%defines 
%define api.namespace {IPL}
%define api.location.type {IPL::location}
%define api.parser.class {Parser}

%define parse.trace
%locations
%code requires {
   #include "location.hh"
   #include "ast.hh"
   namespace IPL {
      class Scanner;
   }

  // # ifndef YY_NULLPTR
  // #  if defined __cplusplus && 201103L <= __cplusplus
  // #   define YY_NULLPTR nullptr
  // #  else
  // #   define YY_NULLPTR 0
  // #  endif
  // # endif

}

%printer { std::cerr << $$; } STRUCT
%printer { std::cerr << $$; } IDENTIFIER
%printer { std::cerr << $$; } VOID
%printer { std::cerr << $$; } INT
%printer { std::cerr << $$; } FLOAT
%printer { std::cerr << $$; } INT_CONSTANT
%printer { std::cerr << $$; } FLOAT_CONSTANT
%printer { std::cerr << $$; } STRING_LITERAL
%printer { std::cerr << $$; } RETURN
%printer { std::cerr << $$; } OR_OP
%printer { std::cerr << $$; } AND_OP
%printer { std::cerr << $$; } EQ_OP
%printer { std::cerr << $$; } NE_OP
%printer { std::cerr << $$; } LE_OP
%printer { std::cerr << $$; } GE_OP
%printer { std::cerr << $$; } PTR_OP
%printer { std::cerr << $$; } INC_OP
%printer { std::cerr << $$; } IF
%printer { std::cerr << $$; } ELSE
%printer { std::cerr << $$; } WHILE
%printer { std::cerr << $$; } FOR
%printer { std::cerr << $$; } PRINTF
%printer { std::cerr << $$; } OTHERS





%parse-param { Scanner  &scanner  }
%locations
%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   #include <string>
   
   
   #include "scanner.hh"
   int nodeCount = 0;

   SymbTab *globalST = new SymbTab();

   std::map < std::string , funEntry*> funcMap;
   std::map < std::string , entry*> structMap;



   dealer dealers;
   
   
   
   std::map < std:: string, abstract_astnode*> ast;

   std::map<std::string, entry*> common_map;

   std::vector<std::pair< IPL::Parser::location_type,return_astnode*> > return_exps;
   std::map<std::string, std::pair< IPL::Parser::location_type,Type*> > struct_exps;

   funEntry* main_entry = nullptr;
   abstract_astnode* main_ast = nullptr;
   std::map< std::string, std::string > strings;


   // Expression list is maintained inside of structMap and funcmap, and respective SymbTab

#undef yylex
#define yylex IPL::Parser::scanner.yylex


}




%define api.value.type variant
%define parse.assert

%start program


%token <std::string> STRUCT
%token <std::string> IDENTIFIER
%token <std::string> VOID
%token <std::string> INT
%token <std::string> FLOAT
%token <std::string> INT_CONSTANT
%token <std::string> FLOAT_CONSTANT
%token <std::string> STRING_LITERAL
%token <std::string> RETURN
%token <std::string> OR_OP
%token <std::string> AND_OP
%token <std::string> EQ_OP
%token <std::string> NE_OP
%token <std::string> LE_OP
%token <std::string> GE_OP
%token <std::string> PTR_OP
%token <std::string> INC_OP
%token <std::string> IF
%token <std::string> ELSE
%token <std::string> WHILE
%token <std::string> FOR
%token <std::string> MAIN
%token <std::string> OTHERS
%token <std::string> PRINTF
%token '{' '}' ';' '(' ')' ',' '[' ']' '=' '<' '>' '+' '-' '*' '/' '.' '!' '&' 

%nterm <int> program 
%nterm <int> translation_unit 
%nterm <statement_astnode*> iteration_statement 
%nterm <entry*> struct_specifier
%nterm <funEntry*> function_definition
%nterm <funEntry*> main_definition
%nterm <Type*> type_specifier
%nterm <funEntry*> fun_declarator
%nterm <funEntry*> parameter_list
%nterm <entry*> parameter_declaration
%nterm <Type*> declarator_arr
%nterm <std::pair<Type*,int> > declarator 
%nterm <std::pair<seq_astnode*,entry*> > compound_statement    // first argument is statement_list, second is declaration_list
%nterm <seq_astnode*> statement_list
%nterm <statement_astnode*> statement
%nterm <assignE_exp*> assignment_expression
%nterm <assignS_astnode*> assignment_statement
%nterm <proccall_astnode*> procedure_call
%nterm <exp_astnode*> expression
%nterm <exp_astnode*> logical_and_expression
%nterm <exp_astnode*> equality_expression
%nterm <exp_astnode*> relational_expression 
%nterm <exp_astnode*> additive_expression
%nterm <exp_astnode*> unary_expression
%nterm <exp_astnode*> multiplicative_expression
%nterm <exp_astnode*> postfix_expression
%nterm <exp_astnode*> primary_expression
%nterm <myVector<exp_astnode*>* > expression_list
%nterm <char> unary_operator
%nterm <if_astnode*> selection_statement
%nterm <entry*> declaration_list
%nterm < myVector<entry*>* > declaration
%nterm < myVector <std::pair<Type*,int> >* > declarator_list
%nterm <statement_astnode*> printf_call


%%

// Things to keep in mind: 

// Make sure that no struct declaration is int or float or string
// 
// Make sure that the size is updated when declaration_list is implemented
//
//
//
//
//
//

program
  : main_definition {
     $$ = ++nodeCount;
  }
  | translation_unit main_definition {
     $$ = $1;
  }
  ;

translation_unit: 
   struct_specifier{
      $$ = ++nodeCount;
   }
   | function_definition{
      $$ = ++nodeCount;
   }
   | translation_unit struct_specifier{
      $$ = ++nodeCount;
   }
   | translation_unit function_definition{
      $$ = ++nodeCount;
   }
   ;

struct_specifier: 
   STRUCT IDENTIFIER '{' declaration_list '}' ';' {
      

      $$ = $4;
      entry* decla = $4;
       
      decla->name_ = "struct " + $2;
      decla->type_ = "struct";
      decla->scope_ = "global";
      decla->offset_ = 0;
      decla->isFun_ = false;
      decla->isstruct_ = true;
      globalST->entries.push_back(decla);
      structMap["struct " + $2] = decla;
      common_map.clear();
      struct_exps.clear();
   }
   ;

function_definition: 
   type_specifier fun_declarator {
      $2->data_or_return_type_ = $1;
   } compound_statement {
      
      $$ = $2;
      $$->type_ = "fun";
      $$->scope_ = "global";
      $$->size_ = 0;
      $$->offset_ = 0;
      $$->data_or_return_type_ = $1;

      if($4.second !=nullptr){
         for(auto u : $4.second->localST_){
            u->offset_ = -(u->offset_ + u->size_);
            $$->localST_.push_back(u);
            $$->var_map_[u->name_] = u;
         }
         
        
         
      }
      if($4.first!=nullptr){
         ast[$$->name_] = $4.first;
      }else{
         ast[$$->name_] = new seq_astnode();
      }

      globalST->entries.push_back($$);
      return_exps.clear();
      common_map.clear();
   }
   ;


main_definition
  : INT MAIN '(' ')' compound_statement {

      $$ = new funEntry();
      $$->name_ = "main";
      $$->type_ = "fun";
      $$->size_ = 0;
      $$->scope_ = "global";
      $$->isFun_ = true;
      funcMap["main"] = $$;
      $$->offset_ = 0;
      $$->data_or_return_type_ = dealers.intType;
      main_entry = $$;
      if($5.second !=nullptr){
         for(auto u : $5.second->localST_){
            u->offset_ = -(u->offset_ + u->size_);
            $$->localST_.push_back(u);
            $$->var_map_[u->name_] = u;
         }
         
         
      }
      if($5.first!=nullptr){
         ast[$$->name_] = $5.first;
      }else{
         ast[$$->name_] = new seq_astnode();
      }
      main_ast = $5.first;

      globalST->entries.push_back($$);
      return_exps.clear();
      common_map.clear();
  }
  ;



type_specifier:
   VOID {
      $$ = dealers.voidType;
   }
   | INT{
      $$ = dealers.intType;
   }
   | FLOAT{
      $$ = dealers.floatType;
   }
   | STRUCT IDENTIFIER{
      if(structMap.find("struct " + $2)!=structMap.end()){
         $$ = new BaseType("struct " + $2, structMap["struct " + $2]->size_);
      }else{
         $$ = new BaseType("struct " + $2, 0);
         struct_exps["struct " + $2] = std::make_pair(@$,$$);
         //error(@$, "No struct with such name : \"" + $2 +"\"");
      }
   }
   ;

fun_declarator:
   IDENTIFIER '(' parameter_list ')' {
      
      $$ = $3;
      int totsize = 12;
      $$->name_ = $1;
      $$->type_ = "fun";
      $$->size_ = 0;
      $$->scope_ = "global";
      $$->isFun_ = true;
      for(int i = ($$->localST_.size()-1) ;i >= 0; i--){
         $$->localST_[i]->offset_ = totsize;
         totsize += $$->localST_[i]->size_;
      }
      funcMap[$1] = $$;
      
   }
   | IDENTIFIER '(' ')' {
      
      $$ = new funEntry();
      $$->name_ = $1;
      $$->type_ = "fun";
      $$->size_ = 0;
      $$->scope_ = "global";
      $$->isFun_ = true;
      funcMap[$1] = $$;
   
      
   }
   ;
parameter_list:
   parameter_declaration {
      $$ = new funEntry();
      $$->type_ = "fun";
      $$->size_ = 0;
      $$->scope_ = "global";
      $$->isFun_ = true;
      $$->localST_.push_back($1);
      $$->var_map_[$1->name_] = $1;
      common_map[$1->name_] = $1;
      $$->parameter_types_.push_back($1->data_or_return_type_);
   }
   | parameter_list ',' parameter_declaration {
      $$ = $1;
      $$->localST_.push_back($3);
      $$->var_map_[$3->name_] = $3;
      common_map[$3->name_] = $3;
      $$->parameter_types_.push_back($3->data_or_return_type_);
   }
   ;

parameter_declaration: 
   type_specifier declarator {
      Type* root, *root1;
      std::string s1;
      root = $2.first;
      if(root->node_type_==BASE_TYPE){
         s1 = ((BaseType*)root)->type_name_;
         //delete root;
         
         root1 = $1;
         for(int k = 0;k < $2.second;k++){
            root1 = new PointerTo(root1);
         }
         $$ = new entry(s1,"var","param",root1->getSize(),0,root1,false);
      }else{
         while(((ChildType*)root)->child_type_->node_type_!=BASE_TYPE){
            root = ((ChildType*)root)->child_type_;
         }
         s1 = ((BaseType*)(((ChildType*)root)->child_type_))->type_name_;
         root1 = $1;
         for(int k = 0;k < $2.second;k++){
            root1 = new PointerTo(root1);
         }
         //delete ((ChildType*)root)->child_type_;
         ((ChildType*)root)->child_type_ = root1;
         $$ = new entry(s1,"var","param",$2.first->getSize(),0,$2.first,false);
      }
   }
   ;

declarator_arr:
   IDENTIFIER {
      $$ = new BaseType($1);
   }
   | declarator_arr '[' INT_CONSTANT ']' {
      if($1->node_type_==BASE_TYPE){
         $$ = new ArrayOf($1, std::stoi($3));
      }else{
         $$ = $1;
         ChildType* root = ((ChildType*)$1);
         while(root->child_type_->node_type_!=BASE_TYPE){
            root = ((ChildType*)root->child_type_);
         }
         root->child_type_ = new ArrayOf(root->child_type_,std::stoi($3));
      }
   }
   ;

declarator:
   declarator_arr {
      $$ = std::make_pair($1,0);
   }
   | '*' declarator{
      $$ = std::make_pair($2.first, $2.second+1);
   }
   ;
compound_statement:
   '{' '}' {
      $$ = std::make_pair( (seq_astnode*)nullptr, (entry*)nullptr);
   }
   | '{' statement_list '}' {
      $$ = std::make_pair($2,nullptr);
   }
   | '{' declaration_list '}' {
      $$ = std::make_pair( (seq_astnode*)nullptr,$2);
   }
   | '{' declaration_list statement_list '}' {
      $$ = std::make_pair($3,$2);
   }
   ;
statement_list: 
   statement {
      $$ = new seq_astnode({$1});
   }
   | statement_list statement {
      $$ = $1;
      $1->addStatement($2);
   }
   ;

statement:
   ';'{
      $$ = new empty_astnode();
   }
   | '{' statement_list '}' {
      $$ = $2;
   }
   | selection_statement {
      $$ = $1;
   }
   | iteration_statement {
      $$ = $1;
   }
   | assignment_statement {
      $$ = $1;
   }
   | procedure_call {
      $$ = $1;
   }
   | printf_call {
      $$ = $1;
   }
   | RETURN expression ';' {
      $$ = new return_astnode($2);
      return_exps.push_back(std::make_pair(@$,((return_astnode*)$$)));
   }
   ;

assignment_expression:
   unary_expression '=' expression {
      
      $$ = new assignE_exp($1, $3);   
      $$->type_ = dealers.intType;
   }
   ;

assignment_statement:
   assignment_expression ';' {
     $$ = new assignS_astnode($1->lhs_,$1->rhs_);
   }
   ;

procedure_call:
   IDENTIFIER '(' ')' ';' {
      funEntry* e = funcMap[$1];
      $$ = new proccall_astnode(new identifier_astnode($1),{});
      
   }
   | IDENTIFIER '(' expression_list ')' ';' {
      funEntry* e = funcMap[$1];
       $$ = new proccall_astnode(new identifier_astnode($1), $3->entries);
   }
   ;

expression:
   logical_and_expression {
      $$ = $1;
   }
   | expression OR_OP logical_and_expression {
     $$ = new op_binary_astnode("OR_OP",$1,$3);
      $$->type_ = dealers.intType;
   }
   ;

logical_and_expression:
   equality_expression{
      $$ = $1;
   }
   | logical_and_expression AND_OP equality_expression{
 
      $$ = new op_binary_astnode("AND_OP",$1,$3);
      $$->type_ = dealers.intType;
      

   }
   ;

equality_expression:
   relational_expression {
      $$ = $1;
   }
   | equality_expression EQ_OP relational_expression {
      
      $$ = new op_binary_astnode("EQ_OP_INT",$1,$3);
      $$->type_ = dealers.intType;
   }
   | equality_expression NE_OP relational_expression {
      $$ = new op_binary_astnode("NE_OP_INT",$1,$3);
      $$->type_ = dealers.intType;
   }

relational_expression:
   additive_expression {
      $$ = $1;
   }
   | relational_expression '<' additive_expression {
      $$ = new op_binary_astnode("LT_OP_INT",$1,$3);
      $$->type_ = dealers.intType;
   }
   | relational_expression '>' additive_expression{
      $$ = new op_binary_astnode("GT_OP_INT",$1,$3);
      $$->type_ = dealers.intType;
   }
   | relational_expression LE_OP additive_expression {
      $$ = new op_binary_astnode("LE_OP_INT",$1,$3);
      $$->type_ = dealers.intType;
   }
   | relational_expression GE_OP additive_expression {
      $$ = new op_binary_astnode("GE_OP_INT",$1,$3);
      $$->type_ = dealers.intType;
   }
   ;

additive_expression:
   multiplicative_expression {
      $$ = $1;
   }
   | additive_expression '+' multiplicative_expression {
      $$ = new op_binary_astnode("PLUS_INT",$1,$3);
      $$->type_ = dealers.intType;
      if($1->type_->node_type_!=BASE_TYPE && $3->type_->node_type_!=BASE_TYPE){
         $$->type_ = dealers.intType;
      }else if($1->type_->node_type_==BASE_TYPE && $3->type_->node_type_!=BASE_TYPE){
         $$->type_ = $3->type_;
      }else if($1->type_->node_type_!=BASE_TYPE && $3->type_->node_type_==BASE_TYPE){
         $$->type_ = $1->type_;
      }
   }
   | additive_expression '-' multiplicative_expression {
      $$ = new op_binary_astnode("MINUS_INT",$1,$3);
      $$->type_ = dealers.intType;
      if($1->type_->node_type_!=BASE_TYPE && $3->type_->node_type_!=BASE_TYPE){
         $$->type_ = dealers.intType;
      }else if($1->type_->node_type_==BASE_TYPE && $3->type_->node_type_!=BASE_TYPE){
         $$->type_ = $3->type_;
      }else if($1->type_->node_type_!=BASE_TYPE && $3->type_->node_type_==BASE_TYPE){
         $$->type_ = $1->type_;
      }
   }
   ;

unary_expression:
   postfix_expression {
      $$ = $1;
   }
   | unary_operator unary_expression {
      if($1=='-') {
         $$ = new op_unary_astnode("UMINUS", $2);
         $$->type_ = $2->type_;
         
      }else if($1=='!'){
         $$ = new op_unary_astnode("NOT", $2);
         $$->type_ = dealers.intType;
         
      }else if($1=='&'){
         $$ = new op_unary_astnode("ADDRESS",$2);
         $$->type_ = new PointerTo($2->type_);
         
      }else if($1=='*') {
         
            $$ = new op_unary_astnode("DEREF",$2);
            $$->type_ = ( (ChildType*)$2->type_)->child_type_;
         
      }
   }
   ;

multiplicative_expression:
   unary_expression {
      $$ = $1;
   }
   | multiplicative_expression '*' unary_expression {
      $$ = new op_binary_astnode("MULT_INT",$1,$3);
      $$->type_ = dealers.intType;
            
   }
   | multiplicative_expression '/' unary_expression {
      $$ = new op_binary_astnode("DIV_INT",$1,$3);
      $$->type_ = dealers.intType;
   }
   ;

postfix_expression:
   primary_expression {
      $$ = $1;
   }
   | postfix_expression '[' expression ']' {
      $$ = new arrayref_astnode($1,$3);
            $$->type_ = ((ChildType*)$1->type_)->child_type_;
   }
   | IDENTIFIER '(' ')' {
      funEntry* e = funcMap[$1];
      $$ = new funcall_astnode(new identifier_astnode($1),{});
      $$->type_ = e->data_or_return_type_;
   }
   | IDENTIFIER '(' expression_list ')' {
      funEntry* e = funcMap[$1];
      $$ = new funcall_astnode(new identifier_astnode($1), $3->entries);
      $$->type_ =  e->data_or_return_type_;
   }
   | postfix_expression '.' IDENTIFIER {
      entry* e = structMap[((BaseType*)$1->type_)->type_name_];
      identifier_astnode* idast = new identifier_astnode($3);
      idast->type_ = e->var_map_[$3]->data_or_return_type_;
      $$ = new member_astnode($1,idast);
      $$->type_ = e->var_map_[$3]->data_or_return_type_;
            
   }
   | postfix_expression PTR_OP IDENTIFIER {
       Type* ch = ((ChildType*)$1->type_)->child_type_;
      entry* e = structMap[((BaseType*)ch)->type_name_];
            
      identifier_astnode* idast = new identifier_astnode($3);
      idast->type_ = e->var_map_[$3]->data_or_return_type_;
      $$ = new arrow_astnode($1,idast);
      $$->type_ = e->var_map_[$3]->data_or_return_type_;
            
         
   }
   | postfix_expression INC_OP {
      $$ = new op_unary_astnode("PP",$1);
      $$->type_ = $1->type_;
   }
   ;

primary_expression:
   IDENTIFIER {
      $$ = new identifier_astnode($1);
      $$->type_ = common_map[$1]->data_or_return_type_;
   }
   | INT_CONSTANT {
      $$ = new intconstant_astnode($1);
      $$->type_ = dealers.intType;
   }
   | FLOAT_CONSTANT{
      $$ = new floatconstant_astnode($1);
      $$->type_ = dealers.floatType;
   }
   | '(' expression ')' {
      $$ = $2;
   }
   ;

expression_list:
   expression {
      $$ = new myVector<exp_astnode*>();
      $$->entries.push_back($1);
   }
   | expression_list ',' expression{
      $$ = $1;
      $$->entries.push_back($3);
   }
   ;

unary_operator:
   '-' {
      $$ = '-';
   }
   | '!' {
      $$ = '!';
   }
   | '&' {
      $$ = '&';
   }
   | '*' {
      $$ = '*';
   }
   ;

selection_statement:
   IF '(' expression ')' statement ELSE statement {
      $$ = new if_astnode($3,$5,$7);
   }
   ;

iteration_statement: 
   WHILE '(' expression ')' statement {
      $$ = new while_astnode($3,$5);
   }
   | FOR '(' assignment_expression ';' expression ';' assignment_expression ')' statement {
      $$ = new for_astnode($3,$7,$5,$9);
   }
   ;

declaration_list:
   declaration{
      $$ = new entry();
      
      $$->offset_ = 0;
      $$->localST_ = $1->entries;
      int totsize = 0;
      for(auto u : $1->entries){
         $$->var_map_[u->name_] = u;
         common_map[u->name_] = u;
         u->offset_ = totsize;
         totsize += u->size_;
      }
      $$->size_ = totsize;
   }
   | declaration_list declaration{
      $$ = $1;
      int totsize = $$->size_;
      for(auto u : $2->entries){
         $$->var_map_[u->name_] = u;
         common_map[u->name_] = u;
         u->offset_ = totsize;
         totsize += u->size_;
         $$->localST_.push_back(u);
      }
      $$->size_ = totsize; 
   }
   ;

declaration:
   type_specifier declarator_list ';' {



      $$ = new myVector<entry*>();
      Type* root, *root1;
      int x;
      std::string s1;
      for(auto u: $2->entries){
         
         root = u.first;
         if(root->node_type_==BASE_TYPE){
            s1 = ((BaseType*)root)->type_name_;
            //delete root;
            
            root1 = $1;
            for(int k = 0;k < u.second;k++){
               root1 = new PointerTo(root1);
            }
            
            $$->entries.push_back(new entry(s1,"var","local",root1->getSize(),0,root1,false));
         }else{
            while(((ChildType*)root)->child_type_->node_type_!=BASE_TYPE){
               root = ((ChildType*)root)->child_type_;
            }
            s1 = ((BaseType*)(((ChildType*)root)->child_type_))->type_name_;
            root1 = $1;
            for(int k = 0;k < u.second;k++){
               root1 = new PointerTo(root1);
            }
            
            //delete ((ChildType*)root)->child_type_;
            ((ChildType*)root)->child_type_ = root1;
            $$->entries.push_back(new entry(s1,"var","local",u.first->getSize(),0,u.first,false));
         }
         
      }
   }
   ;

declarator_list:
   declarator {
      myVector<std::pair<Type*,int> >* myvec = new myVector<std::pair<Type*,int> >();
      myvec->entries.push_back($1);
      $$ = myvec;
   }
   | declarator_list ',' declarator{
      $1->entries.push_back($3);
      $$ = $1;
   }
   ;

printf_call:
   PRINTF '(' STRING_LITERAL ')' ';' {
      std::string na = ".LC" + std::to_string(strings.size());
      strings[na] = $3;
      $$ = new printf_astnode(new stringconstant_astnode(na),{});
   }
   | PRINTF '(' STRING_LITERAL ',' expression_list ')' ';' {
      std::string na = ".LC" + std::to_string(strings.size());
      strings[na] = $3;
      $$ = new printf_astnode(new stringconstant_astnode(na),$5->entries);
   }
   ;




%%
void IPL::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cout << "Error at line " << l.begin.line <<": " << err_message << "\n";
   exit(1);
}


