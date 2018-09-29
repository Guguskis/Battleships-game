#include "stdafx.h"
#include "AI.h"


AI::AI(){
	Player::Player();
}
void AI::Initialize(Player player) {
	for (int i = 0; i < player.GetWidth(); i++) {
		if (i % 2 == 0) {
			for (int j = 0; j < player.GetLength(); j++) {
				if (j % 2 == 0)
					basicShotComm[0].push_back(std::make_pair(i, j));
				else
					basicShotComm[1].push_back(std::make_pair(i, j));
			}
		}
		else {
			for (int j = 1; j < player.GetLength(); j++)
				if (j % 2 == 0)
					basicShotComm[1].push_back(std::make_pair(i, j));
				else
					basicShotComm[0].push_back(std::make_pair(i, j));
		}
	}
}
void AI::ShootAt(Player &player) {
	if (mode == "basic") Basic(player);
	else if (mode == "locking") Locking(player);
	else if (mode == "loading") Loading(player);
	else std::cout << "\nBot mshooting mode not specified.\n";

	//if a ship was destroyed switching back to basic mode
	if (player.DisplayMessage().find("is destroyed!") + 1 != 0)
		mode = "basic";
}
void AI::Basic(Player&player) {
	//since shit might be sunk and some shots could not be fired, cycle again
	do {
		if (basicShotComm[0].size() != 0) {
			/* Aiming pattern:
				x.x.x.x
				.x.x.x.		*/
			int commandIndex = rand() % basicShotComm[0].size();
			y = basicShotComm[0][commandIndex].first;
			x = basicShotComm[0][commandIndex].second;
			basicShotComm[0].erase(basicShotComm[0].begin() + commandIndex);
		}
		else {
			/*Aiming pattern:
				.x.x.x.
				x.x.x.x		*/
			int commandIndex = rand() % basicShotComm[1].size();
			y = basicShotComm[1][commandIndex].first;
			x = basicShotComm[1][commandIndex].second;
			basicShotComm[1].erase(basicShotComm[1].begin() + commandIndex);
		}
	} while (!player.CheckIfCanShoot(y, x));

	player.Shoot(y, x);
	//enabling locking mode if ship was hit AND not destroyed
	if (player.DisplayMessage().find("It's a hit!") + 1 != 0 &&
		player.DisplayMessage().find("is destroyed!") + 1 == 0) {
		/*
			initializing hunting mode variables
		*/
		//deducting at which direction to shoot first (the one with the most water tiles in
		//that direction (max countLength) will be chosen
		int countLength = 4;
		int countWaterTiles[4] = { 0, 0, 0, 0 };
		//North
		for (int i = y - 1; i != y - countLength - 1; i--) {
			if (i >= 0 && i < player.GetWidth()) {
				if (player.mBoard[i][x] == 0 || player.mBoard[i][x] == 1)
					countWaterTiles[0]++;
				else break;
			}
		}
		//East
		for (int i = x + 1; i != x + countLength + 1; i++) {
			if (i >= 0 && i < player.GetLength()) {
				if (player.mBoard[y][i] == 0 || player.mBoard[y][i] == 1)
					countWaterTiles[1]++;
				else break;
			}
		}
		//South
		for (int i = y + 1; i != y + countLength + 1; i++) {
			if (i >= 0 && i < player.GetWidth()) {
				if (player.mBoard[i][x] == 0 || player.mBoard[i][x] == 1)
					countWaterTiles[2]++;
				else break;
			}
		}
		//West
		for (int i = x - 1; i != x - countLength - 1; i--) {
			if (i >= 0 && i < player.GetLength()) {
				if (player.mBoard[y][i] == 0 || player.mBoard[y][i] == 1)
					countWaterTiles[3]++;
				else break;
			}
		}
		//finding the best direciton
		for (int i = 0; i < 4; i++) {
			if (countWaterTiles[i] == *std::max_element(countWaterTiles, countWaterTiles + 4)) {
				lastShotDir = i;
				break;
			}
		}
		mode = "locking";
	}
}
void AI::Locking(Player &player) {
	int tempY , tempX;
	//cycling around original destroyed ship tile until a valid shot can be fired
	for (int i = 0; i < 4; i++) {
		//not rotating on first iteration
		if (i != 0)
			lastShotDir = (lastShotDir + 1) % 4;
		tempY = y;
		tempX = x;
		switch (lastShotDir) {
		case 0: tempY--; break;
		case 1: tempX++; break;
		case 2: tempY++; break;
		case 3: tempX--; break;
		}
		if (player.CheckIfCanShoot(tempY, tempX)) break;
		if (i == 3) {
			std::cout << "\nLocking shot could not be fired. Changing mode to basic, but skipping this shot\n";
			mode = "basic";
		}
	}
	if (player.CheckIfCanShoot(tempY, tempX))
		player.Shoot(tempY, tempX);
	//swithing mode if ship is hit
	if (player.DisplayMessage().find("It's a hit!") + 1 != 0)
		mode = "loading";
}
void AI::Loading(Player &player) {
	int tempY = y, tempX = x;
	//shoots in a line at specific direction and switches to opposite direction after
	//ship's end is reached
	bool canShoot = false;
	while (!canShoot) {
		//North
		if (lastShotDir == 0) {
			//shooting in a straight line until miss or out of bounds
			for (int i = tempY; i >= 0; i--) {
				if (player.CheckIfCanShoot(i, tempX)) {
					tempY = i;
					canShoot = true;
					break;
				}
				else if (player.mBoard[i][tempX] != 2) break;
			}
		}
		//East
		else if (lastShotDir == 1) {
			for (int i = tempX; i < player.GetLength(); i++) {
				if (player.CheckIfCanShoot(tempY, i)) {
					tempX = i;
					canShoot = true;
					break;
				}
				else if (player.mBoard[tempY][i] != 2) break;
			}
		}
		//South
		else if (lastShotDir == 2) {
			for (int i = tempY; i < player.GetWidth(); i++) {
				if (player.CheckIfCanShoot(i, tempX)) {
					tempY = i;
					canShoot = true;
					break;
				}
				else if (player.mBoard[i][tempX] != 2) break;
			}
		}
		//West
		else if (lastShotDir == 3) {
			for (int i = tempX; i >= 0; i--) {
				if (player.CheckIfCanShoot(tempY, i)) {
					tempX = i;
					canShoot = true;
					break;
				}
				else if (player.mBoard[tempY][i] != 2) break;
			}
		}
		if (canShoot)
			player.Shoot(tempY, tempX);
		//switching direction to opposite if end of the ship was reached
		else
			lastShotDir = (lastShotDir + 2) % 4;
	}
}