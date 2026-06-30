#pragma once
#include <GL/glew.h>
#include "Renderer.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <algorithm>
#include <thread>
#include "GlobalClassesAndStructs.h"

#define GenomSize 160
#define StartPopulation 10
#define StartFood 0
#define StartEnergyOfCell 200
#define MapWidth ((1920 * 3) / 2) * 2
#define MapHeight ((1080 * 3 * 3) / 4) * 2
#define CellRadius 8
#define CellDiameter (CellRadius * 2)
#define BackGroundDots 200
#define BackGroundMovevall 100
#define CellStoragevall 48
#define LoseEnergyvall 0.6
#define HungryEnergyvall 150
#define Evolvevall 8
#define QofEvolve 64
#define Pi 0.01745f
#define FpsQ 25
#define QofCellTypes 21
#define QofStatLines 5
#define CellLookingDistance 15

int UpdateCellsCDvall = 25;
int SpawnACompvall = 100;
int CellAgeToDie = 3000;
float gProj[16];

bool viewstor = false, viewage = false, viewtemp = false, viewcomp = false, showgraph = false;

using namespace sf;
using namespace std;

Clock forstat, forstat1;
int stats[8] = { 0 }, statsmax[8] = { 0 }, lol123 = 0;

void updatestat(int i)
{
	stats[i] = int(stats[i] + forstat.getElapsedTime().asMicroseconds()) / 2; forstat.restart();
	if (stats[i] > statsmax[i]) statsmax[i] = stats[i];
}

String statdesc[5] = {
	L"Count of cells",
	L"Count of comps",
	L"Time for debug1",
	L"Time for debug2",
	L"Time for debug3"
};
int ScreenSize[2], camfollow = -1, gen = 0, fpscounter = 0, picked = -1, showintextbuf = 0, 
	whatpressed = -1, statx = 0, statbuf[QofStatLines], whichlineexplain = 0;
bool camgo = false, controls = false, pause = false;
float BGmovecd = 0, UpdateCellsCD = 0, SpawnACompCD = 0, mousecd = 0, controlcd = 0, butcd = 0, timeforlogic[3] = { 0, 0, 0 },
fpsss[FpsQ] = { 0 }, Zoom = 0, TimesGone, statgraphval[2] = { 1, 1 }, statoffsetx = 0, statoffsety = 0, speedboostval = 2.5f;
Clock statclock, tflclock, tflclock1;
Font font; 
Text text("", font, 40);
vector<int> whocounted;
std::vector<int> compgrid[((MapWidth) / CellDiameter) + 1][((MapHeight) / CellDiameter) + 1];
std::vector<int> cellgrid[((MapWidth) / CellDiameter) + 1][((MapHeight) / CellDiameter) + 1];
std::vector<int> visitstamp; int curstamp = 0;
VertexArray statistics[QofStatLines];
ButtonList settingslist;
String celltypesnames[QofCellTypes] =
{
	L"Head cell", //1
	L"B fabricator", //2
	L"C fabricator", //3
	L"DE recycling", //4
	L"FD recycling", //5
	L"EF recycling", //6
	L"G recycling", //7
	L"A harvester", //8
	L"Hunter", //9
	L"Armor", //10
	L"Connect", //11
	L"Fetus", //12
	L"Synthesis D", //13
	L"Synthesis E", //14
	L"Synthesis F", //15
	L"Synthesis G", //16
	L"A fabricator", //17
	L"Temperature stabilizer", //18
	L"Heater", //19
	L"Cooler", //20
	L"Speeduper", //21
};

string CellsInfo[QofCellTypes][2] =
{
	{"", "ABCDEFG"},		//0
	{"ABC", "ACDEF"},		//1
	{"ABC", "ABDEF"},		//2
	{"BCBC", "FBC"},		//3
	{"ACAC", "EAC"},		//4
	{"ABAB", "DAB"},		//5
	{"DEF", "DEF"},			//6
	{"ABC", "BCDEF"},		//7
	{"ABCABC", "ABCDEFG"},	//8
	{"ABCABC", "ABCDEFG"},	//9
	{"ABC", "ABCDEFG"},		//10
	{"ABC", "ABCDEFG"},		//11
	{"ABC", "ABD"},			//12
	{"ABC", "BCE"},			//13
	{"ABC", "CAF"},			//14
	{"DEF", "DEF"},			//15
	{"ABCBC", "ABCDEFG"},	//16
	{"ABC", "ABCDEFG"},	//17
	{"ABC", "ABCDEFG"},	//18
	{"ABC", "ABCDEFG"},	//19
	{"ABC", "ABCDEFG"}	//20
};

