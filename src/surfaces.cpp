#include "surfaces.h"

using namespace std;


map<string, SDL_Texture*> C_Texture::map_textures;


C_Texture::C_Texture():
	m_name("texture"),
	m_seqNbr(1)
{
}

C_Texture::C_Texture(string name, int seqNbr):
	m_name(name),
	m_seqNbr(seqNbr)
{
}

C_Texture::~C_Texture(){
}

SDL_Texture* loadTexture(const string &path, SDL_Renderer *renderer)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *image = IMG_Load(path.c_str());
	if (image != nullptr)
		{
		texture = SDL_CreateTextureFromSurface(renderer,image);
		SDL_FreeSurface(image); //Don't need anymore

		if (texture == nullptr)
			{
			logSDLerror("SDL_CreateTextureFromSurface() failed");
			}
		}
	else
	{
		logSDLerror("IMG_LOAD()");
	}
	return texture;
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y)
{
	if((x >= 0 || x <= C_Settings::getWindowWidth()) && ( y >= 0  || y <= C_Settings::getWindowHeight())){
		SDL_Rect pos;
		SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
		pos.x = x - pos.w/2;
		pos.y = y - pos.h/2;
		SDL_RenderCopy(renderer, texture, NULL, &pos);
		}
}

map<string, SDL_Texture*>  C_Texture::getTextMap(){
	return map_textures;
}

SDL_Texture* C_Texture::getText(string name){
	map<string, SDL_Texture*> t = C_Texture::getTextMap();
	map<string, SDL_Texture*>::iterator search = t.find(name);
	if(search == t.end()){
		cout << name << " not available in the texture map" << endl;
		return nullptr;
	}
	else{
		return t[name];
	}
}

void C_Texture::loadTexturesIntoMap(SDL_Renderer *renderer){
	C_Texture::map_textures = C_Texture::getTextMap();
	map_textures["Tower_00_01.png"] = loadTexture("data/img/original/Tower_00_01.png", renderer);
	map_textures["Tower_01_01.png"] = loadTexture("data/img/original/Tower_01_01.png", renderer);
	map_textures["boat_01_00.png"] = loadTexture("data/img/original/boat_01_00.png", renderer);
	map_textures["SimpleTile.png"] = loadTexture("data/img/original/SimpleTile.png", renderer);
	map_textures["SimpleWaterTile.png"] = loadTexture("data/img/original/SimpleWaterTile.png", renderer);
	map_textures["Tile_Highlight_Grenn.png"] = loadTexture("data/img/original/Tile_Highlight_Grenn.png", renderer);

}

//displayContent of the grid
void displayGridContent(SDL_Renderer *renderer,
			C_GameUnits* grid_units[][TABLE_SIZE]){
	size_t const gridSize = C_Settings::getGridSize();
	for (size_t y = 0; y < gridSize; y++){
		for (size_t x = 0; x < gridSize; x++){
			int x_iso = C_Settings::getWindowWidth()/2 + (x - y)* TILE_HALF_WIDTH;
			int y_iso = (y + x) * TILE_HALF_HEIGHT - C_Settings::getWindowHeight()/2  ;
			if (grid_units[x][y] != nullptr){
				grid_units[x][y]->render(x_iso, y_iso, renderer);
				//cout << "x:"<< x_iso << " y:" << y_iso << endl;
				}
			else{
				renderTexture(C_Texture::getText("SimpleTile.png"), renderer, x_iso,y_iso);

			}
		}
	}
}
