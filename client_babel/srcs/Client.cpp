#include "stdafx.h"
#include "TermGUI.hh"
#include "QTGUI.hh"
#include "Client.hh"

Client::Client()
	: isAuth(false), isConnected(false), cs(CallState::NO), selecter(), server(nullptr), client(nullptr), open(true)
{
  gui = new QTGUI(*this);
  //gui = new TermGUI(*this);
  gui->init();
  audio.subscribeOnRecord(this);
  std::memset(currentUser, 0, 64);
}

Client::~Client()
{
	if (cs != CallState::NO)
		deleteUdpSocket();
	if (server)
		deleteServerSocket();
}

int
Client::initAudio()
{
	int ret;

	ret = audio.initialize();
	audio.startStreams();
	return (ret);
}

int
Client::connect(std::string const &nip, int port)
{
	ISocket *serverSock;

	server = new SocketManager(port, nip);
	server->createSocket("TCP");
	if (server->connectSocket() == -1)
	{
		serverSock = server->getSocket();
		delete serverSock;
		delete server;
		server = nullptr;
		return (0);
	}
	else
	{
		isConnected = true;
		serverSock = server->getSocket();
		sckMtxs[serverSock].w = new std::mutex;
		sckMtxs[serverSock].r = new std::mutex;
		selecter.addSocket(serverSock);
		return (1);
	}
}

void
Client::setAuth(bool auth)
{
	isAuth = auth;
}

int
Client::readHeader(ISocket *socket, t_hdr &pckt)
{
	if (socket->read(&pckt, sizeof(t_hdr)) == -1)
		return (0);
	return (1);
}

int
Client::readRequest(ISocket *socket)
{
	t_hdr pckt;

	if (socket)
	{
		if (client && socket == client->getSocket())
			onReceiveSound(socket);
		else
		{
			if (!readHeader(socket, pckt))
				return (0);
			if (pckt.magic == MAGIC_VALUE)
				doAction(pckt.cmd, socket);
		}
	}
	return (1);
}

unsigned char *
Client::readSound(ISocket *socket, int &size)
{
	t_hs hs;

	std::unique_lock<std::mutex> lock(*sckMtxs[socket].r);
	socket->read((void *)&hs, sizeof(t_hs));
	unsigned char *sound = nullptr;

	sound = new unsigned char[hs.size];
	std::memcpy(sound, hs.sound, hs.size);
	size = hs.size;
	return (hs.magic == MAGIC_VALUE ? sound : nullptr);
}

int
Client::readBytes(ISocket *socket, void *buff, unsigned int length)
{
	std::unique_lock<std::mutex> lock(*sckMtxs[socket].r);
	if (socket->read(buff, length) == -1)
		return (0);
	return (1);
}

void
Client::sendSound(unsigned char const *sound, int size)
{
	ISocket *sock = client->getSocket();
	t_hs 	  hs;
	unsigned int stack = 0;
	{
		std::unique_lock<std::mutex> lock(*sckMtxs[sock].w);
		hs.magic = MAGIC_VALUE;
		hs.cmd = c2c::SOUND;
		std::memset(hs.user, 0, 64);
		std::memcpy(hs.user, currentUser, 64);
		for (unsigned int i = 0; i < (unsigned int)size; i = i + SOUND_SIZE)
		{
			hs.size = MY_MIN(SOUND_SIZE, size - stack);
			std::memset(hs.sound, 0, SOUND_SIZE);
			std::memcpy(hs.sound, &sound[stack], hs.size);
			sock->write((void *)&hs, sizeof(hs));
			stack += SOUND_SIZE;
		}
	}
}

void
Client::sendRequest(cmd::t_cmd const &cmd, void *data, unsigned int length)
{
	unsigned int glength = sizeof(t_hdr) + length;
	ISocket *sock = server->getSocket();
	char *packet = new char[glength];
	{
		std::unique_lock<std::mutex> lock(*sckMtxs[sock].w);
		t_hdr com;

		com.magic = MAGIC_VALUE;
		com.cmd = cmd;
		std::memcpy(packet, &com, sizeof(t_hdr));
		if (data != nullptr)
			std::memcpy(&packet[sizeof(t_hdr)], data, length);
		sock->write((void *)packet, (int)glength);
	}
	delete[] packet;
}

