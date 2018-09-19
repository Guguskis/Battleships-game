#include "stdafx.h"
#include "Ship.h"

Ship::Ship() {}
Ship::Ship(int length) {
	DestroyMessageAnnounced = false;
	Length = length;
	switch (Length) {
	case 1: name = "Battleship"; break;
	case 2: name = "Cruiser"; break;
	case 3: name = "Destroyer"; break;
	case 4: name = "Submarine"; break;
	default: name = "Anomality"; break;
	}
}