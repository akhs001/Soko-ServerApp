#include "TextBox.h"

static bool isFontLoaded = false;
TextBox::TextBox(int xpos , int ypos ,int sizex , int sizey ,std::string text)
{
	if (!isFontLoaded)
	{
		Text::Load("Assets/Fonts/Impact.ttf", "FONT", Text::FontSize::SMALL);
		isFontLoaded = true;
	}
	posX = xpos;
	posY = ypos;
	m_text.SetFont("FONT");
	m_text.SetColor(1, 1, 1);
	m_text.SetSize(sizex, sizey);
	m_text.SetText(text);
}

TextBox::~TextBox()
{
	m_text.Unload();
}

void TextBox::Draw()
{
	m_text.Draw(posX, posY);
}

void TextBox::SetText(std::string text)
{
	m_text.SetText(text);
}

void TextBox::SetPos(int x, int y)
{
	posX = x;
	posY = y;
}
