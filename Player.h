#pragma once
#include <string>
#include <vector>

#include "object.h"

using namespace std;

class Player :
	public Object
{
public:
	Player( void );
	Player( string name );
	~Player( void );

	string Name;
	double Health;

	vector<Object> Inventory;
};

