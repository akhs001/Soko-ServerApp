#include "EndState.h"
#include "Game.h"
#include "Input.h"
#include "MenuState.h"
#include "PlayState.h"
#include "Utils.h"
#include "Sprite.h"
#include "Screen.h"
#include "Player.h"
#include "Movable.h"
#include "Button.h"



PlayState::PlayState()
{
	isLevelComplete = false;
	m_Player1 = nullptr;
	m_Player2 = nullptr;
	m_level = "";
	m_image = nullptr;
	btn_Back = nullptr;
	btn_Reset = nullptr;
}
PlayState::PlayState(std::string file , bool Multiplayer)
{
	m_Player1 = nullptr;
	m_Player2 = nullptr;
	m_level = "";
	m_image = nullptr;
	btn_Back = nullptr;
	btn_Reset = nullptr;
	m_isMultiplayer = Multiplayer;
	strcpy_s( filename , file.c_str() );
}
//------------------------------------------------------------------------------------------------------
//function that creates all game objects for the state
//------------------------------------------------------------------------------------------------------
bool PlayState::OnEnter()
{
	//Send the State to TCPClass
	m_Server.SetState(this);
	TotalBalls = 0;
	isBackPressed = false;
	m_image = new Background("Assets/Images/BG/bg.png");

	//Create the button
	btn_Back = new Button(10, 10, Vector2::vector2({ 100, 50 }), "BACK", "BUTTON", false);
	btn_Back->SetMenuState(this);
	btn_Reset = new Button(10, 70, Vector2::vector2({ 100, 50 }), "RESET", "BUTTON", false);
	btn_Reset->SetMenuState(this);

	if (filename != nullptr && !m_isMultiplayer)
	{
		//Start SinglePlayer game Instantly
		StartGame(filename);
	}
	else
	{
		//Else Start A multiplayer session and wait for opponent
		std::cout << "MutiPlayer Game started" << std::endl;
		if (m_Server.Initialize(1255,m_Server.Get_ip()))
		{
			std::cout << "Server Initialized" << std::endl;
		}

		if (m_Server.OpenSocket())
		{
			std::cout << "Socket Oppened" << std::endl;
		}
		//I start the level first and save it to A variable
		StartGame(filename);
		//Then the listen is running waiting for second player
		//When a player connects I send the level to client using the 
		//Listen socket function
		m_Server.ListenSocket();

		std::cout << "Game Started" << std::endl;
	}

	return true;
}
//------------------------------------------------------------------------------------------------------
//function that reads key presses, mouse clicks and updates all game objects in scene
//------------------------------------------------------------------------------------------------------
float counter = 0.0f;
GameState* PlayState::Update(int deltaTime)
{
	//Check if the level is complete
	CheckIfComplete();


	std::string data;
	//receive message

	if (isBackPressed)
	{
		isBackPressed = false;
		return new MenuState;
	}
	//Update Players
	for (Player* p : Players)
	{
		if (p != nullptr)
			p->Update(1);
	}

	for (Movable* m : Movables)
	{
		if (m != nullptr)
			m->Update(1);
	}

	for (Cell* c : m_Tiles)
	{
		if (c != nullptr)
			c->Update(1);
	}

	btn_Back->Update(1);
	btn_Reset->Update(1);

	if (isLevelComplete)
	{
		counter += 0.1f;

		if (counter > 5)
		{
			Utils::ShowMessage("Level Cleared", "Good Job");
			counter = 0.0f;
			isLevelComplete = false;
			return new MenuState;
			//Save Progress
			//TODO
		}
	}

	m_Server.Receive(data);

	

	//loop through all game objects in vector and update them only if they are active
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if ((*it)->IsActive())
		{
			(*it)->Update(deltaTime);
		}
	}

	//otherwise return reference to self
	//so that we stay in this game state
	return this;

}
//------------------------------------------------------------------------------------------------------
//function that renders all game objects in scene
//------------------------------------------------------------------------------------------------------
bool PlayState::Draw()
{
	//render the background image
	m_image->Draw();

	btn_Back->Draw();
	btn_Reset->Draw();
	//loop through all game objects in vector and 
	//display them only if they are active and visible
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		if ((*it)->IsActive() && (*it)->IsVisible())
		{
			(*it)->Draw();
		}
	}

	for (Cell* c : m_Tiles)
	{
		if (c != nullptr)
			c->Draw();
	}


	for (Movable* m : Movables)
	{
		if (m != nullptr)
			m->Draw();
	}

	for (Player* p : Players)
	{
		if (p != nullptr)
			p->Draw();
	}
	
	return true;
}
//------------------------------------------------------------------------------------------------------
//function that removes all game objects from memory
//------------------------------------------------------------------------------------------------------
void PlayState::OnExit()
{
	//SDLNet_DelSocket(m_Server.)
	//loop through all game objects in vector and remove them from memory
	for (auto it = m_gameObjects.begin(); it != m_gameObjects.end(); it++)
	{
		delete (*it);
	}

	m_gameObjects.clear();
	m_Tiles.clear();
}

bool PlayState::OpenFile()
{
		OPENFILENAME ofn;
		ZeroMemory(&filename, sizeof(filename));
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
		ofn.lpstrFilter = ("Level Files\0*.bin\0");
		ofn.lpstrFile = filename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = ("Select a Level To Play!");
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn))
		{
			std::cout << "File Opened \"" << filename << "\"\n";
			return true;
		}
		else
		{
			std::cout << "Error Opening the file \"" << filename << "\"\n";
			return false;
		}

}



