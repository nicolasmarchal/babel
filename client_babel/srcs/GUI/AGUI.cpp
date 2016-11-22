#include "stdafx.h"
#include "AGUI.hh"
#include "Client.hh"

IEventManager::~IEventManager()
{}

AGUI::AGUI(Client &c)
	: client(c)
{
}

void
AGUI::operator()()
{
	// DataFunc taskPtr;
	//
	// while (true)
	// {
	// 	{
	// 		std::unique_lock<std::mutex> lock(gm.mutex);
	//
	// 		while (gm.client.isOpen() && gm.queueEmpty())
  //               gm.cv.wait(lock);
	// 		if (!gm.client.isOpen())
	// 			break ;
	// 		taskPtr = gm.front();
	// 		gm.popFront();
	// 	}
	// 	(this->*taskPtr.func)(taskPtr.data);
	// }
	// std::cout << "Je sors du thread" << std::endl;
}

AGUI::~AGUI()
{
	client.quit();
}
