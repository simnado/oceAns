/*
 * TVector.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: twiemann
 */

#include <iostream>
#include <cmath>
#include "Vector.hpp"


namespace jumper
{

template<typename T>
const float Vector2<T>::m_epsilon = 0.001;

template<typename T>
Vector2<T>::Vector2(T x, T y)
        :	m_x(x), m_y(y)
{
}

template<typename T>
Vector2<T>::Vector2(const Vector2<T> & other)
{
    m_x = other.m_x;
    m_y = other.m_y;
}

template<typename T>
Vector2<T>::~Vector2()
{
    // TODO Auto-generated destructor stub
}

template<typename T>
T Vector2<T>::distanceTo(const Vector2<T>& o)
{
	T diff_x = this->m_x - o.m_x;
	T diff_y = this->m_y - o.m_y;

	return sqrt(diff_x * diff_x + diff_y * diff_y);
}

template<typename T>
bool Vector2<T>::operator==(const Vector2<T>& other)
{
	return (std::abs(other.m_x - m_x) < m_epsilon) && (std::abs(other.m_y - m_y) < m_epsilon);
}

template<typename T>
bool Vector2<T>::operator!=(const Vector2<T>& other)
{
	return !(*this == other);
}

template<typename T>
Vector2<T>& Vector2<T>::operator =(const Vector2<T> & other)
{
    if(this != &other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
    }
    return *this;
}

template<typename T>
Vector2<T> Vector2<T>::operator +(const Vector2<T> & other) const
{
    Vector2<T> p(*this);
    p += other;
    return p;
}

template<typename T>
void Vector2<T>::operator +=(const Vector2<T> & other)
{
    m_x += other.m_x;
    m_y += other.m_y;
}

template<typename T>
Vector2<T> Vector2<T>::operator *(const T& scalar) const
{
    Vector2<T> p(this->m_x * scalar, this->m_y * scalar);
    return p;
}

template<typename T>
void Vector2<T>::operator *=(const T& scalar)
{
    m_x *= scalar;
    m_y *= scalar;
}

template<typename T>
Vector2<T> Vector2<T>::operator *(const Vector2<T> & vec) const
{
    Vector2<T> p(this->m_x * vec.x(), this->m_y * vec.y());
    return p;
}

template<typename T>
void Vector2<T>::operator *=(const Vector2<T> & vec)
{
    m_x *= vec.x();
    m_y *= vec.y();
}

template<typename T>
T Vector2<T>::x() const
{
    return m_x;
}

template<typename T>
void Vector2<T>::setX(T x)
{
    m_x = x;
}

template<typename T>
T Vector2<T>::y() const
{
    return m_y;
}

template<typename T>
void Vector2<T>::setY(T y)
{
    m_y = y;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const jumper::Vector2<T> & vec)
{
    stream << "(" <<vec.x() << "|" << vec.y() << ")";
    return stream;
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T> &other) const
{
    Vector2<T> p(*this);
    p -= other;
    return p;
}

template<typename T>
void Vector2<T>::operator-=(const Vector2<T> &other)
{
    m_x -= other.m_x;
    m_y -= other.m_y;
}

} /* namespace jumper */
