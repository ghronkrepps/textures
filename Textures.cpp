#include "stdafx.h"

#include "Textures.h"
#include "SDLUtil.h"

#include <stdio.h>
#include <map>
#include <fstream>

/**
*    Textures.cpp
* 
*	This file has all of the functions used to load, store, and recall texture references
*	from files to memory. It includes three maps that store the texture, sprite, and animation
*	data which is referenced by unique names.
*/

std::map<String, SDL_Texture*> textureReferences;
std::map<String, SpriteReference*> spriteReferences;
std::map<String, AnimationReference*> animationReferences;

void InitializeTextures(SDL_Renderer* renderer)
{
	LoadFile("image/sprites.png", "sprites", renderer);
	LoadFile("image/ui.png", "ui", renderer);
	AddFileReference("image/CloudBox.png", "CloudBox", renderer);
	AddSpriteReference("CloudBox", "CloudBox", 392, 294, 0, 0);

	AddFileReference("image/Arrow.png", "Arrow", renderer);
	AddSpriteReference("Arrow", "Arrow", 16, 57, 0, 0);

	AddFileReference("image/ArrowBorder.png", "ArrowBorder", renderer);
	AddSpriteReference("ArrowBorder", "ArrowBorder", 16, 57, 0, 0);

	AddFileReference("image/Tutorial1.png", "Tutorial1", renderer);
	AddSpriteReference("Tutorial1", "Tutorial1", 160, 80, 0, 0);

	AddFileReference("image/BubblePowerClock.png", "BubblePowerClock", renderer);
	AddSpriteReference("BubblePowerClock", "BubblePowerClock", 17, 17, 0, 0);
	
	AddFileReference("image/BubblePowerThree.png", "BubblePowerThree", renderer);
	AddSpriteReference("BubblePowerThree", "BubblePowerThree", 17, 17, 0, 0);
}

void LoadFile(const String& fileName, const String& reference, SDL_Renderer* renderer)
{
	//Setup the text data filename
	String dataFileName = fileName.substr(0, fileName.length() - 4) + ".txt";

	#ifdef __ANDROID__

		//Open the file with RWops function and process each line
		SDL_RWops *rw = SDL_RWFromFile(dataFileName.c_str(), "rb");

		if(rw > 0)
		{
			__android_log_write(ANDROID_LOG_INFO, "Chain Drop", "Loading texture file");
			ProcessAndroidTextFile(rw, reference);
			SDL_FreeRW(rw);
			textureReferences[reference] = LoadTextureFromFile(fileName, renderer);
		}
		else
			logError(std::cout, "LoadFile: error opening: " + dataFileName);

	#elif _WIN32

		//Open the file with a filestream and process each line
		std::ifstream file(dataFileName);

		if (file.is_open())
		{
			String line;

			while(getline(file, line))
			{
				ProcessFileLine(line, reference);
			}
			file.close();
			textureReferences[reference] = LoadTextureFromFile(fileName, renderer);
		}
		else
			logError(std::cout, "LoadFile: error opening: " + dataFileName);
	#endif
}

bool ProcessAndroidTextFile(SDL_RWops* rw,  const String& reference)
{
	String s;
	char c;

	while(rw->read(rw, &c, 1, 1) != 0)
	{
		s += c;
		if(c == '\n')
		{
			s = s.substr(0, s.length() - 2);
			if(!ProcessFileLine(s, reference))
				return false;
			s = "";
		}
	}

	return true;
}

bool ProcessFileLine(const String& line, const String& reference)
{
	StringList mValues;
	mValues = SplitString(line, "\t", true);

	if(mValues.size() == 8)
	{
		//add as first animation frame
		AddAnimationReference(reference, mValues[0], StringToInt(mValues[2]), StringToInt(mValues[3]),
								StringToInt(mValues[4]), StringToInt(mValues[5]), StringToInt(mValues[6]),
								StringToFloat(mValues[7]));

	}
	else if(mValues.size() == 6)
	{
		//add as additional animation frame
		AddAnimationReference(reference, mValues[0], StringToInt(mValues[2]), StringToInt(mValues[3]),
								StringToInt(mValues[4]), StringToInt(mValues[5]));
	}
	else if(mValues.size() == 5)
	{
		//add as single image
		AddSpriteReference(reference, mValues[0], StringToInt(mValues[1]), StringToInt(mValues[2]),
								StringToInt(mValues[3]), StringToInt(mValues[4]));
	}
	else
		return false;

	return true;
}

