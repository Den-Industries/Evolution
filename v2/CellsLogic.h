#pragma once
#include "Global.h"
#include "Cell.h"
#include "Component.h"

vector<int> whocounted;

bool Feet(int cellnum)
{
	int q = 0;
	for (int i = 0; i < CellStoragevall; i++)
		q += cells[cellnum].storage[i] != 0;
	return q > CellStoragevall / 2;
}

bool MayBeHunted(int cellnum)
{
	int q = 0;
	for (int i = 0; i < CellStoragevall; i++) 
		q += cells[cellnum].storage[i] != 0;
	return q > 4;
}

bool ThereIsAngry(int cellnum, bool start = true)
{
	if (start) whocounted.clear();
	if (find(whocounted.begin(), whocounted.end(), cellnum) == whocounted.end())
	{
		whocounted.push_back(cellnum);
		for (int i = 0; i < cells[cellnum].connections.size(); i++)
			if (cells[cells[cellnum].connections[i].first].type == 8) return true;
		for (int i = 0; i < cells[cellnum].connections.size(); i++)
			if (find(whocounted.begin(), whocounted.end(), cells[cellnum].connections[i].first) == whocounted.end())
				return ThereIsAngry(cells[cellnum].connections[i].first, false);
		return false;
	}
	else return false;
}

bool Friend(int cellnum, int othercell, bool start = true)
{
	if (start) whocounted.clear();
	if (find(whocounted.begin(), whocounted.end(), cellnum) == whocounted.end())
	{
		whocounted.push_back(cellnum);
		for (int i = 0; i < cells[cellnum].connections.size(); i++)
			if (cells[cellnum].connections[i].first == othercell) return true;
		for (int i = 0; i < cells[cellnum].connections.size(); i++)
			if (find(whocounted.begin(), whocounted.end(), cells[cellnum].connections[i].first) == whocounted.end())
				return Friend(cells[cellnum].connections[i].first, othercell, false);
		return false;
	}
	else return false;
}

void AgeAllCells(int cellnum, bool seedstoo = false, bool start = true)
{
	if (start) whocounted.clear();
	if (find(whocounted.begin(), whocounted.end(), cellnum) == whocounted.end())
	{
		whocounted.push_back(cellnum);
		cells[cellnum].age = CellAgeToDie + 500 + (seedstoo) * 500;
		for (int i = 0; i < cells[cellnum].connections.size(); i++)
			AgeAllCells(cells[cellnum].connections[i].first, seedstoo, false);
	}

}

void NeighborsLog(int cellnum, Vector2f offset, bool start = true)
{
	if (start) whocounted.clear();
	whocounted.push_back(cellnum);
	for (int i = 0; i < cells[cellnum].connections.size(); i++)
		if (find(whocounted.begin(), whocounted.end(), cells[cellnum].connections[i].first) == whocounted.end())
		{
			cells[cells[cellnum].connections[i].first].position += offset;
			if (i != 0)
			{
				Vector2f pastpos = cells[cellnum].position;
				Vector2f whereshouldbe = cells[cellnum].position;
				float len = Length(cells[cells[cellnum].connections[i].first].position, cells[cellnum].position);
				int angle = int(AngleBtw(cells[cells[cellnum].connections[0].first].position, cells[cellnum].position) + cells[cellnum].connections[i].second) % 360;
				whereshouldbe.x += len * cosf(angle * Pi);
				whereshouldbe.y += len * sinf(angle * Pi);
				Vector2f dif = whereshouldbe - cells[cells[cellnum].connections[i].first].position;
				cells[cells[cellnum].connections[i].first].position.x += min(3.0f, abs(dif.x)) * dif.x / len;
				cells[cells[cellnum].connections[i].first].position.y += min(3.0f, abs(dif.y)) * dif.y / len;
				if (Length(cells[cellnum].position, pastpos) > CellRadius * 3) cout << "WTF4";
			}
			NeighborsLog(cells[cellnum].connections[i].first, offset, false);
		}
}

