#pragma once
#include "Global.h"

void Init()
{
	//srand(unsigned int(time(NULL)));
	GetDesktopResolution();
	for (int i = 0; i < QofStatLines; i++) statbuf[i] = 0;
	view.setCenter(Vector2f(MapWidth / 2, MapHeight / 2));
	//view.setCenter(Vector2f(0, 0));
	view.setSize(Vector2f(float(ScreenSize[0]), float(ScreenSize[1])));
	font.loadFromFile("font.ttf");
	text.setStyle(sf::Text::Bold);
	text.setOutlineThickness(3);
	text.setFillColor(Color(190, 190, 235, 127));
	text.setOutlineColor(Color(120, 0, 120, 127));
	whocounted.reserve(15);
	for (int i = 0; i < QofStatLines; i++) 
		statistics[i].setPrimitiveType(sf::PrimitiveType::Lines);
	settingslist.Create(6, 200);
	settingslist.lbstropen = L"Настройки";
	settingslist.lbstrback = L"Закрыть";
	settingslist.PreCreateAll("timesnewroman.ttf", Color(190, 190, 235, 200),
		Color(60, 60, 105, 127), FloatRect(1615.0f, 5.0f, 300.0f, 50.0f), 30, true, 10);
	settingslist.listbutton.SetText(L"Настройки");
	settingslist.buttons[0].SetText(L"Панель");
	settingslist.buttons[1].SetText(L"Показ запасов");
	settingslist.buttons[2].SetText(L"Показ возраста");
	settingslist.buttons[3].SetText(L"Показ температуры");
	settingslist.buttons[4].SetText(L"Показ компонентов");
	settingslist.buttons[5].SetText(L"График(выход G)");
}