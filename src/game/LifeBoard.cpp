/*
 * LifeBoard.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: doschulze, vtecker
 */

#include "LifeBoard.hpp"
#include <iostream>
#include <list>
using std::cout;
using std::endl;
using std::list;

namespace jumper
{

const int LifeBoard::m_maxScore = 100;


LifeBoard::LifeBoard(SDL_Renderer* renderer, SDL_Texture* texture, int digitWidth, int digitHeight)
	: StaticRenderable(renderer, texture)
{
	m_score			= 0;
	m_digitWidth 	= digitWidth;
	m_digitHeight 	= digitHeight;
}

LifeBoard::LifeBoard(SDL_Renderer* renderer)
	: StaticRenderable(renderer)
{
	m_score = 0;
	m_digitWidth 	= 0;
	m_digitHeight 	= 0;
}

void LifeBoard::setScore(int score)
{
	if(score < m_maxScore)
	{
		m_score = score;
	}
	else
	{
		m_score = m_maxScore;
	}
}

int LifeBoard::getScore() const
{
	return m_score;
}

void LifeBoard::render()
{
	SDL_Rect target;
	SDL_Rect source;

	target.w = m_digitWidth;
	target.h = m_digitHeight;
	source.w = target.w;
	source.h = target.h;

	std::list<int> digits;
	int number = m_score;

	if (0 == number)
	{
		digits.push_back(0);
	}
	else
	{
		while (number != 0)
		{
			int last = number % 10;
			digits.push_front(last);
			number = (number - last) / 10;
		}
	}

	int c = 0;
	for(std::list<int>::iterator it = digits.begin(); it != digits.end(); it++)
	{
		int digit = *it;
		source.x = digit * m_digitWidth;
		source.y = 0;

		target.x = m_position.x() + (c * m_digitWidth) + c;
		target.y = m_position.y();

		SDL_RenderCopy(m_renderer, m_texture, &source, &target);
		c++;
	}

}

void LifeBoard::setPosition(const Vector2i& position)
{
	m_position = position;
}

LifeBoard::~LifeBoard()
{
	// TODO Auto-generated destructor stub
}

} /* namespace jumper */

