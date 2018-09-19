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
	To Do:
		improve enemy AI logic in single player
*/

void Initialization(int &);	//to finish
void GameModeSingle();
void GameModeTwo();			//do when got time
int AlternatePlayer(int);	//finished
void SetConsole(int);		//console custom settings initialization
int main() {
	SetConsole(28);
	bool gameStatus = true;
	while (gameStatus) {
		system("cls");
		GameModeSingle();
		cout << "\n\n Do you want to play again? y/n: ";
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

void Initialization(int &gameMode) {
	srand((unsigned int)time(NULL));
	cout << "1 player\n2 player\n\nChoose gameplay mode: ";
	cin >> gameMode;
}
void GameModeSingle() {
	srand((unsigned int)time(NULL));
	Player player[2];
	int playerTurn = 0;
	// 0 - player, 1 - Bot

	do {
		if (playerTurn == 0) {
			system("cls");
			cout << "Your board: \n";
			player[0].DrawYourBoard();
			cout << "\n\nEnemy board: \n";
			player[1].DrawEnemyBoard();
			cout << endl;
			if (player[1].DisplayMessage() != "") cout << "You: " + player[1].DisplayMessage();
			player[1].ClearMessage();
			if (player[0].DisplayMessage() != "") cout << "\nEnemy player: " + player[0].DisplayMessage();
			player[0].ClearMessage();


			string coords;
			cout << "\nEnter coordinates: ";
			cin >> coords;
			while (!player[1].CheckIfCanShoot(coords)) {
				cout << "\nCannot shoot there. Enter again: ";
				cin >> coords;
			}
			player[1].Shoot(coords);

		}
		if (playerTurn == 1) {
			int x, y;
			do {
				x = rand() % player[0].GetWidth();
				y = rand() % player[0].GetLength();
			} while (!player[0].CheckIfCanShoot(y, x));
			player[0].Shoot(y, x);
		}
		playerTurn = AlternatePlayer(playerTurn);

	} while (!player[0].CheckIfLost() && !player[1].CheckIfLost());
	
	system("cls");
	cout << "Your board: \n";
	player[0].DrawYourBoard();
	cout << "\n\nEnemy board: \n";
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
void GameModeTwo() {

}
int AlternatePlayer(int playerTurn) {
	if (playerTurn == 0) return 1;
	if (playerTurn == 1) return 0;
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