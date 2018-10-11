// Battleships v3.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"		//something that Visual studio requires
#include <iostream>	
#include "Player.h"		//player's class
#include "AI.h"			//AI class (extended player's class)
#include <time.h>		//randomizing seed
#include <string>		
#include <math.h>
#include <windows.h>	//system()
#include <algorithm>
#include <cwchar>		//changing font size
using namespace std;

/*
	Finished:
		Coordinate input (including symbols, mixed input variations, valid input safeguards)
		Single player mode (all main functions should have been implemented)
		Integrate terrain generator
		Implement colors
		Improve enemy AI logic in single player
		Allow player to place ships on its own
	To Do:
			

*/


//the whole game
void MainGame();
//the main game logic and game UI
void GameModeSingle();
//initialization
void PlayerPlacingShips(Player &);
//console custom settings initialization
void SetConsole(int);
//debugging function
void Debug();


int main() {
	//Debug();
	MainGame();
}


void MainGame() {
	SetConsole(42);
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
	Player player;
	AI enemy;
	string turn = "player";
	//initializing AI shooting patterns for basic shot
	enemy.Initialize(player);
	enemy.GenerateSea(false, "");
	//asking if player wants to place ships on its own
	player.GenerateSea(true, "Generating your board...");
	PlayerPlacingShips(player);
	//PLAYER BOARD ANIMATION

	do {
		//draw boards
		system("cls");
		Player::DrawBothBoards(player, enemy);

		//handling messages
		if (enemy.DisplayMessage() != "")
			cout << "You: " + enemy.DisplayMessage();
		if (player.DisplayMessage() != "")
			cout << "\nEnemy player: " + player.DisplayMessage();
		enemy.ClearMessage();
		player.ClearMessage();
		cout << "\n\nEnemy ships remaining: " << enemy.GetRemainingShipCount();

		if (turn == "player") {
			//player shoot input
			string coords;
			cout << "\nEnter coordinates: ";
			cin >> coords;
			while (!enemy.CheckIfCanShoot(coords)) {
				cout << "\nCannot shoot there. Enter again: ";
				cin >> coords;
			}
			player.ShootAt(enemy, coords);
			//changing turn if player didn't hit
			if (enemy.DisplayMessage().find("It's a hit!") + 1 == 0)
				turn = "computer";
		}
		else if (turn == "computer") {
			Sleep(500);
			//bot logic
			enemy.ShootAt(player);
			//changing turn if bot didn't hit
			if (player.DisplayMessage().find("It's a hit!") + 1 == 0)
				turn = "player";
		}
	} while (!player.CheckIfLost() && !enemy.CheckIfLost());

	/*
		displaying battle result message
	*/
	system("cls");
	Player::DrawBothBoards(player, enemy);
	cout << endl;

	if (enemy.DisplayMessage() != "") cout << "You: " + enemy.DisplayMessage();
	enemy.ClearMessage();
	if (player.DisplayMessage() != "") cout << "\nEnemy player: " + player.DisplayMessage();
	player.ClearMessage();

	if (!player.CheckIfLost()) cout << "\n\nCongratulations, you have won!";
	else cout << "\n\nYou have lost!";
}
void Debug() {
	Player player(30, 60);
	player.GenerateSea(true, "");

	cin.get();

}
void PlayerPlacingShips(Player &player) {
	//this function lets player to place ships or generates randomly
	bool generateRandomly;
	string answer;
	//getting player's input
	cout << "Generate your ships randomly? (y/n): ";
	cin >> answer;

	while(!(answer=="y" || answer=="Y" || answer=="n" || answer=="N")){
		cout << "\nWrong input. Enter again: ";
		cin >> answer;
	}
	if (tolower(answer[0]) == 'y') generateRandomly = true;
	else generateRandomly = false;

	//placing ships
	if (generateRandomly) player.PlaceShipsOnBoard();
	else {
		int remainingShips = player.ships.size();
		while (remainingShips != 0) {
			//initializing variables
			Ship *pShip = &player.ships[player.ships.size() - remainingShips];
			system("cls");
			string coords, direction;

			//UI
			cout << "\n";
			player.DrawYourBoard();
			cout << "Ships placed " << player.ships.size() - remainingShips << "/" << player.ships.size();
			cout << "\nPlacing " << pShip->name << " (length " << pShip->Length << ")";
			cout << "\n\nEnter coordinates and ship direction\nH - horizontal\nV - vertical";
			cout << "\nExamples: A5 H, 6b V, 0c v\n";
			cin >> coords >> direction;

			//assigning input to ship variables
			pShip->YFront = player.StringToIntCoordsConverter(coords).first;
			pShip->XFront = player.StringToIntCoordsConverter(coords).second;
			if (tolower(direction[0]) == 'h') pShip->Dir = 0;
			else if (tolower(direction[0]) =='v') pShip->Dir = 1;
			else pShip->Dir = 2; //handling unspecified direction

			//checking if input is valid:
			while (	pShip->Dir==2 ||				//unspecified dir
					!player.CanShipBePlaced(*pShip) //cannot  place ship at these coordinates
				) {
				cout << "Cannot place there. Enter again: ";
				cin >> coords >> direction;

				pShip->YFront = player.StringToIntCoordsConverter(coords).first;
				pShip->XFront = player.StringToIntCoordsConverter(coords).second;
				if (tolower(direction[0]) == 'h') pShip->Dir = 0;
				else if (tolower(direction[0]) == 'v') pShip->Dir = 1;
				else pShip->Dir = 2;
			}
			//placing the ship after making sure input is valid
			//horizontal
			if (pShip->Dir == 0)
				for (int j = pShip->XFront; j < pShip->XFront + pShip->Length; j++)
					player.mBoard[pShip->YFront][j] = 1;
			//vertical
			else
				for (int j = pShip->YFront; j < pShip->YFront + pShip->Length; j++)
					player.mBoard[j][pShip->XFront] = 1;
			remainingShips--;
		}
	}
}
void SetConsole(int size) {
	//Sets console size
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, 505, 0, 440, 380, TRUE);	// width, height


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