bool AddFileReference(const String& fileName, const String& reference, SDL_Renderer* renderer)
{
	if(textureReferences[reference] = LoadTextureFromFile(fileName, renderer))
		return true;
	else
		return false;
}

void AddSpriteReference(const String& fileReference, String spriteReference, int width, int height, int x, int y)
{
	SpriteReference* s = new SpriteReference();

	s->fileReference = fileReference;
	s->spriteReference = spriteReference;
	s->x = x;
	s->y = y;
	s->w = width;
	s->h = height;

	spriteReferences[spriteReference] = s;
}

//void AddAnimationReference(const String& fileReference, String animationReference, int frameCount,
//							int width, int height, int x, int y, int animationType, float frameDelay, int framesPerRow)
//{
//	AnimationReference* a = new AnimationReference();
//
//	a->fileReference = fileReference;
//	a->animationReference = animationReference;
//	a->animationType = animationType;
//	a->frameCount = frameCount;
//	a->frameDelay = frameDelay;
//	a->x = x;
//	a->y = y;
//	a->w = width;
//	a->h = height;
//	a->framesPerRow = framesPerRow;
//
//	animationReferences[animationReference] = a;
//}

void AddAnimationReference(const String& fileReference, String animationReference,
							int width, int height, int x, int y, int animationType, float frameDelay)
{
	AnimationReference* a = GetAnimationReference(animationReference, true);

	if(a)
	{
		a->frames.push_back(AnimationFrame(x, y));
		a->frameCount++;
	}
	else
	{
		a = new AnimationReference();
		a->fileReference = fileReference;
		a->animationReference = animationReference;
		a->animationType = animationType;
		a->frameCount = 0;
		a->frameDelay = frameDelay;
		a->w = width;
		a->h = height;
		a->frames.push_back(AnimationFrame(x, y));
		a->frameCount++;
		animationReferences[animationReference] = a;
	}
}

SDL_Texture* GetTexture(TextureType type, const String& reference)
{
	String textureName;// = nullptr;

	if(type == TextureType_Sprite)
	{
		std::map<String, SpriteReference*>::iterator it = spriteReferences.find(reference);
		if (it == spriteReferences.end())
		{
			logError(std::cout, "GetTextureString: warning " + reference + " not found");
			return nullptr;
		}

		textureName = it->second->fileReference;
	}
	else if(type == TextureType_Animation)
	{
		std::map<String, AnimationReference*>::iterator it = animationReferences.find(reference);
		if (it == animationReferences.end())
		{
			logError(std::cout, "GetTextureString: warning " + reference + " not found");
			return nullptr;
		}

		textureName = it->second->fileReference;
	}

	return GetTextureFileReference(textureName);
}

SDL_Texture* GetTextureFileReference(const String& textureFileReference)
{
	std::map<String, SDL_Texture*>::iterator it = textureReferences.find(textureFileReference);
	if (it == textureReferences.end())
	{
		logError(std::cout, "GetTextureReference: warning " + textureFileReference + " not found");
		return nullptr;
	}

	return it->second;
}

AnimationReference* GetAnimationReference(const String& animationReference, const bool& supressWarning)
{
	std::map<String, AnimationReference*>::iterator it = animationReferences.find(animationReference);
	if (it == animationReferences.end())
	{
		if(!supressWarning)
			logError(std::cout, "GetAnimationReference: warning " + animationReference + " not found");
		return nullptr;
	}

	return it->second;
}

SpriteReference* GetSpriteReference(const String& spriteReference)
{
	std::map<String, SpriteReference*>::iterator it = spriteReferences.find(spriteReference);
	if (it == spriteReferences.end())
	{
		logError(std::cout, "GetSpriteReference: warning " + spriteReference + " not found");
		return nullptr;
	}

	return it->second;
}

void SetSpriteSourceRect(const String& spriteReference, SDL_Rect* source)
{
	std::map<String, SpriteReference*>::iterator it = spriteReferences.find(spriteReference);

	if (it == spriteReferences.end())
	{
		logError(std::cout, "SetSpriteSourceRect: warning " + spriteReference + " not found");
		source->w = 0;
		source->h = 0;
		source->x = 0;
		source->y = 0;

		return;
	}

	source->w = it->second->w;
	source->h = it->second->h;
	source->x = it->second->x;
	source->y = it->second->y;
}

