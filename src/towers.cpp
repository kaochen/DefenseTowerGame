#include "towers.h"
#include "time.h"
#include "grid.h"

#include <SDL2_gfxPrimitives.h>

using namespace std;


C_Towers::C_Towers(string name,
		   int x_grid,
		   int y_grid,
		   int rank):C_Shooter(name, x_grid, y_grid, rank)
{
	m_lastSmokeTime = 0;
	m_smokeNbr = 1;
	m_justAdded = true;
}


void C_Towers::renderSmoke(){
	int imageNbr = m_animation[JUSTADDED]->getAnimNbr(0,7,100);
	//smoke_01_smoke0
	string fileName = "smoke_01_smoke" + to_string(imageNbr);
	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(fileName, m_x_screen,m_y_screen + 36);
	if (imageNbr == 7)
		m_justAdded = false;
}

void C_Towers::render(S_Coord screen){
	renderSelected();
	C_Shooter::render(screen);
	if (m_justAdded)
		renderSmoke();
}

void C_Towers::renderSelected(){
	if (m_selected == true){
		int animNbr = m_animation[SELECTED]->getAnimNbr(0,30,500);
		int width = m_weapon->getFireRange();
		drawEllipse(m_x_screen,m_y_screen + 128,width,animNbr, true);
	}
}

void C_Towers::drag(int x_screen, int y_screen)
{
	m_justAdded = false;
	C_Grid& grid=C_Grid::Instances();
	int xGrid = grid.xScreenToXGrid(x_screen,y_screen);
	int yGrid = grid.yScreenToYGrid(x_screen,y_screen);
	int animNbr = m_animation[DRAG]->getAnimNbr(0,30,500);
	int width = m_weapon->getFireRange();
	int x = xGrid;
	int y = yGrid;
	//draw ellipse
	bool status = grid.isThisConstructible(xGrid,yGrid);
	drawEllipse(x_screen,y_screen,width,animNbr, status);
	//draw square
	for(int i = 0; i < 3; i++){
		y++;
		for(int j = 0; j < 3; j++){
			x++;
			status = grid.isThisConstructible(x-2,y-2);
			int x_s = grid.xGridToXScreen(x,y);
			int y_s = grid.yGridToYScreen(x,y) + 50;
			drawRhombus(x_s,y_s,70,40,status);
			if (i == 1 && j == 1){
				drawRhombus(x_s,y_s,70,90,status);
				}

		}
		x = xGrid;
	}

	S_Coord screen;
	screen.x = x_screen;
	screen.y = y_screen - 170;
	C_Shooter::render(screen);
}

void C_Towers::drawEllipse(int x,
		int y,
		int width,
		int animNbr,
		bool ok){
		C_Window& win=C_Window::Instances();
		int height = width/2;
		int R = 0, G = 200, B = 0, A = 100;
			if(ok == false)
				R = 120, G = 0, B = 0;

		aaellipseRGBA(win.getRenderer(),x,y,width+1,height+1,R,G,B,A);
		filledEllipseRGBA(win.getRenderer(),x,y,width,height,R,G,B,(A/4));
		width += animNbr;
		height = width /2;
		//cout << width <<":" << height << endl;
		aaellipseRGBA(win.getRenderer(),x,y,width,height,R,G,B,(A/2));
}

void C_Towers::drawRhombus(int x, int y, int width, int alpha, bool ok){
	C_Window& win=C_Window::Instances();
	SDL_Renderer * renderer = win.getRenderer();
	Sint16 w =  width/2;
	Sint16 h =  w/2;
	Sint16 x1 = x - w;
	Sint16 yOffset = TILE_HALF_HEIGHT - h;
	if (yOffset > 1) //to avoid division by 0;
		yOffset /= 2;

	Sint16 y1 = y - h - yOffset; //center
	Sint16 x2 = x1 + w;
	Sint16 y2 = y1 + h;
	Sint16 x3 = x1 + (w*2);
	Sint16 y3 = y1;
	Sint16 x4 = x2;
	Sint16 y4 = y1 - h;
	Sint16 vx[] = {x1,x2,x3,x4};
	Sint16 vy[] = {y1,y2,y3,y4};
	int R = 0, G = 200, B = 0, A = alpha;
			if(ok == false)
				R = 120, G = 0, B = 0;
	filledPolygonRGBA(renderer,vx,vy,4,R,G,B,A);

	A = alpha * 2;
	if (alpha > 255)
		alpha = 255;
	aapolygonRGBA(renderer,vx,vy,4,R,G,B,A);

}
//---------------------------------------------------

C_ArcherTower::C_ArcherTower(int x_grid,
		   int y_grid,
		   int rank):C_Towers("ArcherTower", x_grid, y_grid, rank)
{
	m_weapon = new C_Weapon("ARCHER",10,500,200);
}

C_Turbine::C_Turbine(int x_grid,
		   int y_grid,
		   int rank):C_Towers("Turbine", x_grid, y_grid, rank)
{
	m_weapon = new C_Weapon("WIND",15,700,300);
}

void C_Turbine::render(S_Coord screen){
	renderSelected();
	int rotationSpeed = 200;
	if (m_weapon->getShooting())
		rotationSpeed = 50;
	int imageNbr = m_animation[MAIN_ANIM]->getAnimNbr(0,7,rotationSpeed);
	string fileName = m_name + "_0" + to_string(m_rank) + "_" + m_strDirection + "_" + to_string(imageNbr) ;
	//cout << "image name is "<< fileName << endl;

	C_TextureList& t=C_TextureList::Instances();
	t.renderTexture(fileName, screen.x,screen.y + m_y_center_offset);

	if (m_justAdded)
		renderSmoke();
}

void C_Turbine::drawEllipse(int x_screen,
		int y_screen,
		int width,
		int animNbr,
		bool ok){
	C_Window& win=C_Window::Instances();
	SDL_Renderer * renderer = win.getRenderer();

	int R = 0, G = 200, B = 0, A = 100;
			if(ok == false)
				R = 120, G = 0, B = 0;

		Sint16 w = width/2;
		Sint16 h = w/2;
		Sint16 x = x_screen;
		Sint16 y = y_screen;
		if  (m_strDirection == "NE"){
			x = x_screen + w;
			}
		else if(m_strDirection == "SW"){
			x = x_screen - w;
			}
		else if(m_strDirection == "NW"){
			y = y_screen - h;
			}
		else if(m_strDirection == "SE"){
			y = y_screen + h;
			}
		else if(m_strDirection == "EE"){
			x = x_screen + 2*w/3;
			y = y_screen + 2*h/3;
			}
		else if(m_strDirection == "SS"){
			x = x_screen - 2*w/3;
			y = y_screen + 2*h/3;
			}
		else if(m_strDirection == "WW"){
			x = x_screen - 2*w/3;
			y = y_screen - 2*h/3;
			}
		else if(m_strDirection == "NN"){
			x = x_screen + 2*w/3;
			y = y_screen - 2*h/3;
			}
		else{
			cout <<"\""<<m_strDirection << "\" unknow m_strDirection";
		}

		filledEllipseRGBA(renderer,x,y,w,h,R,G,B,A/4);

		A = A * 2;
		if (A > 255)
			A = 255;
		aaellipseRGBA(renderer,x,y,w+1,h+1,R,G,B,A);

		w += animNbr;
		h = w/2;
		//cout << width <<":" << height << endl;
		aaellipseRGBA(win.getRenderer(),x,y,w,h,R,G,B,(A/2));
}
