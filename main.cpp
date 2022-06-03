#include "Global.h"
#include "Cell.h"
#include "Init.h"
#include "BackGroundStuff.h"
#include "DrawCells.h"
#include "CellsLogic.h"
#include "DrawLogComp.h"

int main()
{
	Init();
	BGInit();
	for (int i = 0; i < StartPopulation; i++)
	{
		cells.push_back(Cell(Vector2f(
			rand() % (MapWidth - CellRadius * 4) + CellRadius * 2,
			rand() % (MapHeight - CellRadius * 4) + CellRadius * 2), 0));
		if (cells.size() - 1 <= 1) cells[cells.size() - 1].ExpGenom();
		else if (cells.size() - 1 == 2) cells[cells.size() - 1].ExpGenom2();
		else cells[cells.size() - 1].GenerateGenom();
		cells[cells.size() - 1].storage[0] = 1;
		cells[cells.size() - 1].storage[1] = 2;
		cells[cells.size() - 1].storage[2] = 3;
		cells[cells.size() - 1].storage[3] = 1;
		cells[cells.size() - 1].storage[4] = 2;
		cells[cells.size() - 1].storage[5] = 3;
		cells[cells.size() - 1].storage[6] = 1;
		cells[cells.size() - 1].storage[7] = 2;
		cells[cells.size() - 1].storage[8] = 3;
	}
	//cells.push_back(Cell(Vector2f(MapWidth / 2, MapHeight / 2), 0));
	//cells[cells.size() - 1].ExpGenom3();
	for (int i = 0; i < StartFood; i++) comps.push_back(Component(Vector2f(rand() % MapWidth, rand() % MapHeight), 1));
	window.create(VideoMode(ScreenSize[0], ScreenSize[1]), "Programm", Style::Fullscreen, ContextSettings(0, 0, 8));
	window.setVerticalSyncEnabled(true);
	int Zoom = 1; float TimesGone; Clock clk;
	while (window.isOpen())
	{
		TimesGone = clk.getElapsedTime().asMicroseconds() * 0.001; clk.restart(); 
		if (BGmovecd < BackGroundMovevall) BGmovecd += TimesGone;
		if (mousecd < 300) mousecd += TimesGone;
		UpdateCellsCD += TimesGone; SpawnACompCD += TimesGone;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			if (window.hasFocus())
				if (event.type == sf::Event::MouseWheelScrolled)
				{
					Zoom += event.mouseWheelScroll.delta * 7;
					if (Zoom < -590) Zoom = -590; if (Zoom > 90) Zoom = 90;
					view.setSize(ScreenSize[0] * (100 - Zoom) * 0.01, ScreenSize[1] * (100 - Zoom) * 0.01);
				}
		}
		if (window.hasFocus()) if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
		if (camfollow != -1 && camfollow < cells.size())
			view.setCenter(Vector2f(
				(cells[camfollow].position.x + 2 * view.getCenter().x) / 3,
				(cells[camfollow].position.y + 2 * view.getCenter().y) / 3
			));
		UpdateCells();
		if (window.hasFocus()) ViewUpdate(TimesGone);
		window.setView(view);
		window.clear(Color(20, 20, 85));
		BackGround();
		while (BGmovecd >= BackGroundMovevall) BGmovecd -= BackGroundMovevall;
		UpdateComps();
		DrawCells();
		stringstream ss;
		ss << "Gen: " << gen;
		text.setString(ss.str());
		text.setScale((100 - Zoom) * 0.01, (100 - Zoom) * 0.01);
		text.setPosition(view.getCenter().x - view.getSize().x / 2 + view.getSize().x / 100, 
			view.getCenter().y - view.getSize().y / 2 + view.getSize().x / 100);
		window.draw(text);
		window.display();
		/*
		if (window.hasFocus()) ViewUpdate(TimesGone);
		window.setView(view);
		window.clear(Color(20, 20, 85));
		for (int i = 0; i < 360; i += 30)
		{
			VertexArray curve(LineStrip, 0);
			curve.append(Vertex(Vector2f(i * 10, 0), Color(255, 255, 0, 128)));
			curve.append(Vertex(Vector2f(i * 10 + 20 * cosf(i * 0.01745), 20 * sinf(i * 0.01745)), Color(255, 255, 0, 128)));
			window.draw(curve);
			RectangleShape da(Vector2f(20, 6));
			da.setOrigin(3, 3);
			da.setRotation(AngleBtw(curve[0].position, curve[1].position));
			da.move(i * 10, 0);
			da.setFillColor(Color(255, 255, 255, 120));
			window.draw(da);
		}
		window.display();*/
	}
	return 0;
}
/*
cells[cells.size() - 1].storage[0] = 1;
cells[cells.size() - 1].storage[1] = 2;
cells[cells.size() - 1].storage[2] = 3;
cells[cells.size() - 1].storage[3] = 1;
cells[cells.size() - 1].storage[4] = 2;
cells[cells.size() - 1].storage[5] = 3;*/