/*
This file is part of KingsAndShips.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef TOWERS_H
#define TOWERS_H

#include <iostream>
#include <string>
#include "shooter.h"
#include "../coord.h"


class C_Towers: public C_Shooter {
public:
	//methods
	C_Towers(S_UnitModel model);
	virtual void play();

protected:
	virtual void render(S_Coord screen);
	virtual void renderSelected();
  virtual void renderTowerStatus(std::string name, int x_screen, int y_screen);
	virtual void renderSmoke();
  virtual void renderTowerStatusCircle(std::string name, int x_screen, int y_screen, int fireRate, int fireRange, int damage, std::string color);
	long m_lastSmokeTime;
	int m_smokeNbr;
};


class C_ArcherTower: public C_Towers {
public:
	C_ArcherTower(S_UnitModel model);
	virtual void render(S_Coord screen);
protected:

};

class C_Turbine: public C_Towers {
public:
	C_Turbine(S_UnitModel model);
	virtual void render(S_Coord screen);
protected:

};


class C_Catapult: public C_Towers {
public:
	C_Catapult(S_UnitModel model);
	virtual void render(S_Coord screen);
protected:
	virtual void play();
	void changeState(std::string state){m_state = state;};
	std::string m_state;
	C_GameUnits * m_target;
};




#endif