void
Client::doAction(unsigned int cmd, ISocket *socket)
{
	switch (cmd)
	{
		case cmd::REGISTER:
			onRegister(socket);
			break;
		case cmd::LOGIN:
			onLogin(socket);
			break;
		case cmd::GET_CONTACT_LIST:
			onGetContactList(socket);
			break;
		case cmd::ADD_CONTACT:
			onAddContact(socket);
			break;
		case cmd::DEL_CONTACT:
			onDelContact(socket);
			break;
		case cmd::SEND_CALL:
			onSendCall(socket);
			break;
		case cmd::SEND_TEXT:
			onSendText(socket);
			break;
		case cmd::GET_ALL_CONTACT:
			onGetAllContact(socket);
			break;
		case cmd::GET_CONTACT_ONLINE:
			onGetOnlineContacts(socket);
			break;
		case cmd::GET_CONV_HISTORY:
			onGetConvHistory(socket);
			break;
		case rsp::ADD_CONTACT:
			onAddContactRequest(socket);
			break;
		case rsp::REP_CONTACT:
			onRepContactRequest(socket);
			break;
		case rsp::SEND_CALL:
			onSendCallRequest(socket);
			break;
		case rsp::RESPONSE_CALL:
			onRepCallRequest(socket);
			break;
		case rsp::END_CALL:
			onEndCallRequest(socket);
			break;
		case rsp::RECEIVE_TEXT:
			onReceiveText(socket);
			break;
		case rsp::NEW_CONN:
			onNewConn(socket);
			break;
		case rsp::DECONN:
			onDeconn(socket);
            break;
		case c2c::SOUND:
			onReceiveSound(socket);
			break;
		case c2c::VIDEO:
			onReceiveVideo(socket);
			break;
	}
}

int
Client::loop(IEventManager & app)
{
  Select::SelectIterator si;
  ISocket *sock;

  while (isOpen())
	{
    si = selecter(100000 / 60);
		while (si.hasNext())
		{
			sock = si.getNext();
			if (!readRequest(sock))
				removeSocket(sock);
		}
    app.processEvents();
  }
	return (1);
}

bool
Client::isOpen() const
{
	return (open);
}

void
Client::quit()
{
	open = false;
}

void
Client::onRegister(ISocket *socket)
{
	t_rsp *rsp = new t_rsp;

	readBytes(socket, (void *)rsp, sizeof(*rsp));
	gui->onRegister((void *)rsp);

}

void
Client::onLogin(ISocket *socket)
{
	t_rsp *rsp = new t_rsp;

	readBytes(socket, (void *)rsp, sizeof(*rsp));
	if (rsp->status == OK)
		isAuth = true;
	else
		std::memset(currentUser, 0, 64);
	gui->onLogin((void *)rsp);
}

void
Client::onLogout(ISocket *socket)
{
	(void)socket;
	isAuth = false;
	std::memset(currentUser, 0, 64);
  gui->onLogout(NULL);
}

void
Client::onGetConvHistory(ISocket *socket)
{
	t_conv *conv = new t_conv;

	readBytes(socket, (void *)conv, sizeof(*conv));
    gui->onGetConvHistory(conv);
}

void
Client::onGetContactList(ISocket *socket)
{
	t_usr *usr = new t_usr;

	readBytes(socket, (void *)usr, sizeof(*usr));
	gui->onGetContactList((void *)usr);
}

void
Client::onGetOnlineContacts(ISocket *socket)
{
	t_usr *usr = new t_usr;

	readBytes(socket, (void *)usr, sizeof(*usr));
	gui->onGetOnlineContact((void *)usr);
}

void
Client::onAddContact(ISocket *socket)
{
	t_rsp *rsp = new t_rsp;

	readBytes(socket, (void *)rsp, sizeof(*rsp));
	gui->onAddContact((void *)rsp);
}

void
Client::onDelContact(ISocket *socket)
{
	t_rsp *rsp = new t_rsp;

	readBytes(socket, (void *)rsp, sizeof(*rsp));
	gui->onDelContact((void *)rsp);
}

