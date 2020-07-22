#pragma once
#include <iostream>
#include "Text.h"


class TextBox
{
public:
	TextBox(int xpos, int ypos ,int sizex, int sizey ,std::string text);
	~TextBox();
public:
	void Draw();
	void SetText(std::string text);
	void SetPos(int x, int y);

private :
	Text m_text;
	int posX;
	int posY;
};

