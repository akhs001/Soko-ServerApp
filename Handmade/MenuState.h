#ifndef MENU_STATE_H
#define MENU_STATE_H


#include <SDL.h>
#include "GameState.h"
#include "Sound.h"
#include <vector>
#include "star.h"



class Button;

class MenuState : public GameState
{

public:
	MenuState();
	virtual ~MenuState() {}
public:
	virtual bool OnEnter();
	virtual GameState* Update(int deltaTime);
	virtual bool Draw();
	virtual void OnExit();
	void LoadData(std::vector< LevelData>& data);
	virtual void ShowLevels();
	virtual void StartGame(std::string level, int levelNumber);
	virtual void Quit() { isQuitPressed = true; }
	void CheckforLevels(bool multiplayer);
	virtual void IsMultiPlayer(bool flag) { m_isMultiplayer = flag;  }
	virtual bool IsMultiPlayer() { return m_isMultiplayer;  }
private:
	Sprite Background;

	std::string FILENAME;
	Button* btn_SinglePlayer;
	Button* btn_MultiPlayer;
	Button* btn_Quit;
	bool m_GameStart;
	Sound m_snd;
	bool isQuitPressed;
	std::vector<Button*> LevelBtns;
	bool m_isMultiplayer;

	//Levels 
	std::vector<LevelData> m_levels;
	int currentLevel;
	std::vector<star*> m_stars;

};

#endif