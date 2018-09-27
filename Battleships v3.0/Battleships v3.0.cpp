// Battleships v3.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"		//something that Visual studio requires
#include <iostream>	
#include "Player.h"		//player's class
#include <time.h>		//randomizing seed
#include <string>		
#include <math.h>
#include <windows.h>	//system()
#include <algorithm>
#include <cwchar>		//changing font size
using namespace std;

/*
	Finished:
		coordinate input (including symbols, mixed input variations, valid input safeguards)
		Single player mode (all main functions should have been implemented)
		Integrate terrain generator
		Implement colors
		improve enemy AI logic in single player
	To Do:
		Allow player to place ships on its own

*/

struct AI {
	//there's more info in BotLogic function at mode "basic"
	vector<pair<int, int> > basicShotComm[2];
	//0123->NESW
	int lastShotDir;			
	//modes: basic, locking, loading
	string mode = "basic";		
	//first - y, second - x
	pair<int, int> shootCoords; 

	void InitializeBasicShot(Player player) {
		for (int i = 0; i < player.GetWidth(); i++) {
			if (i % 2 == 0) {
				for (int j = 0; j < player.GetLength(); j++) {
					if (j % 2 == 0)
						basicShotComm[0].push_back(make_pair(i, j));
					else
						basicShotComm[1].push_back(make_pair(i, j));
				}
			}
			else {
				for (int j = 1; j < player.GetLength(); j++)
					if (j % 2 == 0)
						basicShotComm[1].push_back(make_pair(i, j));
					else
						basicShotComm[0].push_back(make_pair(i, j));
			}
		}
	}
};

void GameModeSingle();
Player BotLogic(Player, AI&);
void SetConsole(int);	//console custom settings initialization
void Debug();
void MainGame();


int main() {

	/*
		the reason why I placed everything int otwo functions is to 
		make it easier to debug by commenting only one line
	*/

	//Debug();
	MainGame();
}


