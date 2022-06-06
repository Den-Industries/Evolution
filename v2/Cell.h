#pragma once
#include "Global.h"

struct Genom
{
	int gen[GenomSize][12] = { {} };
	Genom(int empty = 0)
	{
		if (empty != 0) gen[0][0] = -1;
	}
};

struct Cell 
{
	vector<pair<int, int>> connections;
	Vector2f position; Genom g;
	int numberofgenom, type, storage[CellStoragevall], age = 0, CD = 0;
	float energy = StartEnergyOfCell;
	Cell(Vector2f pos, int NofG, Genom SourceGenom = NULL, int Type = 0, bool WithEvolve = false)
	{
		position = pos; numberofgenom = NofG; type = Type;
		if (SourceGenom.gen[0][0] != -1) for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 12; u++) g.gen[i][u] = SourceGenom.gen[i][u];
		for (int i = 0; i < CellStoragevall; i++) storage[i] = 0;
		if (WithEvolve) this->Evolve();
	}
	void GenerateGenom()
	{
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = rand() % (GenomSize * 3);
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 15 + 1;
			}
	}
	void Evolve()
	{
		for (int i = 0; i < QofEvolve; i++)
			if (rand() % Evolvevall == 0)
			{
				g.gen[rand() % GenomSize][(rand() % 4) * 3] = rand() % (GenomSize * 3);
				g.gen[rand() % GenomSize][(rand() % 4) * 3 + 1] = (rand() % 12) * 30;
				g.gen[rand() % GenomSize][(rand() % 4) * 3 + 2] = rand() % 15 + 1;
			}
	}
	void ExpGenom()
	{
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = GenomSize * 2;
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 15 + 1;
			}
		g.gen[0][0] = 1;
		g.gen[0][1] = 180;
		g.gen[0][2] = 11;

		g.gen[1][0] = 2;
		g.gen[1][1] = 180;
		g.gen[1][2] = 11;
		/*
		g.gen[0][0] = 1;
		g.gen[0][1] = 180;
		g.gen[0][2] = 10;

		g.gen[1][0] = 2;
		g.gen[1][1] = 180;
		g.gen[1][2] = 10;

		g.gen[1][3] = 4;
		g.gen[1][4] = 90;
		g.gen[1][5] = 7;
		
		g.gen[2][0] = 3;
		g.gen[2][1] = 180;
		g.gen[2][2] = 10;

		g.gen[2][3] = 4;
		g.gen[2][4] = 90;
		g.gen[2][5] = 1;

		g.gen[3][0] = 5;
		g.gen[3][1] = 180;
		g.gen[3][2] = 10;

		g.gen[3][3] = 4;
		g.gen[3][4] = 90;
		g.gen[3][5] = 2;

		g.gen[5][0] = 4;
		g.gen[5][1] = 270;
		g.gen[5][2] = 11;

		g.gen[5][3] = 4;
		g.gen[5][4] = 90;
		g.gen[5][5] = 11;
		*/
	}
	void ExpGenom2()
	{
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = GenomSize * 2;
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 15 + 1;
			}

		g.gen[0][0] = 1;
		g.gen[0][1] = 180;
		g.gen[0][2] = 11;

		g.gen[1][0] = 2;
		g.gen[1][1] = 180;
		g.gen[1][2] = 8;
	}
	void ExpGenom3()
	{
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = GenomSize * 2;
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 15 + 1;
			}

		g.gen[0][0] = 1;
		g.gen[0][1] = 180;
		g.gen[0][2] = 10;

		g.gen[0][3] = 44;
		g.gen[0][4] = 180;
		g.gen[0][5] = 7;
		
		g.gen[1][0] = 2;
		g.gen[1][1] = 90;
		g.gen[1][2] = 1;

		g.gen[1][6] = 2;
		g.gen[1][7] = 270;
		g.gen[1][8] = 2;

		g.gen[2][0] = 2;
		g.gen[2][1] = 180;
		g.gen[2][2] = 11;

		g.gen[2][3] = 2;
		g.gen[2][4] = 90;
		g.gen[2][5] = 11;

		g.gen[2][6] = 2;
		g.gen[2][7] = 270;
		g.gen[2][8] = 11;
	}
	void EmptyGenom()
	{
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = GenomSize * 2;
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 15 + 1;
			}
	}
};

vector<Cell> cells;

void Give(int cellnum, int type, int inseption = 0)
{
	if (inseption <= 2)
	{
		bool havegiven = false;
		for (int i = 0; i < CellStoragevall; i++)
			if (cells[cellnum].storage[i] == 0)
			{
				cells[cellnum].storage[i] = type;
				havegiven = true; break;
			}
		if (!havegiven)
			for (int i = 0; i < cells[cellnum].connections.size(); i++)
				Give(cells[cellnum].connections[i].first, type, inseption + 1);
	}
}