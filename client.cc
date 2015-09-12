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
  cout << "CLIENTE-MUSIC-PLAYER" << endl;
  context ctx;

  socket s(ctx, socket_type::xreq);
  s.connect("tcp://localhost:5555");
  poller p;
  p.add(s, poller::poll_in);
  
  string username;
  message m;
  cout << "nombre usuario: ";
  cin >> username;  
  int c;
  int estado = 1;
	while(true)
	{
		while(estado == 1)
		{
			cout << "MUSIC-PLAYER "	<< endl;
			cout << "1.Buscar 2.Reproducir 0.Resultado(buscar-reproducir): ";
			cin >> c;

			switch(c)
			{
				case 1: 		                        // m [id,"buscar",number]	  	
					int number;  
					cout << "Ingrese el indice a buscar solo tenemos 1-5: ";
					cin >> number;
					m << "buscar";
					m << number;
					s.send(m);
					break;
					
				case 2:                                  // m [id,"reproducir",number]
					int index_songc;				
					cout << "Entre el indice de la cancion que desea: "; 
					cin >> index_songc;	
					m << "reproducir";
					m << index_songc;
					s.send(m);	
					break;		

				case 0:
					if (p.poll()){
						if (p.has_input(s)){
							message rs;                     // receive string with message answertNSong
							s.receive(rs);	
							string resultFinal;
							rs >> resultFinal;
							//cout << resultFinal.size();
							if (resultFinal.size() < 40){		
								cout << "Nombre de la cancion: " << resultFinal << endl;
							}else{
								playermusica(resultFinal, username);
							}							
						}
					}

					break;
			}

		}
	}
  return 0;
}
