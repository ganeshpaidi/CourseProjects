
#include "scanner.hh"
#include <fstream>
using namespace std;


string filename;
extern SymbTab *globalST; 
extern map<string,abstract_astnode*> ast;
extern map < string , funEntry*> funcMap;
extern map < string , entry*> structMap;
extern funEntry* main_entry;
extern abstract_astnode* main_ast;
extern map<std::string, std::string > strings;


int main(int argc, char **argv)
{
	using namespace std;
	fstream in_file, out_file;
	

	in_file.open(argv[1], ios::in);

	IPL::Scanner scanner(in_file);

	IPL::Parser parser(scanner);

#ifdef YYDEBUG
	parser.set_debug_level(1);
#endif
parser.parse();

	// std::cout <<"Reached here!\n";
	for(auto it : funcMap){
		std::vector<entry*> stt= it.second->localST_;
		int totalsize = 12;
		int totalLocalSize = 0;
		for(int i = (stt.size()-1); i >= 0;i--){
			if(stt[i]->scope_ != "param") continue;
			stt[i]->offset_ = totalsize;
			if(stt[i]->data_or_return_type_->node_type_==ARRAY) {
				totalsize += 4;
			}else{
				totalsize += stt[i]->data_or_return_type_->getSize();
			}
		}
		for(int i = 0; i < stt.size();i++){
			if(stt[i]->scope_ == "local") {
				totalLocalSize += stt[i]->data_or_return_type_->getSize();
			}
		}
		it.second->return_offset = totalsize;
		it.second->totalLocalSize = totalLocalSize;
	}
	// std::cout <<"Reached here!\n";

	std::cout << "	.file	\"" << argv[1] << "\"\n";
	std::cout << "	.text\n";
	if(!strings.empty()){
		std::cout << "	.section	.rodata\n";
		for(auto it : strings) {
			if(it.second.size() > 31){
				std::cout << "	.align 4\n";
			}
			std::cout << it.first <<":\n";
			std::cout << "	.string " << it.second <<"\n";
		}

	}

	if(main_entry!=nullptr || !funcMap.empty() ){
		std::cout << "	.text\n";
	}
	int z = 0;
	for(auto it: funcMap){
		if(it.first == "main")continue;
		std::cout << "	.globl	" << it.first << "\n";
		std::cout << "	.type	" << it.first << ", @function\n" << it.first << ":\n";
		std::cout << "	pushl %ebp\n";
		std::cout << "	movl %esp, %ebp\n";
		std::cout << "	subl $" << it.second->totalLocalSize << ", %esp\n";
		std::cout << "	pushl %esi\n";
		std::cout << "	leal " << it.second->return_offset << "(%ebp), %esi\n";
		z = ast[it.first]->gencode(structMap, it.second, z, funcMap);
		std::cout << "	popl %esi\n";
		std::cout << "	leave\n	ret\n";
		std::cout << "	.size	" << it.first << ", .-" << it.first << "\n";
	}

	std::cout << "	.globl	main\n";
	std::cout << "	.type	main, @function\nmain:\n";
	std::cout << "	pushl %ebp\n";
	std::cout << "	movl %esp, %ebp\n";
	std::cout << "	subl $" << main_entry->totalLocalSize << ", %esp\n";
	z = main_ast->gencode(structMap, main_entry, z, funcMap);
	std::cout << "	leave\n	ret\n";
	std::cout << "	.size	main, .-main\n";

	std::cout << "	.ident	\"GCC: (Ubuntu 8.1.0-9ubuntu1~16.04.york1) 8.1.0\"\n";
	std::cout << "	.section	.note.GNU-stack,\"\",@progbits\n";
		
	fclose(stdout);
}