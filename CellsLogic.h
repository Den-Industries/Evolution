#pragma once
#include "Global.h"
#include "Cell.h"
#include "Component.h"

// ============================================================
//  Константы поведения
// ============================================================
#ifndef PhysicsIterations
#define PhysicsIterations 6
#endif

namespace clogic
{
	constexpr int SearchRingHead = 40;
	constexpr int SearchRingHunter = 40;
	constexpr int SearchRingPull = 30;
}

// ============================================================
//  Утилиты
// ============================================================
static inline int   isign(int x) { return (x > 0) - (x < 0); }
static inline int   GridW() { return MapWidth / CellDiameter; }
static inline int   GridH() { return MapHeight / CellDiameter; }
static inline float SafeLen(float l) { return l < 0.0001f ? 0.0001f : l; }

// --- stamp-обход вместо whocounted + std::find (проверка за O(1)) ---
// В Global.h: std::vector<int> visitstamp; int curstamp = 0;
static inline void NewVisit()
{
	if (int(visitstamp.size()) < int(cells.size()))
		visitstamp.resize(cells.size() + 64, 0);
	++curstamp;
}
static inline bool Seen(int idx)
{
	if (idx < 0 || idx >= int(visitstamp.size())) return true;
	if (visitstamp[idx] == curstamp) return true;
	visitstamp[idx] = curstamp;
	return false;
}

// ============================================================
//  Свойства организма (обходы графа) — return только при находке,
//  рекурсия продолжает перебор всех соседей.
// ============================================================
bool Protected(int cellnum)
{
	if (cells[cellnum].type == 9) return true;
	for (auto& c : cells[cellnum].connections)
		if (cells[c.first].type == 9) return true;
	return false;
}

bool Feet(int cellnum)
{
	int q = 0;
	for (int i = 0; i < CellStoragevall; ++i)
		q += cells[cellnum].storage[i] != 0;
	return q >= CellStoragevall - 1;
}

bool MayBeHunted(int cellnum)
{
	int q = 0;
	for (int i = 0; i < CellStoragevall; ++i)
		q += cells[cellnum].storage[i] != 0;
	return q > 4;
}

bool ThereIsAngry(int cellnum, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return false;
	if (cells[cellnum].type == 8) return true;
	for (auto& c : cells[cellnum].connections)
		if (ThereIsAngry(c.first, false)) return true;
	return false;
}

bool IsItCreatureChoosed(int cellnum, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return false;
	if (cellnum == camfollow) return true;
	for (auto& c : cells[cellnum].connections)
		if (IsItCreatureChoosed(c.first, false)) return true;
	return false;
}

int HowManyChildren(int cellnum, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return 0;
	int q = (cells[cellnum].type == 11) ? 1 : 0;
	for (auto& c : cells[cellnum].connections)
		q += HowManyChildren(c.first, false);
	return q;
}

int CountSpeedBoost(int cellnum, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return 0;
	int q = (cells[cellnum].type == 20) ? 1 : 0;
	for (auto& c : cells[cellnum].connections)
		q += CountSpeedBoost(c.first, false);
	return q;
}

bool Friend(int cellnum, int othercell, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return false;
	if (cellnum == othercell) return true;
	for (auto& c : cells[cellnum].connections)
		if (Friend(c.first, othercell, false)) return true;
	return false;
}

void UnAlertBoss(int cellnum, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return;
	if (cells[cellnum].type == 0) cells[cellnum].alert = false;
	for (auto& c : cells[cellnum].connections)
		UnAlertBoss(c.first, false);
}

void AgeAllCells(int cellnum, bool seedstoo = false, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return;
	cells[cellnum].age = CellAgeToDie + 500 + (seedstoo) * 500;
	for (auto& c : cells[cellnum].connections)
		AgeAllCells(c.first, seedstoo, false);
}

void NeighborsLog(int cellnum, Vector2f& offset, float modif = 1.0f, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return;
	for (int i = 0; i < int(cells[cellnum].connections.size()); ++i)
	{
		int he = cells[cellnum].connections[i].first;
		if (visitstamp[he] == curstamp) continue;
		cells[he].position += offset;
		if (i != 0)
		{
			float len = SafeLen(Length(cells[he].position, cells[cellnum].position));
			int angle = int(AngleBtw(cells[cells[cellnum].connections[0].first].position,
				cells[cellnum].position) + cells[cellnum].connections[i].second) % 360;
			Vector2f target = cells[cellnum].position;
			target.x += len * cosf(angle * Pi);
			target.y += len * sinf(angle * Pi);
			Vector2f dif = target - cells[he].position;
			cells[he].position.x += min(6.0f * modif, abs(dif.x)) * dif.x / len;
			cells[he].position.y += min(6.0f * modif, abs(dif.y)) * dif.y / len;
		}
		NeighborsLog(he, offset, modif, false);
	}
}

