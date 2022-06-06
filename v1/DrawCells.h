#pragma once
#include "Global.h"
#include "Cell.h"

void DrawCells()
{
	if (Mouse::isButtonPressed(sf::Mouse::Left) && mousecd >= 300)
	{
		camfollow = -1; mousecd = 0;
	}
	for (int i = 0; i < cells.size(); i++)
		if (cells[i].position.x > view.getCenter().x - view.getSize().x / 2 - CellRadius * 2 &&
			cells[i].position.x < view.getCenter().x + view.getSize().x / 2 + CellRadius * 2 &&
			cells[i].position.y > view.getCenter().y - view.getSize().y / 2 - CellRadius * 2 &&
			cells[i].position.y < view.getCenter().y + view.getSize().y / 2 + CellRadius * 2)
		{
			CircleShape cellshape(CellRadius);
			cellshape.setOrigin(CellRadius, CellRadius);
			cellshape.setPosition(cells[i].position);
			int clr1 = 20, clr2 = 55;
			int rotatecomp = (cells[i].age % 30) * 12;
			switch (cells[i].type)
			{
			case 0:
				cellshape.setFillColor(Color(255, 255, 255, clr1));
				cellshape.setOutlineColor(Color(255, 255, 225, clr2));
				if (true)
				{
					RectangleShape flagellumshape(Vector2f(CellRadius / 2, CellRadius / 8));
					flagellumshape.setOrigin(CellRadius / 12, CellRadius / 16);
					for (int u = 0; u < 6; u++)
					{
						flagellumshape.setPosition(
							cells[i].position.x + CellRadius * cos(float(u) * 60.0 * 0.01745),
							cells[i].position.y + CellRadius * sin(float(u) * 60.0 * 0.01745));
						flagellumshape.setRotation(float(u) * 60.0 + 
							((abs((cells[i].age % 30) - 15) - 7.5) / 7.5) * 15
						);
						flagellumshape.setFillColor(Color(255, 255, 255, 100));
						flagellumshape.setOutlineThickness(0.4);
						flagellumshape.setOutlineColor(Color(255, 255, 255, 50));
						window.draw(flagellumshape);
					}
				}
				break;
			case 1:
				cellshape.setFillColor(Color(127, 127, 255, clr1));
				cellshape.setOutlineColor(Color(127, 127, 225, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(0, 0, 255, 100));
					mark.setOutlineColor(Color(0, 0, 255, 50));
					window.draw(mark);
				}
				break;
			case 2:
				cellshape.setFillColor(Color(255, 127, 127, clr1));
				cellshape.setOutlineColor(Color(255, 127, 127, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(255, 0, 0, 100));
					mark.setOutlineColor(Color(255, 0, 0, 50));
					window.draw(mark);
				}
				break;
			case 3:
				cellshape.setFillColor(Color(0, 127, 127, clr1));
				cellshape.setOutlineColor(Color(127, 127, 0, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(0, 255, 255, 100));
					mark.setOutlineColor(Color(0, 255, 255, 50));
					window.draw(mark);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp + 180 * 60.0 * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp + 180 * 60.0 * 0.01745));
					mark.setFillColor(Color(255, 255, 0, 100));
					mark.setOutlineColor(Color(255, 255, 0, 50));
					window.draw(mark);
				}
				break;
			case 4:
				cellshape.setFillColor(Color(127, 0, 127, clr1));
				cellshape.setOutlineColor(Color(0, 127, 127, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(255, 0, 255, 100));
					mark.setOutlineColor(Color(255, 0, 255, 50));
					window.draw(mark);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos((rotatecomp + 180) * 0.01745),
						cells[i].position.y + CellRadius * sin((rotatecomp + 180) * 0.01745));
					mark.setFillColor(Color(0, 255, 255, 100));
					mark.setOutlineColor(Color(0, 255, 255, 50));
					window.draw(mark);
				}
				break;
			case 5:
				cellshape.setFillColor(Color(127, 127, 0, clr1));
				cellshape.setOutlineColor(Color(127, 0, 127, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(255, 255, 0, 100));
					mark.setOutlineColor(Color(255, 255, 0, 50));
					window.draw(mark);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos((rotatecomp + 180) * 0.01745),
						cells[i].position.y + CellRadius * sin((rotatecomp + 180) * 0.01745));
					mark.setFillColor(Color(255, 0, 255, 100));
					mark.setOutlineColor(Color(255, 0, 255, 50));
					window.draw(mark);
				}
				break;
			case 6:
				cellshape.setFillColor(Color(127, 127, 127, clr1));
				cellshape.setOutlineColor(Color(255, 255, 225, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(255, 255, 255, 100));
					mark.setOutlineColor(Color(255, 255, 255, 50));
					window.draw(mark);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos((rotatecomp + 120) * 0.01745),
						cells[i].position.y + CellRadius * sin((rotatecomp + 120) * 0.01745));
					window.draw(mark);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos((rotatecomp + 240) * 0.01745),
						cells[i].position.y + CellRadius * sin((rotatecomp + 240) * 0.01745));
					window.draw(mark);
				}
				break;
			case 7:
				cellshape.setFillColor(Color(127, 255, 127, clr1));
				cellshape.setOutlineColor(Color(127, 255, 127, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(0, 0, 255, 100));
					mark.setOutlineColor(Color(0, 0, 255, 50));
					window.draw(mark);
				}
				break;
			case 8:
				cellshape.setFillColor(Color(255, 0, 0, clr1));
				cellshape.setOutlineColor(Color(255, 0, 0, clr2));
				if (true)
				{
					RectangleShape flagellumshape(Vector2f(CellRadius, CellRadius / 8));
					flagellumshape.setOrigin(CellRadius / 12, CellRadius / 16);
					for (int u = 0; u < 6; u++)
					{
						flagellumshape.setPosition(
							cells[i].position.x + CellRadius * cos(float(u) * 60.0 * 0.01745),
							cells[i].position.y + CellRadius * sin(float(u) * 60.0 * 0.01745));
						flagellumshape.setRotation(float(u) * 60.0 +
							((abs((cells[i].age % 30) - 15) - 7.5) / 7.5) * 15
						);
						flagellumshape.setFillColor(Color(255, 0, 0, 100));
						flagellumshape.setOutlineThickness(0.4);
						flagellumshape.setOutlineColor(Color(255, 0, 0, 50));
						window.draw(flagellumshape);
					}
				}
				break;
			case 9:
				cellshape.setFillColor(Color(0, 255, 0, clr1));
				cellshape.setOutlineColor(Color(0, 255, 0, clr2));
				if (true)
				{
					RectangleShape flagellumshape(Vector2f(CellRadius / 2, CellRadius / 2));
					flagellumshape.setOrigin(CellRadius / 12, CellRadius / 4);
					for (int u = 0; u < 6; u++)
					{
						flagellumshape.setPosition(
							cells[i].position.x + CellRadius * cos(float(u) * 60.0 * 0.01745),
							cells[i].position.y + CellRadius * sin(float(u) * 60.0 * 0.01745));
						flagellumshape.setRotation(float(u) * 60.0 +
							((abs((cells[i].age % 30) - 15) - 7.5) / 7.5) * 15
						);
						flagellumshape.setFillColor(Color(125, 0, 180, 180));
						flagellumshape.setOutlineThickness(0.4);
						flagellumshape.setOutlineColor(Color(125, 0, 180, 90));
						window.draw(flagellumshape);
					}
				}
				break;
			case 10:
				cellshape.setFillColor(Color(0, 127, 127, clr1));
				cellshape.setOutlineColor(Color(75, 85, 160, clr2));
				break;
			case 11:
				cellshape.setFillColor(Color(0, 0, 0, clr1));
				cellshape.setOutlineColor(Color(255, 255, 255, clr2));
				break;
			case 12:
				cellshape.setFillColor(Color(0, 255, 255, clr1));
				cellshape.setOutlineColor(Color(0, 255, 225, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(0, 255, 255, 100));
					mark.setOutlineColor(Color(0, 255, 255, 50));
					window.draw(mark);
				}
				break;
			case 13:
				cellshape.setFillColor(Color(255, 255, 0, clr1));
				cellshape.setOutlineColor(Color(255, 255, 0, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(255, 255, 0, 100));
					mark.setOutlineColor(Color(255, 255, 0, 50));
					window.draw(mark);
				}
				break;
			case 14:
				cellshape.setFillColor(Color(255, 0, 255, clr1));
				cellshape.setOutlineColor(Color(255, 0, 225, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(255, 0, 255, 100));
					mark.setOutlineColor(Color(255, 0, 255, 50));
					window.draw(mark);
				}
				break;
			case 15:
				cellshape.setFillColor(Color(255, 255, 255, clr1));
				cellshape.setOutlineColor(Color(127, 127, 127, clr2));
				if (true)
				{
					CircleShape mark(CellRadius / 6);
					mark.setOrigin(CellRadius / 6, CellRadius / 6);
					mark.setPosition(
						cells[i].position.x + CellRadius * cos(rotatecomp * 0.01745),
						cells[i].position.y + CellRadius * sin(rotatecomp * 0.01745));
					mark.setFillColor(Color(255, 255, 255, 100));
					mark.setOutlineColor(Color(255, 255, 255, 50));
					window.draw(mark);
				}
				break;
			}
			for (int u = -2; u < 5; u++)
			{
				cellshape.setOutlineThickness(u);
				window.draw(cellshape);
			}
			if (Mouse::isButtonPressed(sf::Mouse::Left) && cellshape.getGlobalBounds().contains(
				window.mapPixelToCoords(Mouse::getPosition(window)))) camfollow = i;
			cellshape.setFillColor(Color(
				min(int(cells[i].energy), 255),
				min(int(cells[i].energy), 255),
				min(int(cells[i].energy), 255)));
			cellshape.setRadius(CellRadius / 6); cellshape.setOrigin(CellRadius / 6, CellRadius / 6);
			cellshape.setOutlineThickness(0);
			window.draw(cellshape);
			if (cells[i].type == 0)
			for (int u = 0; u < (cells[i].age / 100); u++)
			{
				CircleShape ageshape(float(CellRadius) / 12);
				ageshape.setOrigin(float(CellRadius) / 12, float(CellRadius) / 12);
				ageshape.setPosition(
					cells[i].position.x + (CellRadius * 0.75) * cosf((float(u) / float(CellAgeToDie / 100)) * 6.28f),
					cells[i].position.y + (CellRadius * 0.75) * sinf((float(u) / float(CellAgeToDie / 100)) * 6.28f));
				ageshape.setFillColor(Color(255, 255, 255, 175));
				window.draw(ageshape);
			}
			for (int u = 0; u < cells[i].connections.size(); u++)
			{
				float len = Length(cells[cells[i].connections[u].first].position, cells[i].position);
				CircleShape cellconnectionshape(CellRadius / 4);
				cellconnectionshape.setOrigin(CellRadius / 4, CellRadius / 4);
				cellconnectionshape.setPosition(
					cells[i].position.x + 7 * float(cells[cells[i].connections[u].first].position.x - cells[i].position.x) / len,
					cells[i].position.y + 7 * float(cells[cells[i].connections[u].first].position.y - cells[i].position.y) / len);
				cellconnectionshape.setFillColor(Color(255 - cells[i].numberofgenom, cells[i].numberofgenom, 255, 50));
				cellconnectionshape.setOutlineColor(Color(255 - cells[i].numberofgenom, cells[i].numberofgenom, 255, 20));
				cellconnectionshape.setOutlineThickness(1);
				window.draw(cellconnectionshape);
			}
			for (int u = 0; u < CellStoragevall; u++)
				if (cells[i].storage[u] != 0)
				{
					CircleShape cellconnectionshape(CellRadius / 8);
					cellconnectionshape.setOrigin(CellRadius / 8, CellRadius / 8);
					cellconnectionshape.setPosition(
						cells[i].position.x + 5 * cos((float(u) / float(CellStoragevall)) * 6.28),
						cells[i].position.y + 5 * sin((float(u) / float(CellStoragevall)) * 6.28));
					switch (cells[i].storage[u])
					{
						case 1: cellconnectionshape.setFillColor(Color(0, 255, 0, 50)); break;
						case 2: cellconnectionshape.setFillColor(Color(0, 0, 255, 50)); break;
						case 3: cellconnectionshape.setFillColor(Color(255, 0, 0, 50)); break;
						case 4: cellconnectionshape.setFillColor(Color(0, 255, 255, 50)); break;
						case 5: cellconnectionshape.setFillColor(Color(255, 255, 0, 50)); break;
						case 6: cellconnectionshape.setFillColor(Color(255, 0, 255, 50)); break;
						case 7: cellconnectionshape.setFillColor(Color(255, 255, 255, 50)); break;
					}
					cellconnectionshape.setOutlineThickness(0.1);
					window.draw(cellconnectionshape);
				}
		}
}