void CostOfGrew(int cellnum, int *cost)
{
	for (int u = 0; u < 4; u++)
		if (cells[cellnum].g.gen[cells[cellnum].numberofgenom][u * 3] < GenomSize)
			for (int i = 0; i < CellsInfo[cells[cellnum].g.gen[cells[cellnum].numberofgenom][u * 3 + 2]][0].size(); i++)
			{
				switch (CellsInfo[cells[cellnum].g.gen[cells[cellnum].numberofgenom][u * 3 + 2]][0][i])
				{
					case 'A': cost[0]++; break;
					case 'B': cost[1]++; break;
					case 'C': cost[2]++; break;
					case 'D': cost[3]++; break;
					case 'E': cost[4]++; break;
					case 'F': cost[5]++; break;
					case 'G': cost[6]++; break;
				}
			}
}

void WhatCellHave(int cellnum, int* whathave)
{
	whocounted.push_back(cellnum); 
	for (int i = 0; i < CellStoragevall; i++)
		if (cells[cellnum].storage[i] != 0) whathave[cells[cellnum].storage[i] - 1]++;
}

void WhatCellsHave(int cellnum, int* whathave, bool start = true)
{
	if (start) whocounted.clear();
	WhatCellHave(cellnum, whathave);
	for (int i = 0; i < cells[cellnum].connections.size(); i++)
		if (find(whocounted.begin(), whocounted.end(), cells[cellnum].connections[i].first) == whocounted.end())
		{
			if (cells[cells[cellnum].connections[i].first].type == 10 ||
				cells[cells[cellnum].connections[i].first].type == 0)
				WhatCellsHave(cells[cellnum].connections[i].first, whathave, false);
			else
				WhatCellHave(cells[cellnum].connections[i].first, whathave);
		}
}

void PickFromThisCell(int cellnum, int* whatpick)
{
	whocounted.push_back(cellnum);
	for (int i = 0; i < CellStoragevall; i++)
		if (cells[cellnum].storage[i] != 0)
			if (whatpick[cells[cellnum].storage[i] - 1] > 0)
			{
				whatpick[cells[cellnum].storage[i] - 1]--;
				cells[cellnum].storage[i] = 0;
			}
}

void PickFromThisCells(int cellnum, int* whatpick, bool start = true)
{
	if (start) whocounted.clear();
	PickFromThisCell(cellnum, whatpick);
	for (int i = 0; i < cells[cellnum].connections.size(); i++)
		if (find(whocounted.begin(), whocounted.end(), cells[cellnum].connections[i].first) == whocounted.end())
		{
			if (cells[cells[cellnum].connections[i].first].type == 10 ||
				cells[cells[cellnum].connections[i].first].type == 0)
				PickFromThisCells(cells[cellnum].connections[i].first, whatpick, false);
			else
				PickFromThisCell(cells[cellnum].connections[i].first, whatpick);
		}
}

bool IsCellHaveThis(int cellnum, int type, bool start = true)
{
	if (start) whocounted.clear();
	whocounted.push_back(cellnum);
	for (int i = 0; i < CellStoragevall; i++) if (cells[cellnum].storage[i] == type) return true;
	for (int i = 0; i < cells[cellnum].connections.size(); i++)
		if (find(whocounted.begin(), whocounted.end(), cells[cellnum].connections[i].first) == whocounted.end())
			IsCellHaveThis(cellnum, type, false);
	return false;
}