// ============================================================
//  Запасы / стоимость
// ============================================================
void CostOfGrew(int cellnum, int* cost)
{
	auto& gen = cells[cellnum].g.gen[cells[cellnum].numberofgenom];
	for (int u = 0; u < 4; u++)
		if (gen[u * 3] < GenomSize)
		{
			auto& info = CellsInfo[gen[u * 3 + 2]][0];
			for (size_t i = 0; i < info.size(); ++i)
			{
				char ch = info[i];
				if (ch >= 'A' && ch <= 'G') cost[ch - 'A']++;
			}
		}
}

void WhatCellHave(int cellnum, int* whathave)
{
	for (int i = 0; i < CellStoragevall; ++i)
		if (cells[cellnum].storage[i] != 0) whathave[cells[cellnum].storage[i] - 1]++;
}

void WhatCellsHave(int cellnum, int* whathave, int longval = 0, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return;
	WhatCellHave(cellnum, whathave);
	if (longval > 1) return;
	for (auto& c : cells[cellnum].connections)
	{
		int t = cells[c.first].type;
		WhatCellsHave(c.first, whathave, (t == 10 || t == 0) ? 0 : longval + 1, false);
	}
}

void PickFromThisCell(int cellnum, int* whatpick)
{
	for (int i = 0; i < CellStoragevall; ++i)
		if (cells[cellnum].storage[i] != 0 && whatpick[cells[cellnum].storage[i] - 1] > 0)
		{
			whatpick[cells[cellnum].storage[i] - 1]--;
			cells[cellnum].storage[i] = 0;
		}
}

void PickFromThisCells(int cellnum, int* whatpick, int longval = 0, bool start = true)
{
	if (start) NewVisit();
	if (Seen(cellnum)) return;
	PickFromThisCell(cellnum, whatpick);
	if (longval > 1) return;
	for (auto& c : cells[cellnum].connections)
	{
		int t = cells[c.first].type;
		PickFromThisCells(c.first, whatpick, (t == 10 || t == 0) ? 0 : longval + 1, false);
	}
}

bool IsCellsHaveThis(int cellnum, int type, bool start = true)
{
	if (type <= 0 || type >= 8) return false;
	if (start) NewVisit();
	if (Seen(cellnum)) return false;
	for (int i = 0; i < CellStoragevall; ++i)
		if (cells[cellnum].storage[i] == type) return true;
	for (auto& c : cells[cellnum].connections)
		if (IsCellsHaveThis(c.first, type, false)) return true;
	return false;
}

int HowMuchCellHaveThis(int cellnum, int type)
{
	int out = 0;
	for (int i = 0; i < CellStoragevall; ++i)
		if (cells[cellnum].storage[i] == type) out++;
	return out;
}

// Списать ресурсы по организму, при нехватке — ничего не трогаем.
static inline bool TryConsume(int cellnum, int* cost)
{
	int have[7]; clearit(have);
	WhatCellsHave(cellnum, have);
	for (int i = 0; i < 7; ++i) if (have[i] < cost[i]) return false;
	PickFromThisCells(cellnum, cost);
	return true;
}

// ============================================================
//  Поиск ближайшего объекта в сетке (единый, заменяет 4 копипасты)
//  inComps = true -> compgrid/comps, иначе cellgrid/cells.
// ============================================================
int FindNearestInGrid(const Vector2f& pos, bool inComps, int maxRing, float& outDist, int skipCell = -1)
{
	int nearest = -1;
	float mindist = 1e18f;
	int cx = int(pos.x / CellDiameter), cy = int(pos.y / CellDiameter);
	int gw = GridW(), gh = GridH();

	for (int ring = 0; ring < maxRing; ++ring)
	{
		int sides = (ring == 0) ? 1 : 4;
		for (int side = 0; side < sides; ++side)
		{
			int from = (ring == 0) ? 0 : -ring;
			int to = (ring == 0) ? 1 : ring;
			for (int t = from; t < to; ++t)
			{
				int rx, ry;
				switch (side)
				{
				case 0: rx = cx + t;    ry = cy - ring; break;
				case 1: rx = cx + ring; ry = cy + t;    break;
				case 2: rx = cx - t;    ry = cy + ring; break;
				default:rx = cx - ring; ry = cy - t;    break;
				}
				if (rx < 0 || rx > gw || ry < 0 || ry > gh) continue;

				if (inComps)
				{
					auto& cell = compgrid[rx][ry];
					for (int i = 0; i < int(cell.size()); ++i)
					{
						int ci = cell[i];
						if (comps[ci].type == -1) continue;
						float len = Length(pos, comps[ci].pos);
						if (len < mindist) { mindist = len; nearest = ci; }
					}
				}
				else
				{
					auto& cell = cellgrid[rx][ry];
					for (int i = 0; i < int(cell.size()); ++i)
					{
						int ci = cell[i];
						if (ci == skipCell) continue;
						float len = Length(pos, cells[ci].position);
						if (len < mindist) { mindist = len; nearest = ci; }
					}
				}
			}
		}
		if (nearest != -1) break;
	}
	outDist = mindist;
	return nearest;
}

