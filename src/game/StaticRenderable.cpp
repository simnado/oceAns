/*
 * StaticRenderable.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: twiemann
 */

#include "StaticRenderable.hpp"

namespace jumper
{

StaticRenderable::StaticRenderable(SDL_Renderer* renderer) : Renderable(renderer) {}

StaticRenderable::StaticRenderable(SDL_Renderer* renderer, SDL_Texture* texture) : Renderable(renderer, texture) {}

StaticRenderable::~StaticRenderable() {}

} /* namespace jumper */
