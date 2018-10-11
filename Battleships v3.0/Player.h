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
#include "Topography.h"
#include <iomanip>

#ifndef PLAYER_H
#define PLAYER_H

class Player {
public:
	std::vector<Ship> ships;
	std::vector<std::vector<int> > mBoard;
	/*
		mBoard values explanation:
		0 - water
		1 - ship
		2 - destroyed ship
		3 - missed
	*/
private:
	int mLength, mWidth, mShipCounter;
	/*
		mColorSeeds: number of seeds that will be placed on map
		mColorMin - mColorMax: color values range (should not exceed bounds [0,8])
		recursion depth: the radius of how far recursion will work around the seed
		counter: counts how many times Player object was created
		mMessage: contains shooting result information like hit/miss/destroyed
		ColorTile: is used in color board generation
		mTiles: two dimensional vector representing color board (Sea depth)
	*/
	static int counter;
	std::string mMessage;
	std::vector<std::vector<int> > mSea;
public:
	//constructors
	Player();
	Player(int, int);

	//UI methods
	void DrawYourBoard();
	void DrawEnemyBoard();
	static void DrawBothBoards(Player, Player);
	static void DrawBothBoards(Player, Player, std::string);
	void SetColor(std::string, int);	//supporting method that changes current tile's color
	//cheking if input is valid
	bool CheckIfCanShoot(int, int);
	bool CheckIfCanShoot(std::string);
	//shoot logic
	void Shoot(int, int);	//main logic
	void Shoot(std::string);	//converting input from string to int and then calling Shoot();
	void ShootAt(Player&, int, int);
	void ShootAt(Player &, std::string);	//added for clarity when calling Shoot method from outside of class
	//handling message system
	std::string DisplayMessage();
	void ClearMessage();
	//returns board parameters
	int GetWidth() const;
	int GetLength() const;
	int GetRemainingShipCount();
	//one of main methods that contains smaller methods to handle ship placement
	void PlaceShipsOnBoard();
	//small supporting functions
	bool CheckIfLost();
	void RevealShip(Ship);	//reveal tiles around ship after it is destroyed
	bool CanShipBePlaced(Ship);	//checks whether ship can be placed at given coordinates
	std::pair<int, int> StringToIntCoordsConverter(std::string);	//supporting function to convert input
	//generate and animate topography
	void GenerateSea(bool, std::string);
	void InstaKill();
	void DEBUG();
private:

	//main function to cycle through all the ships and reveal destroyed ones 
	void RevealShipsIfDestroyed();
	//smaller supporting methods
	bool CheckIfShipIsDestroyed(Ship);	//supporting method
};

#endif