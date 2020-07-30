#include "TCPConnection.h"
#include <iostream>
#include <string>
#include "PlayState.h"

static bool isSDLInitialized = false;

TCPConnection::TCPConnection()
{
	m_state = NULL;
	m_socket = nullptr;
	m_listenSocket = nullptr;
	//Initialize the m_ip variable
	m_socketSet = SDLNet_AllocSocketSet(1);
	m_ip = { 0, 0 };
}


//Initialize the SDL And SDLNet
bool TCPConnection::Initialize(Uint16 port , IPaddress& ip)
{
	if (isSDLInitialized) { return true;  }
	//Initialize the SDL and SDLNet
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << "Error initializing SDL" << std::endl;
		system("pause");
		return false;
	}

	if (SDLNet_Init() == -1)
	{
		std::cout << "Error initializing SDL net" << std::endl;
		system("pause");
		return false;
	}

	//Setup the server and listening to the port 1234
	if (SDLNet_ResolveHost(&ip, nullptr , port) == -1)
	{
		std::cout << "Error Creating the server" << std::endl;
		system("pause");
		return 0;
	}
	isSDLInitialized = true;
	return true;
}



bool TCPConnection::OpenSocket()
{
	m_listenSocket = SDLNet_TCP_Open(&m_ip);

	if (!m_listenSocket)
	{
		return false;
	}
	return true;
}



bool TCPConnection::Send(std::string& message)
{

	if (SDLNet_TCP_Send(m_socket, message.c_str(), 15000))
	{
		return true;
}
	else
	{
		return false;
	}

}


bool TCPConnection::ListenSocket()
{
		m_socket = SDLNet_TCP_Accept(m_listenSocket);
		SDL_Delay(100);

		if (!m_socket) { return false;  }

		SDLNet_TCP_AddSocket(m_socketSet, m_socket);
		std::cout << "Othet Player Connected" << std::endl;
		SDLNet_TCP_Close(m_listenSocket);	
		//Close the listen socket.
		//Actualy i can have many Players connected to the game
		//But for this game I only wnat two Players.

		//Prepair the level Data
		std::string theLevel = m_state->GetLevelData();
		//Send the Server dice
		//Send the level to client
		std::string Message = "L" + theLevel;

		Send(Message);
		return true;

}

bool TCPConnection::Receive(std::string& message)
{
	
	int haveMessage = SDLNet_CheckSockets(m_socketSet, 0);

	if (haveMessage > 0)
	{
		std::cout << haveMessage << std::endl;
		if (SDLNet_SocketReady(m_socket))
		{
			char data[15000];

			if (SDLNet_TCP_Recv(m_socket, &data, 15000) > 0)
			{
				message = data;
				if (message != "")
				{
					if (message[0] == 'P')			//Update all Movables Positions
					{
						//Movemtn received
						message = message.substr(1, message.length());
						m_state->UpdateMovables(message);
						return true;
					}
					if (message[0] == 'M')			//Update all Movables Positions
					{
						//Movemtn received
						message = message.substr(1, message.length());
						m_state->UpdateClientPosition(message);
						return true;
					}
				
					return true;
				}

			}

		}

	}
	return false;
}

void TCPConnection::CloseSocket()
{
	m_socket = nullptr;
	m_listenSocket = nullptr;
	SDLNet_TCP_Close(m_socket);
	SDLNet_TCP_Close(m_listenSocket);

	SDLNet_TCP_DelSocket(m_socketSet, m_socket);
	SDLNet_TCP_DelSocket(m_socketSet, m_listenSocket);
}


void TCPConnection::ShutDown()
{
	//shutDown Sdl net
	SDLNet_Quit();

	//shutDown Sdl
	SDL_Quit();
}
