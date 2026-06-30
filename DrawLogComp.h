#pragma once
#include "Global.h"

#define CompLife 1200

void DrawComps()
{
	if (viewcomp)
		for (int i = int(comps.size()) - 1; i >= 0; --i)
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
				Color c;
				switch (comps[i].type)
				{
				case 1: c = Color(0, 255, 0, 170); break;
				case 2: c = Color(0, 0, 255, 170); break;
				case 3: c = Color(255, 0, 0, 170); break;
				case 4: c = Color(0, 255, 255, 170); break;
				case 5: c = Color(255, 255, 0, 170); break;
				case 6: c = Color(255, 0, 255, 170); break;
				case 7: c = Color(255, 255, 255, 170); break;
				default: c = Color(255, 255, 255, 170); break;
				}
				gCircles.add(comps[i].pos.x, comps[i].pos.y, CellRadius / 4.f, c,
					1.f, Color(255, 255, 255, 170));
			}
}

void LogComps()
{
	while (SpawnACompCD > SpawnACompvall * (abs(gen % 10 - 5) + 1))
	{
		for (int i = int(comps.size()) - 1; i >= 0; --i)
		{
			comps[i].howlonglive++;
			if (rand() % (CompLife - min(comps[i].howlonglive, CompLife - 1)) == 0)
				comps[i].type = -1;
		}
		SpawnACompCD -= SpawnACompvall;
		Vector2f bestpos;
		float bestdisttonearcomp = 0;
		for (int i = 0; i < MapWidth / CellDiameter; i++)
			for (int u = 0; u < MapHeight / CellDiameter; u++)
				compgrid[i][u].clear();
		for (int i = int(comps.size()) - 1; i >= 0; i--)
			if (comps[i].pos.x > 0 && comps[i].pos.y > 0)
				compgrid[int(comps[i].pos.x / CellDiameter)]
				[int(comps[i].pos.y / CellDiameter)].push_back(i);
		for (int u = 0; u < 1; u++)
		{
			Vector2f pos(float(rand() % MapWidth), float(rand() % MapHeight));
			int nearest = -1; float mindist = MapWidth + MapHeight;
			int resx = 0, resy = 0, v1, v2, v3,
				cx = int(pos.x / CellDiameter),
				cy = int(pos.y / CellDiameter);
			for (v1 = 0; v1 < 20; v1++)
			{
				for (v2 = 0; v2 < 4 - 3 * (v1 == 0); v2++)
				{
					for (v3 = -v1; v3 < v1; v3++)
					{
						switch (v2)
						{
						case 0:
							resx = cx + v3; resy = cy - v1;
							break;
						case 1:
							resx = cx + v1; resy = cy + v3;
							break;
						case 2:
							resx = cx - v3; resy = cy + v1;
							break;
						case 3:
							resx = cx - v1; resy = cy - v3;
							break;
						}
						if (resx >= 0 && resx < ((MapWidth) / CellDiameter) + 1 &&
							resy >= 0 && resy < ((MapHeight) / CellDiameter) + 1)
							for (int i = 0; i < compgrid[resx][resy].size(); i++)
								if (comps[compgrid[resx][resy][i]].type != -1)
							{
								float len = Length(pos, comps[compgrid[resx][resy][i]].pos);
								if (len < mindist)
								{
									mindist = len; nearest = compgrid[resx][resy][i];
								}
							}
					}
				}
				if (nearest != -1) break;
			}
			if (bestdisttonearcomp < mindist)
			{
				bestdisttonearcomp = mindist;
				bestpos = pos;
			}
		}
		if (comps.size() < MapHeight * 2)
		comps.push_back(Component(bestpos, 1));
	}
	for (int i = int(comps.size()) - 1; i >= 0; --i)
		if (comps[i].type == -1
			|| comps[i].pos.x < CellRadius / 2
			|| comps[i].pos.x > MapWidth - CellRadius / 2
			|| comps[i].pos.y < CellRadius / 2
			|| comps[i].pos.y > MapHeight - CellRadius / 2) comps.erase(comps.begin() + i);
}