void
Client::onSendCall(ISocket *socket)
{
	t_rsp *rsp = new t_rsp;

	readBytes(socket, (void *)rsp, sizeof(*rsp));
	if (rsp->status == OK)
		cs = CallState::WAIT;
	gui->onSendCall((void *)rsp);
}

void
Client::onSendText(ISocket *socket)
{
	t_rsp *rsp = new t_rsp;

	readBytes(socket, (void *)rsp, sizeof(*rsp));
	gui->onSendText((void *)rsp);
}

void
Client::onGetAllContact(ISocket *socket)
{
	t_usr *usr = new t_usr;

	readBytes(socket, (void *)usr, sizeof(*usr));
	gui->onGetAllContact((void *)usr);
}

void
Client::onAddContactRequest(ISocket *socket)
{
	t_usr *usr = new t_usr;

	readBytes(socket, (void *)usr, sizeof(*usr));
	gui->onAddContactRequest((void *)usr);
}

void
Client::onRepContactRequest(ISocket *socket)
{
	t_usrrep *up = new t_usrrep;

	readBytes(socket, (void *)up, sizeof(*up));
	gui->onRepContactRequest((void *)up);
}

void
Client::onRepCallRequest(ISocket *socket)
{
	t_usrrep *up = new t_usrrep;
	int ret;

	readBytes(socket, (void *)up, sizeof(*up));
	if (up->rep == OK)
	{
		if ((ret = initAudio()) == 2)
			{
				gui->onError(new std::string("No input device"));
				return ;
			}
		else if (ret == 1)
			{
				gui->onError(new std::string("No output device"));
				return ;
			}
		else
			cs = CallState::IS;
	}
	else
		cs = CallState::NO;
	gui->onRepCallRequest((void *)up);
}

void
Client::onEndCallRequest(ISocket *socket)
{
	t_usr *usr = new t_usr;

	readBytes(socket, (void *)usr, sizeof(*usr));
	cs = CallState::NO;
	audio.stopStreams();
	gui->onEndCallRequest((void *)usr);
	deleteUdpSocket();
}

void
Client::onReceiveText(ISocket *socket)
{
	t_usrtxt *ut = new t_usrtxt;

	readBytes(socket, ut, sizeof(*ut));
	gui->onReceiveText((void *)ut);
}

void
Client::onSendCallRequest(ISocket *socket)
{
	t_usrsck 	*us = new t_usrsck;

	readBytes(socket, (void *)us, sizeof(*us));
	if (cs != CallState::NO)
		doResponseCall(us, us->user, NO);
	else
	{
		gui->onSendCallRequest((void *)us);
	}
}

void
Client::onNewConn(ISocket *socket)
{
	t_usr 	*u = new t_usr;

	readBytes(socket, (void *)u, sizeof(*u));
	gui->onNewConn((void *)u);
}

void
Client::onDeconn(ISocket *socket)
{
	t_usr 	*u = new t_usr;

	readBytes(socket, (void *)u, sizeof(*u));
	gui->onDeconn((void *)u);
}

void
Client::doSendImage()
{
//	t_usrsck us;
//	std::string ip;

//	if (!isAuth || !isConnected)
//	{
//		gui->onError(new std::string(ERROR));
//		return;
//	}
//	if (cs != CallState::NO)
//	{
//		gui->onError(new std::string(ERR_ALREADY_IN_CALL));
//		return;
//	}
//	if (!createUdpSocket())
//		gui->onError(new std::string("Failed to create UDP socket"));
//	std::memset(us.user, 0, 64);
//	std::memcpy(us.user, user.c_str(), MY_MIN(user.size(), 64));
//	std::memset(us.ip, 0, 64);
//	getIp(client->getSocket(), ip);
//	std::memcpy(us.ip, ip.c_str(), MY_MIN(ip.size(), 64));
//	us.port = getPort(client->getSocket());
//	std::cout << us.ip << " " << us.port << std::endl;
//	sendRequest(cmd::SEND_CALL, &us, sizeof(us));
}

void
Client::doGetConvHistory(const std::string & username)
{
	t_usr u;

	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}
	std::memset(u.user, 0, 64);
    std::memcpy(u.user, username.c_str(), MY_MIN(username.size(), 64));
	sendRequest(cmd::GET_CONV_HISTORY, &u, sizeof(u));
}

