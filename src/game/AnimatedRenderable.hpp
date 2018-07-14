/*
 * AnimatedRenderable.hpp
 *
 *  Created on: Dec 4, 2015
 *      Author: twiemann
 */

#ifndef SRC_ANIMATEDRENDERABLE_HPP_
#define SRC_ANIMATEDRENDERABLE_HPP_

#include "Renderable.hpp"

#include <string>
#include <iostream>

namespace jumper
{

/**
 * @brief Base class of renderables with several animation frames
 */
class AnimatedRenderable: public Renderable
{
public:

	/***
	 * Constructs an animated renderable from the given \ref filename
	 * for the internal \ref renderer
	 * @param renderer		A pointer to a SDL renderer struct
	 * @param filename		A filename with animation definitions
	 */
	AnimatedRenderable(SDL_Renderer* renderer, std::string filename);

	/***
	 * Constructs an animated renderable from given renderer, texture
	 * and frame information.
	 * @param renderer		A pointer to a SDL renderer struct
	 * @param texture		A pointer to valid SDL_Texture struct
	 * @param frameWidth	The width of the frames within the texture
	 * @param frameHeight	The height of the frames within the texture
	 * @param numFrames		The number of frames in the texture
	 */
	AnimatedRenderable(SDL_Renderer* renderer, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames);


	/**
	 * Destructor.
	 */
	virtual ~AnimatedRenderable();

	/**
	 * Renders the current frame
	 */
	virtual void render() = 0;

	/**
	 * Moves the source rect to the next frame
	 */
	void nextFrame();

	/**
	 * Sets frames per second for animation
	 */
	void setFPS(int frames);

	/// Returns the number of frames in the animation
	int numFrames() const { return m_numFrames;}

	/// Returns frame width
	int frameWidth() const { return m_frameWidth;}

	/// Returns the frame height
	int frameHeight() const { return m_frameHeight;}

    /// Returns frame width
    void setFrameWidth(int newFrameWidth) { m_frameWidth = newFrameWidth;}

    /// Returns the frame height
    void setFrameHeight(int newFrameHeight) { m_frameHeight = newFrameHeight;}

    std::string getFilename(){return m_filename;}

    void setupExplosion();

protected:

	///	Number of frames in the animation
	int				m_numFrames;

	/// Current frame number
	int				m_currentFrame;

	/// The width of a frame
	int 			m_frameWidth;

	/// The height of a frame
	int				m_frameHeight;

	/// Ticks count when the last frame was rendered
	Uint32			m_lastRenderTicks;

	/// Timeout between frames
	Uint32			m_frameTimeout;

    /// name of the file for the image
    std::string     m_filename;

    std::string m_explodeFileSmall ="..\\res\\Data\\Explosionen\\Projektil_ex.spr";
    std::string m_explodeFileMedium ="..\\res\\Data\\Explosionen\\Explosion_rauch.spr";
    std::string m_explodeFileBig ="..\\res\\Data\\Explosionen\\Explosion.spr";
    std::string m_explodeFileBlue ="..\\res\\Data\\Explosionen\\blau.spr";
    std::string m_explodeFileGreen ="..\\res\\Data\\Explosionen\\gruen.spr";
    std::string m_explodeFileRred ="..\\res\\Data\\Explosionen\\rot.spr";
    std::string m_explodeFilPink ="..\\res\\Data\\Explosionen\\pink.spr";
    std::string m_explodeFilOrange ="..\\res\\Data\\Explosionen\\orange.spr";
    std::string m_explodeFile="..\\res\\Data\\Explosionen\\Explosion.spr";


};

} /* namespace jumper */

#endif /* SRC_ANIMATEDRENDERABLE_HPP_ */
