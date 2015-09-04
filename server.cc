//Carlos Andres Martinez - Cliente Servidor
#include <iostream>  // std::cout
#include <fstream>  //std::ifstream
#include <string>
#include <cassert>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

//////////////////////////////// find to name song 
string findtoname(int a){

	map<int,string> map_name;
	fstream inputFile;
	int indx=0;
	string nombre;
	string name_song;
	inputFile.open ("namesong.txt",ios::in);
			
		if (inputFile.is_open()) {		
			while (! inputFile.eof() ) { //while not (end of file) 
								
				getline(inputFile , nombre);
				map_name[indx]=nombre;					
				cout<< indx << " | " << nombre << endl;			
				indx++;
			}
			inputFile.close();
		}else cout << "Fichero inexistente" << endl;
	std::map<int, string>::iterator it=map_name.find(a); // buscar
	name_song = (*it).second;
	return name_song;
}	

//////////////////////////////// find to path
string findtofile(int b){

	map<int,string> map_path;
	fstream inputFile;
	int indx=0;
	string nombre;
	string path;
	inputFile.open ("path.txt",ios::in);
			
		if (inputFile.is_open()) {		
			while (! inputFile.eof() ) { //while not (end of file) 
								
				getline(inputFile , nombre);
				map_path[indx]=nombre;					
				cout<< indx << " | " << nombre << endl;			
				indx++;
			}
			inputFile.close();
		}else cout << "Fichero inexistente" << endl;
	std::map<int, string>::iterator it=map_path.find(b); // buscar
	path = (*it).second;
	return path;
}	

////////////////////////////transfer to file 
vector<char> ReadAllBytes(const string &filename)
{
    ifstream ifs(filename, ios::binary|ios::ate); // read filename in binary and stay end file
    ifstream::pos_type pos = ifs.tellg(); // position
    vector<char>  result(pos); // create vector with size pos

    ifs.seekg(0, ios::beg); // stay begin file 
    ifs.read(&result[0], pos); //how much to read 

    return result;
}

void filetomessage(const string &filename, message &r){
	vector <char> bytes = ReadAllBytes(filename);
	r.add_raw(bytes.data(), bytes.size());
}

int main()
{
  cout << "SERVER" << endl;
  context ctx;

  socket s(ctx, socket_type::rep);
  s.bind("tcp://*:5555");
	
	while(true){ 

		message m;
		s.receive(m);       // receive message with buscar and #

		string text;
		m >> text;          // string buscar
		int n;
		m >> n;               // integer #
		string text_server;

		if (text == "buscar"){
			text_server = findtoname(n);
			cout << text_server << endl;

			message r;				
			r << text_server;
			s.send(r);		
		}		

		message l;
		s.receive(l);
		int index_song;
		l >> index_song;

		string name_path;
		name_path = findtofile(index_song);    // path to file 
		cout << name_path << endl;

		message g;
		filetomessage(name_path,g);
		s.send(g);
  }	
	return 0;
}
