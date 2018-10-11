#pragma once
#include <vector>
#include <utility>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include <string>
class Topography
{
public:

private:
	//animation
	bool m_animation = false;
	int m_animationSpeed = 250;
	int m_width = 10, m_length = 10;
	std::string m_message;
	//board 
	struct m_tile {
		int value = -1;
		bool calc = false;
	};
	int m_uncalcTiles;
	std::vector<std::vector<m_tile> > m_board;
	//topography 
	int m_seedCount = 3, m_seedValMin = 0, m_seedValMax = 6;
	int m_tileValMin = 0, m_tileValMax = 6;
	int m_avgRadius = 2;
	std::vector<std::pair<int, int> > m_seed;


public:
	Topography();
	Topography(bool);//animation
	Topography(int, int, bool);//seedValMin, seedValMax, animation
	Topography(int, int, int, int, bool, std::string);//width, lenght, seedValMin, seedValMax, animation

	~Topography();
	void Draw();
	void CopyBoard(std::vector<std::vector<int> >&);
	void Smoothing();
private:
	void Initialization();
	void RecursionInit();
	void Recursion(int, int, int, int, int, std::vector<std::vector<bool> >&);
	int Average(int, int, int);
	int AverageSmoothing(int, int); //Average, but calculating 2x2
	void SetColor(std::string, int);
};

