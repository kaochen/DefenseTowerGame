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

#include "boat.h"
#include "grid.h"

#include "../window.h"
#include "../locator.h"

using namespace std;

C_Boat::C_Boat(S_UnitModel model):C_Shooter(model)
{
	m_speed = model.speed;
	m_coord.centerOnTile();
	//Find a way to town
	C_Grid& grid= C_Locator::getGrid();
	S_Coord town = grid.foundTown();
	m_C_Path = new C_Path(town.x,town.y);
	m_C_Path->calcPath(model.coord.x,model.coord.y,town.x,town.y);
	m_C_Path->showPath();
	m_direction = EAST;
	m_countRegenPath = 0;
	m_targetsTypes.push_back("town");
	m_targetsTypes.push_back("barricade");
	m_targetsTypes.push_back("ArcherTower");
	m_targetsTypes.push_back("Catapult");
	m_state ="Moving";
	m_anim.add(C_Anim("Moving",1,7,80));
	m_anim.add(C_Anim("Waiting",0,0,800));
}

C_Boat::~C_Boat()
{
	delete m_C_Path;
}

void C_Boat::play()
{
	if(alive()){
		if(m_state == "Waiting"){
			if(m_anim.end(m_state)){
				changeState("Moving");
			}
		}
		if(m_state == "Moving"){
			move();
		}
		shoot();
	} else {
		kill();
	}
};

void C_Boat::kill()
{
	C_Shooter::kill();
	C_Wallet& wallet= C_Locator::getWallet();
	wallet.credit(m_cost); //reward when killing a boat
	wallet.cliStatus();
}

void C_Boat::move()
{
	C_Grid& grid= C_Locator::getGrid();
	S_Coord finalDestination = grid.foundTown();
	m_C_Path->regenScreenCoord(); //first refresh the coord for the path in case of the window has moved

	if(m_C_Path->closeToDestination(m_coord.getXGrid(),m_coord.getYGrid(),1) || m_C_Path->getPath().size() <= 1) {
		changeState("Waiting");
		m_anim.get(m_state).play();
	} else {
		if(!nextStepEmpty()) {
			changeState("Moving");
			m_anim.get(m_state).playAndRewind();

			int old_x_grid = m_coord.getXGrid();
			int old_y_grid = m_coord.getYGrid();

			C_Coord destCoord = m_C_Path->getPath().top()->getCoord();
			destCoord.centerOnTile();
			double angle = calcAngle(destCoord);

			m_coord.move(angle,m_speed);
			m_direction = destCoord.angleToDirection(angle);
			m_coord.regenGridCoord();
			grid.moveUnit(old_x_grid, old_y_grid,  m_coord.getXGrid (), m_coord.getYGrid ());


			if(m_coord.closeToCenter(destCoord.getGrid(),2)) {
				m_coord.centerOnTile(); //to not deviate too much from the path
				m_countRegenPath++;
				m_C_Path->goNextStep();
			}
			//recalc path anyway
			if(m_countRegenPath > 3) {
				recalcPath(finalDestination);
				m_countRegenPath = 0;
			}
		} else {
			changeState("Waiting");

			if(!m_C_Path->closeToDestination(m_coord.getXGrid(),m_coord.getYGrid(),3)){
				recalcPath(finalDestination);
			}
		}
	}
}


bool C_Boat::nextStepEmpty(){
	bool ret = false;

	C_Coord destCoord = m_C_Path->getPath().top()->getCoord();
	float angle = calcAngle(destCoord);

	C_Coord tmp = m_coord;
	tmp.move(angle,m_speed);
	tmp.regenGridCoord();
	C_Grid& grid= C_Locator::getGrid();
	ret = grid.mainEmpty(tmp.getXGrid(),tmp.getYGrid(),this);
	return ret;
}

float C_Boat::calcAngle(C_Coord destCoord){
	destCoord.centerOnTile();
	S_Coord start = m_coord.getScreen();
	S_Coord dest = destCoord.getScreen();
	int ab = dest.x - start.x;
	int bc = dest.y - start.y;
	return destCoord.atan2_360(ab,bc);
}




void C_Boat::render(S_Coord screen)
{
	C_Shooter::render(screen);
	m_C_Path->displayPath();
}



void C_Boat::recalcPath(S_Coord dest)
{
	C_Coord oldNext = m_C_Path->getPath().top()->getCoord();
	delete m_C_Path;
	m_C_Path = new C_Path(dest.x,dest.y);
	m_C_Path->calcPath(m_coord.getXGrid(),m_coord.getYGrid(),dest.x,dest.y);
	C_Coord newNext = m_C_Path->getPath().top()->getCoord();
	if(oldNext.getXGrid() != newNext.getXGrid() || oldNext.getYGrid() != newNext.getYGrid() ){
			//force passing by the center of the tile before going to the next step
			if(!m_coord.closeToCenter(m_coord.getGrid(),12)) {
				m_C_Path->addANodeAtTheStartOfThePath(m_coord.getGrid());
			} else {
				m_coord.centerOnTile();
			}
	}
	m_C_Path->showPath();

}

