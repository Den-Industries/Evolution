#pragma once
#include "Global.h"

struct Component
{
	Vector2f pos;
	int type, howlonglive = 0;
	Component(Vector2f Pos, int Type)
	{
		pos = Pos; type = Type;
	}
};

vector<Component> comps;