void UpdateCells() //1A 2B 3C 4D 5E 6F 7D
{
	while (UpdateCellsCD > UpdateCellsCDvall)
	{
		//UpdateCellsCD -= UpdateCellsCDvall;
		UpdateCellsCD = 0;
		for (int cellnum = 0; cellnum < cells.size(); cellnum++)
		{
			int cost[7], whatcellhave[7]; bool allok;
			cells[cellnum].energy -= float(LoseEnergyvall * (1 - 0.5f * (cells[cellnum].type == 11)));
			cells[cellnum].age++;
			if (cells[cellnum].age == CellAgeToDie)
			{
				if (cellnum == 0) gen++;
				AgeAllCells(cellnum);
			}
			if (cells[cellnum].energy < 0) AgeAllCells(cellnum, true);
			if (cells[cellnum].energy < HungryEnergyvall)
			{
				bool pokushal = false;
				for (int comp = -4; comp < 3; comp++)
					if (find(CellsInfo[cells[cellnum].type][1].begin(), 
						CellsInfo[cells[cellnum].type][1].end(), cellnum) == 
						CellsInfo[cells[cellnum].type][1].end())
					{
						for (int i = 0; i < 7; i++) cost[i] = 0;
						cost[(comp + 7) % 7] = 1;
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellsHave(cellnum, whatcellhave);
						for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
						allok = true;
						for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
						if (allok)
						{
							PickFromThisCells(cellnum, cost);
							if ((comp + 7) % 7 < 3) cells[cellnum].energy += 50;
							else if ((comp + 7) % 7 < 6)  cells[cellnum].energy += 150;
							else  cells[cellnum].energy += 300;
							pokushal = true;
							break;
						}
					}
				if (!pokushal)
				{
					float envall = cells[cellnum].energy; int q = 1;
					for (int i = 0; i < cells[cellnum].connections.size(); i++)
						if (cells[cells[cellnum].connections[i].first].type == cells[cellnum].type)
						{
							envall += cells[cells[cellnum].connections[i].first].energy; q++;
						}
					envall = envall / q;
					cells[cellnum].energy = envall;
					for (int i = 0; i < cells[cellnum].connections.size(); i++)
						if (cells[cells[cellnum].connections[i].first].type == cells[cellnum].type)
							cells[cells[cellnum].connections[i].first].energy = envall;
				}
			}
			for (int i = int(comps.size()) - 1; i >= 0; i--)
				if (Length(cells[cellnum].position, comps[i].pos) <= CellRadius + (cells[cellnum].type == 0) * CellRadius)
				{
					Give(cellnum, comps[i].type); comps.erase(comps.begin() + i);
				}
			switch (cells[cellnum].type)
			{
				case 0:
					cells[cellnum].CD++;
					if (!(cells[cellnum].position.x < CellRadius || cells[cellnum].position.x > MapWidth - CellRadius ||
						cells[cellnum].position.y < CellRadius || cells[cellnum].position.y > MapHeight - CellRadius))
					if (cells[cellnum].CD >= 80)
					{
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellHave(cellnum, whatcellhave);
						if (whatcellhave[1] == 0) Give(cellnum, 2);
						else if (whatcellhave[2] == 0) Give(cellnum, 3);
						else if (rand() % 2 == 0) Give(cellnum, 2);
						else Give(cellnum, 3);
						cells[cellnum].CD = 0;
					}
					if (!ThereIsAngry(cellnum) && (!Feet(cellnum) || !IsCellHaveThis(cellnum, 0)))
					{
						int nearest = -1; float mindist = 999999;
						for (int i = 0; i < comps.size(); i++)
						{
							float len = Length(cells[cellnum].position, comps[i].pos) - ((i + cellnum % 4) % 4 == 0) * CellRadius * 2;
							if (len < mindist)
							{
								mindist = len; nearest = i;
							}
						}
						if (nearest != -1)
						{
							mindist = Length(cells[cellnum].position, comps[nearest].pos);
							Vector2f pastpos = cells[cellnum].position;
							cells[cellnum].position.x += 2 * float(comps[nearest].pos.x - cells[cellnum].position.x) / max(mindist, 0.1f);
							cells[cellnum].position.y += 2 * float(comps[nearest].pos.y - cells[cellnum].position.y) / max(mindist, 0.1f);
							if (Length(cells[cellnum].position, pastpos) > CellRadius * 3)
							{
								cout << "WTF1" << endl;
								cout << mindist << " " << comps[nearest].pos.x - cells[cellnum].position.x
									<< " " << comps[nearest].pos.y - cells[cellnum].position.y << endl;
							}
							NeighborsLog(cellnum, Vector2f(
								float(comps[nearest].pos.x - cells[cellnum].position.x) / max(mindist, 0.1f),
								float(comps[nearest].pos.y - cells[cellnum].position.y) / max(mindist, 0.1f)));
						}
					}
					break;
				case 1:
					if (!Feet(cellnum)) cells[cellnum].CD++;
					if (cells[cellnum].CD >= 30)
					{
						Give(cellnum, 2);
						cells[cellnum].CD = 0;
					}
					break;
				case 2:
					if (!Feet(cellnum)) cells[cellnum].CD++;
					if (cells[cellnum].CD >= 30)
					{
						Give(cellnum, 3);
						cells[cellnum].CD = 0;
					}
					break;
				case 3:
					if (true)
					{
						for (int i = 0; i < 7; i++) cost[i] = 0;
						cost[3] = 1;  cost[4] = 1;
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellsHave(cellnum, whatcellhave);
						for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
						allok = true;
						for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
						if (allok)
						{
							PickFromThisCells(cellnum, cost);
							Give(cellnum, 1); Give(cellnum, 1);
							Give(cellnum, 2); Give(cellnum, 3);
							Give(cellnum, 1 + rand() % 3);
						}
					}
					break;
				case 4:
					if (true)
					{
						for (int i = 0; i < 7; i++) cost[i] = 0;
						cost[3] = 1;  cost[5] = 1;
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellsHave(cellnum, whatcellhave);
						for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
						allok = true;
						for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
						if (allok)
						{
							PickFromThisCells(cellnum, cost);
							Give(cellnum, 2); Give(cellnum, 2);
							Give(cellnum, 1); Give(cellnum, 3);
							Give(cellnum, 1 + rand() % 3);
						}
					}
					break;
				case 5:
					if (true)
					{
						for (int i = 0; i < 7; i++) cost[i] = 0;
						cost[4] = 1;  cost[5] = 1;
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellsHave(cellnum, whatcellhave);
						for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
						allok = true;
						for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
						if (allok)
						{
							PickFromThisCells(cellnum, cost);
							Give(cellnum, 3); Give(cellnum, 3);
							Give(cellnum, 2); Give(cellnum, 1);
							Give(cellnum, 1 + rand() % 3);
						}
					}
					break;
				case 6:
					if (true)
					{
						int cost[7];
						for (int i = 0; i < 7; i++) cost[i] = 0;
						cost[6] = 1;
						int whatcellhave[7];
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellsHave(cellnum, whatcellhave);
						for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
						allok = true;
						for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
						if (allok)
						{
							PickFromThisCells(cellnum, cost);
							Give(cellnum, 4); Give(cellnum, 4);
							Give(cellnum, 5); Give(cellnum, 5);
							Give(cellnum, 6); Give(cellnum, 6);
						}
					}
					break;
				case 7:
					if (true)
					{
						int nearest = -1; float mindist = 999999;
						for (int i = 0; i < comps.size(); i++)
						{
							if (Length(cells[cellnum].position, comps[i].pos) < mindist)
							{
								mindist = Length(cells[cellnum].position, comps[i].pos); nearest = i;
							}
						}
						if (nearest != -1)
						{
							comps[nearest].pos.x -= 3 * float(comps[nearest].pos.x - cells[cellnum].position.x) / mindist;
							comps[nearest].pos.y -= 3 * float(comps[nearest].pos.y - cells[cellnum].position.y) / mindist;
						}
					}
					break;
				case 8:
					for (int i = 0; i < cells.size(); i++)
						if (i != cellnum && cells[i].type != 9)
							if (Length(cells[cellnum].position, cells[i].position) <= CellRadius * 3)
							{
								if (!Friend(cellnum, i))
									for (int comp = 0; comp < 7; comp++)
									{
										for (int i = 0; i < 7; i++) cost[i] = 0;
										cost[comp] = 1;
										for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
										WhatCellsHave(i, whatcellhave);
										for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
										allok = true;
										for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
										if (allok)
										{
											PickFromThisCells(i, cost);
											Give(cellnum, comp + 1);
										}
									}
							}
					if (!Feet(cellnum))
					{
						int nearest = -1; float mindist = 999999;
						for (int i = 0; i < cells.size(); i++)
						{
							if (i != cellnum && cells[i].type != 9 && !Friend(cellnum, i) && MayBeHunted(i))
								if (Length(cells[cellnum].position, cells[i].position) < mindist)
								{
									mindist = Length(cells[cellnum].position, cells[i].position); nearest = i;
								}
						}
						if (nearest != -1 || mindist < CellRadius * 16)
						{
							cells[cellnum].position.x += 2.5 * float(cells[nearest].position.x - cells[cellnum].position.x) / mindist;
							cells[cellnum].position.y += 2.5 * float(cells[nearest].position.y - cells[cellnum].position.y) / mindist;
							NeighborsLog(cellnum, Vector2f(
								float(cells[nearest].position.x - cells[cellnum].position.x) / mindist,
								float(cells[nearest].position.y - cells[cellnum].position.y) / mindist));
						}
						else
						{
							nearest = -1; mindist = 999999;
							for (int i = 0; i < comps.size(); i++)
							{
								float len = Length(cells[cellnum].position, comps[i].pos) - ((i + cellnum % 4) % 4 == 0) * CellRadius * 2;
								if (len < mindist)
								{
									mindist = len; nearest = i;
								}
							}
							if (nearest != -1)
							{
								mindist = Length(cells[cellnum].position, comps[nearest].pos);
								cells[cellnum].position.x += 2 * float(comps[nearest].pos.x - cells[cellnum].position.x) / mindist;
								cells[cellnum].position.y += 2 * float(comps[nearest].pos.y - cells[cellnum].position.y) / mindist;
								NeighborsLog(cellnum, Vector2f(
									float(comps[nearest].pos.x - cells[cellnum].position.x) / mindist,
									float(comps[nearest].pos.y - cells[cellnum].position.y) / mindist));
							}
						}
					}
					break;
				case 12:
					if (true)
					{
						int cost[7];
						for (int i = 0; i < 7; i++) cost[i] = 0;
						cost[0] = 1; cost[1] = 1;
						int whatcellhave[7];
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellsHave(cellnum, whatcellhave);
						for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
						allok = true;
						for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
						if (allok)
						{
							PickFromThisCells(cellnum, cost); Give(cellnum, 4);
						}
					}
					break;
				case 13:
					if (true)
					{
						int cost[7];
						for (int i = 0; i < 7; i++) cost[i] = 0;
						cost[0] = 1; cost[2] = 1;
						int whatcellhave[7];
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellsHave(cellnum, whatcellhave);
						for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
						allok = true;
						for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
						if (allok)
						{
							PickFromThisCells(cellnum, cost); Give(cellnum, 5);
						}
					}
					break;
				case 14:
					if (true)
					{
						int cost[7];
						for (int i = 0; i < 7; i++) cost[i] = 0;
						cost[1] = 1; cost[2] = 1;
						int whatcellhave[7];
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellsHave(cellnum, whatcellhave);
						for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
						allok = true;
						for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
						if (allok)
						{
							PickFromThisCells(cellnum, cost); Give(cellnum, 6);
						}
					}
					break;
				case 15:
					if (true)
					{
						int cost[7];
						for (int i = 0; i < 7; i++) cost[i] = 0;
						cost[3] = 1; cost[4] = 1; cost[5] = 1;
						int whatcellhave[7];
						for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
						WhatCellsHave(cellnum, whatcellhave);
						for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
						allok = true;
						for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
						if (allok)
						{
							PickFromThisCells(cellnum, cost); Give(cellnum, 7);
						}
					}
					break;
			}
			for (int i = 0; i < cells.size(); i++)
				if (i != cellnum)
				{
					int iter = 0; float len = Length(cells[cellnum].position, cells[i].position);
					while (len < CellRadius * 2 - 2 && iter < 6)
					{
						Vector2f pastpos = cells[cellnum].position;
						len = Length(cells[cellnum].position, cells[i].position);
						float howmuch = 0.25;
						Vector2f dif = cells[i].position - cells[cellnum].position;
						cells[cellnum].position.x -= howmuch * (dif.x / len);
						cells[cellnum].position.y -= howmuch * (dif.y / len);
						cells[i].position.x += howmuch * (dif.x / len);
						cells[i].position.y += howmuch * (dif.y / len);
						if (Length(cells[cellnum].position, pastpos) > CellRadius * 3) cout << "WTF3";
						iter++;
					}
				}
			for (int i = 0; i < cells[cellnum].connections.size(); i++)
			{
				int iter = 0; float len = Length(cells[cellnum].position, cells[cells[cellnum].connections[i].first].position);
				while (len > CellRadius * 2 + 2 && iter < 6)
				{
					Vector2f pastpos = cells[cellnum].position;
					len = Length(cells[cellnum].position, cells[cells[cellnum].connections[i].first].position);
					float howmuch = 0.25;
					Vector2f dif = cells[cells[cellnum].connections[i].first].position - cells[cellnum].position;
					cells[cellnum].position.x += howmuch * (dif.x / len);
					cells[cellnum].position.y += howmuch * (dif.y / len);
					cells[cells[cellnum].connections[i].first].position.x -= howmuch * (dif.x / len);
					cells[cells[cellnum].connections[i].first].position.y -= howmuch * (dif.y / len);
					if (Length(cells[cellnum].position, pastpos) > CellRadius * 3) cout << "WTF3";
					iter++;
				}
			}
			if (cells[cellnum].numberofgenom != -1)
			{
				for (int i = 0; i < 7; i++) cost[i] = 0;
				CostOfGrew(cellnum, cost);
				for (int i = 0; i < 7; i++) whatcellhave[i] = 0;
				WhatCellsHave(cellnum, whatcellhave);
				for (int i = 0; i < 7; i++) whatcellhave[i] -= cost[i];
				allok = true;
				for (int i = 0; i < 7; i++) if (whatcellhave[i] < 0) allok = false;
				if (allok)
				{
					PickFromThisCells(cellnum, cost);
					for (int u = 0; u < 4; u++)
						if (cells[cellnum].g.gen[cells[cellnum].numberofgenom][u * 3] < GenomSize)
						{
							float resultbornangle = float(cells[cellnum].g.gen[cells[cellnum].numberofgenom][u * 3 + 1]);
							if (cells[cellnum].connections.size() > 0)
								resultbornangle =
									AngleBtw(cells[cells[cellnum].connections[0].first].position, cells[cellnum].position)
									+ cells[cellnum].g.gen[cells[cellnum].numberofgenom][u * 3 + 1];
							Vector2f newcellpos = cells[cellnum].position;
							newcellpos.x = newcellpos.x + 2 * CellRadius *
								cosf(resultbornangle * Pi);
							newcellpos.y = newcellpos.y + 2 * CellRadius *
								sinf(resultbornangle * Pi);
							bool mayborn = true;
							for (int anthrcellnum = 0; anthrcellnum < cells.size(); anthrcellnum++)
								if (Length(newcellpos, cells[anthrcellnum].position) < CellRadius * 2 - 4) mayborn = false;
							if (newcellpos.x < CellRadius || newcellpos.x > MapWidth - CellRadius ||
								newcellpos.y < CellRadius || newcellpos.y > MapHeight - CellRadius) mayborn = false;
							if (mayborn)
							{
								//cout << "CELLBORNED" << endl;
								cells.push_back(Cell(newcellpos, cells[cellnum].g.gen[cells[cellnum].numberofgenom][u * 3],
									cells[cellnum].g, cells[cellnum].g.gen[cells[cellnum].numberofgenom][u * 3 + 2]));
								cells[cellnum].connections.push_back({ int(cells.size() - 1),
									cells[cellnum].g.gen[cells[cellnum].numberofgenom][u * 3 + 1] });
								cells[cells.size() - 1].connections.push_back({ cellnum,
									int(resultbornangle) });
							}
						}
					cells[cellnum].numberofgenom = -1;
				}
			}
		}
		for (int cellnum = int(cells.size()) - 1; cellnum >= 0; cellnum--)
			if (cells[cellnum].age > CellAgeToDie)
			{
				if (cells[cellnum].type == 11 && cells[cellnum].age < CellAgeToDie + 750) 
					cells.push_back(Cell(Vector2f(
						cells[cellnum].position.x + 10 * (rand() % 15 - 7),
						cells[cellnum].position.y + 10 * (rand() % 15 - 7)), 0, cells[cellnum].g, 0, true));
				cells[cellnum].connections.clear();
				for (int i = 0; i < CellStoragevall; i++)
					if (cells[cellnum].storage[i] != 0 && rand() % 2 == 0)
					comps.push_back(Component(
						Vector2f(cells[cellnum].position.x + 2 * (rand() % 15 - 7),
							cells[cellnum].position.y + 2 *(rand() % 15 - 7)),
						cells[cellnum].storage[i]));
				cells.erase(cells.begin() + cellnum);
				for (int i = 0; i < cells.size(); i++) for (int u = 0; u < cells[i].connections.size(); u++)
						if (cells[i].connections[u].first > cellnum) cells[i].connections[u].first--;
			}
	}
}