// Двигать клетку к точке + тащить организм.
static inline void MoveTowards(int cellnum, const Vector2f& target, float spb)
{
	float dx = target.x - cells[cellnum].position.x;
	float dy = target.y - cells[cellnum].position.y;
	float d = SafeLen(sqrtf(dx * dx + dy * dy));
	float sp = 2.0f + spb * speedboostval;
	float so = 1.4f + spb * speedboostval;
	cells[cellnum].position.x += sp * dx / d;
	cells[cellnum].position.y += sp * dy / d;
	Vector2f offs(so * dx / d, so * dy / d);
	NeighborsLog(cellnum, offs);
}

static inline void Wander(int cellnum)
{
	Cell& c = cells[cellnum];
	if (c.wondervec.x == 0 && c.wondervec.y == 0)
	{
		float a = (rand() % 360) * 0.01745f;
		c.wondervec.x = cosf(a); c.wondervec.y = sinf(a);
	}
	else
	{
		c.position.x += c.wondervec.x / 3;
		c.position.y += c.wondervec.y / 3;
		Vector2f offs(c.wondervec.x / 6, c.wondervec.y / 6);
		NeighborsLog(cellnum, offs);
		if (rand() % 300 == 0)
		{
			float a = (rand() % 360) * 0.01745f;
			c.wondervec.x = cosf(a); c.wondervec.y = sinf(a);
		}
	}
}

static inline void FleeTemperature(int cellnum, float spb)
{
	float sp = 2.0f + spb * speedboostval;
	float dir = (cells[cellnum].temperature < 0) ? 1.0f : -1.0f;
	cells[cellnum].position.x += dir * sp;
	Vector2f offs(dir * (1.0f + spb * speedboostval), 0.0f);
	NeighborsLog(cellnum, offs);
}

// Простой крафт: списать cost, при успехе выдать выходы (списком).
static inline void Craft(int cellnum, int* cost, std::initializer_list<int> outputs)
{
	if (TryConsume(cellnum, cost))
		for (int o : outputs) Give(cellnum, o);
}

// ============================================================
//  Физика: одна релаксация (без перестройки сетки).
//  isAngryCache[i] — заранее посчитанный флаг "в организме есть хищник".
// ============================================================
void RelaxCellsOnce(const std::vector<char>& isAngryCache)
{
	int gw = GridW(), gh = GridH();
	for (int cellnum = 0; cellnum < int(cells.size()); ++cellnum)
	{
		Cell& c = cells[cellnum];
		c.position.x = min(max(c.position.x, float(CellRadius)), float(MapWidth - CellRadius));
		c.position.y = min(max(c.position.y, float(CellRadius)), float(MapHeight - CellRadius));

		int cx = int(c.position.x / CellDiameter), cy = int(c.position.y / CellDiameter);
		for (int dx = max(cx - 1, 0); dx <= min(cx + 1, gw - 1); ++dx)
			for (int dy = max(cy - 1, 0); dy <= min(cy + 1, gh - 1); ++dy)
			{
				auto& cell = cellgrid[dx][dy];
				for (int i = 0; i < int(cell.size()); ++i)
				{
					int j = cell[i];
					if (j == cellnum) continue;
					float len = Length(c.position, cells[j].position);
					if (len > 0.0001f && len < CellDiameter)
					{
						float magic = (CellDiameter - len) / 2;
						Vector2f dif = cells[j].position - c.position;
						c.position.x -= (dif.x / len) * magic;
						c.position.y -= (dif.y / len) * magic;
						cells[j].position.x += (dif.x / len) * magic;
						cells[j].position.y += (dif.y / len) * magic;
					}
				}
			}

		// связи
		for (auto& con : c.connections)
		{
			int he = con.first;
			float len = Length(c.position, cells[he].position);
			if (len > CellRadius * 2 && len > 0.0001f)
			{
				Vector2f dif = cells[he].position - c.position;
				float magic = (len - CellDiameter) / 2;
				if ((c.type != 0 || isAngryCache[cellnum]) && c.type != 8)
				{
					c.position.x += (dif.x / len) * magic;
					c.position.y += (dif.y / len) * magic;
				}
				if ((cells[he].type != 0 || isAngryCache[he]) && cells[he].type != 8)
				{
					cells[he].position.x -= (dif.x / len) * magic;
					cells[he].position.y -= (dif.y / len) * magic;
				}
			}
		}
	}
}

