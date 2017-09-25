#include "surfaces.h"

using namespace std;

C_Texture::C_Texture(string name, SDL_Texture* image, int seqNbr):
	m_name(name),
	m_image(image),
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
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;

	SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(renderer, texture, NULL, &pos);
}


SDL_Texture* C_Texture::getImage() const
{
	return m_image;
}
