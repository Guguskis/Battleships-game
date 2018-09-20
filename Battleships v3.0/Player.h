#pragma once
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include "Ship.h"
#include <Windows.h>
#include <algorithm>
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
	void DEBUGTILES();
private:
	int mLength, mWidth, mShipCounter;
	/*
		recommendations for tweaking these settings:
			
	*/
	int mColorSeeds=15, mColorMin = 0, mColorMax = 8, recursionDepth=30;
	static int counter;
	string mMessage;
	vector<Ship> ships;
	struct ColorTile {
		int color;
		bool calculated;
	};
	vector<vector<ColorTile> > mTiles;
	//	-	-	-	-	-
	bool CanShipBePlaced(Ship);
	void PlaceShipsOnBoard();
	void SetColor(string, int);
	bool CheckIfShipIsDestroyed(Ship);
	void RevealShipsIfDestroyed();
	pair<int, int> StringToIntCoordsConverter(string);
	void GenerateColorBoard();
	void GenerateColorBoardRecursion(int, int, int, int);
	int SurroundingTilesAverage(int, int);
	int RandomizeColorTranzitions(int);
};

#endif