float Length(Vector2f a, Vector2f b = Vector2f(0, 0))
{
	return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
}

void GetDesktopResolution()
{
	SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
	ScreenSize[0] = GetSystemMetrics(SM_CXSCREEN);
	ScreenSize[1] = GetSystemMetrics(SM_CYSCREEN);
}

void ViewUpdate(float& TimesGone)
{
	Vector2i MousePos = Mouse::getPosition(window);
	view.setCenter(
		view.getCenter().x + 
		(max(float(abs(MousePos.x - (ScreenSize[0]) / 2)), ScreenSize[0] * 0.45f) - ScreenSize[0] * 0.45f)
		* (MousePos.x - (ScreenSize[0]) / 2) * abs((MousePos.x - (ScreenSize[0]) / 2)) 
		* 0.000000004f * TimesGone * ((min(int(Zoom), 0) * -1) / 25 + 1),
		view.getCenter().y + 
		(max(float(abs(MousePos.y - (ScreenSize[1]) / 2)), ScreenSize[1] * 0.45f) - ScreenSize[1] * 0.45f)
		* (MousePos.y - (ScreenSize[1]) / 2) * abs((MousePos.y - (ScreenSize[1]) / 2)) 
		* 0.000000016f * TimesGone * ((min(int(Zoom), 0) * -1) / 25 + 1));
	if (view.getCenter().x < 0) view.setCenter(0, view.getCenter().y);
	if (view.getCenter().y < 0) view.setCenter(view.getCenter().x, 0);
	if (view.getCenter().x > MapWidth) view.setCenter(MapWidth, view.getCenter().y);
	if (view.getCenter().y > MapHeight) view.setCenter(view.getCenter().x, MapHeight);
}

float AngleBtw(Vector2f& a, Vector2f& b)
{
	return atan2f(a.y - b.y, a.x - b.x) * 57.2957f;
}

int GetFps()
{
	float all = 0;
	for (auto a : fpsss) all += a;
	return int(all / float(FpsQ));
}