void PlayState::StartGame( std::string fileName)
{
	Movables.clear();
	Players.clear();
	m_Tiles.clear();
	TotalBalls = 0;
	BallsOnPlace = 0;


	std::ifstream file(fileName, std::ios_base::binary);

	//get the size
	int width = 0;
	int height = 0;
	file.read((char*)(&width), sizeof(int));
	file.read((char*)(&height), sizeof(int));


	if (width < 5 || height < 5 || width > MAX_WIDTH || height > MAX_HEIGHT)
	{
		Utils::ShowMessage("An error found when loading the level.Maybe the leves is corrupted", "Error level");
		return;
	}
	m_level += std::to_string( width) + "," + std::to_string( height) + ",";
	int NumCells = width * height;

	int _width = Screen::Instance()->GetResolution().x;
	int _height = Screen::Instance()->GetResolution().y;
	//Calculate the tile size
	 tileS = (_height - 100) / width;

	float middleX = _width * 0.5f - (tileS * width * 0.5f);
	float middleY = _height * 0.5f - (tileS * height * 0.5f);
	//m_currentWidth = width;
	//m_currentHeight = height;

	Cell* thecell;
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			//check the Number of the cell
			int cellNumber;
			file.read((char*)&cellNumber, sizeof(int));

			//LOAD ONLY THE TILES WE NEED FOR THE LEVEL***
			std::string name = std::to_string(cellNumber) + ".png";
			std::string filename = "Assets/mapImages/Decor_Tiles/" + name;
			Sprite::Load(filename, std::to_string(cellNumber));
			//********************************************
	
			m_level += std::to_string( cellNumber )+ ",";

			if (cellNumber == 32)
			{
				Movable* ball = new Movable(i * tileS + middleX, j * tileS + middleY, tileS, std::to_string(32));
				ball->SetPlayState(this);
				Movables.push_back(ball);
				TotalBalls++;
			}
			else if (cellNumber == 28 || cellNumber == 29)		//If the tile is Player Put the player 28= Player 1 , 29= Player2
			{

				switch (cellNumber)
				{
				case 28:
					Player * p1;
					p1 = new Player(i * tileS + middleX, j * tileS + middleY, tileS, std::to_string(28));
					p1->IsControllable(true);	//Can control this Player
					p1->SetPlayState(this);		//Pass the PlayState to the Player
					SetPlayer(1, *p1);			//Put the Player in the vector
					break;
				case 29:
					Player * p2;
					p2 = new Player(i * tileS + middleX, j * tileS + middleY, tileS, std::to_string(29));
					p2->IsControllable(false);	//This Player is for Player2
					p2->SetPlayState(this);		//Pass the PlayState to the Player
					SetPlayer(1, *p2);			//Put the Player in the vector
					break;
				}
			}
			else
			{
				std::string name = std::to_string(cellNumber) + ".png";
				std::string id = "TILE_" + std::to_string(cellNumber);
				std::string filename = "Assets/mapImages/Decor_Tiles/" + name;
				thecell = new Cell(i * tileS + middleX, j * tileS + middleY, tileS, std::to_string(cellNumber));
				thecell->SetTile(cellNumber);
				thecell->SetWalkable(thecell->IsWalkable());		//Set if the cell is walkable or not
				m_Tiles.push_back(thecell);
			}
		}
	}
	file.close();
}


void PlayState::SetPlayer(int player, Player& playerObject)
{
	switch (player)
	{
	case 1:
		m_Player1 = &playerObject;
		Players.push_back(m_Player1);
		break;
	case 2:
		m_Player2 = &playerObject;
		Players.push_back(m_Player2);
		break;
	}
}

void PlayState::CheckIfComplete()
{
	if (BallsOnPlace == TotalBalls)
	{
		//Level complete
		isLevelComplete = true;
	}
	else
	{
		isLevelComplete = false;
	}


}


//====================NETWORKING CODE =============================//

void PlayState::UpdateMovables(std::string Data)
{
	//Get All the Positions
	if (!IsMultiPlayer()) { return; }
	std::vector < std::string > Positions = Utils::Split(Data, ',');

	int c = 0;
	for (Movable* m : Movables)
	{
		//Get the pos

		std::vector <std::string> pos = Utils::Split(Positions[c], ':');

		m->SetPos({ std::stoi(pos[0]) ,std::stoi(pos[1]) });
		c++;
	}
}

void PlayState::UpdatePlayer()
{
	if (!IsMultiPlayer()) { return; }
	std::string positions = "M";

	for (Player* p : Players)
	{
		positions += std::to_string(p->GetPos().x) + ":" + std::to_string(p->GetPos().y) + ",";
	}

	m_Server.Send(positions);
}

void PlayState::UpdateClientPosition(std::string Data)
{
	if (!IsMultiPlayer()) { return; }
	std::vector < std::string > Positions = Utils::Split(Data, ',');

	int c = 0;
	for (Player* p : Players)
	{
		//Get the pos

		std::vector <std::string> pos = Utils::Split(Positions[c], ':');

		p->SetPos({ std::stoi(pos[0]) ,std::stoi(pos[1]) });
		p->GetCollider().SetPosition(std::stoi(pos[0]), std::stoi(pos[1]));
		c++;
	}
}

void PlayState::UpdateMovables()
{
	if (!IsMultiPlayer()) { return; }
	std::string positions = "P";

	for (Movable* m : Movables)
	{
		positions += std::to_string( m->GetPos().x) + ":" + std::to_string( m->GetPos().y) + ",";
	}

	m_Server.Send(positions);
}

//====================NETWORKING CODE =============================//






