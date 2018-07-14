/*
 * TexturedLayer.hpp
 *
 *  Created on: Jan 2, 2016
 *      Author: twiemann
 *      Author: ledelkott
 *      Author: tstahn
 */

#ifndef SRC_TEXTUREDLAYER_HPP_
#define SRC_TEXTUREDLAYER_HPP_

#include "StaticRenderable.hpp"

namespace jumper
{

/**
 * @brief A class to render a background bitmap
 */
class TexturedLayer: public StaticRenderable
{
public:

    /**
     * Constructs a layer for the given renderer
     *
     * @param renderer      The SDL-Renderer to render the layer
     */
    TexturedLayer(SDL_Renderer* renderer);

    /**
     * Constructs a layer for the given renderer
     *
     * @param renderer      The SDL-Renderer to render the layer
     * @param texture
     */
    TexturedLayer(SDL_Renderer* renderer, SDL_Texture* texture);

    /**
     * Renders the texture to both given layer
     */
    virtual void render();

    /**
     * Desturctor.
     */
    virtual ~TexturedLayer();

    /**
     * Sets the scroll speed for the layer
     *
     * @param speed
     */
    void setScrollSpeed(float speed) { m_scrollSpeed = speed; };

    /**
     * Sets the scroll speed for the upperlayer
     *
     * @param speed
     */
    void setUpperScrollSpeed(float speed) { m_upperScrollSpeed = speed; };

    /**
     * Sets texture for the layer
     *
     * @param texture
     */
    void setTexture(SDL_Texture* texture);

    /**
     * Sets upper texture for the layer
     *
     * @param texture
     */
    void setUpperTexture(SDL_Texture* texture);

    /**
     * Returns the hight of the upperlayer, 0 if no upperlayer exists
     *
     * @return hight
     */
    int getUpperTextureHeight() { return (m_upperTexture) ? m_upperSourceRect.h : 0; };

private:

    /**
     * Renders the uppertexture to the given layer if exists,
     * the upperlayer is rendered once on top of the layer
     */
    void renderUpperLayer();

    /**
     * Renders the texture to the given layer
     * the texture is rendered ongoing till the end of the layer
     */
    void renderLayer();

    /// Upper Texture
    SDL_Texture* m_upperTexture;

    /// Source rect in the texture
    SDL_Rect m_upperSourceRect;

    /// Curent scroll speed
    float   m_scrollSpeed;

    /// Curent scroll speed for upper texture
    float   m_upperScrollSpeed;

};

} /* namespace jumper */

#endif /* SRC_TEXTUREDLAYER_HPP_ */