void ControlsShow()
{
	if (controls || (!controls && controlcd > 0))
	{
		CircleShape button((text.getCharacterSize() / 2) * (100 - Zoom) * 0.01f);
		stringstream ss1;
		ss1 << "UpdCells:" << UpdateCellsCDvall << "   SpwnFood:" << SpawnACompvall << "   AgeToDie:" << CellAgeToDie;
		text.setString(ss1.str());
		text.setScale((100 - Zoom) * 0.01f, (100 - Zoom) * 0.01f);
		text.setPosition(view.getCenter().x - text.getGlobalBounds().width / 2,
			view.getCenter().y + view.getSize().y / 2 - 7.5f * text.getGlobalBounds().height
			+ (7.5f * text.getGlobalBounds().height * (!controls)) 
			+ 7.5f * text.getGlobalBounds().height * controlcd * 0.0025f * float(1 - 2 * (!controls)));
		window.draw(text);
		button.setOutlineThickness(2 * (100 - Zoom) * 0.01f);
		if (whatpressed == 0 && butcd < 300) button.setFillColor(Color(255, 0, 128, 40));
		else button.setFillColor(Color(255, 0, 128, 120));
		button.setOutlineColor(Color(255, 0, 128, 60));
		button.setPosition(view.getCenter().x - text.getGlobalBounds().width / 2,
			view.getCenter().y + view.getSize().y / 2 - 6 * text.getGlobalBounds().height
			+ (7.5f * text.getGlobalBounds().height * (!controls)) + 7.5f * text.getGlobalBounds().height * controlcd * 0.0025f * (1 - 2 * (!controls)));
		window.draw(button);
		if (button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))
			&& Mouse::isButtonPressed(sf::Mouse::Left) && mousecd >= 100)
		{
			mousecd = 0; whatpressed = 0; butcd = 0; 
			if (UpdateCellsCDvall <= 5)
			{
				UpdateCellsCDvall -= 1;
				if (UpdateCellsCDvall < 1) UpdateCellsCDvall = 1;
			}
			else
			{
				UpdateCellsCDvall -= 5;
			}
		}
		if (whatpressed == 1 && butcd < 300) button.setFillColor(Color(255, 0, 128, 40));
		else button.setFillColor(Color(255, 0, 128, 120));
		button.setPosition(view.getCenter().x - (text.getCharacterSize() / 2) * 2.5f * (100 - Zoom) * 0.01f,
			view.getCenter().y + view.getSize().y / 2 - 6 * text.getGlobalBounds().height
			+ (7.5f * text.getGlobalBounds().height * (!controls)) 
			+ (7.5f * text.getGlobalBounds().height * controlcd * 0.0025f * (1 - 2 * (!controls))));
		window.draw(button);
		if (button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))
			&& Mouse::isButtonPressed(sf::Mouse::Left) && mousecd >= 100)
		{
			mousecd = 0; whatpressed = 1; butcd = 0; SpawnACompvall -= 5; if (SpawnACompvall < 5) SpawnACompvall = 5;
		}
		if (whatpressed == 2 && butcd < 300) button.setFillColor(Color(255, 0, 128, 40));
		else button.setFillColor(Color(255, 0, 128, 120));
		button.setPosition(view.getCenter().x + text.getGlobalBounds().width / 2 - (text.getCharacterSize() / 2) * 5 * (100 - Zoom) * 0.01f,
			view.getCenter().y + view.getSize().y / 2 - 6 * text.getGlobalBounds().height
			+ (7.5f * text.getGlobalBounds().height * (!controls)) + 7.5f * text.getGlobalBounds().height * controlcd * 0.0025f * (1 - 2 * (!controls)));
		window.draw(button);
		if (button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))
			&& Mouse::isButtonPressed(sf::Mouse::Left) && mousecd >= 100)
		{
			mousecd = 0; whatpressed = 2; butcd = 0; CellAgeToDie -= 50; if (CellAgeToDie < 50) CellAgeToDie = 50;
		}
		button.setOutlineColor(Color(0, 255, 128, 60));
		if (whatpressed == 3 && butcd < 300) button.setFillColor(Color(0, 255, 128, 40));
		else button.setFillColor(Color(0, 255, 128, 120));
		button.setPosition(view.getCenter().x - text.getGlobalBounds().width / 2 + (text.getCharacterSize() / 2) * 3 * (100 - Zoom) * 0.01f,
			view.getCenter().y + view.getSize().y / 2 - 6 * text.getGlobalBounds().height
			+ (7.5f * text.getGlobalBounds().height * (!controls)) + 7.5f * text.getGlobalBounds().height * controlcd * 0.0025f * (1 - 2 * (!controls)));
		window.draw(button);
		if (button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))
			&& Mouse::isButtonPressed(sf::Mouse::Left) && mousecd >= 100)
		{
			mousecd = 0; whatpressed = 3; butcd = 0;
			if (UpdateCellsCDvall <= 5)
			{
				UpdateCellsCDvall += 1;
			}
			else
			{
				UpdateCellsCDvall += 5;
			}
			if (UpdateCellsCDvall > 1000) UpdateCellsCDvall = 1000;
		}
		if (whatpressed == 4 && butcd < 300) button.setFillColor(Color(0, 255, 128, 40));
		else button.setFillColor(Color(0, 255, 128, 120));
		button.setPosition(view.getCenter().x + (text.getCharacterSize() / 2) * 0.5f * (100 - Zoom) * 0.01f,
			view.getCenter().y + view.getSize().y / 2 - 6 * text.getGlobalBounds().height
			+ (7.5f * text.getGlobalBounds().height * (!controls)) + 7.5f * text.getGlobalBounds().height * controlcd * 0.0025f * (1 - 2 * (!controls)));
		window.draw(button);
		if (button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))
			&& Mouse::isButtonPressed(sf::Mouse::Left) && mousecd >= 100)
		{
			mousecd = 0; whatpressed = 4; butcd = 0; SpawnACompvall += 5; if (SpawnACompvall > 1000) SpawnACompvall = 1000;
		}
		if (whatpressed == 5 && butcd < 300) button.setFillColor(Color(0, 255, 128, 40));
		else button.setFillColor(Color(0, 255, 128, 120));
		button.setPosition(view.getCenter().x + text.getGlobalBounds().width / 2 - (text.getCharacterSize() / 2) * 2 * (100 - Zoom) * 0.01f,
			view.getCenter().y + view.getSize().y / 2 - 6 * text.getGlobalBounds().height
			+ (7.5f * text.getGlobalBounds().height * (!controls)) + 7.5f * text.getGlobalBounds().height * controlcd * 0.0025f * (1 - 2 * (!controls)));
		window.draw(button);
		if (button.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window)))
			&& Mouse::isButtonPressed(sf::Mouse::Left) && mousecd >= 100)
		{
			mousecd = 0; whatpressed = 5; butcd = 0; CellAgeToDie += 50; if (CellAgeToDie > 5000) CellAgeToDie = 5000;
		}
	}
}

void clearit(int* thingtobecleared)
{
	for (int i = 0; i < 7; i++) thingtobecleared[i] = 0;
}