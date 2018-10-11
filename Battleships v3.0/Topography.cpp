#include "stdafx.h"
#include "Topography.h"


Topography::Topography() {
	m_uncalcTiles = m_width * m_length;
	Initialization();
	RecursionInit();
}
Topography::Topography(bool animation) {
	m_animation = animation;

	m_uncalcTiles = m_width * m_length;
	Initialization();
	RecursionInit();
}
Topography::Topography(int seedValMin, int seedValMax, bool animation) {
	m_seedValMin = seedValMin;
	m_seedValMax = seedValMax;
	m_animation = animation;

	m_uncalcTiles = m_width * m_length;
	Initialization();
	RecursionInit();
}
Topography::Topography(int width, int lenght, int seedValMin, int seedValMax, bool animation, std::string message) {
	m_width = width;
	m_length = lenght;
	m_seedValMin = seedValMin;
	m_seedValMax = seedValMax;
	m_animation = animation;
	m_message = message;
	m_uncalcTiles = m_width * m_length;
	Initialization();
	RecursionInit();
}
Topography::~Topography() {
}
void Topography::Initialization() {

	std::vector<m_tile> line;
	for (int i = 0; i < m_length; i++) {
		line.push_back(m_tile());
	}
	for (int i = 0; i < m_width; i++) {
		m_board.push_back(line);
	}
}
void Topography::RecursionInit() {
	//generating seeds
	for (int i = 0; i < m_seedCount; i++) {
		int tempY = rand() % m_width, tempX = rand() % m_length;

		while (m_board[tempY][tempX].calc) {
			tempY = rand() % m_width;
			tempX = rand() % m_length;
		}
		int seedValue = rand() % (m_seedValMax - m_seedValMin + 1) + m_seedValMin;
		m_board[tempY][tempX].value = seedValue;
		m_board[tempY][tempX].calc = true;
		m_seed.push_back(std::make_pair(tempY, tempX));
		m_uncalcTiles--;
	}
	//iterating recursion one radius unit at a time
	int radius = 1;
	while (m_uncalcTiles != 0) {
		if (m_animation) {
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD c;
			c.X = 0;
			c.Y = 0;
			SetConsoleCursorPosition(h, c);
			SetColor("black", 7);
			std::cout << m_message << std::endl;
			Draw();
			std::cout << std::endl;
			Sleep(m_animationSpeed);
		}
		for (int i = 0; i < m_seedCount; i++) {
			//generating an empty new board to pass for recursion
			std::vector<std::vector<bool> > tempBoard(
				m_width,
				std::vector<bool>(m_length, false));

			Recursion(
				m_seed[i].first, m_seed[i].second,
				m_seed[i].first, m_seed[i].second,
				radius, tempBoard);
		}
		radius++;
	}
}
void Topography::Recursion(
	int y, int x, int yOrigin, int xOrigin, int radius,
	std::vector<std::vector<bool> > &tempBoard) {
	if (y >= 0 && y < m_width && x >= 0 && x < m_length) { //checking bounds

		/*
			if original tile was not calculated - calculate it
		*/
		if (!m_board[y][x].calc) {
			int yTemp = (yOrigin - y)*(yOrigin - y);
			int xTemp = (xOrigin - x)*(xOrigin - x);

			if (sqrt(yTemp + xTemp) <= radius) {
				m_board[y][x].value = Average(y, x, m_avgRadius);

				//randomizing jump value
				int tempJump, jump;
				do {
					tempJump = rand() % 100;
					if (tempJump < 15) jump = -1;
					else if (tempJump >= 85) jump = 1;
					else jump = 0;
				} while (
					m_board[y][x].value + jump < m_tileValMin ||
					m_board[y][x].value + jump > m_tileValMax);

				m_board[y][x].value += jump;
				m_board[y][x].calc = true;

				m_uncalcTiles--;
			}
			tempBoard[y][x] = true;
		}
		/*
			if original tile was not calculated, then continue recursion if
			this tile wasn't used before
		*/
		else if (!tempBoard[y][x]) {
			tempBoard[y][x] = true;
			Recursion(y + 1, x, yOrigin, xOrigin, radius, tempBoard);
			Recursion(y, x + 1, yOrigin, xOrigin, radius, tempBoard);
			Recursion(y - 1, x, yOrigin, xOrigin, radius, tempBoard);
			Recursion(y, x - 1, yOrigin, xOrigin, radius, tempBoard);
		}
	}
}
int Topography::Average(int y, int x, int radius) {
	int sum = 0, count = 0;
	for (int i = y - radius; i <= y + radius; i++) {
		for (int j = x - radius; j <= x + radius; j++) {
			if (i >= 0 && i < m_width && j >= 0 && j < m_length) {
				if (m_board[i][j].calc) {
					sum += m_board[i][j].value;
					count++;
				}
			}
		}
	}

	if (count == 0) {
		return round((m_seedValMax + m_seedValMin)*1.0 / 2);
	}
	//USING FLOOR INCREASES DEPTH THE FURTHER FROM SEED
	//USING ROUND MAKES DEPTH CONSISTENT
	else return floor(sum*1.0 / count);
}
int Topography::AverageSmoothing(int y, int x) {
	int sum = 0, count = 0;
	for (int i = y; i <= y + 1; i++) {
		for (int j = x; j <= x + 1; j++) {
			if (i >= 0 && i < m_width && j >= 0 && j < m_length) {
				if (m_board[i][j].calc) {
					sum += m_board[i][j].value;
					count++;
				}
			}
		}
	}

	if (count == 0) {
		std::cout << "\nDivizion of zero in Average";
		return (m_seedValMax + m_seedValMin) / 2;
	}
	else return floor(sum*1.0 / count);
}
void Topography::Draw() {
	std::cout << "\n";
	for (int i = 0; i < m_width; i++) {
		std::cout << "      ";
		for (int j = 0; j < m_length; j++) {

			SetColor("water", m_board[i][j].value);
			std::cout << "~";
		}
		SetColor("black", 0);
		std::cout << std::endl;
	}
	SetColor("black", 0);
}
void Topography::SetColor(std::string object, int color) {
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
		default: bgValue = 0; break;
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
			default: charValue = 0; break;
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
		charValue = 7;	//TEST: change back to 7 (white symbol)
	}

	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, bgValue + charValue);
}
void Topography::CopyBoard(std::vector<std::vector<int> > &board) {
	board.clear();
	board.resize(m_width);
	for (int i = 0; i < m_width; i++) {
		board[i].resize(m_length);
	}
	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_length; j++) {
			board[i][j] = m_board[i][j].value;
		}
	}

}
void Topography::Smoothing() {
	//creating temporary board to calculate average tile values
	std::vector<std::vector<m_tile> > boardTemp = m_board;
	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_length; j++) {
			boardTemp[i][j].value = AverageSmoothing(i, j);
		}
	}
	//transfering smoothed board
	for (int i = 0; i < m_width; i++) {
		for (int j = 0; j < m_length; j++) {
			m_board[i][j].value = boardTemp[i][j].value;
		}
		if (m_animation) {
			HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD c;
			c.X = 0;
			c.Y = 0;
			SetConsoleCursorPosition(h, c);
			SetColor("black", 0);
			std::cout << m_message << std::endl;
			Draw();
			std::cout << std::endl;
			Sleep(m_animationSpeed / 2);
		}
	}
}