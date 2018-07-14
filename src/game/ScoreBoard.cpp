/*
 * ScoreBoard.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: twiemann
 */

#include "ScoreBoard.hpp"

#include <iostream>
#include <list>
using std::cout;
using std::endl;
using std::list;

namespace jumper
{

const int ScoreBoard::m_maxScore = 1000000;


ScoreBoard::ScoreBoard(SDL_Renderer* renderer, SDL_Texture* texture, int digitWidth, int digitHeight)
    : StaticRenderable(renderer, texture)
{
    m_score         = 0;
    m_digitWidth    = digitWidth;
    m_digitHeight   = digitHeight;
}

ScoreBoard::ScoreBoard(SDL_Renderer* renderer)
    : StaticRenderable(renderer)
{
    m_score = 0;
    m_digitWidth    = 0;
    m_digitHeight   = 0;
}

void ScoreBoard::setScore(int score)
{

    if(score < m_maxScore)
    {
        m_score = score;
    }
    else
    {
        m_score = m_maxScore;
    }
    if (score < 0)
    {
        m_score = 0;
    }
}

int ScoreBoard::getScore() const
{
    return m_score;
}

void ScoreBoard::render()
{
    SDL_Rect target;
    SDL_Rect source;

    target.w = m_digitWidth;
    target.h = m_digitHeight;
    source.w = target.w;
    source.h = target.h;

    std::list<int> digits;
    int number = m_score;

    /// update scoreboard position
    if (10 <= number && number <= 99)setPosition(Vector2i(758,10));
    if (100 <= number && number <= 999)setPosition(Vector2i(738,10));
    if (1000 <= number && number <= 9999)setPosition(Vector2i(718,10));
    if (10000 <= number && number <= 99999)setPosition(Vector2i(698,10));
    if (100000 <= number && number <= 999999)setPosition(Vector2i(678,10));
    if (1000000 <= number && number <= 9999999)setPosition(Vector2i(658,10));


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

void ScoreBoard::setPosition(const Vector2i& position)
{
    m_position = position;
}

ScoreBoard::~ScoreBoard()
{
    // TODO Auto-generated destructor stub
}

} /* namespace jumper */

