#pragma once
#include <string>
#include <SDL.h>
#include <SDL_net.h>



class PlayState;

class TCPConnection
{
public:
	TCPConnection();

public :
	bool Initialize(Uint16 port, IPaddress& ip);

	virtual bool OpenSocket() ;

	virtual bool Send(std::string& message) ;
	virtual bool ListenSocket() ;
	IPaddress& Get_ip() { return m_ip;  }
	 bool Receive(std::string& message) ;

	virtual void CloseSocket() ;
	void SetState(PlayState* state) { m_state = state; }
	void ShutDown();

private:
	SDLNet_SocketSet m_socketSet;
	IPaddress m_ip;

	TCPsocket m_listenSocket;
	TCPsocket m_socket;
	PlayState* m_state;
};

