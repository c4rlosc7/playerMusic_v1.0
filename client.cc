//Carlos Andres Martinez - Cliente Servidor - Buscar - Reproducir 
#include <iostream>
#include <string>
#include <cassert>
#include <fstream>
#include <zmqpp/zmqpp.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace zmqpp;

////////////////////////////play to music 
void playermusica(string string_in, string usr){
	string songPlay = usr + "-" + "song.ogg";
	fstream ob(songPlay, ios::out);
	ob << string_in;	
	sf::Music music;
	if (!music.openFromFile(songPlay))
	    cout<<"error";//return -1; // error
	music.play();
    int i;
    cin >> i;	
    ob.close();		
}

int main(int argc, char **argv){	
  string ip; 		// 10.253.96.236 U, 192.168.1.12 CASA 
  ip = argv[1];
  //cout << "CLIENTE-MUSIC-PLAYER" << endl;
  context ctx;

  socket s(ctx, socket_type::xreq);
  s.connect("tcp://"+ip+":5555");
  poller p;
  p.add(s, poller::poll_in);
  string username;
  cout << "Nombre de usuario: ";
  cin >> username;
  int c;
  string keyword;
  string keysong;
  
  message m;                 // mensaje m
while(true)
{
	cout << "-----------------CLIENT-MUSIC-PLAYER-----------------"	<< endl;
	cout << "1.Buscar  2.Reproducir : ";
	cin >> c;
	switch(c)
	{
		case 1: 			  	
  			cout << "Esperando respuesta...\n\n";
  			m << c;
  			s.send(m);	                  
  			for(size_t ii = 0; ii < m.parts(); ii++) {
				cout << m.get(ii) << endl;
			}
			break;            // id, 1
		case 2:                           
			m << c;
			
			int index;
			cout << "Indice: ";				
			cin >> index;
			m << index;

			cout << "Esperando respuesta...\n\n";			
		  	for(size_t i = 0; i < m.parts(); i++) {
				cout << m.get(i) << endl;
			}
			s.send(m);	      // id, 2
			break;	
		default: cout << "Ingrese opcion correcta";
    		break;															
	}		
  message r;
  s.receive(r);

  int tarea;
  r >> tarea;    
  if(tarea == 1){
  	cout << "Canciones encontradas: " << r.parts()-1<< endl;
	for(size_t i = 1; i < r.parts(); i++) {
		cout << "name song[" << i-1 <<"]: " << r.get(i) << endl;
	}
  }else if(tarea == 2){
  	cout << "partes recibidas: " << r.parts() <<endl;
  	string stringsong, nombre_cancion;
  	r >> nombre_cancion >> stringsong;
  	//cout << nombre_cancion << username << stringsong <<endl;
  	playermusica(stringsong, username);
  	//for(size_t i = 0; i < r.parts(); i++) {
	//	cout << r.get(i) << endl;
	//}
  }

}
  return 0;
}