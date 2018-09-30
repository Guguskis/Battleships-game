#pragma once
#include "Player.h"

class AI : public Player{
public:
	AI();
	void Initialize(Player player);
	void ShootAt(Player &player);
private:
	//stores basic shot pattern. More info at method "Basic"
	std::vector<std::pair<int, int> > basicShotComm[2];

	int y, x, lastShotDir;
	std::string mode = "basic";

	void Basic(Player &);
	void Locking(Player&);
	void Loading(Player&);
};

