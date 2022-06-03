#pragma once
#include "Global.h"

vector<Vector2f> BGDots;

void BGInit()
{
	for (int i = 0; i < BackGroundDots; i++)
		BGDots.push_back(Vector2f(rand() % int(MapWidth * 0.98) + MapWidth * 0.01,
			rand() % int(MapHeight * 0.98) + MapHeight * 0.01));
}

void BackGround()
{
	RectangleShape bg(Vector2f(MapWidth - 6, MapHeight - 6));
	bg.setFillColor(Color(0,0,0,0));
	bg.setOutlineColor(Color(75, 85, 160, 70));
	bg.setOutlineThickness(6); bg.setPosition(3, 3); window.draw(bg);
	bg.setSize(Vector2f(MapWidth - 4, MapHeight - 4));
	bg.setOutlineThickness(4); bg.setPosition(2, 2); window.draw(bg);
	bg.setSize(Vector2f(MapWidth - 2, MapHeight - 2));
	bg.setOutlineThickness(2); bg.setPosition(1, 1); window.draw(bg);
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
			BGDots[i].x = int(BGDots[i].x + MapWidth - (2 + i % 3) * 2) % (MapWidth - (2 + i % 3) * 2);
			BGDots[i].y = int(BGDots[i].y + MapHeight - (2 + i % 3) * 2) % (MapHeight - (2 + i % 3) * 2);
			CircleShape cellshape(2 + i % 3);
			cellshape.setOrigin(2 + i % 3, 2 + i % 3);
			cellshape.setPosition(BGDots[i]);
			cellshape.setFillColor(Color(100 + i % 100,
				100 + int(abs(BGDots[i].x - MapWidth / 2) + abs(BGDots[i].y - MapHeight / 2)) % 100, 255, 30));
			for (int u = 1; u < 2 + i % 3; u++)
			{
				cellshape.setOutlineThickness(u);
				cellshape.setOutlineColor(Color(100 + i % 100,
					100 + int(abs(BGDots[i].x - MapWidth / 2) + abs(BGDots[i].y - MapHeight / 2)) % 100, 
					255, 80 / (2 + i % 3)));
				window.draw(cellshape);
			}
		}
	}
}