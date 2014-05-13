#include "Player.h"


Player::Player( void )
{
	Health = 100.0;
	Inventory.clear();
}

Player::Player( string name )
{
	Name = name;
	Health = 100.0;
	Inventory.clear();
}


Player::~Player( void )
{
}
