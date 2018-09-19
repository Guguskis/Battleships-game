#pragma once
#include <iostream>
using namespace std;

#ifndef SHIP_H
#define SHIP_H

class Ship {
public:
	string name;
	int Length, XFront, YFront, Dir;
	bool DestroyMessageAnnounced;
	// dir: 0 - horizontal, 1 - vertical
	//	-	-	-	-	-
	Ship();
	Ship(int);
};

#endif