#pragma once

#include <mutex>
#include <list>
#include "protocol.hh"

class RequestManager
{
public:
	RequestManager();
	~RequestManager();
	void addSendedRequest(t_hdr const &);
	void addReceiveRequest(t_hdr const &);
private:
	void removeSendedRequest(t_hdr const &receive);
	std::mutex mtx;
	std::list<t_hdr> receives;
	std::list<t_hdr> sended;
	unsigned int reqId;
};