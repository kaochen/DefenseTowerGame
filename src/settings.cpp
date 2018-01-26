#include "settings.h"

using namespace std;

C_Set C_Set::m_instance=C_Set();

C_Set::C_Set()
{
	cout << "Construct C_Set" << endl;
	m_gridSize = GRID_SIZE;
	m_windowWidth = WINDOW_WIDTH;
	m_windowHeight = WINDOW_WIDTH * ASPECT_RATIO;
	m_gridNbrOfLine = 28;
	m_gridNbrOfRow = 15;
	m_first_tile_x = 13;
	m_first_tile_y = 0;
}

C_Set::~C_Set()
{
}

C_Set& C_Set::Instances()
{
	return m_instance;
}

int C_Set::getGridSize(){
	return m_gridSize;
}

int C_Set::getWindowWidth(){
	return m_windowWidth;
}

int C_Set::getWindowHeight(){
	return m_windowHeight;
}


int C_Set::getGridWidth(){
	return m_windowWidth / TILE_HALF_WIDTH;
}

int C_Set::getGridHeight(){
	return m_windowHeight / TILE_HALF_HEIGHT;
}

int C_Set::getGridNbrOfLine(){
	return m_gridNbrOfLine;
}

int C_Set::getGridNbrOfRow(){
	return m_gridNbrOfRow;
}

int C_Set::getGridFirstTileX(){
	return m_first_tile_x;
}

int C_Set::getGridFirstTileY(){
	return m_first_tile_y;
}

//---------------------------------------------------------------

C_Coord::C_Coord(int x_grid, int y_grid){

	if (x_grid < 0)
		x_grid = 0;
	if (y_grid < 0)
		y_grid = 0;
	if (x_grid > GRID_SIZE)
		x_grid = GRID_SIZE;
	if (y_grid > GRID_SIZE)
		y_grid = GRID_SIZE;

	m_this.grid.x = x_grid;
	m_this.grid.y = y_grid;
	m_this.screen = gridToScreen(m_this.grid);
}

C_Coord::C_Coord(S_Coord coord){
	if (coord.x < 0) //to quite gcc
		cout << "";
}

C_Coord::~C_Coord(){
};

S_Coord C_Coord::screenToGrid(S_Coord screen){
		C_Set& settings=C_Set::Instances();
		float xOffset = (settings.getWindowWidth() /2);
		float yOffset = (settings.getWindowHeight() /2);
		float tempX = 0.0, tempY = 0.0;
		S_Coord coord;
		tempX = ( ((screen.x - xOffset ) / TILE_HALF_WIDTH + (screen.y + yOffset)/TILE_HALF_HEIGHT )/2);
		tempY = ( (screen.y + yOffset )/(TILE_HALF_HEIGHT*2) - (screen.x - xOffset)/(TILE_HALF_WIDTH*2));
		coord.x = tempX;
		coord.y = tempY;
		return coord;
		}

S_Coord C_Coord::gridToScreen(S_Coord grid){
			C_Set& settings=C_Set::Instances();
			S_Coord screen;
			screen.x = settings.getWindowWidth()/2 + (grid.x - grid.y)* TILE_HALF_WIDTH;
			screen.y = (grid.y + grid.x) * TILE_HALF_HEIGHT - settings.getWindowHeight()/2;
			return screen;
}

void C_Coord::displayStatus(){
 	cout << "\tx_grid:"<< m_this.grid.x << " y_grid:"<< m_this.grid.y;
 	cout << "\tx_screen:"<< m_this.screen.x << " y_screen:"<< m_this.screen.y << endl;
}

S_Coord C_Coord::getGrid(){
	return m_this.grid;
}

int C_Coord::getXGrid(){
	return m_this.grid.x;
}

int C_Coord::getYGrid(){
	return m_this.grid.y;
}

S_Coord C_Coord::getScreen(){
	return m_this.screen;
}

void C_Coord::updateScreen(S_Coord new_screen){
	m_this.screen = new_screen;
}

int C_Coord::getXScreen(){
	return m_this.screen.x;
}
int C_Coord::getYScreen(){
	return m_this.screen.y;
}

void C_Coord::centerOnTile(){
	C_CoordGrid tmp(m_this.grid);
	m_this.screen = tmp.getScreen ();
	m_this.screen.y += TILE_HALF_HEIGHT;
	m_this.grid = tmp.getGrid ();
}

int C_Coord::angleToDirection(double angle){
			int  direction = UNKNOWN;
			if (angle > -22.5 && angle <= 22.5){
				direction = SOUTH;
				}
			else if (angle > 22.5 && angle <= 67.5){
				direction = SOUTH_EAST;
			}
			else if(angle > 67.5 && angle <=112.5){
				direction = EAST;
				}
			else if(angle > 112.5 && angle <=157.5){
				direction = NORTH_EAST;
				}
			else if((angle > 157.5 && angle <=180) || (angle > -180 && angle <= -157.5)){
				direction = NORTH;
				}
			else if(angle > -157.5 && angle <= -112.5){
				direction = NORTH_WEST;
				}
			else if(angle > -112.5 && angle <=-67.5){
				direction = WEST;
				}
			else if(angle > -67.5 && angle <=-22.5){
				direction = SOUTH_WEST;
				}
			else{
				direction = UNKNOWN;
				}
			return direction;
}

