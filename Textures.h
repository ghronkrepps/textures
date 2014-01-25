#ifndef TEXTURES_H
#define TEXTURES_H

#include "StringUtil.h"
#include "Animation.h"

/**
*    Textures.h
* 
*	This file has all of the functions used to load, store, and recall texture references
*	from files to memory.
*/

enum TextureType
{
	TextureType_Sprite,
	TextureType_Animation
};

/**
*    Load textures and add all animation and sprite references
* 
* @param renderer A pointer to the SDL_Renderer to be used for rendering the images
*/
void InitializeTextures(SDL_Renderer* renderer);

/**
*    Destroy all textures cached. Call once before shutting down the game.
* 
* @param renderer A pointer to the SDL_Renderer to be used for rendering the images
*/
void ShutdownTextures();

/**
*	Load a file with animation or sprite data. The image file name and data file name must match.
*	Files are tab delimited and each line follows one of the following three formats
*
*	Sprite							reference	width	height	x	y
*	First Animation Frame			reference	frameNumber	width	height	x	y	AnimationType	frameDelay
*	Additional Animation Frame		reference	frameNumber	width	height	x	y
* 
* @param fileName The path and name of the file 
* @param reference The unique name to refer to the file as
* @param renderer A pointer to the SDL_Renderer to be used for rendering this file
*/
void LoadFile(const String& fileName, const String& reference, SDL_Renderer* renderer);

/**
* Prepares a text data file for line processing by removing any '\n' character
* 
* @param rw A pointer to the file as a SDL_RWops pointer
* @param reference The unique name of the file reference to store the lines
* @return bool True if the file was processed correctly; false if there was an error
*/
bool ProcessAndroidTextFile(SDL_RWops* rw, const String& reference);

/**
* Processes an individual line of an animation or sprite data file to add each line as an
* animation or sprite reference
* 
* @param line the line to process
* @param reference The unique name of the sprite or animation reference to refer back to
* @return bool True if the line was processed correctly; false if there was an error
*/
bool ProcessFileLine(const String& line, const String& reference);

/**
* Add an image file to be stored
* 
* @param fileName The path and name of the file 
* @param reference The unique name to refer to the file as
* @param renderer A pointer to the SDL_Renderer to be used for rendering this file
* @return bool True if the texture was generated and added; false if there was an error
*/
bool AddFileReference(const String& fileName, const String& reference, SDL_Renderer* renderer);

/**
*    Add animation information of an animation stored on prereferenced file. If the 
*    animationReference is found already the frame will be added to the end as part of
*    the animation. Only the first frame must include the animationType and frameDelay.
*    Each additional frame added with the same reference name will use the first frames values.
* 
*    Be sure to add all of the animations frames in order.
*
* @param fileReference The path and name of the file 
* @param animationReference The unique name to refer to the animation as
* @param width The width of the frame in pixels
* @param height The height of the frame in pixels
* @param x The x location of the upper left pixel of the frame
* @param y The y location of the upper left pixel of the frame
* @param animationType The AnimationType of the animation (only required for the first frame)
* @param frameDelay The delay between frames of the animation (only required for the first frame)
*/
void AddAnimationReference(const String& fileReference, String animationReference,
							int width, int height, int x, int y, int animationType = 0, float frameDelay = 0.0);

/**
*    Add sprite information stored on prereferenced file.
*
* @param fileReference The path and name of the file 
* @param spriteReference The unique name to refer to the sprite as
* @param width The width of the sprite in pixels
* @param height The height of the sprite in pixels
* @param x The x location of the upper left pixel of the sprite
* @param y The y location of the upper left pixel of the sprite
*/
void AddSpriteReference(const String& fileReference, String spriteReference, int width, int height, int x, int y);

/**
*	Called by GetTexture. Gets the texture file stored under the given fileReference
*
* @param fileReference The unique name given to the file
* @return SDL_Texture* A pointer to the SDL_Texture for the given reference
*/
SDL_Texture* GetTextureFileReference(const String& textureReference);

/**
*	Return the SDL_Texture pointer stored under the given reference
*
* @param type The TextureType of the texture to get
* @return SDL_Texture* A pointer to the SDL_Texture for the given reference
*/
SDL_Texture* GetTexture(TextureType type, const String& reference);

/**
*	Return the AnimationReference information stored under the given animationReference name
*
* @param animationReference The unique name given to the animation
* @param supressWarning Whether to display a warning when the animation is not found
* @return AnimationReference* A pointer to the AnimationReference information stored under the unique animationReference name
*/
AnimationReference* GetAnimationReference(const String& animationReference, const bool& supressWarning = false);

/**
*	Return the SpriteReference information stored under the given spriteReference name
*
* @param spriteReference The unique name given to the sprite
* @param supressWarning Whether to display a warning when the sprite is not found
* @return SpriteReference* A pointer to the SpriteReference information stored under the unique spriteReference name
*/
SpriteReference* GetSpriteReference(const String& spriteReference);

/**
*	Fills a supplied SDL_Rect pointer with the location of the given animationReference and frame
*
* @param animationReference The animationReference to get location information for
* @param frame The frame number to get
* @param source A pointer to the SDL_Rect to fill
*/
void SetAnimationSourceRect(const String& animationReference, const int frame, SDL_Rect* source);

/**
*	Fills a supplied SDL_Rect pointer with the location of the given spriteReference
*
* @param animationReference The spriteReference to get location information for
* @param source A pointer to the SDL_Rect to fill
*/
void SetSpriteSourceRect(const String& spriteReference, SDL_Rect* source);





/* Deprecated functions

void AddSprite(const String& fileName, String spriteName, SDL_Renderer* renderer);

void AddAnimation(const String& fileName, const String& animationName,
							int animationType, int width, int height, float frameDelay, SDL_Renderer* renderer);

void AddAnimationFrame(const String& fileName, const String& animationName,
							int animationType, float frameDelay, SDL_Renderer* renderer);

SDL_Texture* GetSpriteTexture(const String& spriteName);
SDL_Texture* GetAnimationTexture(size_t animationFrame);

*/

#endif //TEXTURES_H
