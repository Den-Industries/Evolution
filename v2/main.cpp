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
			float(rand() % (MapWidth - CellRadius * 4) + CellRadius * 2),
			float(rand() % (MapHeight - CellRadius * 4) + CellRadius * 2)), 0));
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
	for (int i = 0; i < StartFood; i++) comps.push_back(Component(Vector2f(float(rand() % MapWidth), float(rand() % MapHeight)), 1));
	window.create(VideoMode(ScreenSize[0], ScreenSize[1]), "Programm", Style::Fullscreen, ContextSettings(0, 0, 8));
	window.setVerticalSyncEnabled(true);
	float Zoom = 0, TimesGone; 
	Clock clk, alltimeclk; 
	while (window.isOpen())
	{
		TimesGone = clk.getElapsedTime().asMicroseconds() * 0.001f; clk.restart(); 
		if (fpscounter % 2 == 0) fpsss[fpscounter / 2] = (1000.0 / TimesGone);
		fpscounter = (fpscounter + 1) % (FpsQ * 2);
		if (BGmovecd < BackGroundMovevall) BGmovecd += TimesGone;
		if (mousecd < 300) mousecd += TimesGone;
		UpdateCellsCD += TimesGone; SpawnACompCD += TimesGone;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			if (window.hasFocus())
			{
				if (event.type == sf::Event::MouseWheelScrolled)
				{
					Zoom += event.mouseWheelScroll.delta * 7; if (Zoom < -590) Zoom = -590; if (Zoom > 90) Zoom = 90;
					view.setSize(ScreenSize[0] * (100 - Zoom) * 0.01f, ScreenSize[1] * (100 - Zoom) * 0.01f);
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && mousecd >= 300)
				{
					mousecd = 0; camfollow = picked;
				}
			}
		}
		if (window.hasFocus()) if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
		if (camfollow != -1 && camfollow < cells.size())
			view.setCenter(Vector2f(
				(cells[camfollow].position.x + 12 * view.getCenter().x) / 13.f,
				(cells[camfollow].position.y + 12 * view.getCenter().y) / 13.f
			));
		UpdateCells();
		if (window.hasFocus()) ViewUpdate(TimesGone);
		window.setView(view);
		window.clear(Color(20, 20, 85));
		if (window.hasFocus()) BackGround();
		while (BGmovecd >= BackGroundMovevall) BGmovecd -= BackGroundMovevall;
		LogComps();
		if (window.hasFocus()) DrawComps();
		if (window.hasFocus()) DrawCells();
		stringstream ss;
		ss << "Gen: " << gen << "\nTime: " << int(alltimeclk.getElapsedTime().asSeconds()) << " s\nFps" << GetFps();
		text.setString(ss.str());
		text.setScale((100 - Zoom) * 0.01f, (100 - Zoom) * 0.01f);
		text.setPosition(view.getCenter().x - view.getSize().x / 2 + view.getSize().x / 100, 
			view.getCenter().y - view.getSize().y / 2 + view.getSize().x / 100);
		window.draw(text);
		window.display();
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