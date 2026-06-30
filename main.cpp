#include "Global.h"
#include "Cell.h"
#include "Init.h"
#include "BackGroundStuff.h"
#include "DrawCells.h"
#include "CellsLogic.h"
#include "DrawLogComp.h"

void SwitchToCharts()
{
	if (showgraph)
	{
		statoffsetx = min(0.0f, 1890 - statx * statgraphval[0]);
		for (int i = 0; i < statx * 2; i++)
		{
			for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.x *= statgraphval[0];
			for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.x += 15 + statoffsetx;
			for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.y *= statgraphval[1];
			for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.y = ScreenSize[1] - statistics[u][i].position.y - (15 + statoffsety);
		}
	}
	else
	{
		for (int i = 0; i < statx * 2; i++)
		{
			for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.x -= 15 + statoffsetx;
			for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.x /= statgraphval[0];
			for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.y = ScreenSize[1] - statistics[u][i].position.y - (15 + statoffsety);
			for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.y /= statgraphval[1];
		}
	}
}

int main()
{
	Init();
	BGInit();
	for (int i = 0; i < StartPopulation; i++)
	{
		cells.push_back(Cell(Vector2f(//100, 100), 0));
			float(rand() % (MapWidth - CellRadius * 4) + CellRadius * 2),
			float(rand() % (MapHeight - CellRadius * 4) + CellRadius * 2)), 0));
		cells[cells.size() - 1].GenerateGenom();
		cells[cells.size() - 1].ExpGenom();
		/*
		if (cells.size() - 1 <= 1) cells[cells.size() - 1].ExpGenom();
		else if (cells.size() - 1 == 2) cells[cells.size() - 1].ExpGenom2();
		else if (cells.size() - 1 == 3) cells[cells.size() - 1].ExpGenom4();
		else  cells[cells.size() - 1].GenerateGenom();*/
		for (int j = 0; j < 1; j++)
		{
			cells[cells.size() - 1].storage[0 + j * 3] = 1;
			cells[cells.size() - 1].storage[1 + j * 3] = 2;
			cells[cells.size() - 1].storage[2 + j * 3] = 3;
		}
	}
	//for (int i = 0; i < StartFood; i++) comps.push_back(Component(Vector2f(float(rand() % MapWidth), float(rand() % MapHeight)), 1));
	window.create(VideoMode(ScreenSize[0], ScreenSize[1]), "Programm",
		Style::Fullscreen, ContextSettings(24, 8, 0, 3, 3, ContextSettings::Default)); // >>> CHANGED
	window.setActive(true);
	window.setFramerateLimit(60);

	// >>> CHANGED: GLEW + рендеры
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { std::cerr << "glewInit failed\n"; return -1; }
	gCircles.init();
	gRects.init();
	float TimesGone; 
	Clock clk, alltimeclk, updateclock, drawclock; 
	text.setCharacterSize((40 * ScreenSize[0]) / 1920);
	while (window.isOpen())
	{
		forstat.restart();
		if (window.hasFocus()) 
			if (Keyboard::isKeyPressed(Keyboard::Space) && Keyboard::isKeyPressed(Keyboard::LControl) && cells.size() == 0)
		{
				Sleep(300);
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
				cells[cells.size() - 1].storage[9] = 1;
				cells[cells.size() - 1].storage[10] = 2;
				cells[cells.size() - 1].storage[11] = 3;
				cells[cells.size() - 1].storage[12] = 1;
				cells[cells.size() - 1].storage[13] = 2;
				cells[cells.size() - 1].storage[14] = 3;
				cells[cells.size() - 1].storage[15] = 1;
				cells[cells.size() - 1].storage[16] = 2;
				cells[cells.size() - 1].storage[17] = 3;
			}
		}
		TimesGone = clk.getElapsedTime().asMicroseconds() * 0.001f; clk.restart(); 
		if (fpscounter % 2 == 0) fpsss[fpscounter / 2] = (1000.0f / TimesGone);
		fpscounter = (fpscounter + 1) % (FpsQ * 2);
		if (BGmovecd < BackGroundMovevall) BGmovecd += TimesGone;
		if (mousecd < 300) mousecd += TimesGone;
		if (butcd < 300) butcd += TimesGone;
		if (controlcd > 0) controlcd -= TimesGone; else controlcd = 0;
		UpdateCellsCD += TimesGone; SpawnACompCD += TimesGone;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) window.close();
			if (window.hasFocus())
			{
				if (event.type == sf::Event::MouseWheelScrolled)
				{
					if (showgraph)
					{
						if (Keyboard::isKeyPressed(Keyboard::LControl))
						{
							for (int i = 0; i < statx * 2; i++)
							{
								for (int u = 0; u < QofStatLines; u++)
									statistics[u][i].position.y = ScreenSize[1] - statistics[u][i].position.y - 15;
								for (int u = 0; u < QofStatLines; u++)
									statistics[u][i].position.y /= statgraphval[1];
								for (int u = 0; u < QofStatLines; u++)
									statistics[u][i].position.y *= max(statgraphval[1] + event.mouseWheelScroll.delta * 0.2f, 0.1f);
								for (int u = 0; u < QofStatLines; u++)
									statistics[u][i].position.y = ScreenSize[1] - statistics[u][i].position.y - 15;
							}
							statgraphval[1] += event.mouseWheelScroll.delta * 0.2f;
							if (statgraphval[1] < 0.1f) statgraphval[1] = 0.1f;
							if (statgraphval[1] > 100) statgraphval[1] = 100;
						}
						else
						{
							for (int i = 0; i < statx * 2; i++)
							{
								for (int u = 0; u < QofStatLines; u++)
									statistics[u][i].position.x -= 15 + statoffsetx;
								for (int u = 0; u < QofStatLines; u++)
									statistics[u][i].position.x /= statgraphval[0];
								for (int u = 0; u < QofStatLines; u++)
									statistics[u][i].position.x *= max(statgraphval[0] + event.mouseWheelScroll.delta, 1.0f);
								for (int u = 0; u < QofStatLines; u++)
									statistics[u][i].position.x += 15 + statoffsetx;
							}
							statgraphval[0] += event.mouseWheelScroll.delta * 0.3f;
							if (statgraphval[0] < 0.1f) statgraphval[0] = 0.1f;
							if (statgraphval[0] > 300) statgraphval[0] = 300;
						}
					}
					else
					{
						Zoom += event.mouseWheelScroll.delta * 7; if (Zoom < -590) Zoom = -590; if (Zoom > 90) Zoom = 90;
						view.setSize(ScreenSize[0] * (100 - Zoom) * 0.01f, ScreenSize[1] * (100 - Zoom) * 0.01f);
					}
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && mousecd >= 300)
				{
					picked = -1; //cout << "#"; 
					window.setView(view);
					Vector2f buf = window.mapPixelToCoords(Mouse::getPosition(window));
					for (int i = 0; i < cells.size(); ++i)
					{
						if (Length(cells[i].position, buf) < CellRadius) picked = i;
					}
					mousecd = 0; camfollow = picked;
				}
			}
		}
		if (window.hasFocus()) if (Keyboard::isKeyPressed(Keyboard::Num1)) whichlineexplain = 0;
		if (window.hasFocus()) if (Keyboard::isKeyPressed(Keyboard::Num2)) whichlineexplain = 1;
		if (window.hasFocus()) if (Keyboard::isKeyPressed(Keyboard::Num3)) whichlineexplain = 2;
		if (window.hasFocus()) if (Keyboard::isKeyPressed(Keyboard::Num4)) whichlineexplain = 3;
		if (window.hasFocus()) if (Keyboard::isKeyPressed(Keyboard::Num5)) whichlineexplain = 4;
		if (window.hasFocus()) if (Keyboard::isKeyPressed(Keyboard::U))
		{
			cout << "Current:\n\t0: " << stats[0] * 0.001f << "\n"
				<< "\t1: " << stats[1] * 0.001f << "\n"
				<< "\t2: " << stats[2] * 0.001f << "\n"
				//<< "\tLogic components: " << stats[4] << "\n"
				//<< "\tDraw components: " << stats[5] << "\n"
				//<< "\tDraw cells: " << stats[6] << "\n"
				//<< "\tFinal stuff: " << stats[7] 
				<< "\n\n";
			cout << "Max:\n\t0: " << statsmax[0] * 0.001f << "\n"
				<< "\t1: " << statsmax[1] * 0.001f << "\n"
				<< "\t2: " << statsmax[2] * 0.001f << "\n"
				//<< "\tLogic components: " << statsmax[4] << "\n"
				//<< "\tDraw components: " << statsmax[5] << "\n"
				//<< "\tDraw cells: " << statsmax[6] << "\n"
				//<< "\tFinal stuff: " << statsmax[7] 
				<< "\n\n\n\n";
			Sleep(100);
		}
		if (window.hasFocus()) if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
		if (window.hasFocus() && Keyboard::isKeyPressed(Keyboard::C) && mousecd >= 300)
		{
			controls = !controls; mousecd = 0; controlcd = 400;
		}
		if (window.hasFocus() && Keyboard::isKeyPressed(Keyboard::G) && mousecd >= 300)
		{
			showgraph = !showgraph; mousecd = 0; SwitchToCharts();
		}
		if (window.hasFocus() && Keyboard::isKeyPressed(Keyboard::Space) && mousecd >= 300)
		{
			pause = !pause; mousecd = 0;
			UpdateCellsCD = 0;
			SpawnACompCD = 0;
		}
		if (camfollow != -1 && camfollow < cells.size())
			view.setCenter(Vector2f(
				(cells[camfollow].position.x + 12 * view.getCenter().x) / 13.f,
				(cells[camfollow].position.y + 12 * view.getCenter().y) / 13.f
			));
		while (BGmovecd >= BackGroundMovevall) BGmovecd -= BackGroundMovevall;
		if (statclock.getElapsedTime().asMilliseconds() > 200)
		{
			statclock.restart();
			if (!showgraph)
			{
				statistics[0].append(Vertex(Vector2f(float(statx - 1), float(statbuf[0])), Color(255, 0, 0)));
				statistics[0].append(Vertex(Vector2f(float(statx), float(cells.size())), Color(255, 0, 0)));

				statistics[1].append(Vertex(Vector2f(float(statx - 1), float(statbuf[1])), Color(0, 255, 0)));
				statistics[1].append(Vertex(Vector2f(float(statx), float(comps.size())), Color(0, 255, 0)));

				statistics[2].append(Vertex(Vector2f(float(statx - 1), float(statbuf[2])), Color(0, 0, 255)));
				statistics[2].append(Vertex(Vector2f(float(statx), timeforlogic[0] * 10), Color(0, 0, 255)));

				statistics[3].append(Vertex(Vector2f(float(statx - 1), float(statbuf[3])), Color(0, 255, 255)));
				statistics[3].append(Vertex(Vector2f(float(statx), timeforlogic[1] * 10), Color(0, 255, 255)));

				statistics[4].append(Vertex(Vector2f(float(statx - 1), float(statbuf[4])), Color(255, 0, 255)));
				statistics[4].append(Vertex(Vector2f(float(statx), timeforlogic[2] * 10), Color(255, 0, 255)));

				statbuf[0] = int(cells.size());
				statbuf[1] = int(comps.size());
				statbuf[2] = int(timeforlogic[0] * 10);
				statbuf[3] = int(timeforlogic[1] * 10);
				statbuf[4] = int(timeforlogic[2] * 10);
				statx++;
			}
		}
		if (updateclock.getElapsedTime().asMilliseconds() > 10 && !pause)
		{
			updateclock.restart();
			if (!showgraph)
			{
				UpdateCells();
				LogComps();
			}
		}
		if (window.hasFocus() && !showgraph) ViewUpdate(TimesGone);
		if (showgraph)
			if (Mouse::isButtonPressed(Mouse::Button::Left) && mousecd >= 300)
			{
				for (int i = 0; i < statx * 2; i += 2)
					if (statistics[whichlineexplain][i + 1].position.x >= Mouse::getPosition(window).x)
					{
						showintextbuf = (ScreenSize[1] - 
							statistics[whichlineexplain][i + 1].position.y - int(15 + statoffsety)) /
							statgraphval[1];
						break;
					}
				mousecd = 0;
			}
		if (drawclock.getElapsedTime().asMilliseconds() > 20)
		{
			drawclock.restart();
			if (!showgraph)
			{
				window.setView(view);
				window.clear(Color(40, 40, 85));

				// === сырой OpenGL: наполняем батчеры ===
				if (window.hasFocus()) BackGround();
				if (window.hasFocus()) DrawComps();
				if (window.hasFocus()) DrawCells();

				// === flush: матрица из view, порядок = прямоугольники под кругами ===
				BuildProjFromView(view, gProj);
				gRects.flush(gProj);     // жгутики/рамка снизу
				gCircles.flush(gProj);   // круги сверху

				// === SFML UI ===
				window.pushGLStates();
				window.setView(view);
				stringstream ss;
				ss << "Gen: " << gen << "\nTime: " << int(alltimeclk.getElapsedTime().asSeconds()) << " s\n" << GetFps() << " \n";
				if (camfollow != -1) ss << "Choosen: " << camfollow << " Type: " << celltypesnames[cells[camfollow].type].toAnsiString();
				if (cells.size() > 0) ss << "\n\nGenProgress: " << int((cells[0].age / float(CellAgeToDie)) * 1000) * 0.001f;
				text.setString(ss.str());
				text.setScale((100 - Zoom) * 0.01f, (100 - Zoom) * 0.01f);
				text.setPosition(view.getCenter().x - view.getSize().x / 2 + view.getSize().x / 100,
					view.getCenter().y - view.getSize().y / 2 + view.getSize().x / 100);
				window.draw(text);
				if (window.hasFocus()) ControlsShow();
				CircleShape cursor(3);
				cursor.setOrigin(3, 3);
				cursor.setPosition(window.mapPixelToCoords(Mouse::getPosition(window)));
				window.draw(cursor);
				window.setView(window.getDefaultView());
				int res = settingslist.Update();
				if (res == 0) { controls = !controls; controlcd = 400; }
				if (res == 1) { viewstor = !viewstor; }
				if (res == 2) { viewage = !viewage; }
				if (res == 3) { viewtemp = !viewtemp; }
				if (res == 4) { viewcomp = !viewcomp; }
				if (res == 5) { showgraph = !showgraph; SwitchToCharts(); }
				settingslist.Draw();
				window.popGLStates();   // >>> CHANGED

				window.display();
			}
			else
			{
				window.clear(Color(50, 50, 95));
				window.pushGLStates();   // >>> CHANGED
				window.setView(window.getDefaultView());
				int res = settingslist.Update();
				if (res == 0) { controls = !controls; controlcd = 400; }
				if (res == 1) { viewstor = !viewstor; }
				if (res == 2) { viewage = !viewage; }
				if (res == 3) { viewtemp = !viewtemp; }
				if (res == 4) { viewcomp = !viewcomp; }
				if (res == 5) { showgraph = !showgraph; SwitchToCharts(); }
				for (int u = 0; u < QofStatLines; u++) window.draw(statistics[u]);
				stringstream ss;
				ss << "Val: " << showintextbuf << "\nDesc: " << statdesc[whichlineexplain].toAnsiString();
				text.setString(ss.str());
				text.setScale(1, 1);
				text.setPosition(10, 10);
				window.draw(text);
				settingslist.Draw();
				window.display();
			}
		}
		if (showgraph)
		{
			Vector2i MousePos = Mouse::getPosition(window);
			for (int i = 0; i < statx * 2; i++) for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.x -= statoffsetx;
			if (MousePos.x < ScreenSize[0] / 5) statoffsetx += TimesGone;
			if (MousePos.x > 4 * (ScreenSize[0] / 5)) statoffsetx -= TimesGone;
			if (statoffsetx < -statx * statgraphval[0]) statoffsetx = -statx * statgraphval[0];
			if (statoffsetx > max(0.0f, statx * statgraphval[0] - 1920)) statoffsetx = max(0.0f, statx * statgraphval[0] - 1920);
			for (int i = 0; i < statx * 2; i++) for (int u = 0; u < QofStatLines; u++)
				statistics[u][i].position.x += statoffsetx;
		}
		//updatestat(7); 
	}
	return 0;
}