void SetAnimationSourceRect(const String& animationReference, const int frame, SDL_Rect* source)
{
	std::map<String, AnimationReference*>::iterator it = animationReferences.find(animationReference);

	if (it == animationReferences.end())
	{
		logError(std::cout, "SetAnimationSourceRect: warning " + animationReference + " not found");
		source->w = 0;
		source->h = 0;
		source->x = 0;
		source->y = 0;

		return;
	}

	source->w = it->second->w;
	source->h = it->second->h;
	source->x = it->second->frames[frame].mX;
	source->y = it->second->frames[frame].mY;
}

void ShutdownTextures()
{
	//clear all textures and sprites
	while(!textureReferences.empty())
	{
		SDL_DestroyTexture((*textureReferences.begin()).second);
        textureReferences.erase(textureReferences.begin());
	}

	while(!spriteReferences.empty())
	{
		delete (*spriteReferences.begin()).second;
        spriteReferences.erase(spriteReferences.begin());
	}

	while(!animationReferences.empty())
	{
		delete(*animationReferences.begin()).second;
        animationReferences.erase(animationReferences.begin());
	}
}


/* Deprecated functions

void AddSprite(const String& fileName, String spriteName, SDL_Renderer* renderer)
{
	theSprites[spriteName] = LoadTextureFromFile(fileName, renderer);
}

void AddAnimation(const String& fileName, const String& animationName,
							int animationType, int width, int height, float frameDelay, SDL_Renderer* renderer)
{
	Animation* a = new Animation();

	a->AnimationType = animationType;
	a->StartFrame = theTextures.size();

	SDL_Surface* image = LoadSurfaceFromFile(fileName);
	SDL_Surface* frame;

	SDL_Rect source;
	source.w = width;
	source.h = height;

	int columns = image->w / width;
	int rows = image->h / height;
	int frameCount = 0;

	for(int r = 0; r < rows; r++)
	{
		for(int c = 0; c < columns; c++)
		{
			source.x = c * width;
			source.y = r * height;

			frame = CreateSurface(width, height);
			SDL_gfxBlitRGBA(image, &source, frame, NULL);
			theTextures.push_back(SDL_CreateTextureFromSurface(renderer, frame));
			SDL_FreeSurface(frame);
			
			frameCount++;
		}
	}

	SDL_FreeSurface(image);

	a->EndFrame = a->StartFrame + frameCount - 1;
	a->FrameCount = frameCount;
	a->FrameDelay = frameDelay;

	theAnimations[animationName] = a;
}


void AddAnimationFrame(const String& fileName, const String& animationName,
							int animationType, float frameDelay, SDL_Renderer* renderer)
{
	std::map<String, Animation*>::iterator it = theAnimations.find(animationName);

	//load the image as a texture and store it
	SDL_Texture* frame;
	frame = LoadTextureFromFile(fileName, renderer);

	if (it == theAnimations.end())
	{
		//no Animation found create and add one
		Animation* a = new Animation;

		a->AnimationType = animationType;
		a->StartFrame = theTextures.size();
		a->EndFrame = a->StartFrame;
		a->FrameCount = 1;
		a->FrameDelay = frameDelay;

		theAnimations[animationName] = a;
	}

	else
	{
		//found the animation so add this frame to it
		it->second->EndFrame = theTextures.size();
		it->second->FrameCount = it->second->FrameCount + 1;
	}
		theTextures.push_back(frame);
}


SDL_Texture* GetSpriteTexture(const String& spriteName)
{
	std::map<String, SDL_Texture*>::iterator it = theSprites.find(spriteName);
	if (it == theSprites.end())
	{
		logError(std::cout, "GetSpriteTexture: warning " + spriteName + " not found");
		return nullptr;
	}

	return it->second;
}

SDL_Texture* GetAnimationTexture(size_t animationFrame)
{
	if (theTextures.size() <= animationFrame)
	{
		logError(std::cout, "GetAnimationTexture: warning animation" + IntToString(animationFrame) + " not found");
		return nullptr;
	}

	return theTextures[animationFrame];
}

*/
