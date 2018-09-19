#pragma once
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include "Ship.h"
using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
	vector<vector<int> > mBoard;
	// 0 - water, 1 - ship, 2 - shipDestroyed, 3 - missed
	//	-	-	-	-	-
	Player();
	Player(int, int);
	void DrawYourBoard();
	void DrawEnemyBoard();
	void RevealShip(Ship);
	bool CheckIfCanShoot(int, int);
	bool CheckIfCanShoot(string);
	void Shoot(int, int);
	void Shoot(string);
	string DisplayMessage();
	void ClearMessage();
	bool CheckIfLost();
	int GetWidth() const;
	int GetLength() const;
private:
	int mLength, mWidth, mShipCounter;
	static int counter;
	string mMessage;
	vector<Ship> ships;
	//	-	-	-	-	-
	bool CanShipBePlaced(Ship);
	void PlaceShipsOnBoard();
	void SetColor(int);
	bool CheckIfShipIsDestroyed(Ship);
	void RevealShipsIfDestroyed();
	pair<int, int> StringToIntCoordsConverter(string);
};

#endif