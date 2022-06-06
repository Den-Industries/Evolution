#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <Windows.h>
#include <sstream>
#include <algorithm>

#define GenomSize 200
#define StartPopulation 50
#define StartFood 500
#define StartEnergyOfCell 200
#define MapWidth 1920 * 2
#define MapHeight 1080 * 2
#define CellRadius 10
#define BackGroundDots 300
#define BackGroundMovevall 80
#define UpdateCellsCDvall 40
#define CellStoragevall 24
#define SpawnACompvall 30
#define CellAgeToDie 1000
#define LoseEnergyvall 0.65
#define HungryEnergyvall 150
#define Evolvevall 4
#define QofEvolve 196
#define Pi 0.01745f
#define FpsQ 25

using namespace sf;
using namespace std;

int ScreenSize[2], camfollow = -1, gen = 0, fpscounter = 0, picked = -1; bool camgo = false;
float BGmovecd = 0, UpdateCellsCD = 0, SpawnACompCD = 0, mousecd = 0, fpsss[FpsQ] = { 0 };
RenderWindow window;
View view;
Font font; 
Text text("", font, 40);

string CellsInfo[16][2] =
{
	{"", "ABC"},			//0
	{"ABC", "AC"},			//1
	{"ABC", "AB"},			//2
	{"BCF", "FBC"},			//3
	{"ACE", "EAC"},			//4
	{"ABD", "DAB"},			//5
	{"DEF", "DEF"},			//6
	{"ABC", "BC"},			//7
	{"ABCABC", "ABCDEFG"},	//8
	{"ABCABC", "ABCDEFG"},	//9
	{"ABC", "ABCDEFG"},		//10
	{"ABC", "ABCDEFG"},		//11
	{"ABC", "ABD"},			//12
	{"ABC", "BCE"},			//13
	{"ABC", "CAF"},			//14
	{"DEF", "DEF"}			//15
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

void ViewUpdate(float TimesGone)
{
	Vector2i MousePos = Mouse::getPosition(window);
	view.setCenter(view.getCenter().x + (max(abs(MousePos.x - (ScreenSize[0]) / 2), ScreenSize[0] / 4) - ScreenSize[0] / 4)
		* (MousePos.x - (ScreenSize[0]) / 2) * abs((MousePos.x - (ScreenSize[0]) / 2)) * 0.000000001f * TimesGone,
		view.getCenter().y + (max(abs(MousePos.y - (ScreenSize[1]) / 2), ScreenSize[1] / 4) - ScreenSize[1] / 4)
		* (MousePos.y - (ScreenSize[1]) / 2) * abs((MousePos.y - (ScreenSize[1]) / 2)) * 0.000000004f * TimesGone);
	if (view.getCenter().x < 0) view.setCenter(0, view.getCenter().y);
	if (view.getCenter().y < 0) view.setCenter(view.getCenter().x, 0);
	if (view.getCenter().x > MapWidth) view.setCenter(MapWidth, view.getCenter().y);
	if (view.getCenter().y > MapHeight) view.setCenter(view.getCenter().x, MapHeight);
}

float AngleBtw(Vector2f a, Vector2f b)
{
	return atan2f(a.y - b.y, a.x - b.x) * 57.2957f;
}

int GetFps()
{
	float all = 0;
	for (auto a : fpsss) all += a;
	return all / float(FpsQ);
}