// Перестройка сетки клеток (один раз перед серией релаксаций).
void RebuildCellGrid()
{
	int gw = GridW(), gh = GridH();
	for (int i = 0; i <= gw; ++i)
		for (int u = 0; u <= gh; ++u) cellgrid[i][u].clear();
	for (int i = int(cells.size()) - 1; i >= 0; --i)
		if (cells[i].position.x >= 0 && cells[i].position.y >= 0 &&
			cells[i].position.x <= MapWidth && cells[i].position.y <= MapHeight)
			cellgrid[int(cells[i].position.x / CellDiameter)]
			[int(cells[i].position.y / CellDiameter)].push_back(i);
}

// Полная физика: сетка строится ОДИН раз, релаксация прогоняется N раз
// (FABRIK-подобный солвер устаканивает связи).
void AllPhisicsCells()
{
	RebuildCellGrid();

	std::vector<char> isAngryCache(cells.size(), 0);
	for (int i = 0; i < int(cells.size()); ++i)
		isAngryCache[i] = ThereIsAngry(i) ? 1 : 0;

	for (int it = 0; it < PhysicsIterations; ++it)
		RelaxCellsOnce(isAngryCache);
}

// ============================================================
//  Рождение клетки из генома (born) — проверка свободного места
//  через сетку, без O(n) перебора всех клеток.
// ============================================================
static bool SpotIsFree(const Vector2f& p)
{
	if (p.x < CellRadius || p.x > MapWidth - CellRadius ||
		p.y < CellRadius || p.y > MapHeight - CellRadius) return false;
	int cx = int(p.x / CellDiameter), cy = int(p.y / CellDiameter);
	int gw = GridW(), gh = GridH();
	for (int dx = max(cx - 1, 0); dx <= min(cx + 1, gw); ++dx)
		for (int dy = max(cy - 1, 0); dy <= min(cy + 1, gh); ++dy)
		{
			auto& cell = cellgrid[dx][dy];
			for (int i = 0; i < int(cell.size()); ++i)
				if (Length(p, cells[cell[i]].position) < CellRadius * 2 - 4) return false;
		}
	return true;
}

void TryBorn(int cellnum)
{
	Cell& c = cells[cellnum];
	if (c.numberofgenom == -1) return;

	int cost[7]; clearit(cost);
	CostOfGrew(cellnum, cost);

	int have[7]; clearit(have);
	WhatCellsHave(cellnum, have);
	for (int i = 0; i < 7; ++i) if (have[i] < cost[i]) return;

	auto& gen = c.g.gen[c.numberofgenom];

	// базовый угол организма
	auto bornAngle = [&](int u) -> float
		{
			float a = float(gen[u * 3 + 1]);
			if (!c.connections.empty())
				a = AngleBtw(cells[c.connections[0].first].position, c.position) + gen[u * 3 + 1];
			return a;
		};
	auto bornPos = [&](int u) -> Vector2f
		{
			float a = bornAngle(u);
			Vector2f p = c.position;
			p.x += 2 * CellRadius * cosf(a * Pi);
			p.y += 2 * CellRadius * sinf(a * Pi);
			return p;
		};

	// проверяем, что ВСЕ почки могут разместиться
	for (int u = 0; u < 4; ++u)
		if (gen[u * 3] < GenomSize && !SpotIsFree(bornPos(u))) return;

	// списываем ресурсы и рождаем
	PickFromThisCells(cellnum, cost);
	for (int u = 0; u < 4; ++u)
		if (gen[u * 3] < GenomSize)
		{
			Vector2f p = bornPos(u);
			float a = bornAngle(u);
			cells.push_back(Cell(p, gen[u * 3], c.g, gen[u * 3 + 2]));
			int newIdx = int(cells.size()) - 1;
			cells[cellnum].connections.push_back({ newIdx, gen[u * 3 + 1] });
			cells[newIdx].connections.push_back({ cellnum, int(a) });
			cells[newIdx].CD += rand() % 40;
		}
	cells[cellnum].numberofgenom = -1;
}

