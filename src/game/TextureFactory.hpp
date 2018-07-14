/*
 * TextureFactory.hpp
 *
 *  Created on: Dec 11, 2015
 *      Author: twiemann
 */

#ifndef SRC_TEXTUREFACTORY_HPP_
#define SRC_TEXTUREFACTORY_HPP_

#include <SDL2/SDL.h>
#include <string>
#include <map>
using std::string;
using std::map;

namespace jumper
{

/**
 * @brief 	Factory class to generate and manage SDL_Texture* structs
 * 			from filenames. If a filename is given more than once,
 * 			already generated pointers are returned. Be careful not
 * 			to free them in your code. Use deleteTexture(filename) or deleteTexture(SDL_Texture*)
 * 			instead. To free all resources uses deleteAll().
 */
class TextureFactory
{
public:

	/**
	 * Returns the factory's instance.
	 */
	static TextureFactory& instance(SDL_Renderer* renderer);

	/// Get an SDL_Texture pointer to a texture generated from
	/// the given \ref filename.
	SDL_Texture* getTexture(const string& filename);

	/// Get an SDL_Texture pointer to a texture generated from
	/// the given \ref filename with the defined key color.
	SDL_Texture* getTexture(const string& filename, unsigned char r, unsigned char g, unsigned char b);

	/// Deletes a texture pointer
	void deleteTexture(SDL_Texture* texture);

	/// Deletes the texture belonging to the given file
	void deleteTexture(string filename);

	/// Deletes all textures
	void deleteAll();
private:

	/// Loads a new texture from file
	SDL_Texture* loadTexture(string filename);

	/// Loads a new texture from file with key color
	SDL_Texture* loadTexture(string filename, unsigned char r, unsigned char g, unsigned char b);

	/// Constructor
	TextureFactory(SDL_Renderer* renderer) : m_renderer(renderer) {};

	/// Map to assiciate filenames and textures
	map<string, SDL_Texture*> m_textures;

	/// The renderer for which the texture objects are created
	SDL_Renderer* m_renderer;

	typedef map<string, SDL_Texture*>::iterator texMapIt;
public:

	// Ensure that no copy can be generated
	TextureFactory(TextureFactory const&) = delete;

	// Forbid assignment.
	void operator=(TextureFactory const&) = delete;

	// Note: Scott Meyers mentions in his Effective Modern
	//       C++ book, that deleted functions should generally
	//       be public as it results in better error messages
	//       due to the compilers behavior to check accessibility
	//       before deleted status
};

} /* namespace jumper */

#endif /* SRC_TEXTUREFACTORY_HPP_ */
