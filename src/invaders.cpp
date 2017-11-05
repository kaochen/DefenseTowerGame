#include "invaders.h"

using namespace std;


C_invaders::C_invaders(int x_grid,
			 int y_grid,
			 int rank,
			 C_GameUnits::S_layer grid[][TABLE_SIZE]):C_Shooter("boat", x_grid, y_grid ,rank, grid)
{
}

C_invaders::~C_invaders()
{
}


void C_invaders::move(int direction,
		      C_GameUnits::S_layer grid[][TABLE_SIZE])
{
	int speed = 2;
	switch (direction){
		case EAST:
			m_x_screen += speed;
			m_y_screen += speed/2;
		break;
		case WEST:
			m_x_screen -= speed;
			m_y_screen -= speed/2;
		break;
		case NORTH:
			m_x_screen += speed;
			m_y_screen -= speed/2;
		break;
		case SOUTH:
			m_x_screen -= speed;
			m_y_screen += speed/2;
		break;
	}
	grid[m_x_grid][m_y_grid].main = nullptr; //delete previous position
	xyScreenToXYGrid();
	grid[m_x_grid][m_y_grid].main = this; //move to new position
}



void C_invaders::renderLifeBar(int x_screen, int y_screen, SDL_Renderer *renderer)
	{
		//add a life status above the boat
		int l = m_life / 2;
		int red = 0, green = 200;
		SDL_Rect r, b;
		    r.x = x_screen - TILE_HALF_WIDTH/2;
		    r.y = y_screen + 64;
		    r.w = l;
		    r.h = 4;
		    if (m_life < MAX_LIFE/2){
		    	red = 200;
		    	green = 0 ;
		    	}
		    b.x = r.x - 1;
		    b.y = r.y - 1;
		    b.w = MAX_LIFE/2 + 2;
		    b.h = r.h + 2;

		    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
		    SDL_RenderFillRect( renderer, &b );
		    SDL_SetRenderDrawColor( renderer, red, green, 0, 255 );
		    // Render rect
		    SDL_RenderFillRect( renderer, &r );
	}

void C_invaders::render(int x_screen, int y_screen, SDL_Renderer *renderer){
	C_GameUnits::render(x_screen, y_screen,renderer);
	renderLifeBar(x_screen, y_screen, renderer);
}