void C_Coord::move(double angle, int speed){

	double rot = 22.5;
	angle = angle *180/3.14159265359 + 2*rot + rot/2;
	cout << "Angle : " << angle << endl;
	int x_45 = speed*75/100;
	int y_45 = x_45/2;

	if (angle > 0 && angle <= rot){
			cout << "SOUTH" << endl;
			m_this.screen.x -= x_45;
			m_this.screen.y += y_45;
			}
	else if (angle > rot && angle <= 2*rot){
	   		cout << "SOUTH_SOUTH_EAST" << endl;
	   		m_this.screen.x -= speed*38/100 ;
			m_this.screen.y += speed/2;
			}
	else if (angle > 2*rot && angle <= 3*rot){
			cout << "SOUTH_EAST" << endl;
			m_this.screen.y +=speed/2;
			}
	else if(angle > 3*rot && angle <= 4*rot){
			cout << "EAST_SOUTH_EAST" << endl;
	   		m_this.screen.x -= speed*38/100 ;
			m_this.screen.y += speed/2;
				}
	else if(angle > 4*rot && angle <= 5*rot){
			cout << "EAST" << endl;
			m_this.screen.x += x_45;
			m_this.screen.y += y_45;
				}
	else if(angle > 5*rot && angle <= 6*rot){
			cout << "EAST_NORTH_EAST" << endl;
			m_this.screen.x += speed*90/100;
			m_this.screen.y += speed*25/100;
				}
	else if(angle > 6*rot && angle <= 7*rot){
			cout << "NORTH_EAST" << endl;
			m_this.screen.x +=speed;
				}
	else if(angle > 7*rot && angle <= 8*rot){
			cout << "NORTH_NORTH_EAST" << endl;
			m_this.screen.x += speed*90/100;
			m_this.screen.y -= speed*12/100;
				}
	else if(angle > 8*rot && angle <= 9*rot){
			cout << "NORTH" << endl;
			m_this.screen.x += x_45;
			m_this.screen.y -= y_45;
				}
	else if(angle > 9*rot && angle <= 10*rot){
			cout << "NORTH_NORTH_WEST" << endl;
	   		m_this.screen.x += speed*38/100 ;
			m_this.screen.y -= speed/2;
				}
	else if(angle > 10*rot && angle <= 11*rot){
			cout << "NORTH_WEST" << endl;
			m_this.screen.y -=speed/2;
				}
	else if(angle > 11*rot && angle <= 12*rot){
			cout << "WEST_NORTH_WEST" << endl;
	   		m_this.screen.x -= speed*38/100 ;
			m_this.screen.y += speed/2;
				}
	else if(angle > 12*rot && angle <= 13*rot){
			cout << "WEST" << endl;
			m_this.screen.x -= x_45;
			m_this.screen.y -= y_45;
				}
	else if(angle > 13*rot && angle <= 14*rot){
			cout << "WEST_SOUTH_WEST" << endl;
			m_this.screen.x -= speed*90/100;
			m_this.screen.y += speed*25/100;
				}
	else if(angle > 14*rot && angle <= 15*rot){
			cout << "SOUTH_WEST" << endl;
				m_this.screen.x -=speed;
				}
	else if(angle > 15*rot && angle <= 16*rot){
			cout << "SOUTH_SOUTH_WEST" << endl;
				}
	else{
		cout << "Angle is not between 0 and 360 : " << angle << endl;
	}

}

void C_Coord::regenGridCoord(){
		m_this.grid = screenToGrid(m_this.screen);
}


bool C_Coord::closeToCenter(){
		C_CoordGrid tmp(m_this.grid);
		tmp.centerOnTile();
		S_Coord center = tmp.getScreen();
		int l = m_this.screen.x - center.x;
		int h = m_this.screen.y - center.y;
		if (l < 0)
			l *=-1;
		if (h < 0)
			h *=-1;
		if(l < TILE_HALF_WIDTH/2 && h < TILE_HALF_HEIGHT/2){
			cout << "center true" << endl;
			return true;
			}
		else{
			cout << "center false" << endl;
			return false;
			}



}


C_CoordGrid::C_CoordGrid(S_Coord coord): C_Coord(coord){
		m_this.grid.x = coord.x;
		m_this.grid.y = coord.y;
		m_this.screen = gridToScreen(m_this.grid);
		//cout << "grid to screen" << endl;
}

C_CoordGrid::C_CoordGrid(int x_grid, int y_grid ): C_Coord(x_grid, y_grid){
		if (x_grid < 0)
			x_grid = 0;
		if (y_grid < 0)
			y_grid = 0;
		if (x_grid > GRID_SIZE)
			x_grid = GRID_SIZE;
		if (y_grid > GRID_SIZE)
			y_grid = GRID_SIZE;

		S_Coord coord;
		coord.x = x_grid;
		coord.y = y_grid;
		C_CoordGrid tmp(coord);
		m_this.screen = tmp.getScreen ();
		m_this.grid = tmp.getGrid ();
}

C_CoordGrid::~C_CoordGrid()
{
};

C_CoordScreen::C_CoordScreen(S_Coord coord): C_Coord(coord){
		m_this.screen.x = coord.x;
		m_this.screen.y = coord.y;
		m_this.grid = screenToGrid(m_this.screen);
		//cout << "screen to grid" << endl;
}

C_CoordScreen::C_CoordScreen(int x_screen, int y_screen ): C_Coord(x_screen, y_screen){

		C_Set& settings=C_Set::Instances();
		if (x_screen < 0)
			x_screen = 0;
		if (y_screen < 0)
			y_screen = 0;
		if (x_screen > settings.getWindowWidth())
			x_screen = settings.getWindowWidth();
		if (y_screen > settings.getWindowHeight())
			y_screen = settings.getWindowHeight();

		S_Coord coord;
		coord.x = x_screen;
		coord.y = y_screen;
		C_CoordScreen tmp(coord);
		m_this.screen = tmp.getScreen ();
		m_this.grid = tmp.getGrid ();
}

C_CoordScreen::~C_CoordScreen()
{
};