void
Client::doGetContacts()
{
	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}
	sendRequest(cmd::GET_CONTACT_LIST);
}

void
Client::doAddContact(std::string const &user)
{
	t_usr u;

    if (!isAuth || !isConnected)
    {
        gui->onError(new std::string(ERROR));
        return;
    }
	std::memset(u.user, 0, 64);
	std::memcpy(u.user, user.c_str(), MY_MIN(user.size(), 64));
	sendRequest(cmd::ADD_CONTACT, &u, sizeof(u));
}

void
Client::doRegister(std::string const &user, std::string const &pwd)
{
	t_usrpwd up;

	if (!isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}
	std::memset(up.user, 0, 64);
	std::memset(up.pwd, 0, 256);
	std::memcpy(up.user, user.c_str(), MY_MIN(user.size(), 64));
	std::memcpy(up.pwd, pwd.c_str(), MY_MIN(pwd.size(), 256));
	sendRequest(cmd::REGISTER, &up, sizeof(t_usrpwd));
}

void
Client::doLogin(std::string const &user, std::string const &pwd)
{
	t_usrpwd up;

	if (!isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}

	std::memset(up.user, 0, 64);
	std::memset(up.pwd, 0, 256);
	std::memcpy(up.user, user.c_str(), MY_MIN(user.size(), 64));
	std::memcpy(currentUser, up.user, 64);
	std::memcpy(up.pwd, pwd.c_str(), MY_MIN(pwd.size(), 256));
	sendRequest(cmd::LOGIN, &up, sizeof(t_usrpwd));
}

void
Client::doLogout()
{
	if (!isAuth)
	{
		gui->onError(new std::string(ERROR));
		return;
	}

	isAuth = false;
	sendRequest(cmd::LOGOUT);
    gui->onLogout(NULL);
}


void
Client::doEndCall()
{
	t_usr usr;

	std::memset(usr.user, 0, 64);
	std::memcpy(usr.user, currentUser, 64);

	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}

	cs = CallState::NO;
	audio.stopStreams();
	sendRequest(cmd::END_CALL, &usr, sizeof(usr));
	deleteUdpSocket();
}

void
Client::doDelContact(std::string const &user)
{
	t_usr				u;

	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}

	std::memset(u.user, 0, 64);
	std::memcpy(u.user, user.c_str(), MY_MIN(user.size(), 64));
	sendRequest(cmd::DEL_CONTACT, &u, sizeof(u));
}

void
Client::doRepContact(std::string const &user, unsigned int rep)
{
	t_usrrep up;

	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}

	std::memset(up.user, 0, 64);
	std::memcpy(up.user, user.c_str(), MY_MIN(user.size(), 64));
	up.rep = rep;
	sendRequest(cmd::REP_CONTACT, &up, sizeof(up));
}

void
Client::doSendCall(std::string const &user)
{
	t_usrrep 		us;

	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}
	if (cs != CallState::NO)
	{
		gui->onError(new std::string(ERR_ALREADY_IN_CALL));
		return;
	}
	if (!createUdpSocket())
		gui->onError(new std::string("Failed to create UDP socket"));
	std::memset(us.user, 0, 64);
	std::memcpy(us.user, user.c_str(), MY_MIN(user.size(), 64));
	us.rep = getPort(client->getSocket());
	sendRequest(cmd::SEND_CALL, &us, sizeof(us));
}

void
Client::doSendText(std::string const &user, std::string const &text)
{
	t_usrtxt ut;

	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}

	std::memset(ut.user, 0, 64);
	std::memcpy(ut.user, user.c_str(), MY_MIN(user.size(), 64));
	std::memset(ut.txt, 0, 1024);
	std::memcpy(ut.txt, text.c_str(), MY_MIN(text.size(), 1024));
	sendRequest(cmd::SEND_TEXT, &ut, sizeof(ut));
}

void
Client::doGetOnlineContacts()
{
	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}

	sendRequest(cmd::GET_CONTACT_ONLINE);
}

