#pragma once
#include "Global.h"

struct Component
{
	Vector2f pos;
	int type;
	Component(Vector2f Pos, int Type)
	{
		pos = Pos; type = Type;
	}
};

vector<Component> comps;