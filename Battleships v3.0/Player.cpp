#include "stdafx.h"
#include "Player.h"

int Player::counter = 0;


Player::Player() {
	//initializing variables and vectors
	counter++;
	srand(time(NULL) + counter);
	mMessage = "";
	mWidth = mLength = 10;
	
	mBoard.resize(mWidth);
	for (int i = 0; i < mWidth; i++) {
		mBoard[i].resize(mLength);
	}

	//adding ships to vector;
	ships = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	mShipCounter = ships.size();
}
Player::Player(int width, int length) {
	counter++;
	srand(time(NULL) + counter);
	mMessage = "";
	mWidth = width;
	mLength = length;
	
	mBoard.resize(mWidth);
	for (int i = 0; i < mWidth; i++) {
		mBoard[i].resize(mLength);
	}

	//adding ships to vector;
	ships = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	mShipCounter = ships.size();
}
void Player::DrawYourBoard() {
	//drawing top line
	std::cout << "      ";
	for (int i = 0; i < mLength; i++)
		std::cout << i;
	std::cout << std::endl;
	//drawing middle section
	for (int i = 0; i < mWidth; i++) {
		SetColor("black", 0);
		std::cout << "    " << (char)('A' + i) << " ";
		for (int j = 0; j < mLength; j++) {
			switch (mBoard[i][j]) {
			case 0:
				SetColor("water", mSea[i][j]);
				std::cout << "~"; break;
			case 1:
				SetColor("ship", mSea[i][j]);
				std::cout << "#"; break;
			case 2:
				SetColor("shipDestroyed", mSea[i][j]);
				std::cout << "X"; break;
			case 3:
				SetColor("missed", mSea[i][j]);
				std::cout << "+"; break;
			default: std::cout << "Tile not specified in Player.DrawBoardAll();"; break;
			}
		}
		SetColor("black", 0);
		std::cout << " " << (char)('A' + i) << std::endl;
	}
	//drawing bottom line
	SetColor("black", 0);
	std::cout << "      ";
	for (int i = 0; i < mLength; i++)
		std::cout << i;
	std::cout << std::endl;
}
void Player::DrawEnemyBoard() {
	//drawing top line
	std::cout << "                ";
	for (int i = 0; i < mLength; i++)
		std::cout << i;
	std::cout << std::endl;
	//drawing middle section
	for (int i = 0; i < mWidth; i++) {
		SetColor("black", 0);
		std::cout << "              " << (char)('A' + i) << " ";
		for (int j = 0; j < mLength; j++) {
			switch (mBoard[i][j]) {
			case 0:
				SetColor("water", mSea[i][j]);
				std::cout << "~"; break;
			case 1:
				SetColor("water", mSea[i][j]);
				std::cout << "~"; break;
			case 2:
				SetColor("shipDestroyed", mSea[i][j]);
				std::cout << "X"; break;
			case 3:
				SetColor("missed", mSea[i][j]);
				std::cout << "+"; break;
			default: std::cout << "Tile not specified in Player.DrawBoardAll();"; break;
			}
			
		}
		SetColor("black", 0);
		std::cout << " " << (char)('A' + i) << std::endl;
	}
	//drawing bottom line
	SetColor("black", 0);
	std::cout << "                ";
	for (int i = 0; i < mLength; i++)
		std::cout << i;
	std::cout << std::endl;
}
void Player::DrawBothBoards(Player player, Player enemy) {
	//"     "
	/*
		UPPER UI PART
	*/
	std::cout << "   Your battlefield      Enemy battlefield\n      ";
	for (int i = 0; i < player.mLength; i++)
		std::cout << i;
	std::cout << "             ";
	for (int i = 0; i < enemy.mLength; i++)
		std::cout << i;
	std::cout << std::endl;
	/*
		MIDDLE UI PART
	*/
	for (int i = 0; i < player.GetLength(); i++) {
		/*
			MIDDLE UI LEFT SIDE
		*/

		std::cout << "    " << (char)('A' + i) << " ";
		for (int j = 0; j < player.mLength; j++) {
			switch (player.mBoard[i][j]) {
			case 0:
				player.SetColor("water", player.mSea[i][j]);
				std::cout << "~"; break;
			case 1:
				player.SetColor("ship", player.mSea[i][j]);
				std::cout << (char)178; break;
			case 2:
				player.SetColor("shipDestroyed", player.mSea[i][j]);
				std::cout << "X"; break;
			case 3:
				player.SetColor("missed", player.mSea[i][j]);
				std::cout << "+"; break;
			default: std::cout << "Tile not specified in Player.DrawBoardAll();"; break;
			}
		}
		player.SetColor("black", 0);
		std::cout << " " << (char)('A' + i) << "         ";
		/*
			MIDDLE UI RIGHT SIDE
		*/
		std::cout << (char)('A' + i) << " ";
		for (int j = 0; j < enemy.mLength; j++) {
			switch (enemy.mBoard[i][j]) {
			case 0:
				enemy.SetColor("water", enemy.mSea[i][j]);
				std::cout << "~"; break;
			case 1:
				enemy.SetColor("water", enemy.mSea[i][j]);
				std::cout << "~"; break;
			case 2:
				enemy.SetColor("shipDestroyed", enemy.mSea[i][j]);
				std::cout << "X"; break;
			case 3:
				enemy.SetColor("missed", enemy.mSea[i][j]);
				std::cout << "+"; break;
			default: std::cout << "Tile not specified in Player.DrawBoardAll();"; break;
			}
		}
		enemy.SetColor("black", 0);
		std::cout << " " << (char)('A' + i);
		std::cout << std::endl;
	}
	/*
		BOTTOM UI PART
	*/
	std::cout << "      ";
	for (int i = 0; i < player.mLength; i++)
		std::cout << i;
	std::cout << "             ";
	for (int i = 0; i < enemy.mLength; i++)
		std::cout << i;
	std::cout << std::endl;
}
void Player::DrawBothBoards(Player player, Player enemy, std::string) {
	//"     "
	/*
		UPPER UI PART
	*/
	std::cout << "   Your battlefield      Enemy battlefield\n      ";
	for (int i = 0; i < player.mLength; i++)
		std::cout << i;
	std::cout << "             ";
	for (int i = 0; i < enemy.mLength; i++)
		std::cout << i;
	std::cout << std::endl;
	/*
		MIDDLE UI PART
	*/
	for (int i = 0; i < player.GetLength(); i++) {
		/*
			MIDDLE UI LEFT SIDE
		*/

		std::cout << "    " << (char)('A' + i) << " ";
		for (int j = 0; j < player.mLength; j++) {
			switch (player.mBoard[i][j]) {
			case 0:
				player.SetColor("water", player.mSea[i][j]);
				std::cout << "~"; break;
			case 1:
				player.SetColor("ship", player.mSea[i][j]);
				std::cout << (char)178; break;
			case 2:
				player.SetColor("shipDestroyed", player.mSea[i][j]);
				std::cout << "X"; break;
			case 3:
				player.SetColor("missed", player.mSea[i][j]);
				std::cout << "+"; break;
			default: std::cout << "Tile not specified in Player.DrawBoardAll();"; break;
			}
		}
		player.SetColor("black", 0);
		std::cout << " " << (char)('A' + i) << "         ";
		/*
			MIDDLE UI RIGHT SIDE
		*/
		std::cout << (char)('A' + i) << " ";
		for (int j = 0; j < enemy.mLength; j++) {
			switch (enemy.mBoard[i][j]) {
			case 0:
				enemy.SetColor("water", enemy.mSea[i][j]);
				std::cout << "~"; break;
			case 1:
				enemy.SetColor("ship", enemy.mSea[i][j]);
				std::cout << (char)178; break;
			case 2:
				enemy.SetColor("shipDestroyed", enemy.mSea[i][j]);
				std::cout << "X"; break;
			case 3:
				enemy.SetColor("missed", enemy.mSea[i][j]);
				std::cout << "+"; break;
			default: std::cout << "Tile not specified in Player.DrawBoardAll();"; break;
			}
		}
		enemy.SetColor("black", 0);
		std::cout << " " << (char)('A' + i);
		std::cout << std::endl;
	}
	/*
		BOTTOM UI PART
	*/
	std::cout << "      ";
	for (int i = 0; i < player.mLength; i++)
		std::cout << i;
	std::cout << "             ";
	for (int i = 0; i < enemy.mLength; i++)
		std::cout << i;
	std::cout << std::endl;
}
bool Player::CheckIfCanShoot(int y, int x) {
	//a shot can be made if it is not out of bounds and the tiles are either water or ship(that player doesn't know about)
	if (y >= 0 && y < mWidth && x >= 0 && x < mLength) {
		if (mBoard[y][x] == 0 || mBoard[y][x] == 1)
			return true;
	}
	return false;
}
bool Player::CheckIfCanShoot(std::string coord) {
	//method that takes input as a string, converts it to integer coordinates and calls another shoot method
	std::pair<int, int> temp = StringToIntCoordsConverter(coord);
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
void Player::Shoot(std::string coord) {
	std::pair<int, int> temp = StringToIntCoordsConverter(coord);
	Shoot(temp.first, temp.second);
}
void Player::ShootAt(Player &player, int y, int x) {
	player.Shoot(y, x);
}
void Player::ShootAt(Player &player, std::string coords) {
	player.Shoot(coords);
}
std::string Player::DisplayMessage() {
	return mMessage;
}
void Player::ClearMessage() {
	mMessage = "";
}
int Player::GetWidth() const {
	return mWidth;
}
int Player::GetLength() const {
	return mLength;
}
int Player::GetRemainingShipCount() {
	return mShipCounter;
}
void Player::PlaceShipsOnBoard() {
	for (int i = 0; i < ships.size(); i++) {
		//randomizing coordinates until ship can be placed
		do {
			ships[i].XFront = rand() % mLength;
			ships[i].YFront = rand() % mWidth;
			ships[i].Dir = rand() % 2;
		} while (!CanShipBePlaced(ships[i]));

		//placing the ship if its horizontal
		if (ships[i].Dir == 0)
			for (int j = ships[i].XFront; j < ships[i].XFront + ships[i].Length; j++)
				mBoard[ships[i].YFront][j] = 1;
		//vertical
		else
			for (int j = ships[i].YFront; j < ships[i].YFront + ships[i].Length; j++)
				mBoard[j][ships[i].XFront] = 1;
	}
}
bool Player::CheckIfLost() {
	if (mShipCounter == 0) return true;
	else return false;
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
	else {
		for (int i = ship.YFront - 1; i <= ship.YFront + ship.Length; i++)
			for (int j = ship.XFront - 1; j <= ship.XFront + 1; j++)
				if (i >= 0 && i < mWidth && j >= 0 && j < mLength)
					if (mBoard[i][j] != 2) //Reveal ship method only changes tiles that are not shipDestroyed
						mBoard[i][j] = 3;
	}
}
bool Player::CanShipBePlaced(Ship ship) {
	//cheking bounds
	if (ship.XFront<0 || ship.XFront>mLength || ship.YFront<0 || ship.YFront>mWidth)
		return false;
	//horizontal
	if (ship.Dir == 0) {
		if (ship.XFront + ship.Length > mLength)	//is ship out of bounds
			return false;
		//checking is done by checking all tiles 1 square away from ship (basically rectangle)
		for (int i = ship.YFront - 1; i <= ship.YFront + 1; i++) {
			for (int j = ship.XFront - 1; j <= ship.XFront + ship.Length; j++) {
				if (i >= 0 && i < mWidth && j >= 0 && j < mLength)	//making sure indexes are not out of bounds
					if (mBoard[i][j] != 0)	//if any of the tiles are not water, then ship cannot be placed in that rectangle
						return false;
			}
		}
	}
	//vertical
	if (ship.Dir == 1) {
		if (ship.YFront + ship.Length > mWidth)
			return false;
		for (int i = ship.YFront - 1; i <= ship.YFront + ship.Length; i++) {
			for (int j = ship.XFront - 1; j <= ship.XFront + 1; j++) {
				if (i >= 0 && i < mWidth && j >= 0 && j < mLength)
					if (mBoard[i][j] != 0)
						return false;
			}
		}
	}
	return true;
}
std::pair<int, int> Player::StringToIntCoordsConverter(std::string coord) {
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
		if ((coord[0] >= 'a' && coord[0] <= 'z' + mWidth) || (coord[0] >= 'A' && coord[0] <= 'Z' + mWidth)) {
			if (coord[0] >= 'a' && coord[0] <= 'z' + mWidth) {
				yInt = coord[0] - 'a';
				yIntSet = true;
			}
			else if (coord[0] >= 'A' && coord[0] <= 'Z' + mWidth) {
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
		else if ((coord[coord.length() - 1] >= 'a' && coord[coord.length() - 1] <= 'z' + mWidth) || (coord[coord.length() - 1] >= 'A' && coord[coord.length() - 1] <= 'Z' + mWidth)) {
			if (coord[coord.length() - 1] >= 'a' && coord[coord.length() - 1] <= 'z' + mWidth) {
				yInt = coord[coord.length() - 1] - 'a';
				yIntSet = true;
			}
			else if (coord[coord.length() - 1] >= 'A' && coord[coord.length() - 1] <= 'Z' + mWidth) {
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
			std::pair<int, int> temp;
			temp.first = yInt;
			temp.second = xInt;
			return temp;
		}
	}
	//returning invalid input
	//coordinates (-1, -1) are not valid coordinates, thus (if string input is invalid) those coordinates are returned 
	std::pair<int, int> temp;
	temp.first = -1;
	temp.second = -1;
	return temp;
}
void Player::RevealShipsIfDestroyed() {
	//cycling through all the ships and cheking if they are not destroyed
	for (int i = 0; i < ships.size(); i++) {
		if (CheckIfShipIsDestroyed(ships[i])) {
			//if ship is destroyed, then a method that changes surrounding tiles to missed is called
			RevealShip(ships[i]);
			//making sure that ship destruction message is displayed only once per ship
			if (!ships[i].DestroyMessageAnnounced) {
				mMessage += ships[i].name + " is destroyed!";
				mShipCounter--;
				ships[i].DestroyMessageAnnounced = true;
			}
		}
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
	//vertical
	else {
		for (int i = ship.YFront; i < ship.YFront + ship.Length; i++)
			if (mBoard[i][ship.XFront] != 2)
				return false;
	}
	return true;
}
void Player::SetColor(std::string object, int color) {
	int bgValue = 0, charValue = 0;

	/*
		Blue color values (from darkest to lightest):
			15 - dark blue
			143 - light blue
			47 - cyan
			175 - light cyan
	*/

	if (object != "black") {
		//setting background values
		switch (color) {
		case 0: bgValue = 15; break;
		case 1: bgValue = 15; break;
		case 2: bgValue = 15; break;
		case 3: bgValue = 143; break;
		case 4: bgValue = 143; break;
		case 5: bgValue = 143; break;
		case 6: bgValue = 47; break;
		}

		//setting character values
		if (object == "water") {
			switch (color) {
			case 0: charValue = 2; break;
			case 1: charValue = 10; break;
			case 2: charValue = 2; break;
			case 3: charValue = 10; break;
			case 4: charValue = 12; break;
			case 5: charValue = 10; break;
			case 6: charValue = 12; break;
			}
		}
		else if (object == "ship") charValue = 1;
		else if (object == "shipDestroyed") {
			charValue = 5;
			//bgValue = 191;
		}
		else if (object == "missed") charValue = 16;
	}
	else {
		bgValue = 0;
		charValue = 7;
	}

	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, bgValue + charValue);
}
void Player::GenerateSea(bool animate, std::string message) {
	//hiding cursor to prevent its wild rampaging across the screen
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);

	Topography board(mWidth, mLength, 3, 4, animate, message);
	board.Smoothing();
	board.CopyBoard(mSea);

	cursorInfo.bVisible = true; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
void Player::InstaKill() {
	mShipCounter = 0;
}
void Player::DEBUG() {
	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mLength; j++) {
			std::cout << mSea[i][j];
		}
		std::cout << std::endl;
	}
}
