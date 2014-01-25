#ifndef ANIMATION_H
#define ANIMATION_H

/**
*    Animation.h
* 
*	This file defines all of the animation information and data structures
*	used when creating and referencing animations.
*/

enum AnimationType
{
	AnimationType_None,
	AnimationType_Loop,
	AnimationType_PingPong,
	AnimationType_OneShot
};

struct SpriteReference
{
	String fileReference;
	String spriteReference;
	int x;
	int y;
	int w;
	int h;
};

struct AnimationFrame
{
	int mX;
	int mY;

	AnimationFrame()
	{
		mX = 0;
		mY = 0;
	}
	AnimationFrame(int x, int y)
	{
		mX = x;
		mY = y;
	}
};

/**
*	A struct to store the data of each animation
*
* @param fileReference The reference of the file this animations frames are stored
* @param animationReference The unique name to refer to the animation as
* @param animationType The AnimationType to determine how frames are played
* @param frameCount The number of frames in this animation
* @param frameDelay The delay between animation frames
* @param w The width of a single animation frame
* @param h The height of a single animation frame
* @param frames A vector of AnimationFrame structs that stores the x, y location of each frame on the image
*/
struct AnimationReference
{
	String fileReference;
	String animationReference;
	int animationType;
	int frameCount;
	float frameDelay;
	int w;
	int h;
	std::vector<AnimationFrame> frames;
};

#endif //ANIMATION_H
