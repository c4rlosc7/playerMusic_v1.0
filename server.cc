//Carlos Andres Martinez - Cliente Servidor
#include <iostream>  // std::cout
#include <fstream>  //std::ifstream
#include <string>
#include <list>
#include <cassert>
#include <zmqpp/zmqpp.hpp>
using namespace std;
using namespace zmqpp;
/*
//////////////////////////////// find to name song 
map<int, string> getList()
{
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
				//cout<< indx << " | " << nombre << endl;			
				indx++;
			}
			inputFile.close();
		}else cout << "Fichero inexistente" << endl;
	return map_name;
}	

//////////////////////////////// find to path
string getSong(int b)
{
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
				indx++;
			}
			inputFile.close();
		}else cout << "Fichero inexistente" << endl;
	std::map<int, string>::iterator it=map_path.find(b); // buscar
	path = (*it).second;
	return path;
}	
*/
/////////////////CLASS-SONG-OBJECT-// name, artist, filename, Metodo string Getname
class song{

	private:
		string name;
		string artist;
		string filename;
	public:	
		song(){}  
		song(const string &name1, const string &artist1, const string &filename1){			
			name = name1;			
			artist = artist1;
			filename = filename1;
		}
		const string& getname() const { 
			return name; 
		}		
		const string& getartistic() const { 
			return artist; 
		}
		const string& getfilename() const { 
			return filename; 
		}				
};
/////////////////CLASS-PLAYLIST-OBJECT-// lista de canciones name, Metodo add,size
class playlist{
	private:
		list <song> songs;  // lista de objetos de tipo song 
		string name;		
	public:		
		playlist(){}
		playlist(const string &n){ 
			name = n;
		}		
		void add(song s){ //Add element at the end
		songs.push_back(s);
		}		
		int size(){ // Returns the number of elements on list
		return songs.size();
		}				
};
/////////////////CLASS-PLAYER-OBJECT-// mylist Metodo addsong
class player{

	private:
		list <song> all;
		list <playlist> mylists; // lista de obj de tipo playlist 

	public:
		player(){}
		list <song> find(string name){ //funcion de tipo list <song> llamada find() all songs
			list <song> result;// create to mylist
			for(const song& s: all){
				auto p = s.getname().find(name); 	
				//cout << s.getname() << endl;
				if (p != string::npos){ 
					result.push_back(s);					
					//cout << s.getname() << endl;
				}
			}
			return result;         // retorna la lista de las canciones con la similitud de la busqueda
		}
/*
		list <song> findtofilename(string name){ //funcion de tipo list <song> llamada find() all songs
			list <song> result;// create to mylist
			for(const song& s: all){
				auto p = s.getfilename().find(name); 	
				//cout << s.getname() << endl;
				if (p != string::npos){ 
					result.push_back(s);					
					//cout << s.getname() << endl;
				}
			}
			return result;         // retorna la lista de las canciones con la similitud de la busqueda
		}						
*/		
		//Add element at the end
		void addsong(song s){ 
			all.push_back(s);			
		}	
};
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
  cout << "SERVER-MUSIC-PLAYER" << endl;
  song a("Milagro", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/0_Milagro.ogg"); 
  song b("Familia Es la Oportunidad", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/1_Familia_Es_la_Oportunidad_.ogg");
  song c("Los Que Marcan el Camino", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/2_Los_Que_Marcan_el_Camino.ogg");
  song d("Poder Latino", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/3_Poder_Latino.ogg");
  song e("Loco Pro", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/4_Loco_Pro.ogg");
  song f("Dejar de Ser", "A.N.I.M.A.L", "/home/carlos/Escritorio/musica/5_Dejar_de_Ser.ogg");
  playlist rock("rock"); // playlist rock 
  rock.add(a);   //adicionar obj tipo song a la lista rock
  rock.add(b);
  rock.add(c);
  rock.add(d);
  rock.add(e);
  rock.add(f);
  player p;    // lista de obj playlist p
  p.addsong(a);
  p.addsong(b);
  p.addsong(c);
  p.addsong(d);
  p.addsong(e);
  p.addsong(f);
  
  context ctx;
  socket s(ctx, socket_type::xrep);
  s.bind("tcp://*:5555");
	
	while(true)
	{ 
		message r;
		s.receive(r);
		string id;        // id 
		r >> id;
		int operador;     // 1 buscar, 2 reproducir   
		r >> operador;
		message sc;

		if(operador == 1){             // 1 message [id,"lista",name-song]
			string wordtofind;
			r >> wordtofind;
			list <song> s1 = p.find(wordtofind);				
			sc << id;
			sc << "lista";
			for(const song& ss: s1){
				sc << ss.getname();
			}
		  	int cap;  		  		
		  	cap = s1.size();		
		  	cout << "Canciones encontradas: " << cap << endl;  		  		
		  	cout << "Partes: " << sc.parts() << endl;
		  	s.send(sc);
		}else if(operador == 2){       // 2 message [id,"reproducir",namesong,string bytes]
			string word;
			r >> word;
			list <song> s1 = p.find(word);				
			sc << id;
			sc << "reproducir";
			string pathfile;
			for(const song& ss: s1){
				pathfile = ss.getfilename();
				sc << ss.getname();
			}
		  	//int cap;  		  				  	
		  	//cap = s1.size();		
		  	cout << "ruta: " << pathfile << endl;  
		  	filetomessage(pathfile,sc);
			s.send(sc);		  	
		  	//cout << "Partes: " << sc.parts() << endl;
		  	//s.send(sc);			
		}		

  }	
	return 0;
}
