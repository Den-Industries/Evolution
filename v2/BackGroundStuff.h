#pragma once
#include "Global.h"

vector<Vector2f> BGDots;

void BGInit()
{
	for (int i = 0; i < BackGroundDots; i++)
		BGDots.push_back(Vector2f(rand() % int(MapWidth * 0.98f) + MapWidth * 0.01f,
			rand() % int(MapHeight * 0.98f) + MapHeight * 0.01f));
}

void SolidBG()
{
	RectangleShape bg;
	bg.setFillColor(Color(75, 165, 210, 140));
	bg.setOutlineColor(Color(75, 165, 210, 60));
	bg.setOutlineThickness(2);
	bg.setSize(Vector2f(MapWidth, 4));
	bg.setPosition(0, -2);
	window.draw(bg);
	bg.setPosition(0, MapHeight - 2);
	window.draw(bg);
	bg.setSize(Vector2f(4, MapHeight));
	bg.setPosition(-2, 0);
	window.draw(bg);
	bg.setPosition(MapWidth - 2, 0);
	window.draw(bg);
	bg.setOutlineThickness(1);
	bg.setSize(Vector2f(MapWidth, 4));
	bg.setPosition(0, -2);
	window.draw(bg);
	bg.setPosition(0, MapHeight - 2);
	window.draw(bg);
	bg.setSize(Vector2f(4, MapHeight));
	bg.setPosition(-2, 0);
	window.draw(bg);
	bg.setPosition(MapWidth - 2, 0);
	window.draw(bg);
}

void BackGround()
{
	SolidBG();
	for (int i = 0; i < BackGroundDots; i++)
	{
		if (BGDots[i].x > view.getCenter().x - view.getSize().x / 2 &&
			BGDots[i].x < view.getCenter().x + view.getSize().x / 2 &&
			BGDots[i].y > view.getCenter().y - view.getSize().y / 2 &&
			BGDots[i].y < view.getCenter().y + view.getSize().y / 2)
		{
			if (BGmovecd >= BackGroundMovevall)
			{
				if (rand() % 8 == 0) BGDots[i].x += rand() % 3 - 1;
				if (rand() % 8 == 0) BGDots[i].y += rand() % 3 - 1;
			}
			BGDots[i].x = float(int(BGDots[i].x + MapWidth - (2 + i % 3) * 2) % (MapWidth - (2 + i % 3) * 2));
			BGDots[i].y = float(int(BGDots[i].y + MapHeight - (2 + i % 3) * 2) % (MapHeight - (2 + i % 3) * 2));
			CircleShape cellshape(float(2 + i % 3));
			cellshape.setOrigin(float(2 + i % 3), float(2 + i % 3));
			cellshape.setPosition(BGDots[i]);
			cellshape.setFillColor(Color(100 + i % 100,
				100 + int(abs(BGDots[i].x - MapWidth / 2) + abs(BGDots[i].y - MapHeight / 2)) % 100, 255, 30));
			for (int u = 1; u < 1 + i % 3; u++)
			{
				cellshape.setOutlineThickness(float(u));
				cellshape.setOutlineColor(Color(100 + i % 100,
					100 + int(abs(BGDots[i].x - MapWidth / 2) + abs(BGDots[i].y - MapHeight / 2)) % 100, 
					255, 80 / (2 + i % 3)));
				window.draw(cellshape);
			}
		}
	}
}