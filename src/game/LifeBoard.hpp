/*
 * LifeBoard.hpp
 *
 *  Created on: Feb 23, 2016
 *      Author: doschulze, vtecker
 */

#ifndef SRC_LifeBoard_HPP_
#define SRC_LifeBoard_HPP_

#include "StaticRenderable.hpp"

namespace jumper
{

/**
 * @brief A class to display life using a digit map for rendering
 */
class LifeBoard : public StaticRenderable
{
public:


	/**
	 * Constructs a LifeBoard for the given renderer
	 *
	 * @param renderer			A valid SDL_Renderer structure
	 * @param texture			A texture that contains the 10 digits (from 0 to 9 in
	 * 							that order)
	 * @param digitWidth		The width of a digit within the texture
	 * @param digitHeight		The height of a digit within the texture
	 */
	LifeBoard(SDL_Renderer* renderer, SDL_Texture* texture, int digitWidth, int digitHeight);

	/**
	 * Constructs an empty LifeBoard for the renderer
	 *
	 * @param renderer			A valid SDL_Renderer structure
	 */
	LifeBoard(SDL_Renderer* renderer);

	/// Sets the displayed score value
	void setScore(int score);

	/// Returns the current score value
	int getScore() const;

	/// Sets the screen position of the LifeBoard. The digits are rendered
	/// to the left.
	void setPosition(const Vector2i& position);

	/// Renders the LifeBoard
	virtual void render();

	/// Destructor
	virtual ~LifeBoard();

private:

	/// Current score
	int 					m_score;

	/// Digit width
	int						m_digitWidth;

	/// Digit height
	int						m_digitHeight;

	/// LifeBoard position
	Vector2i				m_position;

	/// Maxim score that can be displayed
	const static int 		m_maxScore;
};

} /* namespace jumper */

#endif /* SRC_LIFEBOARD_HPP_ */
