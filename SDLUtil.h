#ifndef SDLUTIL_H
#define SDLUTIL_H

#include "StringUtil.h"

#include "SDL_image.h"
#include "SDL_ttf.h"

#if defined(__ANDROID__)
	#include <android/log.h>
#endif

#include <iostream>
#include <fstream>
#include <time.h>

//#include "SDL_gfxBlitFunc.h"

/**
*    SDLUtil.h
* 
*	This file has all of the general functions used to initialize and setup SDL as
*	well as creating surfaces and fonts from their files, rendering functions, and
*   other general functions.
*/

/**
* Initialize SDL and create a window
* @param windowName The title of the window
* @param windowWidth The width of the window
* @param windowHeight The height of the window
* @return a pointer to the window or null if there was an error
*/
SDL_Window* InitSDL(const char* windowName, int windowWidth, int windowHeight);

/**
* Create a renderer and set it up
* @param window A pointer to the SDL_Window object to create the renderer for
* @return a pointer to the renderer or null if there was an error
*/
SDL_Renderer* InitSDLRenderer(SDL_Window* window);

/**
* Create a surface in memory
*
* @param The width of the surface in pixels
* @param The height of the surface in pixels
* @return The loaded surface or nullptr if something went wrong
*/
SDL_Surface* CreateSurface(int width, int height);

/**
* Loads any compatible image into into a surface
*
* @param file The image file to load
* @return the loaded surface or nullptr if something went wrong
*/
SDL_Surface* LoadSurfaceFromFile(const String& file);

/**
* Loads any compatible image into a texture on the rendering device
*
* @param file The image file to load
* @param renderer The renderer to load the texture onto
* @return the loaded texture or nullptr if something went wrong
*/
SDL_Texture* LoadTextureFromFile(const String& file, SDL_Renderer *renderer);

/**
* Use the special GFX library blit function to avoid transparency loss
*
* @param texture The source texture we want to draw
* @param renderer The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
*/
//function removed

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving the texture's width and height
*
* @param texture The source texture we want to draw
* @param renderer The renderer we want to draw too
* @param x The x coordinate to draw too
* @param y The y coordinate to draw too
*/
void DrawTextureToRenderer(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y);

/**
* Open a TTF_Font object with the specified size
*
* @param file The file name of the font
* @param fontSize The size of the font to use
*/
TTF_Font* LoadFont(const String& file, int fontSize);

/**
* Return a texture of a ttf message
*
* @param message The message to display
* @param fontFile The font file
* @param color The SDL_Color to use
* @param renderer The renderer to use
*/
SDL_Texture* RenderText(const String& message, SDL_Color color, TTF_Font *font, SDL_Renderer* renderer);

/**
* Return a texture of a ttf message that has an outline
*
* @param message The message to display
* @param color The SDL_Color to use
* @param color The SDL_Color to use for the outline
* @param outlineSize The size of the outline (used to offset the foreground)
* @param fontFile The font file
* @param fontFile The font file for the outline
* @param renderer The renderer to use
*/
SDL_Texture* RenderOutlinedText(const String& message, SDL_Color color, SDL_Color outlineColor,
								TTF_Font *font, TTF_Font *outlinefont, SDL_Renderer* renderer);

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg);

/**
* Log a general error
* @param os The output stream to write the message too
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logError(std::ostream &os, const std::string &msg);

/**
* Check if a file exists
* @param filename the file to check for
* @return true if the file exists; false otherwise
*/
bool fileExists(const char *filename);

const String GetCurrentDateTime();

#endif
