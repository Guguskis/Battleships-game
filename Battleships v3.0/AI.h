#pragma once
#include "Player.h"

class AI : public Player{
public:
	AI();
	void Initialize(Player player);
	void ShootAt(Player &player);
private:
	//stores basic shot shoot pattern. More info at method "basicShot"
	std::vector<std::pair<int, int> > basicShotComm[2];

	int lastShotDir;
	std::string mode = "basic";
	int y, x;

	void Basic(Player &);
	void Locking(Player&);
	void Loading(Player&);
};

