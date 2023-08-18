
#include "scanner.hh"
#include <fstream>
using namespace std;


string filename;
extern SymbTab *globalST; 
extern map<string,abstract_astnode*> ast;
extern map < string , funEntry*> funcMap;
extern map < string , entry*> structMap;


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
// create gstfun with function entries only


map<std::string, entry*> newgst;
for(auto u: globalST->entries){
	newgst[u->name_] = u;
}

cout << "{\"globalST\": " << "\n";
std::cout <<"[";
std::map<std::string, entry*> entr;
for(auto u: globalST->entries){
	entr[u->name_] = u;
}
for(auto it = entr.begin(); it != entr.end(); ++it){
	
	if (next(it,1) != entr.end()) 
	it->second->printentry1();
	else it->second->printentry();

}
std::cout <<"]\n";
cout << "," << endl;

cout << "  \"structs\": [" << endl;
for (auto it = structMap.begin(); it != structMap.end(); ++it)

{   cout << "{" << endl;
	cout << "\"name\": " << "\"" << it->first << "\"," << endl;
	cout << "\"localST\": " << endl;
	std::cout <<"[\n";
    std::map<std::string, entry*> entr;
    for(auto u: it->second->localST_){
        entr[u->name_] = u;
    }
    for(auto it1 = entr.begin(); it1 != entr.end(); ++it1){
        it1->second->printentry();
        if (next(it1,1) != entr.end()) 
        std::cout << ",\n";
        
    }
    std::cout <<"]\n";
	cout << "}" << endl;
	if (next(it,1) != structMap.end()) 
	cout << "," << endl;
}
cout << "]," << endl;
cout << "  \"functions\": [" << endl;

for (auto it = funcMap.begin(); it != funcMap.end(); ++it)

{
	cout << "{" << endl;
	cout << "\"name\": " << "\"" << it->first << "\"," << endl;
	cout << "\"localST\": " << endl;
	std::cout <<"[\n";
    std::map<std::string, entry*> entr;
    for(auto u: it->second->localST_){
        entr[u->name_] = u;
    }
    for(auto it1 = entr.begin(); it1 != entr.end(); ++it1){
        it1->second->printentry();
        if (next(it1,1) != entr.end()) 
        std::cout << ",\n";
        
    }
    std::cout <<"]\n";
	cout << "," << endl;
	cout << "\"ast\": " << endl;
	ast[it->first]->print(0);
	cout << "}" << endl;
	if (next(it,1) != funcMap.end()) cout << "," << endl;
	
}
	cout << "]" << endl;
	cout << "}" << endl;

	fclose(stdout);
}