void MainGame() {
	SetConsole(28);
	bool gameStatus = true;
	while (gameStatus) {
		system("cls");
		GameModeSingle();
		cout << "\n\nDo you want to play again? y/n: ";
		string answer;
		cin >> answer;
		while (answer.length() != 1 || !(tolower(answer[0]) == 'n' || tolower(answer[0] == 'y'))) {
			cout << "That is not a valid option. Enter again: ";
			cin >> answer;
		}
		if (tolower(answer[0]) == 'n')
			gameStatus = false;
	}
}
void GameModeSingle() {
	srand((unsigned int)time(NULL));
	Player player[2];
	// 0 - player, 1 - Bot
	//bot AI variables
	AI bot;
	bot.InitializeBasicShot(player[0]);
	do {
		/*
			player logic
		*/
		//draw boards
		system("cls");
		cout << "             Your battlefield \n";
		player[0].DrawYourBoard();
		cout << "\n            Enemy battlefield \n";
		player[1].DrawEnemyBoard();
		cout << endl;
		//display messages
		if (player[1].DisplayMessage() != "") {
			cout << "You: " + player[1].DisplayMessage();
			//play a sound on hit
			/*if (player[1].DisplayMessage().find("hit!") + 1 != 0)
				cout << (char)(7);	*/
		}
		player[1].ClearMessage();
		if (player[0].DisplayMessage() != "") cout << "\nEnemy player: " + player[0].DisplayMessage();
		player[0].ClearMessage();
		cout << "\n\nEnemy ships remaining: " << player[1].GetRemainingShipCount();
		//shoot input
		string coords;
		cout << "\nEnter coordinates: ";
		cin >> coords;
		while (!player[1].CheckIfCanShoot(coords)) {
			cout << "\nCannot shoot there. Enter again: ";
			cin >> coords;
		}
		player[1].Shoot(coords);
		/*
			bot logic
		*/

		player[0] = BotLogic(player[0], bot);
	} while (!player[0].CheckIfLost() && !player[1].CheckIfLost());

	//displaying win or loss message
	system("cls");
	cout << "          Your board: \n";
	player[0].DrawYourBoard();
	cout << "\n\n          Enemy board: \n";
	player[1].DrawEnemyBoard();
	cout << endl;
	if (player[1].DisplayMessage() != "") cout << "You: " + player[1].DisplayMessage();
	player[1].ClearMessage();
	if (player[0].DisplayMessage() != "") cout << "\nEnemy player: " + player[0].DisplayMessage();
	player[0].ClearMessage();

	if (!player[0].CheckIfLost())
		cout << "\n\nCongratulations, you have won!";
	else cout << "\n\nYou have lost!";
}
Player BotLogic(Player player, AI &bot) {
	/*
		basic shot: shoot randomly in specific pattern
		locking: shoot around a specific tile after basic shot hits
		loading: shoot in a straight line after ship's direction in "locking" mode is found out
	*/
	int y = bot.shootCoords.first, x = bot.shootCoords.second;

	if (bot.mode == "basic") {
		int y, x;
		//since shit might be sunk and some shots could not be fired, cycle again
		do {
			if (bot.basicShotComm[0].size() != 0) {
				/* Aiming pattern:
					x.x.x.x
					.x.x.x.		*/
				int commandIndex = rand() % bot.basicShotComm[0].size();
				y = bot.basicShotComm[0][commandIndex].first;
				x = bot.basicShotComm[0][commandIndex].second;
				bot.basicShotComm[0].erase(bot.basicShotComm[0].begin() + commandIndex);
			}
			else {
				/*Aiming pattern:
					.x.x.x.
					x.x.x.x		*/
				int commandIndex = rand() % bot.basicShotComm[1].size();
				y = bot.basicShotComm[1][commandIndex].first;
				x = bot.basicShotComm[1][commandIndex].second;
				bot.basicShotComm[1].erase(bot.basicShotComm[1].begin() + commandIndex);
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
					bot.lastShotDir = i;
					break;
				}
			}
			bot.shootCoords.first = y;
			bot.shootCoords.second = x;
			bot.mode = "locking";
		}
	}
	else if (bot.mode == "locking") {
		//cycling around original destroyed ship tile until a valid shot can be fired
		for(int i=0; i<4; i++){
			if (i!=0)
				bot.lastShotDir = (bot.lastShotDir + 1) % 4;
			y = bot.shootCoords.first;
			x = bot.shootCoords.second;
			
			switch (bot.lastShotDir) {
				case 0: y--; break;
				case 1: x++; break;
				case 2: y++; break;
				case 3: x--; break;
			}
			if (player.CheckIfCanShoot(y, x)) break;
			if (i == 3) {
				cout << "\nLocking shot could not be fired. Changing mode to basic, but skipping this shot\n";
				bot.mode = "basic";
			}
		}
		if (player.CheckIfCanShoot(y, x))
			player.Shoot(y, x);
		//swithing mode if ship is hit
		if (player.DisplayMessage().find("It's a hit!") + 1 != 0)
			bot.mode = "loading";

	}
	else if (bot.mode == "loading") {
		//shoots in a line at specific direction and switches to opposite direction after
		//ship's end is reached
		bool canShoot = false;
		while (!canShoot) {
			//North
			if (bot.lastShotDir == 0) {
				//shooting in a straight line until miss or out of bounds
				for (int i = y; i >= 0; i--) {
					if (player.CheckIfCanShoot(i, x)) {
						y = i;
						canShoot = true;
						break; 
					}
					else if (player.mBoard[i][x] != 2) break;
				}
			}
			//East
			else if (bot.lastShotDir == 1) {
				for (int i = x; i <player.GetLength(); i++) {
					if (player.CheckIfCanShoot(y, i)) {
						x = i;
						canShoot = true;
						break;
					}
					else if (player.mBoard[y][i] != 2) break;
				}
			}
			//South
			else if (bot.lastShotDir == 2) {
				for (int i = y; i <player.GetWidth(); i++) {
					if (player.CheckIfCanShoot(i, x)) {
						y = i;
						canShoot = true;
						break;
					}
					else if (player.mBoard[i][x] != 2) break;
				}
			}
			//West
			else if (bot.lastShotDir == 3) {
				for (int i = x; i >= 0; i--) {
					if (player.CheckIfCanShoot(y, i)) {
						x = i;
						canShoot = true;
						break;
					}
					else if (player.mBoard[y][i] != 2) break;
				}
			}
			if (canShoot)
				player.Shoot(y, x);
			//switching direction to opposite if end of the ship was reached
			else
				bot.lastShotDir = (bot.lastShotDir + 2) % 4;
		}
	}
	else {
		cout << "\nBot mode not specified.\n";
	}

	//if a ship was destroyed switching back to basic mode
	if (player.DisplayMessage().find("is destroyed!") + 1 != 0)
		bot.mode = "basic";

	return player;
}
void Debug() {
	string temp;
	SetConsole(28);
	Player player(10, 10);
	AI bot;
	bot.InitializeBasicShot(player);
	while (!player.CheckIfLost()) {
		system("cls");
		player = BotLogic(player, bot);
		player.DrawYourBoard();
		cout << "Message: " << player.DisplayMessage();
		player.ClearMessage();
		cin >> temp;
	}
}
void SetConsole(int size) {
	//Sets console size
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left + 505, r.top, 500, 900, TRUE);	// width, height


	//Sets console font
	//28 is the optimal size
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX();
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
	lpConsoleCurrentFontEx->dwFontSize.X = size;
	lpConsoleCurrentFontEx->dwFontSize.Y = size;
	SetCurrentConsoleFontEx(out, 0, lpConsoleCurrentFontEx);
}
int Player::GetRemainingShipCount() {
	return mShipCounter;
}