// ============================================================
//  Сбор компонентов из соседних ячеек сетки.
// ============================================================
void CollectComps(int cellnum)
{
	Cell& c = cells[cellnum];
	int cx = int(c.position.x / CellDiameter), cy = int(c.position.y / CellDiameter);
	int gw = GridW(), gh = GridH();
	float reach = CellRadius + (c.type == 0) * CellRadius;
	for (int dx = max(cx - 1, 0); dx <= min(cx + 1, gw); ++dx)
		for (int dy = max(cy - 1, 0); dy <= min(cy + 1, gh); ++dy)
		{
			auto& cell = compgrid[dx][dy];
			for (int i = 0; i < int(cell.size()); ++i)
			{
				int ci = cell[i];
				if (comps[ci].type != -1 && Length(c.position, comps[ci].pos) <= reach)
				{
					Give(cellnum, comps[ci].type);
					comps[ci].type = -1; // помечаем на удаление, чистим позже
				}
			}
		}
}

// ============================================================
//  Передача температуры по связям + проверка alert у боссов/хищников.
// ============================================================
void TransferTemperature(int cellnum, float hot, float cold)
{
	Cell& c = cells[cellnum];
	for (auto& con : c.connections)
	{
		Cell& he = cells[con.first];
		if ((c.type == 0 || c.type == 8) &&
			abs(he.temperature) > 15 && (hot > 0.4f || cold > 0.4f))
			c.alert = true;

		int diff = he.temperature - c.temperature;
		if (diff != 0)
		{
			int skok = min(10, abs(diff)) * isign(diff);
			he.temperature -= skok;
			c.temperature += skok;
		}
	}
}

