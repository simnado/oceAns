/*
 * Camera.cpp
 *
 *  Created on: Nov 13, 2015
 *      Author: twiemann
 */

#include "Camera.hpp"
#include "Actor.hpp"

namespace jumper
{



int Camera::x()
{
    return m_position.x();
}

int Camera::y()
{
    return m_position.y();
}

Camera::Camera()
    : m_width(0), m_height(0)
{
    m_position.setX(0);
    m_position.setY(0);
}

void Camera::move(const Vector2i& p)
{
     m_position = p;
}

Camera::Camera(const Vector2i& Vector2i, int w, int h)
    : m_position(Vector2i), m_width(w), m_height(h)
{
}

Vector2i& Camera::position()
{
    return m_position;
}

Camera::~Camera()
{
    // Nothing to do yet
}

int Camera::w()
{
    return m_width;
}

int Camera::h()
{
    return m_height;
}
    
Collision Camera::resolveCameraCollision(Actor* player)
{
    Collision c;

    Vector2f desiredPosition;
    desiredPosition = player->position();

    /*
     * if player moves out of the left edge of the camera,
     * it is pushed to the right
     */
    if(desiredPosition.x() < this->x())
    {
        desiredPosition.setX(desiredPosition.x() + 1);
        c.setType(LEFT);
    }

    /*
     * if player moves out of the right edge of the camera,
     * it is pushed to the left
     */
    if(desiredPosition.x() + player->w() >= this->x() + this->w())
    {
        desiredPosition.setX(desiredPosition.x() - 1);
        c.setType(RIGHT);
    }

    /*
     * if player moves out of the upper edge of the camera,
     * it is pushed down
     */
    if(desiredPosition.y() < this->y())
    {
        desiredPosition.setY(desiredPosition.y() + 1);
        c.setType(UP);
    }

    /*
     * if player moves out of the lower edge of the camera,
     * it is pushed up
     */
    if(desiredPosition.y() + player->h() > this->y() + this->h() + 2) // +2 ggf entfernen
    {
        desiredPosition.setY(desiredPosition.y() - 1);
        c.setType(DOWN);
    }
    player->setPosition(Vector2f(desiredPosition.x(), desiredPosition.y()));
    
    ///Werte noch anpassen!!
    return c;
}

} /* namespace jumper */
