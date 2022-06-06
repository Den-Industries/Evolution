#pragma once
#include "Global.h"

void Init()
{
	srand(time(NULL));
	GetDesktopResolution();
	view.setCenter(Vector2f(MapWidth / 2, MapHeight / 2));
	//view.setCenter(Vector2f(0, 0));
	view.setSize(Vector2f(ScreenSize[0], ScreenSize[1]));
	font.loadFromFile("font.ttf");
	text.setStyle(sf::Text::Bold);
	text.setOutlineThickness(3);
	text.setFillColor(Color(0, 120, 120, 127));
	text.setOutlineColor(Color(120, 0, 120, 127));
}