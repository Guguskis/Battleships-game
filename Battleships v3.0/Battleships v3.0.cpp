// Battleships v3.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"		//something that Visual studio requires
#include <iostream>	
#include "Player.h"		//player's class
#include "AI.h"
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


//the main game logic and UI
void GameModeSingle();
//console custom settings initialization
void SetConsole(int);
//debugging function
void Debug();
//the whole game logic
void MainGame();


int main() {

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
	Player player;
	AI enemy;

	//initializing AI shooting patterns for basic shot
	enemy.Initialize(player);

	do {
		//draw boards
		system("cls");
		cout << "             Your battlefield \n";
		player.DrawYourBoard();
		cout << "\n            Enemy battlefield \n";
		enemy.DrawEnemyBoard();

		//handling messages
		if (enemy.DisplayMessage() != "")
			cout << "You: " + enemy.DisplayMessage();
		if (player.DisplayMessage() != "")
			cout << "\nEnemy player: " + player.DisplayMessage();
		enemy.ClearMessage();
		player.ClearMessage();
		cout << "\n\nEnemy ships remaining: " << enemy.GetRemainingShipCount();

		//player shoot input
		string coords;
		cout << "\nEnter coordinates: ";
		cin >> coords;
		while (!enemy.CheckIfCanShoot(coords)) {
			cout << "\nCannot shoot there. Enter again: ";
			cin >> coords;
		}
		enemy.Shoot(coords);

		//bot logic
		enemy.ShootAt(player);

	} while (!player.CheckIfLost() && !enemy.CheckIfLost());

	//displaying win or loss message
	system("cls");
	cout << "          Your board: \n";
	player.DrawYourBoard();
	cout << "\n\n          Enemy board: \n";
	enemy.DrawEnemyBoard();
	cout << endl;
	if (enemy.DisplayMessage() != "") cout << "You: " + enemy.DisplayMessage();
	enemy.ClearMessage();
	if (player.DisplayMessage() != "") cout << "\nEnemy player: " + player.DisplayMessage();
	player.ClearMessage();

	if (!player.CheckIfLost())
		cout << "\n\nCongratulations, you have won!";
	else cout << "\n\nYou have lost!";
}
void Debug() {
	/*string temp;
	SetConsole(24);
	Player player;
	AI bot;
	bot.Initialize(player);
	while (!player.CheckIfLost()) {
		system("cls");
		player.DrawYourBoard();
		cout << "Message: " << player.DisplayMessage();
		player.ClearMessage();
		bot.ShootAt(player);
		cin >> temp;
	}*/
	vector<Player> vect;
	string temp;
	while (1) {
		while (1) {
			vect.push_back(Player());
			Sleep(0);
			if (GetAsyncKeyState(VK_SPACE)) break;
		}
	vect.clear();
	cout << "Broke";

	}
	cin >> temp;

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