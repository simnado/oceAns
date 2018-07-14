/*
 * TextureFactory.cpp
 *
 *  Created on: Dec 11, 2015
 *      Author: twiemann
 */

#include "TextureFactory.hpp"
#include <SDL2/SDL_image.h>

#include <iostream>
using std::cout;
using std::endl;

namespace jumper
{

TextureFactory& TextureFactory::instance(SDL_Renderer* renderer)
{
    static TextureFactory instance(renderer);
    return instance;
}

SDL_Texture *TextureFactory::loadTexture(std::string texFileName,
                                     unsigned char keyR,
                                     unsigned char keyG,
                                     unsigned char keyB)
{
    // The loaded texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(texFileName.c_str());
    if( loadedSurface == NULL )
    {
        std::cout << "Unable to load image! SDL_image Error: " <<  IMG_GetError() << std::endl;
    }
    else
    {
        // Set keying color
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, keyR, keyG, keyB) );

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface );
        if( newTexture == NULL )
        {
            std::cout <<  "Unable to create texture from! SDL Error: " << texFileName <<  SDL_GetError() << std::endl;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}


SDL_Texture *TextureFactory::loadTexture(std::string texFileName)
{
    // The loaded texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(texFileName.c_str());
    if( loadedSurface == NULL )
    {
        std::cout << "Unable to load image! SDL_image Error: " <<  IMG_GetError() << std::endl;
    }
    else
    {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(m_renderer, loadedSurface );
        if( newTexture == NULL )
        {
            std::cout <<  "Unable to create texture from! SDL Error: " << texFileName <<  SDL_GetError() << std::endl;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

SDL_Texture* TextureFactory::getTexture(const string& filename)
{
    // Check, if texture already exists
    texMapIt it = m_textures.find(filename);
    if(it != m_textures.end())
    {
        return it->second;
    }

    // Try to generate a new texture from given file name
    SDL_Texture* texture = loadTexture(filename);

    // Insert texture in to map and return
    if(texture != 0)
    {
        m_textures[filename] = texture;
        return texture;
    }

    return 0;
}

SDL_Texture* TextureFactory::getTexture(const string& filename, unsigned char r,
        unsigned char g, unsigned char b)
{
    // Check, if texture already exists
    texMapIt it = m_textures.find(filename);
    if(it != m_textures.end())
    {
        return it->second;
    }

    // Try to generate a new texture from given file name
    SDL_Texture* texture = loadTexture(filename, r, g, b);

    // Insert texture in to map and return
    if(texture != 0)
    {
        m_textures[filename] = texture;
        return texture;
    }

    return 0;
}

void TextureFactory::deleteTexture(SDL_Texture* texture)
{
    texMapIt it;
    for(it = m_textures.begin(); it != m_textures.end(); it++)
    {
        if(it->second == texture)
        {
            m_textures.erase(it);
            SDL_DestroyTexture(texture);
        }
    }
}

void TextureFactory::deleteTexture(string filename)
{
    texMapIt it = m_textures.find(filename);
    if(it != m_textures.end())
    {
        // Save pointer to texture
        SDL_Texture* texture = it->second;
        m_textures.erase(it);

        // Free texture resources
        SDL_DestroyTexture(texture);
    }
}

void TextureFactory::deleteAll()
{
    texMapIt it;
    for(it = m_textures.begin(); it != m_textures.end(); it++)
    {
        SDL_DestroyTexture(it->second);
        m_textures.erase(it);
    }
}


} /* namespace jumper */
