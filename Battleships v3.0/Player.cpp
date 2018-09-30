#include "stdafx.h"
#include "Player.h"

int Player::counter = 0;

Player::Player() {
	//initializing variables and vectors
	counter++;
	srand(time(NULL) + counter);
	mMessage = "";
	mWidth = mLength = 10;
	std::vector<int> tempLength;
	ColorTile tempTile;
	std::vector<ColorTile> tempTileLength;
	tempTile.color = 0;
	tempTile.calculated = false;

	for (int i = 0; i < mLength; i++) {
		tempLength.push_back(0);
		tempTileLength.push_back(tempTile);
	}
	for (int i = 0; i < mWidth; i++) {
		mBoard.push_back(tempLength);
		mTiles.push_back(tempTileLength);
	}

	//adding ships to vector;
	ships = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };
	mShipCounter = ships.size();
	//generating color board
	GenerateColorBoard();
}
Player::Player(int width, int length) {
	counter++;
	srand(time(NULL) + counter);
	mMessage = "";
	mWidth = width;
	mLength = length;
	std::vector<int> tempLength;
	std::vector<ColorTile> tempTileLength;
	ColorTile tempTile;
	tempTile.color = 0;
	tempTile.calculated = false;
	for (int i = 0; i < mLength; i++) {
		tempLength.push_back(0);
		tempTileLength.push_back(tempTile);
	}
	for (int i = 0; i < mWidth; i++) {
		mBoard.push_back(tempLength);
		mTiles.push_back(tempTileLength);
	}
	//generating ships
	ships.push_back(4);
	for (int i = 0; i < 2; i++)
		ships.push_back(3);
	for (int i = 0; i < 3; i++)
		ships.push_back(2);
	for (int i = 0; i < 4; i++)
		ships.push_back(1);
	PlaceShipsOnBoard();
	mShipCounter = ships.size();
	//generating color board
	GenerateColorBoard();
}
void Player::DrawYourBoard() {
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
				SetColor("water", mTiles[i][j].color);
				std::cout << "~"; break;
			case 1:
				SetColor("ship", mTiles[i][j].color);
				std::cout << "#"; break;
			case 2:
				SetColor("shipDestroyed", mTiles[i][j].color);
				std::cout << "X"; break;
			case 3:
				SetColor("missed", mTiles[i][j].color);
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
				SetColor("water", mTiles[i][j].color);
				std::cout << "~"; break;
			case 1:
				SetColor("water", mTiles[i][j].color);
				std::cout << "~"; break;
			case 2:
				SetColor("shipDestroyed", mTiles[i][j].color);
				std::cout << "X"; break;
			case 3:
				SetColor("missed", mTiles[i][j].color);
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
	//coordinates (-1, -1) are not valid coordinates, thus (if string input is invalid) those coordinates are returned 
	else {
		std::pair<int, int> temp;
		temp.first = -1;
		temp.second = -1;
		return temp;
	}
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
void Player::GenerateColorBoard() {
	/*
		plastd::cing random seeds
	*/
	std::pair<int, int> tempPair;
	std::vector<std::pair<int, int> > seeds;

	for (int i = 0; i < mColorSeeds; i++) {
		int y, x, tempColor;
		tempColor = rand() % (mColorMax - mColorMin + 1) + mColorMin;
		/*
			generating seeds first and only after that will call recursion
			to avoid infinite loop (because all tiles would have been calcualted)
		*/
		do {
			//substracting 1 avoid calling recursion out of bounds
			y = rand() % (mWidth - 1);
			x = rand() % mLength;
		} while (mTiles[y][x].calculated);
		mTiles[y][x].color = tempColor;
		mTiles[y][x].calculated = true;
		tempPair.first = y;
		tempPair.second = x;
		seeds.push_back(tempPair);
	}
	/*
		using seeds to fill the board
	*/
	for (int i = 0; i < seeds.size(); i++) {
		int tempDepth = 0;
		GenerateColorBoardRecursion(seeds[i].first, seeds[i].second, seeds[i].first + 1, seeds[i].second);
	}
	/*
		filling remaining board 
		(applied experimental change that seems to work, 
		but idk for sure if it won't infinite loop)
	*/
	int DisplayErrorMessageCounter = 0;
	bool unprocessedTileRemains = false;
	do {
		DisplayErrorMessageCounter++;
		if(DisplayErrorMessageCounter>1000)
			std::cout << "If you see this, then program is stuck at Player::GenerateColorBoard -> while loop at filling the board.\n This shouldn't happen\nRESTART PROGRAM";
		
		unprocessedTileRemains = false;
		for (int i = 0; i < mWidth; i++) {
			for (int j = 0; j < mLength; j++)
				if (mTiles[i][j].calculated) {
					GenerateColorBoardRecursion(i + 1, j, i, j);
					GenerateColorBoardRecursion(i, j + 1, i, j);
					GenerateColorBoardRecursion(i - 1, j, i, j);
					GenerateColorBoardRecursion(i, j - 1, i, j);
				}
		}
		for (int i = 0; i < mWidth; i++) {
			for (int j = 0; j < mLength; j++)
				if (!mTiles[i][j].calculated) {
					unprocessedTileRemains = true;
					i = mWidth;
					j = mLength;
				}
		}
	} while (unprocessedTileRemains);

	/*
		normalizing values and randomizing colors that are on the edge of two different blue shades
	*/
	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mLength; j++) {
			mTiles[i][j].color = RandomizeColorTranzitions(mTiles[i][j].color);
		}
	}
}
void Player::GenerateColorBoardRecursion(int y, int x, int yOrigin, int xOrigin) {
	/*
		recursionDepth prevents single seed domination
	*/
	if (y >= 0 && y < mWidth && x >= 0 && x < mLength && sqrt(pow(yOrigin - y, 2) + pow(xOrigin - x, 2)) < recursionDepth) {
		if (!mTiles[y][x].calculated) {
			mTiles[y][x].color = SurroundingTilesAverage(y, x);
			int temp;
			/*
				loop guarantees that generated values are valid
			*/
			do {
				/*
					changing the frequency of how often color value jumps
				*/
				temp = rand() % 5;
				switch (temp) {
				case 0: temp = -1; break;
				case 4: temp = 1; break;
				default: temp = 0; break;
				}
			} while (!(mTiles[y][x].color + temp >= mColorMin && mTiles[y][x].color + temp <= mColorMax));
			mTiles[y][x].color += temp;
			mTiles[y][x].calculated = true;
			this->GenerateColorBoardRecursion(y - 1, x, yOrigin, xOrigin);
			this->GenerateColorBoardRecursion(y, x + 1, yOrigin, xOrigin);
			this->GenerateColorBoardRecursion(y + 1, x, yOrigin, xOrigin);
			this->GenerateColorBoardRecursion(y, x - 1, yOrigin, xOrigin);
		}
	}
}
int Player::SurroundingTilesAverage(int y, int x) {
	//calculates the average of 3x3 area 
	int sum = 0, tileCount = 0;
	for (int i = y - 1; i <= y + 1; i++) {
		for (int j = x - 1; j <= x + 1; j++) {
			if (i >= 0 && i < mWidth && j >= 0 && j < mLength) {
				if (mTiles[i][j].calculated) {
					sum += mTiles[i][j].color;
					tileCount++;
				}
			}
		}
	}
	//for some reason at element [0][0] it counts zero tiles after filling remaining board in GenerateColorBoard
	if (tileCount != 0) return round(sum*1.0 / tileCount);
	else return round((1.0*mColorMax - mColorMin) / 2);
}
int Player::RandomizeColorTranzitions(int color) {

	/*
		this function randomizes tile colors that are in transition with each other
		e.g. tile with value 5 will be changed into 4 or 6
	*/
	int temp = rand() % 2;
	switch (color) {
	case 0: return 0;
	case 1: return 1;
	case 2:
		if (temp == 0) return 1;
		else return 2;
	case 3: return 2;
	case 4: return 3;
	case 5: return 4;
	case 6:
		if (temp == 0) return 4;
		else return 5;
	case 7: return 5;
	case 8: return 6;
	default:
		std::cout << "\nColor value " << color << " not specified in RandomizeColorTranzitions function. Returning -1\n";
		return -1;
	}
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
void Player::DEBUG() {
	for (int i = 0; i < mWidth; i++) {
		for (int j = 0; j < mLength; j++) {
			std::cout << mTiles[i][j].calculated;
		}
		std::cout << std::endl;
	}
}

