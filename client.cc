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

int main(){
  //cout << "CLIENTE-MUSIC-PLAYER" << endl;
  context ctx;

  socket s(ctx, socket_type::xreq);
  s.connect("tcp://localhost:5555");
  poller p;
  p.add(s, poller::poll_in);
  string username;
  cout << "Ingrese el nombre de usuario: ";
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
  			cout << "-----------------Buscar-----------------" << endl;
  			
  			cout << "palabra a buscar: ";
  			cin >> keyword;  

  			m << c;
  			m << keyword;
  			s.send(m);	                  
			break;            // id, 1, keyword

		case 2:                           
			cout << "-----------------Reproducir-----------------" << endl;	
				
			cout << "Nombre cancion: "; 
			cin >> keysong;	
			m << c;
			m << keysong;
			s.send(m);	      // id, 2, keysong
			break;	

		default: cout << "Ingrese opcion correcta";
    		break;															
	}		

  message r;
  s.receive(r);
  string tarea;
  r >> tarea;  
  string stringsong;
  r >> stringsong;
  cout << "Canciones encontradas: " << r.parts() << endl;

  if(tarea == "lista"){
	  for(size_t i = 0; i < r.parts(); i++) {
			cout << r.get(i) << endl;
		}
  }else if(tarea == "reproducir"){
  	playermusica(stringsong, username);
  }
}
  return 0;
}