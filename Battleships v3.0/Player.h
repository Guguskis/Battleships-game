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
	//constructors
	Player();
	Player(int, int);
	//UI functions
	void DrawYourBoard();
	void DrawEnemyBoard();
	
	void RevealShip(Ship);	//reveal tiles around ship after it is destroyed
	//cheking if input is valid
	bool CheckIfCanShoot(int, int);
	bool CheckIfCanShoot(std::string);
	//shoot logic
	void Shoot(int, int);
	void Shoot(std::string);
	
	std::string DisplayMessage();	//displays hit/miss/destroyed message
	void ClearMessage();
	
	bool CheckIfLost();		//checks if player has lost
	//returns board parameters
	int GetWidth() const;
	int GetLength() const;
	int GetRemainingShipCount();
	void DEBUG();
private:
	int mLength, mWidth, mShipCounter;
	/*
		recursion depth: the radius of how far recursion will work around the seed
		mColorSeeds: number of seeds that will be placed on map
		mColorMin - mColorMax: color values range. Should not be changed, because wider range is not supported yet
		counter: instantiated Player class object count
		mMessage: contains shooting result information like hit/miss/destroyed
		ships: contains all data about player's ships
		ColorTile: is used in color board generation
		mTiles: color board array
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
	
	bool CanShipBePlaced(Ship);	//checks whether ship can be placed at given coordinates
	//one of main functions that contains smaller functions to handle ship placement
	void PlaceShipsOnBoard();
	bool CheckIfShipIsDestroyed(Ship);	//supporting method
	//main function to through cycle all the ships and reveal destroyed ones 
	void RevealShipsIfDestroyed();
	std::pair<int, int> StringToIntCoordsConverter(std::string);	//supporting function to convert input
	//Functions to initialize board terrain
	void GenerateColorBoard();
	void GenerateColorBoardRecursion(int, int, int, int);
	int SurroundingTilesAverage(int, int);
	int RandomizeColorTranzitions(int);
	void SetColor(std::string, int);	//supporting method that changes current tile's color
};

#endif
