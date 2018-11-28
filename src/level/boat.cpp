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

using namespace std;

C_Boat::C_Boat(S_UnitModel boat):C_Shooter("boat", boat.coord.x, boat.coord.y ,boat.rank)
{
    m_rank = boat.rank;
    m_max_health = boat.health;
    m_health = m_max_health;
	m_weapon = new C_Weapon("BOAT",boat.weapon.damage,0,boat.weapon.fireRate,boat.weapon.fireRange);
	m_moving = false;
	m_speed = boat.speed;
	m_speedImpact = 0;
	m_speedImpactLoop = 60;
	m_coord->centerOnTile();
	//Find a way to town
	C_Grid& grid=C_Grid::Instances();
	S_Coord town = grid.foundTown();
	m_C_Path = new C_Path(town.x,town.y);
	m_C_Path->calcPath(boat.coord.x,boat.coord.y,town.x,town.y);
	m_C_Path->showPath();
	m_direction = EAST;
	m_animDirection = new C_AnimTime();
	m_countStop = 0;
	m_countRegenPath = 0;
}

C_Boat::~C_Boat()
{
    delete m_animDirection;
    delete m_C_Path;
}

void C_Boat::play(){
	this->move();
	string list[4] = {"town","barricade","ArcherTower","Turbine"};
	this->shoot(list, 4);

	if(!this->alive())
		this->kill();
};

void C_Boat::kill(){
    C_Shooter::kill();
    C_Wallet& wallet=C_Wallet::Instances();
    wallet.credit(m_cost); //reward when killing a boat
    wallet.cliStatus();
}

void C_Boat::move()
{
    m_C_Path->regenScreenCoord(); //first refresh the coord for the path in case of the window has moved
	m_moving = true;
	C_Grid& grid=C_Grid::Instances();
	S_Coord town =  grid.foundTown();
	*m_old_coord = *m_coord;
	std::stack<C_Node*> path;
	path = m_C_Path->getPath();
	if(!m_C_Path->closeToDestination(m_coord->getXGrid(),m_coord->getYGrid(),1)){
       if(path.size() > 0){

		    //determine an angle

		    int old_x_grid = m_coord->getXGrid();
		    int old_y_grid = m_coord->getYGrid();

		    C_Coord destCoord = *path.top()->getCoord();
		    destCoord.centerOnTile();
		    S_Coord start = m_coord->getScreen();
		    S_Coord dest = destCoord.getScreen();
		    int ab = dest.x - start.x;
		    int bc = dest.y - start.y;
		    double angle = destCoord.atan2_360(ab,bc);

            int speed = calcSpeed();

		    //move following angle and speed
		    C_Coord tmp = *m_coord;
		    tmp.move(angle,speed);
		    tmp.regenGridCoord();
		    bool nextEmpty = grid.mainEmpty(tmp.getXGrid(),tmp.getYGrid(),this);
            if(!nextEmpty){
		        m_coord->move(angle,speed);
		        m_countStop = 0;
                m_direction = destCoord.angleToDirection(angle);
		        m_coord->regenGridCoord();

		        grid.moveUnit(old_x_grid, old_y_grid,  m_coord->getXGrid (), m_coord->getYGrid ());
			        if(m_coord->closeToCenter(destCoord.getGrid(),2)){
			            m_coord->centerOnTile(); //to not deviate too much from the path
			            m_countRegenPath++;
				        m_C_Path->goNextStep();
			        }
		    }
		    else{
		        m_countStop++;
		        int count = FRAMERATE;
		        if(!m_C_Path->closeToDestination(m_coord->getXGrid(),m_coord->getYGrid(),3)){
		            count *= 3;
		            }
		        if (m_countStop > count){
		                recalcPath(town);
		        }
		    }
      }
      if(m_countRegenPath > 3){
            recalcPath(town);
            if(!m_coord->closeToCenter(m_coord->getGrid(),12)){
                m_C_Path->addANodeAtTheStartOfThePath(m_coord->getGrid());
             }
             else{
                m_coord->centerOnTile();
             }

            m_countRegenPath = 0;
      }
	}

	if(!m_C_Path->closeToDestination(m_coord->getXGrid(),m_coord->getYGrid(),1)){
	   int	pauseNbr = m_animation[PAUSESEARCHPATH]->getAnimNbr(1,2,600);
       if(path.size() == 0 && pauseNbr == 2){
		            recalcPath(town);
       }
	}

}




void C_Boat::render(S_Coord screen){
	C_TextureList& t=C_TextureList::Instances();

	int imageNbr = 0;
	int status = ALIVE;
	if (m_moving)
		imageNbr = m_animation[MAIN_ANIM]->getLoopAnimNbr(1,7,80);

	if (this->alive()){
	     status = ALIVE;
	    if (m_weapon->getShooting())
		    m_weapon->render();
	    renderLifeBar(screen.x, screen.y);
        m_C_Path->displayPath();
	    }
	else {
	        status = DEAD;
	        imageNbr = 0;
	     }
    string fileName = imageName(status,m_direction,imageNbr);
    t.renderTexture(fileName, screen.x,screen.y,CENTER_TILE);
}


void C_Boat::receiveDamage(S_Weapon weapon)
{
	m_health -=weapon.damage;
	m_speedImpact = weapon.speedImpact;
	if (m_health < 0)
	{
		m_health = 0;
	}
}

void C_Boat::recalcPath(S_Coord dest){
		delete m_C_Path;
		m_C_Path = new C_Path(dest.x,dest.y);
	    m_C_Path->calcPath(m_coord->getXGrid(),m_coord->getYGrid(),dest.x,dest.y);
		m_C_Path->showPath();
}

int C_Boat::calcSpeed(){
            int speed = m_speed - m_speedImpact;
            if (speed < 0){
                    speed = VERY_SLOW;
                    }
            //reset malus on speed every x moves
            //cout << "speed" << speed << "=" << m_speed << "-" << m_speedImpact << endl;
            if (m_speedImpactLoop > 0){
                m_speedImpactLoop--;
            }
            else{
                m_speedImpact = 0;
                m_speedImpactLoop=10;
                }
           return speed;
}

