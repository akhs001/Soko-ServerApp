#ifndef PLAY_STATE_H
#define PLAY_STATE_H

#include <iostream>
#include <vector>
#include "GameObject.h"
#include "GameState.h"
#include "Cell.h"
#include "Player.h"
#include <Windows.h>
#include <fstream>
#include "Sprite.h"
#include "TCPConnection.h"
#include "TextBox.h"
#include "Dice.h"

const int MAX_WIDTH = 50;
const int MAX_HEIGHT = 50;

class Button;
class Movable;
class PlayState : public GameState
{
public:
	PlayState(std::string file , bool Multiplayer, std::vector<LevelData> SavedData, int current);
	virtual ~PlayState() {}

public:

	virtual bool OnEnter();
	virtual GameState* Update(int deltaTime);
	virtual bool Draw();
	virtual void OnExit();

public:	//My staff
	std::vector<Cell*> GetAllTiles() { return m_Tiles;  }
	std::vector<Movable*> GetAllMovables() { return Movables;  }
	std::vector<Player*> GetAllPlayers() { return Players; }
	bool OpenFile();			//Check if the file is opened successfuly
	bool IsMultiPlayer() { return m_isMultiplayer;  }
	std::vector<Player*> GetPlayerVector() { return Players;  }
	void AddMovable( Movable& moveable) { Movables.push_back(&moveable); }
	void SetPlayer(int player, Player& playerObject);
	virtual void GoBack() { isBackPressed = true; }
	void SetFileName(std::string fileName) { strcpy_s(filename,  fileName.c_str() ); }
	void CheckIfComplete();
	virtual std::string GetFilename() { return filename;  }
	virtual void StartGame( std::string fileName,int  currentLevel);			//Load the Level from the file 
	void AddBallInPlace() { BallsOnPlace++;  std::cout << "Ball Add in place " << BallsOnPlace << "/" <<  TotalBalls << std::endl; }
	void MoveBallOutOfPlace() { BallsOnPlace--;   std::cout << "Ball removed from place " << BallsOnPlace << "/" << TotalBalls << std::endl;}
	int GetBallsOnPlace() { return BallsOnPlace;  }
	int GetTotalBalls() { return TotalBalls;  }
	void SaveData(std::vector<LevelData>& data);
	int GetCurrentLevel() { return currentLevel; }
	void EnemyConnected() { enemyConnected = true;  }
	bool IsGaneStarted();

	//SYNC MOVEMENT
	void UpdateMovables();
	void UpdateMovables(std::string Data);
	void UpdatePlayer();

	void UpdateClientPosition(std::string Data);

	std::string GetLevelData() { return m_level;  }
private:
	Sprite Background;
	//My staff
	int tileS;
	int currentLevel;
	bool isBackPressed;
	char filename[MAX_PATH];	//char array for storing the filePath
	TCPConnection* m_Server;
	std::string m_level;		//The current level Loaded
	//The two Players *Only the player one is Playeable from this app
	Player* m_Player1;
	Player* m_Player2;
	bool m_isMultiplayer;
	 std::vector<Movable*> Movables;  //Store all the movables here
	std::vector<Player*> Players;	//Vector for storing the Players
	std::vector<Cell*> m_Tiles;		//Vector Storing All the tiles
	bool isLevelComplete;
	Button* btn_Back;
	Button* btn_Reset;
	int TotalBalls;
	int BallsOnPlace;
	bool enemyConnected;
	bool isGameStarted;
	TextBox* t;
	float m_timeToStart;

	//Levels 
	std::vector<LevelData> m_levels;

	std::vector<TextBox*> m_texts;

	Dice* m_myDice;

};

#endif