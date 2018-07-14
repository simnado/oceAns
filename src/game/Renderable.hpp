//
// Created by isaak on 25.11.15.
//

#ifndef JUMPER_RENDERABLE_H
#define JUMPER_RENDERABLE_H

#include <SDL2/SDL.h>
#include <string>
#include "Camera.hpp"
#include "Mixer.hpp"

namespace jumper
{


/**
 * @brief Base class for all objects that can be rendered.
 */
class Renderable
{

public:

    /**
     * Constructs a Renderable for the given renderer
     *
     * @param m_renderer            A pointer to a valid SDL_Renderer structure.
     */
    Renderable(SDL_Renderer *m_renderer);

    /***
     * Contructs a renderable from given texture and renderer.
     */
    Renderable(SDL_Renderer* renderer, SDL_Texture* texture);

    /**
     * @brief   Renderer getter
     * @return  SDL_Renderer
     */
    SDL_Renderer *getRenderer() const;

    /// Retruns the texture pointer
    SDL_Texture *getTexture() const;

    void setTexture(SDL_Texture* texture){ m_texture = texture; }

    /**
    * @brief   Renders the object.
    */
    virtual void render() = 0;

    /// Returns the with of the rendered object
    int w() const;

    /// Returns the height of the rendered object
    int h() const;

    /// A global camera object that is used to determine
    /// the offset wrt. the current camera position when
    /// rendering
    static Camera       m_camera;

    ///set mixer for audio

    void setMixer(Mixer* mixer){m_mixer = mixer;}


    /// Destructor
    virtual ~Renderable();

protected:

    /// Renderer
    SDL_Renderer*       m_renderer;

    /// A texture object
    SDL_Texture*        m_texture;

    /// Source rect in the texture
    SDL_Rect            m_sourceRect;

    /// Mixer
    Mixer*              m_mixer;
};

} /* namespace jumper */

#endif //JUMPER_RENDERABLE_H
