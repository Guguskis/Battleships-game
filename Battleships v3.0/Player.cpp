#include "stdafx.h"
#include "Player.h"

int Player::counter = 0;

Player::Player() {
	counter++;
	mMessage = "";
	mWidth = mLength = 10;
	vector<int> tempLength;
	for (int i = 0; i<mLength; i++)
		tempLength.push_back(0);
	for (int i = 0; i<mWidth; i++)
		mBoard.push_back(tempLength);
	//generating ships
	ships.push_back(4);
	for (int i = 0; i<2; i++)
		ships.push_back(3);
	for (int i = 0; i<3; i++)
		ships.push_back(2);
	for (int i = 0; i<4; i++)
		ships.push_back(1);
	PlaceShipsOnBoard();
	mShipCounter = ships.size();
}
Player::Player(int width, int length) {
	counter++;
	mMessage = "";
	mWidth = width;
	mLength = length;
	vector<int> tempLength;
	for (int i = 0; i<mLength; i++)
		tempLength.push_back(0);
	for (int i = 0; i<mWidth; i++)
		mBoard.push_back(tempLength);
	//generating ships
	ships.push_back(4);
	for (int i = 0; i<2; i++)
		ships.push_back(3);
	for (int i = 0; i<3; i++)
		ships.push_back(2);
	for (int i = 0; i<4; i++)
		ships.push_back(1);
	PlaceShipsOnBoard();
	mShipCounter = ships.size();
}
void Player::DrawYourBoard() {
	cout << "   ";
	for (int i = 0; i < mLength; i++)
		cout << i;
	cout << endl;
	for (int i = 0; i<mWidth; i++) {
		cout << " " << (char)('A' + i) << " ";
		for (int j = 0; j<mLength; j++) {
			switch (mBoard[i][j]) {
			case 0: cout << "~"; break;
			case 1: cout << "O"; break;
			case 2: cout << "X"; break;
			case 3: cout << "."; break;
			default: cout << "Tile not specified in Player.DrawBoardAll();"; break;
			}
		}
		cout << " " << (char)('A'+i) << endl;
	}
	cout << "   ";
	for (int i = 0; i < mLength; i++)
		cout << i;
	cout << endl;
}
void Player::DrawEnemyBoard() {
	//TODO: add coordinates for shooting guidance
	cout << "   ";
	for (int i = 0; i < mLength; i++)
		cout << i;
	cout << endl;
	for (int i = 0; i<mWidth; i++) {
		cout << " " << (char)('A' + i) << " ";
		for (int j = 0; j<mLength; j++) {
			switch (mBoard[i][j]) {
			case 0: cout << "~"; break;
			case 1: cout << "~"; break;
			case 2: cout << "X"; break;
			case 3: cout << "."; break;
			default: cout << "Tile not specified in Player.DrawBoardAll();"; break;
			}
		}
		cout << " " << (char)('A' + i) << endl;
	}
	cout << "   ";
	for (int i = 0; i < mLength; i++)
		cout << i;
	cout << endl;
}
bool Player::CanShipBePlaced(Ship ship) {
	//horizontal
	if (ship.Dir == 0) {
		if (ship.XFront + ship.Length >= mLength)	//is ship out of bounds
			return false;
		//checking is done by checking all tiles 1 square away from ship (basically rectangle)
		for (int i = ship.YFront - 1; i <= ship.YFront + 1; i++) {
			for (int j = ship.XFront - 1; j <= ship.XFront + ship.Length; j++) {
				if (i >= 0 && i<mWidth && j >= 0 && j<mLength)	//making sure indexes are not out of bounds
					if (mBoard[i][j] != 0)	//if any of the tiles are not water, then ship cannot be placed in that rectangle
						return false;
			}
		}
	}
	//vertical
	if (ship.Dir == 1) {
		if (ship.YFront + ship.Length >= mWidth)
			return false;
		for (int i = ship.YFront - 1; i <= ship.YFront + ship.Length; i++) {
			for (int j = ship.XFront - 1; j <= ship.XFront + 1; j++) {
				if (i >= 0 && i<mWidth && j >= 0 && j<mLength)
					if (mBoard[i][j] != 0)
						return false;
			}
		}
	}
	return true;
}
void Player::PlaceShipsOnBoard() {
	srand(time(NULL) + counter);
	for (int i = 0; i<ships.size(); i++) {
		//randomizing coordinates until ship can be placed
		do {
			ships[i].XFront = rand() % mLength;
			ships[i].YFront = rand() % mWidth;
			ships[i].Dir = rand() % 2;
		} while (!CanShipBePlaced(ships[i]));
		
		//placing the ship if horizontal
		if (ships[i].Dir == 0)
			for (int j = ships[i].XFront; j < ships[i].XFront + ships[i].Length; j++)
				mBoard[ships[i].YFront][j] = 1;
		//vertical
		else
			for (int j = ships[i].YFront; j < ships[i].YFront + ships[i].Length; j++)
				mBoard[j][ships[i].XFront] = 1;
	}
}
void Player::RevealShip(Ship ship) {
	//horizontal
	if (ship.Dir == 0) {
		//all tiles 1 tile away from the ship are revealed as missed (basically a rectangle around the ship)
		for (int i = ship.YFront - 1; i <= ship.YFront + 1; i++)
			for (int j = ship.XFront - 1; j <= ship.XFront + ship.Length; j++)
				if (i >= 0 && i < mWidth && j >= 0 && j < mLength)	//making sure indexes are not out of bounds
					if (mBoard[i][j] != 2)
						mBoard[i][j] = 3;
	}
	//vertical
	else{
		for(int i=ship.YFront-1; i<=ship.YFront+ship.Length; i++)
			for(int j=ship.XFront-1; j<=ship.XFront+1; j++)
				if(i>=0 && i<mWidth && j>=0 && j<mLength)
					if (mBoard[i][j] != 2) //Reveal ship method only changes tiles that are not shipDestroyed
						mBoard[i][j] = 3;
	}
}
bool Player::CheckIfShipIsDestroyed(Ship ship) {
	//horizontal
	if (ship.Dir == 0) {
		//if any of the ship tiles are not hit, then ship as a whole is not yet destroyed
		for (int i = ship.XFront; i < ship.XFront + ship.Length; i++)
			if (mBoard[ship.YFront][i] != 2)
				return false;
	}
	//vertica
	else{
		for (int i = ship.YFront; i < ship.YFront + ship.Length; i++)
			if (mBoard[i][ship.XFront] != 2)
				return false;
	}
	return true;
}
void Player::RevealShipsIfDestroyed() {
	//cycling through all the ships and cheking if they are not destroyed
	for (int i = 0; i < ships.size(); i++) {
		if (CheckIfShipIsDestroyed(ships[i])) {
			//if ship is destroyed, then a method that changes surrounding tiles to missed is called
			RevealShip(ships[i]);
			//adding message that ship was destroyed only once
			if (!ships[i].DestroyMessageAnnounced) {
				mMessage += ships[i].name + " is destroyed!";
				mShipCounter--;
				ships[i].DestroyMessageAnnounced = true;
			}
		}
	}
}
bool Player::CheckIfCanShoot(int y, int x) {
	//a shot can be made if it is not out of bounds and the tiles are either water or ship(that player doesn't know about)
	if (y >= 0 && y < mWidth && x >= 0 && x < mLength) {
		if (mBoard[y][x] == 0 || mBoard[y][x]==1)
			return true;
	}
	return false;
}
bool Player::CheckIfCanShoot(string coord) {
	//method that takes input as a string, converts it to integer coordinates and calls another shoot method
	pair<int, int> temp = StringToIntCoordsConverter(coord);
	if (CheckIfCanShoot(temp.first, temp.second)) return true;
	else return false;
}
void Player::Shoot(int y, int x) {
	switch (mBoard[y][x]) {
	case 0: mBoard[y][x] = 3; mMessage += "Missed. "; RevealShipsIfDestroyed(); break;
	case 1: mBoard[y][x] = 2; mMessage += "It's a hit! "; RevealShipsIfDestroyed(); break;
	default: break;
	}
}
void Player::Shoot(string coord) {
	pair<int, int> temp = StringToIntCoordsConverter(coord);
	Shoot(temp.first, temp.second);
}
string Player::DisplayMessage() {
	return mMessage;
}
void Player::ClearMessage() {
	mMessage = "";
}
pair<int, int> Player::StringToIntCoordsConverter(string coord) {
	//this method converts string input into two integers 
	int yInt, xInt;
	bool yIntSet = false, xIntSet = false;
	/*
		cheking if string is valid by looking at it's length.
		Valid input examples: B1, 2C, 11d, 0a, 5n 
		Minimum length = char(1 symbol) + digit(1 symbol)
		Maximum length = char(1 symbol) + digits(ceil(log10(maxCoordinate))+1)
	*/
	if (coord.length() >= 2 && coord.length() <= (ceil(log10(mWidth)) + 1)) {
		//logic when/if the first element is symbol coordinate
		if ((coord[0] >= 'a' && coord[0] <= 'z') || (coord[0] >= 'A' && coord[0] <= 'Z')) {
			if (coord[0] >= 'a' && coord[0] <= 'z') {
				yInt = coord[0] - 'a';
				yIntSet = true;
			}
			else if (coord[0] >= 'A' && coord[0] <= 'Z') {
				yInt = coord[0] - 'A';
				yIntSet = true;
			}
			//extracting the number
			bool allDigits = true;
			for (int i = 1; i < coord.length(); i++) {
				if (!(coord[i] >= '0' && coord[i] <= '9'))
					allDigits = false;
			}
			if (allDigits) {
				xInt = stoi(coord.substr(1));
				xIntSet = true;
			}
		}
		//logic when/if the last element is symbol coordinate
		else if ((coord[coord.length() - 1] >= 'a' && coord[coord.length() - 1] <= 'z') || (coord[coord.length() - 1] >= 'A' && coord[coord.length() - 1] <= 'Z')) {
			if (coord[coord.length() - 1] >= 'a' && coord[coord.length() - 1] <= 'z') {
				yInt = coord[coord.length() - 1] - 'a';
				yIntSet = true;
			}
			else if (coord[coord.length() - 1] >= 'A' && coord[coord.length() - 1] <= 'Z') {
				yInt = coord[coord.length() - 1] - 'A';
				yIntSet = true;
			}
			//extracting the number
			bool allDigits = true;
			for (int i = 0; i < coord.length() - 1; i++) {
				if (!(coord[i] >= '0' && coord[i] <= '9'))
					allDigits = false;
			}
			if (allDigits) {
				xInt = stoi(coord.substr(0, coord.length() - 1));
				xIntSet = true;
			}
		}
		//returning a valid output only if both coordinates were extracted
		if (yIntSet && xIntSet) {
			pair<int, int> temp;
			temp.first = yInt;
			temp.second = xInt;
			return temp;
		}
	}
	//coordinates (-1, -1) are not valid coordinates, thus (if string input is invalid) those coordinates are returned 
	else {
		pair<int, int> temp;
		temp.first = -1;
		temp.second = -1;
		return temp;
	}
}
bool Player::CheckIfLost() {
	if (mShipCounter == 0) return true;
	else return false;
}
int Player::GetWidth() const{
	return mWidth;
}
int Player::GetLength() const{
	return mLength;
}
void Player::SetColor(int color) {
	//to finish
}