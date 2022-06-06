#pragma once
#include "Global.h"

void UpdateComps()
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
			CircleShape cellconnectionshape(CellRadius / 4);
			cellconnectionshape.setOrigin(CellRadius / 4, CellRadius / 4);
			cellconnectionshape.setPosition(comps[i].pos);
			switch (comps[i].type)
			{
			case 1: cellconnectionshape.setFillColor(Color(0, 255, 0, 50)); break;
			case 2: cellconnectionshape.setFillColor(Color(0, 0, 255, 50)); break;
			case 3: cellconnectionshape.setFillColor(Color(255, 0, 0, 50)); break;
			case 4: cellconnectionshape.setFillColor(Color(0, 255, 255, 50)); break;
			case 5: cellconnectionshape.setFillColor(Color(255, 255, 0, 50)); break;
			case 6: cellconnectionshape.setFillColor(Color(255, 0, 255, 50)); break;
			case 7: cellconnectionshape.setFillColor(Color(255, 255, 255, 50)); break;
			}
			cellconnectionshape.setOutlineThickness(1);
			cellconnectionshape.setOutlineColor(Color(255, 255, 255));
			window.draw(cellconnectionshape);
		}
	while (SpawnACompCD > SpawnACompvall)
	{
		SpawnACompCD -= SpawnACompvall;
		if (comps.size() < MapHeight)
		comps.push_back(Component(Vector2f(rand() % MapWidth, rand() % MapHeight), 1));
	}
}