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
void playermusica(){
	sf::Music music;
	if (!music.openFromFile("Hello.ogg"))
	    cout<<"error";//return -1; // error
	music.play();
    int i;
    cin >> i;			
}

int main(){
  cout << "CLIENTE" << endl;
  context ctx;

  socket s(ctx, socket_type::req);
  s.connect("tcp://localhost:5555");
  while(true){
	  string operation;                           // Enter operation 
	  cout << "Entre la operacion : ";
	  cin >> operation;

	  int number;                                 // Enter number 
	  cout << "Entre el numero 1-5: ";
	  cin >> number;

	  message m;                            // make message to send 
	  m << operation;
	  m << number;
	  s.send(m);
		
	  message r;                     // receive string exitoso
	  s.receive(r);
		
	  string text;
	  r >> text;
	  cout << "El nombre de la cancion es: " << text << endl;

	  int index_songc;
	  cout << "Entre el indice de la cancion que desea: ";     // make message to send 
	  cin >> index_songc;
	  message u;
	  u << index_songc;
	  s.send(u);

	  message i;
	  s.receive(i);
	  
	  string path;
	  i >> path;
	  //cout << "La ruta es: " << path << endl;  	

	  fstream o("Hello.ogg", ios::out);
	  o << path;
	  playermusica();
	  o.close();
	  int y;
	  cin >> y;	  
  }  	
  return 0;
}
