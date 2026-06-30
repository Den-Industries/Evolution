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
	short int numberofgenom, type, storage[CellStoragevall], age = 0, CD = 0, temperature = 0, periodupdate = 0, bufnearest = -1;
	float energy = StartEnergyOfCell; bool alert = false;
	int bufnearestType = 0;
	Vector2f wondervec;
	Cell(Vector2f pos, int NofG, Genom SourceGenom = NULL, int Type = 0, int temp = 0, bool WithEvolve = false)
	{
		temperature = temp;
		position = pos; numberofgenom = NofG; type = Type;
		if (SourceGenom.gen[0][0] != -1) g = SourceGenom;
			//for (int i = 0; i < GenomSize; i++)
			//for (int u = 0; u < 12; u++) g.gen[i][u] = SourceGenom.gen[i][u];
		for (int i = 0; i < CellStoragevall; i++) storage[i] = 0;
		if (WithEvolve) this->Evolve();
		wondervec.x = 0;
		wondervec.y = 0;
	}
	void GenerateGenom()
	{
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = rand() % (GenomSize * 2);
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = (rand() % (QofCellTypes - 1)) + 1;
			}
	}
	void Evolve()
	{
		for (int i = 0; i < QofEvolve; i++)
			if (rand() % Evolvevall == 0)
			{
				g.gen[rand() % GenomSize][(rand() % 4) * 3] = rand() % int(GenomSize * 2);
				g.gen[rand() % GenomSize][(rand() % 4) * 3 + 1] = (rand() % 12) * 30;
				g.gen[rand() % GenomSize][(rand() % 4) * 3 + 2] = (rand() % (QofCellTypes - 1)) + 1;
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
	void ExpGenom4()
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

		g.gen[0][6] = 44;
		g.gen[0][7] = 90;
		g.gen[0][8] = 1;

		g.gen[0][9] = 44;
		g.gen[0][10] = 270;
		g.gen[0][11] = 2;

		g.gen[1][0] = 2;
		g.gen[1][1] = 180;
		g.gen[1][2] = 10;

		g.gen[1][3] = 44;
		g.gen[1][4] = 90;
		g.gen[1][5] = 12;

		g.gen[1][6] = 44;
		g.gen[1][7] = 270;
		g.gen[1][8] = 13;

		g.gen[2][0] = 44;
		g.gen[2][1] = 180;
		g.gen[2][2] = 3;

		g.gen[2][3] = 44;
		g.gen[2][4] = 90;
		g.gen[2][5] = 11;

		g.gen[2][6] = 44;
		g.gen[2][7] = 270;
		g.gen[2][8] = 11;
	}
	void ExpGenom5()
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

		g.gen[0][3] = 44;
		g.gen[0][4] = 180;
		g.gen[0][5] = 16;

		g.gen[0][6] = 44;
		g.gen[0][7] = 90;
		g.gen[0][8] = 1;

		g.gen[0][9] = 44;
		g.gen[0][10] = 270;
		g.gen[0][11] = 2;

		g.gen[1][0] = 2;
		g.gen[1][1] = 180;
		g.gen[1][2] = 11;

		g.gen[1][3] = 2;
		g.gen[1][4] = 90;
		g.gen[1][5] = 1;

		g.gen[1][6] = 2;
		g.gen[1][7] = 270;
		g.gen[1][8] = 2;
	}
	void ExpGenom6()
	{
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = GenomSize * 2;
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 15 + 1;
			}
		g.gen[0][0] = 2;
		g.gen[0][1] = 90;
		g.gen[0][2] = 17;

		g.gen[0][3] = 1;
		g.gen[0][4] = 180;
		g.gen[0][5] = 11;

		g.gen[1][0] = 2;
		g.gen[1][1] = 180;
		g.gen[1][2] = 11;
	}
	void ExpGenom7() //растение палочка
	{
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = GenomSize * 2;
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 15 + 1;
			}
		
		g.gen[0][0] = 2;
		g.gen[0][1] = 90;
		g.gen[0][2] = 16;

		g.gen[0][3] = 1;
		g.gen[0][4] = 180;
		g.gen[0][5] = 1;

		g.gen[1][0] = 2;
		g.gen[1][1] = 180;
		g.gen[1][2] = 2;

		g.gen[2][0] = 3;
		g.gen[2][1] = 180;
		g.gen[2][2] = 11;
		/*
		g.gen[0][0] = 2;
		g.gen[0][1] = 90;
		g.gen[0][2] = 2;

		g.gen[0][3] = 2;
		g.gen[0][4] = 180;
		g.gen[0][5] = 1;

		g.gen[2][0] = 3;
		g.gen[2][1] = 180;
		g.gen[2][2] = 11;*/
	}
	void ExpGenom8() //растение палочка
	{
		for (int i = 0; i < 6; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = GenomSize * 2;
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 15 + 1;
			}
		g.gen[0][0] = 3;
		g.gen[0][1] = 90;
		g.gen[0][2] = 16;

		g.gen[0][3] = 1;
		g.gen[0][4] = 180;
		g.gen[0][5] = 1;

		g.gen[1][0] = 2;
		g.gen[1][1] = 180;
		g.gen[1][2] = 2;

		g.gen[2][0] = 4;
		g.gen[2][1] = 180;
		g.gen[2][2] = 11;

		g.gen[3][0] = 5;
		g.gen[3][1] = 180;
		g.gen[3][2] = 11;
	}
	void ExpGenom9() //растение палочка
	{
		/*
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = GenomSize * 2;
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 15 + 1;
			}
			*/
		g.gen[0][0] = 3;
		g.gen[0][1] = 90;
		g.gen[0][2] = 16;

		g.gen[0][3] = 1;
		g.gen[0][4] = 180;
		g.gen[0][5] = 1;

		g.gen[1][0] = 2;
		g.gen[1][1] = 180;
		g.gen[1][2] = 2;

		g.gen[2][0] = 4;
		g.gen[2][1] = 180;
		g.gen[2][2] = 20;

		g.gen[3][0] = 5;
		g.gen[3][1] = 180;
		g.gen[3][2] = 20;
	}
	void ExpGenomX() //Гигантская хрень
	{
		cout << "what";
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

		g.gen[0][3] = 48;
		g.gen[0][4] = 150;
		g.gen[0][5] = 7;

		g.gen[0][6] = 48;
		g.gen[0][7] = 210;
		g.gen[0][8] = 7;

		g.gen[1][0] = 48;
		g.gen[1][1] = 60;
		g.gen[1][2] = 9;

		g.gen[1][3] = 48;
		g.gen[1][4] = 300;
		g.gen[1][5] = 9;

		g.gen[1][6] = 2;
		g.gen[1][7] = 180;
		g.gen[1][8] = 10;

		g.gen[2][0] = 51;
		g.gen[2][1] = 90;
		g.gen[2][2] = 1;

		g.gen[2][3] = 3;
		g.gen[2][4] = 180;
		g.gen[2][5] = 10;

		g.gen[2][6] = 51;
		g.gen[2][7] = 270;
		g.gen[2][8] = 2;

		g.gen[3][0] = 48;
		g.gen[3][1] = 90;
		g.gen[3][2] = 11;

		g.gen[3][3] = 2;
		g.gen[3][4] = 180;
		g.gen[3][5] = 10;

		g.gen[3][6] = 48;
		g.gen[3][7] = 270;
		g.gen[3][8] = 11;

		g.gen[51][6] = 48;
		g.gen[51][7] = 180;
		g.gen[51][8] = 9;
	}
	void EmptyGenom()
	{
		for (int i = 0; i < GenomSize; i++)
			for (int u = 0; u < 4; u++)
			{
				g.gen[i][u * 3] = GenomSize * 2;
				g.gen[i][u * 3 + 1] = (rand() % 12) * 30;
				g.gen[i][u * 3 + 2] = rand() % 16 + 1;
			}
	}
};

vector<Cell> cells;

bool Give(int& cellnum, int type, int inseption = 0)
{
	if (inseption <= 2)
	{
		bool havegiven = false;
		for (int i = 0; i < CellStoragevall; i++)
			if (cells[cellnum].storage[i] == 0)
			{
				cells[cellnum].storage[i] = type;
				havegiven = true; return true;
				break;
			}
		if (!havegiven)
			for (int i = 0; i < cells[cellnum].connections.size(); i++)
			{
				havegiven = Give(cells[cellnum].connections[i].first, type, inseption + 1);
				if (havegiven)
				{
					return true;
					break;
				}
			}
	}
	return false;
}