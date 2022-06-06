#pragma once
#include "Global.h"

void DrawComps()
{
	for (int i = 0; i < comps.size(); i++)
		if (comps[i].pos.x > view.getCenter().x - view.getSize().x / 2 - CellRadius &&
			comps[i].pos.x < view.getCenter().x + view.getSize().x / 2 + CellRadius &&
			comps[i].pos.y > view.getCenter().y - view.getSize().y / 2 - CellRadius &&
			comps[i].pos.y < view.getCenter().y + view.getSize().y / 2 + CellRadius)
		{
			if (SpawnACompCD > SpawnACompvall)
			{
				if (rand() % 8 == 0) comps[i].pos.x += rand() % 7 - 3;
				if (rand() % 8 == 0) comps[i].pos.y += rand() % 7 - 3;
			}
			CircleShape compshape(CellRadius / 4);
			compshape.setOrigin(CellRadius / 4, CellRadius / 4);
			compshape.setPosition(comps[i].pos);
			switch (comps[i].type)
			{
			case 1: compshape.setFillColor(Color(0, 255, 0, 170)); break;
			case 2: compshape.setFillColor(Color(0, 0, 255, 170)); break;
			case 3: compshape.setFillColor(Color(255, 0, 0, 170)); break;
			case 4: compshape.setFillColor(Color(0, 255, 255, 170)); break;
			case 5: compshape.setFillColor(Color(255, 255, 0, 170)); break;
			case 6: compshape.setFillColor(Color(255, 0, 255, 170)); break;
			case 7: compshape.setFillColor(Color(255, 255, 255, 170)); break;
			}
			compshape.setOutlineThickness(1);
			compshape.setOutlineColor(Color(255, 255, 255, 170));
			window.draw(compshape);
		}
}
void LogComps()
{
	while (SpawnACompCD > SpawnACompvall)
	{
		SpawnACompCD -= SpawnACompvall;
		if (comps.size() < MapHeight / 3)
		comps.push_back(Component(Vector2f(float(rand() % MapWidth), float(rand() % MapHeight)), 1));
	}
}