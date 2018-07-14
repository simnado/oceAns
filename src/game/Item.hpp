/*
 * Item.hpp
 *
 *  Created on: Jan 1, 2016
 *      Author: twiemann
 */

#ifndef SRC_ITEM_HPP_
#define SRC_ITEM_HPP_

#include "Actor.hpp"

namespace jumper
{

enum ItemType
{
	STONE,
	HEART,
	LIGHTNING,
	COIN,
	ENDING
};
/**
 * @brief 	An Item is a special actor that stays at it's given
 * 			position and performs an animation. In contrast to
 * 			an AnimatedRenderable it can perform collision tests.
 */
class Item: public jumper::Actor
{
public:

	/// sets Item type
    void setItemType(ItemType t);

    /// returns the item type of current item
	ItemType getItemType() { return m_type;}

	/**
	 * Creates an Item for the given renderer from the given file
	 *
	 * @param renderer
	 * @param filename
	 */
    Item(SDL_Renderer* renderer, std::string filename, Mixer* mixer);

	/**
	 * Creates an Item.
	 *
	 * @param renderer		A SDL-Renderer to render the item
	 * @param texture		A SDL-Texture that contains the bitmaps of the frames
	 * @param frameWidth	Frame width
	 * @param frameHeight	Frame height
	 * @param numFrames		Number of frames
	 */
    Item(SDL_Renderer* renderer, SDL_Texture* texture, int frameWidth, int frameHeight, int numFrames, Mixer* mixer);

	/**
	 * Moves the item and checks collisions with the given level. This default
	 * implementation just calls \ref nextFrame, i.e., the item does not move
	 *
	 * @param level
	 */
	virtual void move(Level& level, float dt);

    virtual std::vector<Actor*> onDeath();

	/// Destructor.
	virtual ~Item();
private:
	
	ItemType			m_type;
};

} // namespace jumper
#endif /* SRC_ITEM_HPP_ */
