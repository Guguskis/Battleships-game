// Battleships v3.0.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"		//something that Visual studio requires
#include <iostream>	
#include "Player.h"		//player's class
#include <time.h>		//randomizing seed
#include <string>		
#include <Windows.h>	//system()
#include <cwchar>		//changing font size
using namespace std;

/*
	Finished:
		coordinate input (including symbols, mixed input variations, valid input safeguards)
		Single player mode (all main functions should have been implemented)
		Integrate terrain generator
		Implement colors
	To Do:
		improve enemy AI logic in single player
		improve board looks
			possibly animations?

*/

void GameModeSingle();
void SetConsole(int);	//console custom settings initialization
void Debug();
int main() {

	//Debug();

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
void Debug() {
	const int n = 20;
	Player player[20];
	for (int i = 0; i < 20; i++) {
		player[i].DrawEnemyBoard();
		cout << endl;
	}
}
void GameModeSingle() {
	srand((unsigned int)time(NULL));
	Player player[2];
	int playerTurn = 0;
	// 0 - player, 1 - Bot
	/*
		make sound
		cout << (char)(7);
	*/
	do {
		if (playerTurn == 0) {
			//draw boards
			system("cls");
			cout << "           Your battlefield \n";
			player[0].DrawYourBoard();
			cout << "\n          Enemy battlefield \n";
			player[1].DrawEnemyBoard();
			cout << endl;
			//display messages
			if (player[1].DisplayMessage() != "") {
				cout << "You: " + player[1].DisplayMessage();
				//play a sound on hit
				if (player[1].DisplayMessage().find("hit!") + 1 != 0)
					cout << (char)(7);	
			}
			player[1].ClearMessage();
			if (player[0].DisplayMessage() != "") cout << "\nEnemy player: " + player[0].DisplayMessage();
			player[0].ClearMessage();
			//shoot input
			string coords;
			cout << "\nEnter coordinates: ";
			cin >> coords;
			while (!player[1].CheckIfCanShoot(coords)) {
				cout << "\nCannot shoot there. Enter again: ";
				cin >> coords;
			}
			player[1].Shoot(coords);
		}
		//bot logic
		if (playerTurn == 1) {
			int x, y;
			do {
				x = rand() % player[0].GetWidth();
				y = rand() % player[0].GetLength();
			} while (!player[0].CheckIfCanShoot(y, x));
			player[0].Shoot(y, x);
		}
		//alternating player turns
		if (playerTurn == 0) playerTurn = 1;
		else if (playerTurn == 1) playerTurn = 0;

	} while (!player[0].CheckIfLost() && !player[1].CheckIfLost());
	//displaying win or loss message
	system("cls");
	cout << "        Your board: \n";
	player[0].DrawYourBoard();
	cout << "\n\n        Enemy board: \n";
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
void SetConsole(int size) {
	//Sets console size
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left+1080/2, r.top, 500, 900, TRUE);	// width, height

	
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