#pragma once
#include "Global.h"
#include "Cell.h"
#include "CellsLogic.h"

void DrawCells()
{
	for (int i = 0; i < cells.size(); ++i)
		if (cells[i].position.x > view.getCenter().x - view.getSize().x / 2 - CellRadius * 2 && cells[i].position.x < view.getCenter().x + view.getSize().x / 2 + CellRadius * 2 &&
			cells[i].position.y > view.getCenter().y - view.getSize().y / 2 - CellRadius * 2 && cells[i].position.y < view.getCenter().y + view.getSize().y / 2 + CellRadius * 2)
		{
			float px = cells[i].position.x, py = cells[i].position.y;
			Color fillCol, outlineCol;
			int clr1 = 22, clr2 = 55;
			float rotatecomp = float(cells[i].age % 30) * 12;
			int need2 = abs(((cells[i].age % 60) * 6) - 180);

			// флагелла-хелпер (жгутики через gRects)
			// в оригинале origin был у кра€: setOrigin(CellRadius/12, h/2)
			// пр€моугольник длиной L, толщиной T. ÷ентр смещЄн на (L/2 - originX) вдоль угла.
			auto flagellum = [&](float L, float T, float deg, const Color& c)
				{
					float ox = CellRadius / 12.f;            // origin по X (как в оригинале)
					float rad = deg * 0.01745329f;
					// позици€ левого кра€ = заданна€ точка attach; центр = attach + (L/2 - ox) вдоль угла
					float halfShift = L / 2.f - ox;
					// attach координата задаЄтс€ отдельно при вызове
					// здесь возвращаем только параметры Ч см. ниже
					(void)halfShift; (void)rad; (void)c; (void)T;
				};

			switch (cells[i].type)
			{
			case 0:
				fillCol = Color(255, 255, 255, clr1); outlineCol = Color(255, 255, 225, clr2);
				for (int u = 0; u < 6; u++)
				{
					float ax = px + CellRadius * cosf(float(u) * 60.0f * Pi);
					float ay = py + CellRadius * sinf(float(u) * 60.0f * Pi);
					float deg = float(u) * 60.0f + ((abs((cells[i].age % 30) - 15) - 7.5f) / 7.5f) * 15;
					float L = CellRadius / 2.f, T = CellRadius / 8.f, ox = CellRadius / 12.f;
					float rad = deg * 0.01745329f;
					float ccx = ax + (L / 2.f - ox) * cosf(rad);
					float ccy = ay + (L / 2.f - ox) * sinf(rad);
					gRects.add(ccx, ccy, L / 2.f, T / 2.f, deg, Color(255, 255, 255, 100));
				}
				break;
			case 1:
				fillCol = Color(127, 127, 255, clr1); outlineCol = Color(127, 127, 225, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(0, 0, 255, 100), 0.5f, Color(0, 0, 255, 50));
				break;
			case 2:
				fillCol = Color(255, 127, 127, clr1); outlineCol = Color(255, 127, 127, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(255, 0, 0, 100), 0.5f, Color(255, 0, 0, 50));
				break;
			case 3:
				fillCol = Color(0, 127, 127, clr1); outlineCol = Color(127, 127, 0, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(0, 255, 255, 100), 0.5f, Color(0, 255, 255, 50));
				gCircles.add(px + CellRadius * cosf((rotatecomp + 180) * 60.0f * Pi), py + CellRadius * sinf((rotatecomp + 180) * 60.0f * Pi),
					CellRadius / 6.f, Color(255, 255, 0, 100), 0.5f, Color(255, 255, 0, 50));
				break;
			case 4:
				fillCol = Color(127, 0, 127, clr1); outlineCol = Color(0, 127, 127, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(255, 0, 255, 100), 0.5f, Color(255, 0, 255, 50));
				gCircles.add(px + CellRadius * cosf((rotatecomp + 180) * Pi), py + CellRadius * sinf((rotatecomp + 180) * Pi),
					CellRadius / 6.f, Color(0, 255, 255, 100), 0.5f, Color(0, 255, 255, 50));
				break;
			case 5:
				fillCol = Color(127, 127, 0, clr1); outlineCol = Color(127, 0, 127, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(255, 255, 0, 100), 0.5f, Color(255, 255, 0, 50));
				gCircles.add(px + CellRadius * cosf((rotatecomp + 180) * Pi), py + CellRadius * sinf((rotatecomp + 180) * Pi),
					CellRadius / 6.f, Color(255, 0, 255, 100), 0.5f, Color(255, 0, 255, 50));
				break;
			case 6:
				fillCol = Color(127, 127, 127, clr1); outlineCol = Color(255, 255, 225, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(255, 255, 255, 100), 0.5f, Color(255, 255, 255, 50));
				gCircles.add(px + CellRadius * cosf((rotatecomp + 120) * Pi), py + CellRadius * sinf((rotatecomp + 120) * Pi),
					CellRadius / 6.f, Color(255, 255, 255, 100), 0.5f, Color(255, 255, 255, 50));
				gCircles.add(px + CellRadius * cosf((rotatecomp + 240) * Pi), py + CellRadius * sinf((rotatecomp + 240) * Pi),
					CellRadius / 6.f, Color(255, 255, 255, 100), 0.5f, Color(255, 255, 255, 50));
				break;
			case 7:
				fillCol = Color(127, 255, 127, clr1); outlineCol = Color(127, 255, 127, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(0, 0, 255, 100), 0.5f, Color(0, 0, 255, 50));
				break;
			case 8:
				fillCol = Color(255, 0, 0, clr1); outlineCol = Color(255, 0, 0, clr2);
				for (int u = 0; u < 6; u++)
				{
					float ax = px + CellRadius * cosf(float(u) * 60.0f * Pi);
					float ay = py + CellRadius * sinf(float(u) * 60.0f * Pi);
					float deg = float(u) * 60.0f + ((abs((cells[i].age % 30) - 15) - 7.5f) / 7.5f) * 15;
					float L = CellRadius, T = CellRadius / 8.f, ox = CellRadius / 12.f;
					float rad = deg * 0.01745329f;
					float ccx = ax + (L / 2.f - ox) * cosf(rad);
					float ccy = ay + (L / 2.f - ox) * sinf(rad);
					gRects.add(ccx, ccy, L / 2.f, T / 2.f, deg, Color(255, 0, 0, 100));
				}
				break;
			case 9:
				fillCol = Color(0, 255, 0, clr1); outlineCol = Color(0, 255, 0, clr2);
				for (int u = 0; u < 6; u++)
				{
					float ax = px + CellRadius * cosf(float(u) * 60.0f * Pi);
					float ay = py + CellRadius * sinf(float(u) * 60.0f * Pi);
					float deg = float(u) * 60.0f + ((abs((cells[i].age % 30) - 15) - 7.5f) / 7.5f) * 15;
					float L = CellRadius / 2.f, T = CellRadius / 2.f, ox = CellRadius / 12.f;
					float rad = deg * 0.01745329f;
					float ccx = ax + (L / 2.f - ox) * cosf(rad);
					float ccy = ay + (L / 2.f - ox) * sinf(rad);
					gRects.add(ccx, ccy, L / 2.f, T / 2.f, deg, Color(125, 0, 180, 180));
				}
				break;
			case 10:
				fillCol = Color(0, 127, 127, clr1); outlineCol = Color(75, 85, 160, clr2);
				break;
			case 11:
				fillCol = Color(0, 0, 0, clr1); outlineCol = Color(255, 255, 255, clr2);
				break;
			case 12:
				fillCol = Color(0, 255, 255, clr1); outlineCol = Color(0, 255, 225, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(0, 255, 255, 100), 0.5f, Color(0, 255, 255, 50));
				break;
			case 13:
				fillCol = Color(255, 255, 0, clr1); outlineCol = Color(255, 255, 0, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(255, 255, 0, 100), 0.5f, Color(255, 255, 0, 50));
				break;
			case 14:
				fillCol = Color(255, 0, 255, clr1); outlineCol = Color(255, 0, 225, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(255, 0, 255, 100), 0.5f, Color(255, 0, 255, 50));
				break;
			case 15:
				fillCol = Color(255, 255, 255, clr1); outlineCol = Color(127, 127, 127, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(255, 255, 255, 100), 0.5f, Color(255, 255, 255, 50));
				break;
			case 16:
				fillCol = Color(47, 125, 47, clr1); outlineCol = Color(127, 188, 127, clr2);
				gCircles.add(px + CellRadius * cosf(rotatecomp * Pi), py + CellRadius * sinf(rotatecomp * Pi),
					CellRadius / 6.f, Color(0, 255, 0, 100), 0.5f, Color(0, 255, 0, 50));
				break;
			case 17:
				clr1 = 35; clr2 = 65;
				fillCol = Color(need2 + 75, 100, 180 - need2 + 65, clr1); outlineCol = Color(180 - need2 + 75, 100, need2 + 65, clr2);
				break;
			case 18:
				clr1 = 35; clr2 = 65;
				fillCol = Color(need2 + 75, 75, 0, clr1); outlineCol = Color(need2 + 75, 75, 0, clr2);
				break;
			case 19:
				clr1 = 35; clr2 = 65;
				fillCol = Color(0, need2 / 2 + 95, need2 + 65, clr1); outlineCol = Color(0, need2 / 2 + 95, need2 + 65, clr2);
				break;
			case 20:
				fillCol = Color(255, 200, 255, clr1); outlineCol = Color(255, 200, 255, clr2);
				for (int u = 0; u < 6; u++)
				{
					float ax = px + CellRadius * cosf(float(u) * 60.0f * Pi);
					float ay = py + CellRadius * sinf(float(u) * 60.0f * Pi);
					float deg = float(u) * 60.0f + ((abs((cells[i].age % 30) - 15) - 7.5f) / 7.5f) * 15;
					float L = CellRadius, T = CellRadius / 8.f, ox = CellRadius / 12.f;
					float rad = deg * 0.01745329f;
					float ccx = ax + (L / 2.f - ox) * cosf(rad);
					float ccy = ay + (L / 2.f - ox) * sinf(rad);
					gRects.add(ccx, ccy, L / 2.f, T / 2.f, deg, Color(255, 200, 255, 100));
				}
				break;
			default:
				fillCol = Color(255, 255, 255, clr1); outlineCol = Color(255, 255, 255, clr2);
				break;
			}

			// температурное кольцо
			if (viewtemp)
			{
				Color tf(
					min(max(cells[i].temperature / 2, 0), 255),
					min(max(-(abs(cells[i].temperature / 2) - 300), 0) * 2, 255),
					min(max(-cells[i].temperature / 2, 0), 255), 100);
				// был CircleShape r=CellRadius*2 + outline=CellRadius
				gCircles.add(px, py, CellRadius * 2.f, tf, CellRadius,
					Color(tf.r, tf.g, tf.b, 50));
			}

			// выделение
			if (i == camfollow)
				gCircles.add(px, py, float(CellDiameter), Color(255, 255, 255, 50));
			else if (IsItCreatureChoosed(i))
				gCircles.add(px, py, CellDiameter * 0.75f, Color(255, 255, 255, 40));

			// тело клетки: 3 обводки (u=-1..1 толщина u*2) + сама заливка
			// объедин€ем в один круг с обводкой 2 (внешний контур) Ч визуально эквивалентно
			gCircles.add(px, py, float(CellRadius), fillCol, 2.f, outlineCol);

			// центральна€ точка энергии
			int e = min(int(cells[i].energy), 255);
			gCircles.add(px, py, CellRadius / 6.f, Color(e, e, e, 255));

			// возраст
			if (cells[i].type == 0 && viewage)
				for (int u = 0; u < (cells[i].age / 100); u++)
					gCircles.add(
						px + (CellRadius * 0.75f) * cosf((float(u) / float(CellAgeToDie / 100)) * 6.28f),
						py + (CellRadius * 0.75f) * sinf((float(u) / float(CellAgeToDie / 100)) * 6.28f),
						float(CellRadius) / 12.f, Color(255, 255, 255, 175));

			// св€зи
			for (int u = 0; u < cells[i].connections.size(); u++)
			{
				float len = Length(cells[cells[i].connections[u].first].position, cells[i].position);
				if (len < 0.001f) len = 0.001f;
				gCircles.add(
					px + 7 * float(cells[cells[i].connections[u].first].position.x - px) / len,
					py + 7 * float(cells[cells[i].connections[u].first].position.y - py) / len,
					CellRadius / 4.f,
					Color(255 - cells[i].numberofgenom, cells[i].numberofgenom, 255, 50),
					1.f, Color(255 - cells[i].numberofgenom, cells[i].numberofgenom, 255, 20));
			}

			// хранилище
			if (viewstor)
				for (int u = 0; u < CellStoragevall; u++)
					if (cells[i].storage[u] != 0)
					{
						Color sc;
						switch (cells[i].storage[u])
						{
						case 1: sc = Color(0, 255, 0, 50); break;
						case 2: sc = Color(0, 0, 255, 50); break;
						case 3: sc = Color(255, 0, 0, 50); break;
						case 4: sc = Color(0, 255, 255, 50); break;
						case 5: sc = Color(255, 255, 0, 50); break;
						case 6: sc = Color(255, 0, 255, 50); break;
						case 7: sc = Color(255, 255, 255, 50); break;
						default: sc = Color(255, 255, 255, 50); break;
						}
						gCircles.add(
							px + 5 * cosf((float(u) / float(CellStoragevall)) * 6.28f),
							py + 5 * sinf((float(u) / float(CellStoragevall)) * 6.28f),
							CellRadius / 8.f, sc, 0.1f, sc);
					}
		}
}