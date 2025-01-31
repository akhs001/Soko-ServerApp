#pragma once
#include "GameObject.h"
#include "Text.h"
#include "Sprite.h"
#include "AABB.h"
#include "Sound.h"
#include "Vector2.h"
#include "ToolTip.h"


class GameState;

class Button : public GameObject
{
public:
	//Button();
	Button(int  x, int y, Vector2::vector2 size, const std::string& text, const  std::string& ID, bool isLevel);
	~Button();
public:
	virtual void Update(int deltaTime);
	virtual bool Draw();

	bool isClicked();
	bool CanClick();
	void SetColor(int color);
	void SetMenuState(GameState* state);
	void CanClick(bool flag);
	int GetColor();
	Vector2::vector2 GetPos();
	void SetPos(Vector2::vector2 pos);
	void SetLevel(std::string nameOfLevel) { m_levelAssigned = nameOfLevel; }
	GameState* GetState();
	bool IsLevel() { return m_isLevel;  }
	void IsLevel(bool flag) {  m_isLevel =flag; }
	Vector2::vector2 GetSize();
	void SetImage(std::string  ImageID);

private:
	int m_color;
	bool m_canClick;
	Sound m_click;
	std::string m_ID;
	std::string m_levelAssigned;
	Text m_text;
	Sprite m_image;
	Vector2::vector2 m_size;
	AABB m_collider;
	Vector2::vector2 m_pos;
	GameState* m_state;
	bool m_isLevel;
	bool isTooltip;
	ToolTip* m_tooltip;
	bool isHover();
};

