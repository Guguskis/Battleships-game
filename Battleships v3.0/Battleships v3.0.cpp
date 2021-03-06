// Battleships v3.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"		//something that Visual studio requires
#include <iostream>	
#include "Player.h"		//player's class
#include "AI.h"			//AI class (extended player's class)
#include <ctime>		//randomizing seed
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
//detecting a click and returning cursor coordinates
pair<int, int> RegisterClick();
clock_t lastShotTimer;//

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
	int invalidClicks = 0;//used in the minigame
	bool minigameWon = false;
	lastShotTimer = 0;


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
		if (!minigameWon)
			Player::DrawBothBoards(player, enemy);
		else
			Player::DrawBothBoards(player, enemy, "123"); //this is drawn when player completed minigame
		//*************************************************************************
		//handling messages
		if (enemy.DisplayMessage() != "")
			cout << "You: " + enemy.DisplayMessage();
		if (player.DisplayMessage() != "")
			cout << "Enemy player: " + player.DisplayMessage();
		enemy.ClearMessage();
		player.ClearMessage();
		cout << "\n\nEnemy ships remaining: " << enemy.GetRemainingShipCount();
		//*************************************************************************
		//main logic
		if (turn == "player") {
			//restarting counter after valid shot
			invalidClicks = 0;
			//player shoot input
			pair<int, int> coords;
			cout << "\nClick where you want to shoot\n";
			coords = RegisterClick();
			while (!enemy.CheckIfCanShoot(coords.first - 2, coords.second - 29)) {
				invalidClicks++;
				//redrawing board every 5 missed shots in a row
				if (invalidClicks % 5 == 0) {
					system("cls");
					if (!minigameWon)
						Player::DrawBothBoards(player, enemy);
					else
						Player::DrawBothBoards(player, enemy, "I'm too lazy to make this nice, I'm just gonna overload this method to show enemy board");
				}
				//redrawing board after unlocking the achievement
				else if (minigameWon) {
					system("cls");
					Player::DrawBothBoards(player, enemy, "123");
				}

#pragma region minigame
				if (invalidClicks == 5) cout << "\nStop messing around.";
				else if (invalidClicks == 10) cout << "\nI said stop it!";
				else if (invalidClicks == 15) cout << "\nCan't you read?!";
				else if (invalidClicks == 20) cout << "\nYou are wasting your time...";
				else if (invalidClicks == 25) cout << "\n... precious time...";
				else if (invalidClicks == 30) cout << "\n... my precio.. Just stop it, alright?";
				else if (invalidClicks == 35) cout << "\n35 misses is not an accident >.<";
				else if (invalidClicks == 40) cout << "\nOkay, okay. Why not just reach 50?";
				else if (invalidClicks > 40 && invalidClicks < 50) cout << endl << 50 - invalidClicks;
				else if (invalidClicks == 50) cout << "\nCongratulations, you have earned an achievement! You are like a movie baddie or something. Now stop it.";
				else if (invalidClicks == 55) cout << "\n...So you are still here... get some help :|";
				else if (invalidClicks == 70) cout << "\nYou are now blinking manually.";
				else if (invalidClicks == 75) cout << "\nYou are now BREATHING manually";
				else if (invalidClicks == 80) cout << "\nWell, you know what? Let's make a deal. If you reach 111 without blinking, you will win! Okay?";
				else if (invalidClicks == 85) cout << "\nBegin!";
				else if (invalidClicks == 111) {
					minigameWon = true;
					cout << "\nHere it is, the whole 111 missed shots. You wasted 111 seconds of your life, but you won.. Happy?";
					cout << "\nHere's a little surprise for you :)";
					Sleep(1500);
				}
				else cout << "\nCannot shoot there.";
#pragma endregion minimage
				if (invalidClicks != 111)
					coords = RegisterClick();
			}
			if (invalidClicks != 111)
				player.ShootAt(enemy, coords.first - 2, coords.second - 29);
			//quick fix to disable clicking pause after a ship was hit
			lastShotTimer = -10000;
			//changing turn if player missed
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
	//*************************************************************************
	//displaying battle result message
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
	while (1) {
		cout << "a";
		while (1) {
			break;
		}
		cout << "b";
	}
}
void PlayerPlacingShips(Player &player) {
	//this function lets player to place ships or generates randomly
	bool generateRandomly;
	string answer;
	//getting player's input
	cout << "Generate your ships randomly? (y/n): ";
	cin >> answer;

	while (!(answer == "y" || answer == "Y" || answer == "n" || answer == "N")) {
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
			else if (tolower(direction[0]) == 'v') pShip->Dir = 1;
			else pShip->Dir = 2; //handling unspecified direction

			//checking if input is valid:
			while (pShip->Dir == 2 ||				//unspecified dir
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
	MoveWindow(console, 505, 0, 1100, 1025, TRUE);	// width, height


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
std::pair<int, int> RegisterClick() {
	int clickLatency = 1000;

	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD InputRecord;
	DWORD Events;
	COORD coord;
	CONSOLE_CURSOR_INFO cci;

	cci.bVisible = false;
	SetConsoleCursorInfo(hout, &cci);
	SetConsoleMode(hin, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	while (1) {
		ReadConsoleInput(hin, &InputRecord, 1, &Events);
		switch (InputRecord.EventType) {
		case MOUSE_EVENT:
			if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED && clock() - lastShotTimer > clickLatency) {
				lastShotTimer = clock();
				return std::make_pair(InputRecord.Event.MouseEvent.dwMousePosition.Y, InputRecord.Event.MouseEvent.dwMousePosition.X);
			}
		}
	}
}