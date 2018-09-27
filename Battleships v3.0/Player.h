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


#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
	std::vector<std::vector<int> > mBoard;
	// 0 - water, 1 - ship, 2 - shipDestroyed, 3 - missed
	//	-	-	-	-	-
	Player();
	Player(int, int);
	void DrawYourBoard();
	void DrawEnemyBoard();
	void RevealShip(Ship);
	bool CheckIfCanShoot(int, int);
	bool CheckIfCanShoot(std::string);
	void Shoot(int, int);
	void Shoot(std::string);
	std::string DisplayMessage();
	void ClearMessage();
	bool CheckIfLost();
	int GetWidth() const;
	int GetLength() const;
	int GetRemainingShipCount();
private:
	int mLength, mWidth, mShipCounter;
	/*
		recursion depth: the radius of how far recursion will work around the seed
	*/
	int mColorSeeds = 3, mColorMin = 0, mColorMax = 8, recursionDepth = 4;
	static int counter;
	std::string mMessage;
	std::vector<Ship> ships;
	struct ColorTile {
		int color;
		bool calculated;
	};
	std::vector<std::vector<ColorTile> > mTiles;
	//	-	-	-	-	-
	bool CanShipBePlaced(Ship);
	void PlaceShipsOnBoard();
	void SetColor(std::string, int);
	bool CheckIfShipIsDestroyed(Ship);
	void RevealShipsIfDestroyed();
	std::pair<int, int> StringToIntCoordsConverter(std::string);
	//Functions to initialize board terrain
	void GenerateColorBoard();
	void GenerateColorBoardRecursion(int, int, int, int);
	int SurroundingTilesAverage(int, int);
	int RandomizeColorTranzitions(int);
};

#endif
