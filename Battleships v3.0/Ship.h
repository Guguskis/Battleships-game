#pragma once
#include <iostream>

#ifndef SHIP_H
#define SHIP_H

class Ship {
public:
	std::string name;
	int Length, XFront, YFront, Dir;
	bool DestroyMessageAnnounced;
	// dir: 0 - horizontal, 1 - vertical
	//	-	-	-	-	-
	Ship();
	Ship(int);
};

#endif