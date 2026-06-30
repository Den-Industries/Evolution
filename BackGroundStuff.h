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
	// рамку рисуем пр€моугольниками (gRects). center = середина полоски.
	Color f(85, 165, 210, 140);
	// верх
	gRects.add(MapWidth / 2.f, 0.f, MapWidth / 2.f, 2.f, 0, f);
	// низ
	gRects.add(MapWidth / 2.f, MapHeight - 0.f, MapWidth / 2.f, 2.f, 0, f);
	// лево
	gRects.add(0.f, MapHeight / 2.f, 2.f, MapHeight / 2.f, 0, f);
	// право
	gRects.add(MapWidth - 0.f, MapHeight / 2.f, 2.f, MapHeight / 2.f, 0, f);
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

			float radius = float(8 + (i % 3) * 4);
			int hot = int(float(max(0, int(BGDots[i].x) - (MapWidth / 3) * 2)) / float((MapWidth / 3))) * 600,
				cold = int(float(max(0, (MapWidth / 3) - int(BGDots[i].x))) / float((MapWidth / 3))) * 600,
				pos = int(abs(BGDots[i].x - MapWidth / 2) + abs(BGDots[i].y - MapHeight / 2)) % 100;
			Color fill(
				min(max(int(100 + i % 40 + hot - cold), 0), 255),
				min(max(int(140 + pos - hot / 3), 0), 255),
				min(max(int(100 + i % 40 - hot + cold), 0), 255), 35);

			// несколько колец обводки (как было u=1..i%3)
			for (int u = 1; u < 1 + i % 3; u++)
			{
				float ot = float(u) * 4;
				gCircles.add(BGDots[i].x, BGDots[i].y, radius, fill, ot,
					Color(fill.r, fill.g, fill.b, 20));
			}
			// сам кружок (если i%3==0, обводок не было Ч рисуем заливку)
			if (i % 3 == 0)
				gCircles.add(BGDots[i].x, BGDots[i].y, radius, fill);
		}
	}
}