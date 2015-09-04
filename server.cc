//Carlos Andres Martinez - Cliente Servidor - Buscar - Reproducir 
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
	fstream ficheroEntrada;
	int indx=0;
	string nombre;
	string name_song;
	ficheroEntrada.open ("namesong.txt",ios::in);
			
		if (ficheroEntrada.is_open()) {		
			while (! ficheroEntrada.eof() ) { //while not (end of file) 
								
				getline(ficheroEntrada , nombre);
				map_name[indx]=nombre;					
				cout<< indx << " | " << nombre << endl;			
				indx++;
			}
			ficheroEntrada.close();
		}else cout << "Fichero inexistente" << endl;
	//return map_musica;	
	//cout << "Ingrese Key a buscar: " << endl;
	//cin >> a;
	std::map<int, string>::iterator it=map_name.find(a); // buscar
	//cout << "Key => " << (*it).second << endl;
	name_song = (*it).second;
	return name_song;
	//for (map<int, string>::iterator it=map_musica.begin(); it!=map_musica.end(); it++){
	//	cout << (*it).first << " => " << (*it).second << endl;
	//}		
}	

//////////////////////////////// find to path
string findtofile(int b){

	map<int,string> map_path;
	fstream ficheroEntrada;
	int indx=0;
	string nombre;
	string path;
	ficheroEntrada.open ("path.txt",ios::in);
			
		if (ficheroEntrada.is_open()) {		
			while (! ficheroEntrada.eof() ) { //while not (end of file) 
								
				getline(ficheroEntrada , nombre);
				map_path[indx]=nombre;					
				cout<< indx << " | " << nombre << endl;			
				indx++;
			}
			ficheroEntrada.close();
		}else cout << "Fichero inexistente" << endl;
	//return map_musica;	
	//cout << "Ingrese Key a buscar: " << endl;
	//cin >> a;
	std::map<int, string>::iterator it=map_path.find(b); // buscar
	//cout << "Key => " << (*it).second << endl;
	path = (*it).second;
	return path;
	//for (map<int, string>::iterator it=map_musica.begin(); it!=map_musica.end(); it++){
	//	cout << (*it).first << " => " << (*it).second << endl;
	//}		
}	

////////////////////////////transfer to file 
vector<char> ReadAllBytes(const string &filename)
{
    ifstream ifs(filename, ios::binary|ios::ate); // read filename in binary and stay end file
    ifstream::pos_type pos = ifs.tellg(); // position
    //cout << pos <<endl;
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
		//g << name_path;
		filetomessage(name_path,g);
		s.send(g);
  }	
	return 0;
}
