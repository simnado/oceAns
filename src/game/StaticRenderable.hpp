/*
 * StaticRenderable.hpp
 *
 *  Created on: Dec 4, 2015
 *      Author: twiemann
 */

#ifndef SRC_STATICRENDERABLE_HPP_
#define SRC_STATICRENDERABLE_HPP_

#include "Renderable.hpp"

namespace jumper
{

/**
 * @brief Marker interface for static objects to render
 */
class StaticRenderable : public Renderable
{
public:

	/**
	 * Constructor.
	 * @param renderer		A pointer to a SDL_Renderer struct
	 */
	StaticRenderable(SDL_Renderer* renderer);

	/**
	 * Constructs a static renderable for the given renderer
	 * using the given texture
	 *
	 * @param renderer		A pointer to a valid SDL_Renderer struct
	 * @param texture		A pointer to a valid SDL_Renderer struct
	 */
	StaticRenderable(SDL_Renderer* renderer, SDL_Texture* texture);

	/**
	 * Renders the object to its renderer
	 */
	virtual void render() = 0;

	/**
	 * Destructor
	 */
	virtual ~StaticRenderable();
};

} /* namespace jumper */

#endif /* SRC_STATICRENDERABLE_HPP_ */