// ============================================================
//  Основной апдейт мира
// ============================================================
void UpdateCells() // 1A 2B 3C 4D 5E 6F 7G
{
	bool firstcell = true;
	forstat.restart();

	if (UpdateCellsCD > UpdateCellsCDvall * 20)
		UpdateCellsCD = float(UpdateCellsCDvall * 20);

	while (UpdateCellsCD > UpdateCellsCDvall)
	{
		if (GetFps() < 40) UpdateCellsCD = 0;
		else UpdateCellsCD -= UpdateCellsCDvall;

		int gw = GridW(), gh = GridH();

		// --- компактизация мёртвых компонентов (type == -1) ---
		for (int i = int(comps.size()) - 1; i >= 0; --i)
			if (comps[i].type == -1)
			{
				comps[i] = comps.back();
				comps.pop_back();
			}

		// --- построение сеток ---
		for (int i = 0; i <= gw; ++i)
			for (int u = 0; u <= gh; ++u) { compgrid[i][u].clear(); cellgrid[i][u].clear(); }

		for (int i = int(comps.size()) - 1; i >= 0; --i)
			if (comps[i].pos.x > 0 && comps[i].pos.y > 0 &&
				comps[i].pos.x <= MapWidth && comps[i].pos.y <= MapHeight)
				compgrid[int(comps[i].pos.x / CellDiameter)]
				[int(comps[i].pos.y / CellDiameter)].push_back(i);

		for (int i = int(cells.size()) - 1; i >= 0; --i)
			if (cells[i].position.x >= 0 && cells[i].position.y >= 0 &&
				cells[i].position.x <= MapWidth && cells[i].position.y <= MapHeight)
				cellgrid[int(cells[i].position.x / CellDiameter)]
				[int(cells[i].position.y / CellDiameter)].push_back(i);

		// =====================================================
		//  ЛОГИКА КЛЕТОК
		// =====================================================
		tflclock.restart();
		for (int cellnum = 0; cellnum < int(cells.size()); ++cellnum)
		{
			Cell& c = cells[cellnum];
			int cost[7], whatcellhave[7];

			// --- энергопотери ---
			c.energy -= float(LoseEnergyvall *
				(1 - 0.25f * (c.type == 11) - 0.75f * (c.type == 9) + 1.0f * (c.type == 20)));
			c.age++;

			// --- температура от среды ---
			float hot = 4 * (float(max(0, int(c.position.x) - (MapWidth / 3) * 2)) / float(MapWidth / 3)),
				cold = 4 * (float(max(0, (MapWidth / 3) - int(c.position.x))) / float(MapWidth / 3));
			c.temperature += int(hot - cold);
			if (abs(c.temperature) > 40 && (hot > 1.0f || cold > 1.0f)) c.alert = true;
			if (c.temperature > 600 || c.temperature < -600 || c.energy < 0)
				AgeAllCells(cellnum, true);

			// --- кэш свойств организма (раз на клетку, а не по N раз в case) ---
			float spb = float(CountSpeedBoost(cellnum));
			bool  angry = ThereIsAngry(cellnum);

			// --- ГОЛОД ---
			if (c.energy < HungryEnergyvall)
			{
				clearit(whatcellhave);
				WhatCellsHave(cellnum, whatcellhave);

				// приоритет переваривания по ценности (без vector + sort)
				static const int valmul[7] = { 60, 60, 60, 300, 300, 300, 10000 };
				int order[7] = { 0, 1, 2, 3, 4, 5, 6 };
				int score[7];
				for (int i = 0; i < 7; ++i) score[i] = whatcellhave[i] * valmul[i];
				for (int a = 1; a < 7; ++a) // insertion sort по убыванию score
				{
					int key = order[a], i = a - 1;
					while (i >= 0 && score[order[i]] < score[key]) { order[i + 1] = order[i]; --i; }
					order[i + 1] = key;
				}

				bool eaten = false;
				for (int k = 0; k < 7 && !eaten; ++k)
				{
					int comp = order[k];
					bool canDigest =
						find(CellsInfo[c.type][1].begin(), CellsInfo[c.type][1].end(),
							char(comp + 65)) != CellsInfo[c.type][1].end();
					if (canDigest && rand() % 6 <= 3)
					{
						clearit(cost); cost[comp] = 1;
						if (TryConsume(cellnum, cost))
						{
							c.energy += (comp < 3) ? 60 : (comp < 6 ? 200 : 1000);
							eaten = true;
						}
					}
				}

				if (!eaten)
				{
					// усреднение энергии по соседям
					float envall = c.energy; int q = 1;
					for (auto& con : c.connections) { envall += cells[con.first].energy; ++q; }
					envall /= q;
					c.energy = envall;
					for (auto& con : c.connections) cells[con.first].energy = envall;
				}
			}

			// --- ПОВЕДЕНИЕ ПО ТИПУ ---
			switch (c.type)
			{
			case 0: // Башка
				c.CD++;
				if (!c.alert)
				{
					// производство компонентов (не у границ)
					bool atBorder = c.position.x < CellRadius || c.position.x > MapWidth - CellRadius ||
						c.position.y < CellRadius || c.position.y > MapHeight - CellRadius;
					if (!atBorder && c.CD >= 80)
					{
						clearit(whatcellhave);
						WhatCellHave(cellnum, whatcellhave);
						if (whatcellhave[1] < 2) Give(cellnum, 2);
						else if (whatcellhave[2] < 2) Give(cellnum, 3);
						else Give(cellnum, (rand() % 2 == 0) ? 2 : 3);
						c.CD = 0;
					}

					// поиск компонента / блуждание
					if ((!Feet(cellnum) || !IsCellsHaveThis(cellnum, 1)) && !angry)
					{
						float dist;
						int nearest = FindNearestInGrid(c.position, true, clogic::SearchRingHead, dist);
						if (nearest != -1 && nearest < int(comps.size()))
						{
							c.bufnearest = nearest;
							c.bufnearestType = 2;
							MoveTowards(cellnum, comps[nearest].pos, spb);
						}
						else Wander(cellnum);
					}

					// остывание/нагрев к нулю
					if (c.temperature != 0)
						c.temperature -= min(3, abs(c.temperature)) * isign(c.temperature);
				}
				else
				{
					if (abs(c.temperature) < 30 || (hot < 1.5f && cold < 1.5f)) c.alert = false;
					FleeTemperature(cellnum, spb);
				}
				break;

			case 1: // Синие -> B
				if (!Feet(cellnum)) c.CD++;
				if (c.CD >= 35) { Give(cellnum, 2); c.CD = 0; }
				break;

			case 2: // Красные -> C
				if (!Feet(cellnum)) c.CD++;
				if (c.CD >= 35) { Give(cellnum, 3); c.CD = 0; }
				break;

			case 3: // D + E -> ...
				if (!Feet(cellnum)) c.CD++; else c.CD += rand() % 2;
				if (c.CD >= 40)
				{
					c.CD = 0;
					clearit(cost); cost[3] = 1; cost[4] = 1;
					Craft(cellnum, cost, { 1, 1, 2, 3, 1 + rand() % 3, 1 + rand() % 3 });
				}
				break;

			case 4: // F + D -> ...
				if (!Feet(cellnum)) c.CD++; else c.CD += rand() % 2;
				if (c.CD >= 40)
				{
					c.CD = 0;
					clearit(cost); cost[3] = 1; cost[5] = 1;
					Craft(cellnum, cost, { 2, 2, 1, 3, 1 + rand() % 3, 1 + rand() % 3 });
				}
				break;

			case 5: // E + F -> ...
				if (!Feet(cellnum)) c.CD++; else c.CD += rand() % 2;
				if (c.CD >= 40)
				{
					c.CD = 0;
					clearit(cost); cost[4] = 1; cost[5] = 1;
					Craft(cellnum, cost, { 3, 3, 2, 1, 1 + rand() % 3, 1 + rand() % 3 });
				}
				break;

			case 6: // G -> ...
				if (!Feet(cellnum)) c.CD++; else c.CD += rand() % 2;
				if (c.CD >= 30)
				{
					c.CD = 0;
					clearit(cost); cost[6] = 1;
					Craft(cellnum, cost, { 4, 4, 5, 5, 6, 6 });
				}
				break;

			case 7: // Притягивает компоненты
			{
				float dist;
				int nearest = FindNearestInGrid(c.position, true, clogic::SearchRingPull, dist);
				if (nearest != -1 && nearest < int(comps.size()))
				{
					c.bufnearest = nearest;
					c.bufnearestType = 2;
					float d = SafeLen(Length(c.position, comps[nearest].pos));
					comps[nearest].pos.x -= 4 * (comps[nearest].pos.x - c.position.x) / d;
					comps[nearest].pos.y -= 4 * (comps[nearest].pos.y - c.position.y) / d;
				}
				break;
			}

			case 8: // Хищник
				if (!c.alert)
				{
					// поедание соседних незащищённых не-друзей
					int cx = int(c.position.x / CellDiameter), cy = int(c.position.y / CellDiameter);
					for (int dx = max(cx - 1, 0); dx <= min(cx + 1, gw); ++dx)
						for (int dy = max(cy - 1, 0); dy <= min(cy + 1, gh); ++dy)
						{
							auto& cell = cellgrid[dx][dy];
							for (int i = 0; i < int(cell.size()); ++i)
							{
								int victim = cell[i];
								if (victim == cellnum || Protected(victim)) continue;
								if (Length(c.position, cells[victim].position) > CellDiameter) continue;
								if (Friend(cellnum, victim)) continue;

								for (int comp = 0; comp < 7; ++comp)
								{
									clearit(cost); cost[comp] = 1;
									if (TryConsume(victim, cost)) Give(cellnum, comp + 1);
								}
							}
						}

					// движение к добыче / еде / блуждание
					if (!Feet(cellnum))
					{
						float dist;
						int prey = FindNearestInGrid(c.position, false, clogic::SearchRingHunter, dist, cellnum);
						if (prey != -1 && prey < int(cells.size()))
						{
							c.bufnearest = prey; c.bufnearestType = 1;
							MoveTowards(cellnum, cells[prey].position, spb);
						}
						else
						{
							int comp = FindNearestInGrid(c.position, true, clogic::SearchRingHunter, dist);
							if (comp != -1 && comp < int(comps.size()))
							{
								c.bufnearest = comp; c.bufnearestType = 2;
								MoveTowards(cellnum, comps[comp].pos, spb);
							}
							else Wander(cellnum);
						}
					}
				}
				else
				{
					FleeTemperature(cellnum, spb);
				}
				break;

				// 9 - защита, 10 - провод, 11 - плод (без активной логики)

			case 12: // -> D
				if (!Feet(cellnum)) c.CD++;
				if (c.CD >= 30 + (HowMuchCellHaveThis(cellnum, 4) > 5) * 15)
				{
					c.CD = 0;
					clearit(cost); cost[0] = 1; cost[1] = 1;
					Craft(cellnum, cost, { 4 });
				}
				break;

			case 13: // -> E
				if (!Feet(cellnum)) c.CD++;
				if (c.CD >= 30 + (HowMuchCellHaveThis(cellnum, 5) > 5) * 20)
				{
					c.CD = 0;
					clearit(cost); cost[0] = 1; cost[2] = 1;
					Craft(cellnum, cost, { 5 });
				}
				break;

			case 14: // -> F
				if (!Feet(cellnum)) c.CD++;
				if (c.CD >= 30 + (HowMuchCellHaveThis(cellnum, 6) > 5) * 20)
				{
					c.CD = 0;
					clearit(cost); cost[1] = 1; cost[2] = 1;
					Craft(cellnum, cost, { 6 });
				}
				break;

			case 15: // -> G
				if (!Feet(cellnum)) c.CD++;
				if (c.CD >= 35 + (HowMuchCellHaveThis(cellnum, 7) > 5) * 20)
				{
					c.CD = 0;
					clearit(cost); cost[3] = 1; cost[4] = 1; cost[5] = 1;
					Craft(cellnum, cost, { 7 });
				}
				break;

			case 16: // Делает зелёные (A)
			{
				int hasA = HowMuchCellHaveThis(cellnum, 1);
				if (!Feet(cellnum)) c.CD++;
				if (c.CD >= 45 + (hasA > 5) * 15 + (hasA > 0) * 15)
				{
					c.CD = 0;
					clearit(cost); cost[1] = 1; cost[2] = 1;
					Craft(cellnum, cost, { 1 });
				}
				break;
			}

			case 17: // охлаждает к нулю быстро
				if (c.temperature != 0)
					c.temperature -= min(10, abs(c.temperature)) * isign(c.temperature);
				break;

			case 18: // греет
				if (c.temperature < 0)
					c.temperature += min(40, abs(c.temperature));
				break;

			case 19: // студит
				if (c.temperature > 0)
					c.temperature -= min(40, abs(c.temperature));
				break;

			case 20: // Speed (пассив)
				break;
			}
			if (firstcell) updatestat(0);

			// --- передача температуры по связям + alert ---
			TransferTemperature(cellnum, hot, cold);
			if (firstcell) updatestat(1);

			// --- рождение ---
			TryBorn(cellnum);
			if (firstcell) updatestat(2);

			// --- сбор компонентов ---
			CollectComps(cellnum);

			firstcell = false;
		}
		timeforlogic[0] = tflclock.getElapsedTime().asMicroseconds() * 0.001f;

		// =====================================================
		//  СТАРЕНИЕ / СМЕРТЬ / РАЗМНОЖЕНИЕ ПЛОДОВ
		// =====================================================
		tflclock.restart();

		// клетки достигшие порога смерти убивают весь организм
		for (int cellnum = 0; cellnum < int(cells.size()); ++cellnum)
			if (cells[cellnum].age == CellAgeToDie)
			{
				if (cellnum == 0) gen++;
				AgeAllCells(cellnum);
			}

		// плоды порождают новый организм (семя)
		for (int cellnum = int(cells.size()) - 1; cellnum >= 0; --cellnum)
			if (cells[cellnum].age > CellAgeToDie &&
				cells[cellnum].type == 11 && cells[cellnum].age < CellAgeToDie + 750)
			{
				int hmc = HowManyChildren(cellnum);
				if (hmc <= 0) continue;

				int whatcellhave[7]; clearit(whatcellhave);
				WhatCellsHave(cellnum, whatcellhave);

				cells.push_back(Cell(Vector2f(
					cells[cellnum].position.x + 6 * (rand() % 15 - 7),
					cells[cellnum].position.y + 6 * (rand() % 15 - 7)),
					0, cells[cellnum].g, 0, cells[cellnum].temperature, true));
				int buf = int(cells.size()) - 1;

				for (int g = 0; g < 3; ++g)
					for (int i = 0; i < 7; ++i)
						for (int u = 0; u < whatcellhave[i] / hmc; u += 3) Give(buf, i + 1);

				int taken[7]; clearit(taken);
				WhatCellHave(buf, taken);
				PickFromThisCells(cellnum, taken);
			}

		// фактическое удаление мёртвых + выброс ресурсов + починка индексов
		for (int cellnum = int(cells.size()) - 1; cellnum >= 0; --cellnum)
			if (cells[cellnum].age > CellAgeToDie)
			{
				cells[cellnum].connections.clear();

				for (int i = 0; i < CellStoragevall; ++i)
					if (cells[cellnum].storage[i] != 0 && rand() % 4 <= 2)
						comps.push_back(Component(
							Vector2f(
								min(max(cells[cellnum].position.x + 15 * (rand() % 15 - 7), float(CellRadius)), float(MapWidth - CellRadius)),
								min(max(cells[cellnum].position.y + 15 * (rand() % 15 - 7), float(CellRadius)), float(MapHeight - CellRadius))),
							cells[cellnum].storage[i]));

				cells.erase(cells.begin() + cellnum);

				// чиним индексы связей и кэш bufnearest у всех уцелевших клеток
				for (int i = 0; i < int(cells.size()); ++i)
				{
					for (int u = 0; u < int(cells[i].connections.size()); ++u)
						if (cells[i].connections[u].first > cellnum)
							cells[i].connections[u].first--;

					if (cells[i].bufnearestType == 1) // кэш указывает на клетку
					{
						if (cells[i].bufnearest == cellnum)
						{
							cells[i].bufnearest = -1; cells[i].bufnearestType = 0;
						}
						else if (cells[i].bufnearest > cellnum)
							cells[i].bufnearest--;
					}
				}
			}
		timeforlogic[1] = tflclock.getElapsedTime().asMicroseconds() * 0.001f;

		// =====================================================
		//  ФИЗИКА (сетка строится 1 раз, релаксация прогоняется N раз)
		// =====================================================
		tflclock.restart();
		AllPhisicsCells();
		timeforlogic[2] = tflclock.getElapsedTime().asMicroseconds() * 0.001f;
	}
}