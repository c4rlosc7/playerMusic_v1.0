//Carlos Andres Martinez - Cliente Servidor
#include <iostream>  // std::cout
#include <fstream>  //std::ifstream
#include <string>
#include <cassert>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

//////////////////////////////// find to name song 
string loadToNameSong(int a){

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
string findToPath(int b){

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

    int bytes = 100 * 1024; //102400    
    int interSize = 0;    

    for (int i = 2; bytes < pos; i++)		
    {
    	vector<char>  part(bytes);
	    ifs.seekg(interSize, ios::beg); // stay begin file, beginning of the stream
	    ifs.read(part.data(), bytes); //read     	
		interSize = ifs.gcount(); //Get character count 102400 -      		
		bytes = interSize * i;	   
		cout << interSize; 
		return part;
    }        
    //return result;
}

void filetomessage(const string &filename, message &r){
	vector <char> bytes = ReadAllBytes(filename);
	r.add_raw(bytes.data(), bytes.size());
}

int main()
{
  cout << "SERVER-MUSIC-PLAYER" << endl;
  context ctx;

  socket s(ctx, socket_type::xrep);
  s.bind("tcp://*:5555");
	
	while(true){ 

		message r;
		s.receive(r);       // r [id, "buscar" o "reproducir" ,number]

		string id;          // id
		r >> id;

		string operation;  // operation (buscar)(reproducir)
		r >> operation;   

		int n;             // number to search
		r >> n;               

		string text_server;  // nombre de la cancion / name to song 
		string name_path;    // ruta / path 
		message sc;
		if (operation == "buscar"){

			text_server = loadToNameSong(n);
			cout << text_server << endl;
			
			sc << id;
			sc << text_server;
			s.send(sc);		

		}else if(operation == "reproducir"){
			name_path = findToPath(n);    // path to file 
			cout << name_path << endl;
			sc << id;
			filetomessage(name_path,sc);
			s.send(sc);
		}		
  }	
	return 0;
}