void
Client::doResponseCall(t_usrsck const *us, std::string const &user, unsigned int rep)
{
	t_usrrep up;
	int ret;

	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		return;
	}

	std::memset(up.user, 0, 64);
	std::memcpy(up.user, user.c_str(), MY_MIN(user.size(), 64));
	up.rep = rep;
	if (up.rep == OK)
	{
		if ((ret = initAudio()) == 2)
		{
			up.rep = NO;
			gui->onError(new std::string("No input device"));
			return ;
		}
		else if (ret == 1)
		{
			up.rep = NO;
			gui->onError(new std::string("No output device"));
			return ;
		}
		else
		{
			connectUdpSocket(us->ip, us->port);
			cs = CallState::IS;
		}
	}
	sendRequest(cmd::RESPONSE_CALL, &up, sizeof(up));
}

void
Client::onReceiveSound(ISocket *socket)
{
	int size = 0;
	unsigned char *sound = nullptr;

	if (!isAuth || !isConnected)
	{
		gui->onError(new std::string(ERROR));
		deleteUdpSocket();
		onLogout(NULL);
		return;
	}
	sound = readSound(socket, size);
	audio.setPlayBuffer(audio.getEncoder()->decodeAudio(sound, size));
	if (sound)
		delete[] sound;
}

void
Client::onReceiveVideo(ISocket *socket)
{
	(void)socket;
}

void
Client::onRecordAudio(unsigned char const *audio, int size)
{
	if (cs == CallState::IS && client != nullptr)
		sendSound(audio, size);
}

void
Client::getIp(ISocket *socket, std::string &s) const
{
	char name[255];
	char *ip;
	struct hostent *hostinfo;

	(void)socket;
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
		}
	s = std::string(ip);
}

int
Client::getPort(ISocket *socket) const
{
	struct sockaddr	addr;
	SOCKADDR_IN	*addr_in;
	int			size;

	size = sizeof(addr);
	if (getsockname(socket->getFd(), &addr, (socklen_t *)&size) == -1)
		return (-1);
	addr_in = (struct sockaddr_in	*)&addr;
	return (ntohs(addr_in->sin_port));
}

void
Client::connectUdpSocket(STRING const ip, unsigned int port)
{
	client = new SocketManager(port, ip);
	ClientUDP *c;

	client->createSocket("UDP");
	c = new ClientUDP(client->getSocket());
	c->setSin(client->getSin());
	client->setSocket(c);
	selecter.addSocket(c);
	sckMtxs[c].r = new std::mutex;
	sckMtxs[c].w = new std::mutex;
}

int
Client::createUdpSocket()
{
	client = new SocketManager(0);
	ClientUDP *c;

	client->createSocket("UDP");
	if (client->bindSocket() == -1)
		return (0);
	c = new ClientUDP(client->getSocket(), true);
	c->setSin(client->getSin());
	client->setSocket(c);
	selecter.addSocket(c);
	sckMtxs[c].r = new std::mutex;
	sckMtxs[c].w = new std::mutex;
	return (1);
}

void
Client::deleteUdpSocket()
{
	ISocket *sock;

	cs = CallState::NO;
	if (client != nullptr)
	{
		sock = client->getSocket();
		if (sock != nullptr)
		{
			delete sckMtxs[sock].r;
			delete sckMtxs[sock].w;
			client->closeSocket();
			selecter.removeSocket(sock);
			delete sock;
		}
		delete client;
		client = nullptr;
	}
}

void
Client::deleteServerSocket()
{
	ISocket *sock;

	isAuth = false;
	isConnected = false;
	if (server != nullptr)
	{
		sock = server->getSocket();
		if (sock != nullptr)
		{
			server->closeSocket();
			selecter.removeSocket(sock);
			delete sckMtxs[sock].r;
			delete sckMtxs[sock].w;
			delete sock;
		}
		delete server;
		server = nullptr;
	}
}

void
Client::removeSocket(ISocket *socket)
{
	if (server != nullptr && socket == server->getSocket())
	{
		deleteServerSocket();
		gui->onError(new std::string("Server socket is unreadable"));
	}
	else if (client != nullptr && socket == client->getSocket())
	{
		deleteUdpSocket();
		gui->onError(new std::string("Client socket is unreadable"));
	}
}
