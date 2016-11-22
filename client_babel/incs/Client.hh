#pragma once

#ifndef CLIENT_HH_
# define CLIENT_HH_
# include <cstdlib>
# include <string>
# include <thread>
# include <iostream>
# include <map>
# include <mutex>
# include <list>
# include <cstring>
# include <string.h>

# include "ISocket.hh"
# include "ClientUDP.hh"
# include "ClientSocket.hh"
# include "Select.hh"
# include "protocol.hh"
# include "AGUI.hh"
# include "AudioEncapsuler.hh"
# include "IAudioObserver.hh"
# include "SocketManager.hh"

#define MY_MIN(x, y) x > y ? y : x
#define ERROR "You are not logged or connected"
#define ERR_ALREADY_IN_CALL "You already are in call"

namespace CallState
{
	typedef enum e_callState
	{
		NO,
		WAIT,
		IS
	}			cs;
}

struct rwMutex
{
	std::mutex *r;
	std::mutex  *w;
};

class Client : public IAudioObserver
{
public:
	Client();
	~Client();
	int connect(std::string const &ip, int port);
  int loop(IEventManager &);
	void quit();
	// thread safe read
	unsigned char *readSound(ISocket *, int &size);
	int readBytes(ISocket *, void *buff, unsigned int length);
	// thread safe send
	void sendSound(unsigned char const *, int);
	void sendRequest(cmd::t_cmd const &, void * = nullptr, unsigned int length = 0);
	bool isOpen() const;
	void setAuth(bool isAuth);

	// ui callable functions
	void doEndCall(void);
	void doGetContacts(void);
	void doGetOnlineContacts(void);
	void doAddContact(std::string const &user);
	void doRegister(std::string const &user, std::string const &pwd);
	void doLogin(std::string const &user, std::string const &pwd);
	void doLogout(void);
	void doDelContact(std::string const &user);
	void doRepContact(std::string const &user, unsigned int rep);
	void doResponseCall(t_usrsck const *us, std::string const &user, unsigned int rep);
	void doGetConvHistory(const std::string &);
	// create udp socket
	void doSendCall(std::string const &user);
	void doSendText(std::string const &user, std::string const &text);

    //extra
    void doSendImage();

	// IAudioObserver inherited function
	void onRecordAudio(unsigned char const *audio, int size);

private:
	// return status functions
	void onRegister(ISocket *socket);
	void onLogin(ISocket *socket);
	void onLogout(ISocket *socket);
	void onGetContactList(ISocket *socket);
	void onAddContact(ISocket *socket);
	void onDelContact(ISocket *socket);
	void onSendCall(ISocket *socket);
	void onSendText(ISocket *socket);
	void onGetAllContact(ISocket *socket);
	void onGetOnlineContacts(ISocket *socket);
	void onGetConvHistory(ISocket * socket);

	void onAddContactRequest(ISocket *socket);
	void onRepContactRequest(ISocket *socket);
	void onRepCallRequest(ISocket *socket);
	void onEndCallRequest(ISocket *socket);
	void onReceiveText(ISocket *socket);
	void onSendCallRequest(ISocket *socket);
	void onNewConn(ISocket *socket);
	void onDeconn(ISocket *socket);

	void onReceiveSound(ISocket *socket);
	void onReceiveVideo(ISocket *socket);
private:
	int initAudio();

	void doAction(unsigned int cmd, ISocket *soccket);
	int	 readRequest(ISocket *);
	int	 readHeader(ISocket *, t_hdr &pckt);
	void connectUdpSocket(STRING const ip, unsigned int port);
	int	 createUdpSocket();
	void getIp(ISocket *, std::string &storage) const;
	int getPort(ISocket *) const;

	void removeSocket(ISocket *sock);
	void deleteServerSocket();
	void deleteUdpSocket();
private:
	bool 		isAuth;
	bool		isConnected;
	unsigned int soundId;
	CallState::cs cs;

	Select	selecter;
	SocketManager *server;
	SocketManager *client;
	std::map<ISocket *, rwMutex> sckMtxs;

	IGUI 							*gui;
	AudioEncapsuler		audio;

	STRING 						currentUser;

	bool open;
};

typedef void (Client::*ClientFunc)(ISocket *);

#endif /* !CLIENT_HH_ */
