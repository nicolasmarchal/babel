// client_babel.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include <cstdlib>
#include "stdafx.h"
#include "NetworkError.hh"
#include "TermGUI.hh"
#include "Client.hh"

int main(int ac, char **av)
{
	  Client client;
		IEventManager *app = new TermEventManager(ac, av);

  //  try
   // {
    //    client.connect(std::string("127.0.0.1"), 4242);
   // } catch (NetworkError *e)
 //   {
		//	std::cout << e->what() << std::endl;
   //   delete e;
    //  return (1);
   // }
    client.loop(*app);
		delete app;
		return (0);
}
