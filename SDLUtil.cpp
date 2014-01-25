#include "stdafx.h"

#include "SDLUtil.h"
#include "SDL_image.h"
#include "SDL_opengl.h"

SDL_Window* InitSDL(const char* windowName, int windowWidth, int windowHeight)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_Window *window;
	#if defined(_WIN32)
		window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	#elif defined(__ANDROID__)
		window = SDL_CreateWindow(windowName, 0, 0, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
	#endif

	if (window == nullptr)
	{
		logSDLError(std::cout, "InitSDL");
		return nullptr;
	}

	return window;
}

SDL_Renderer* InitSDLRenderer(SDL_Window* window)
{
	//SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,SDL_RENDERER_ACCELERATED);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,	0);
	
	if(renderer != nullptr)
	{
		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
		#ifdef __ANDROID__
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
		#elif _WIN32
			SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
		#endif
		//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
		
		SDL_RenderSetLogicalSize(renderer, 640, 360);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	}
	if (renderer == nullptr)
	{
		logSDLError(std::cout, "InitSDLRenderer");
		return nullptr;
	}
	
	return renderer;
}

SDL_Surface* LoadSurfaceFromFile(const String &file)
{
	SDL_Surface *surface = IMG_Load(file.c_str());

	if (surface == nullptr)
		logSDLError(std::cout, "LoadSurfaceFromFile");

	return surface;
}

SDL_Surface* CreateSurface(int width, int height)
{
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const int rmask = 0xff000000;
    const int gmask = 0x00ff0000;
    const int bmask = 0x0000ff00;
	const int amask = 0x000000ff;	
	#else
    const int rmask = 0x000000ff;
    const int gmask = 0x0000ff00;
    const int bmask = 0x00ff0000;
	const int amask = 0xff000000;
	#endif

	SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);

	if (surface == nullptr)
		logSDLError(std::cout, "CreateSurface");

	return surface;
}

SDL_Texture* LoadTextureFromFile(const String &file, SDL_Renderer *renderer)
{
	SDL_Texture *texture = nullptr;
	SDL_Surface *loadedImage = nullptr;

	//android
	#if defined(__ANDROID__)
		SDL_RWops *f = SDL_RWFromFile(file.c_str(), "rb");
		loadedImage = IMG_Load_RW(f , 1);

		if(f > 0)
			__android_log_write(ANDROID_LOG_INFO, "Chain Drop", "File Loaded");
	#elif defined(_WIN32)
		loadedImage = IMG_Load(file.c_str());
	#endif

	//If the loading went ok, convert to texture and return the texture
	if (loadedImage != nullptr)
	{
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);
		//Make sure converting went ok too
		if (texture == nullptr)
			logSDLError(std::cout, "LoadTextureFromFile");
	}
	else
		logSDLError(std::cout, "LoadTextureFromFile");

	return texture;
}

TTF_Font* LoadFont(const String &file, int fontSize)
{
	TTF_Font *font = nullptr;

	#if defined(__ANDROID__)
		SDL_RWops *f = SDL_RWFromFile(file.c_str(), "rb");
		font = TTF_OpenFontRW(f, 1, fontSize);
	#elif defined(_WIN32)
		font = TTF_OpenFont(file.c_str(), fontSize);
	#endif

	if (font == nullptr)
		logSDLError(std::cout, "OpenFont");

	return font;
}

SDL_Texture* RenderText(const String& message, SDL_Color color, TTF_Font *font, SDL_Renderer* renderer)
{
	//Render the message to an SDL_Surface and create a texture to return
	SDL_Surface *surface = nullptr;
	surface = TTF_RenderText_Blended(font, message.c_str(), color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	
	//Clean up unneeded stuff
	SDL_FreeSurface(surface);
 
	return texture;
}

SDL_Texture* RenderOutlinedText(const String& message, SDL_Color color, SDL_Color outlineColor, TTF_Font *font, TTF_Font *outlineFont, SDL_Renderer* renderer)
{
	//Render the message to an SDL_Surface and create a texture to return
	//SDL_Surface *bgSurface = TTF_RenderText_Blended(outlineFont, message.c_str(), outlineColor);
	//SDL_Surface *fgSurface = TTF_RenderText_Blended(font, message.c_str(), color);

	SDL_Surface *bgSurface = TTF_RenderText_Blended(outlineFont, message.c_str(), outlineColor);
	SDL_Surface *fgSurface = TTF_RenderText_Blended(font, message.c_str(), color);
	
	SDL_Rect r = {TTF_GetFontOutline(outlineFont), TTF_GetFontOutline(outlineFont), fgSurface->w, fgSurface->h};

	/* blit text onto its outline */
	SDL_SetSurfaceBlendMode(fgSurface, SDL_BLENDMODE_BLEND);
	SDL_BlitSurface(fgSurface, NULL, bgSurface, &r);

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bgSurface);
 
	//Clean up unneeded stuff
	SDL_FreeSurface(fgSurface); 
	SDL_FreeSurface(bgSurface);

	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height
* @param texture The source texture we want to draw
* @param renderer The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
*/
void DrawTextureToRenderer(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y)
{
	//Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	//Query the texture to get its width and height to use
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void logSDLError(std::ostream &os, const std::string &msg)
{
	os << msg << " error: " << SDL_GetError() << std::endl;
}

void logError(std::ostream &os, const std::string &msg)
{
	os << "Error: " << msg << std::endl;
	
	#if defined(__ANDROID__)
		__android_log_write(ANDROID_LOG_ERROR, "Chain Drop", msg.c_str());
	#endif
}

bool fileExists(const char *filename)
{
	std::ifstream ifile(filename);
	if(ifile)
		return true;
	else
		return false;
}

const String GetCurrentDateTime()
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%m-%d-%Y %X", &tstruct);

	return buf;
}
