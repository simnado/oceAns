/*
 * TVector.hpp
 *
 *  Created on: Dec 4, 2015
 *      Author: twiemann
 */

#ifndef SRC_VECTOR_HPP_
#define SRC_VECTOR_HPP_

#include <ostream>

namespace jumper
{

/***
 * A class to represent a Vector2f
 */
template<typename T>
class Vector2
{
public:

    /**
     * Constructor
     * @param x		x position of the Vector2
     * @param y		y position of the Vector2
     */
    Vector2(T x = 0, T y = 0);

    /***
     * Copy constructor
     *
     * @param other	The Vector2f that is copied
     */
    Vector2(const Vector2<T> & other);

    /***
     * Destructor
     */
    virtual ~Vector2();

    /***
     * Assignment operator
     *
     * @param other	The Vector2 that is copied into this Vector2f
     * @return		The modified instance.
     */
    Vector2<T>& operator=(const Vector2<T> & other);

    /***
     * Adds an offset encoded in the other Vector2f
     *
     * @param other	A Vector2 offset added to the current position
     * @return		A modified Vector2
     */
    Vector2<T> operator+(const Vector2<T> & other) const;

    /***
     * Adds an offset encoded in the other Vector2<T>
     *
     * @param other	A Vector2 offset added to the current position
     */
    void operator+=(const Vector2<T> & other);

    /***
  * Adds an offset encoded in the other Vector2
  *
  * @param other		A Vector2 offset added to the current position
  */
    Vector2<T> operator-(const Vector2<T> & other) const;

    /***
     * Adds an offset encoded in the other Vector2<T>
     *
     * @param other		A Vector2 offset added to the current position
     */
    void operator-=(const Vector2<T> & other);


    /***
    * Scales the Vector2
    *
    * @param other	A T with which the Vector should be multiplied
    * @return		A modified Vector2
    */
    Vector2<T> operator*(const T& other) const;

    /***
     *
     * @param scalar with which to multiply
     */
    void operator*=(const T& other);

    /***
    * Scales the Vector2
    *
    * @param other	A T with which the Vector should be multiplied
    * @return		A modified Vector2
    */
    Vector2<T> operator*(const Vector2<T> & other) const;

    /***
     *
     * @param scalar with which to multiply
     */
    void operator*=(const Vector2<T> & other);

    /// Returns the current x value
    T x() const;

    /***
     * Sets a new x value
     *
     * @param x		A new x value
     */
    void setX(T x);

    /// Returns the current y value
    T y() const;

    /***
     * Sets a new y value
     *
     * @param y 	A new y value
     */
    void setY(T y);

    /// Tests if two vectors are equivalent
    bool operator==(const Vector2<T>& other);

    /// Tests if two vectors are not equivalent
    bool operator!=(const Vector2<T>& other);

    // Returns the euclidean distance to another vector
    T distanceTo(const Vector2<T>& other);

    template<typename S>
    friend std::ostream& operator<< (std::ostream& stream, const jumper::Vector2<S> & vec);

private:

    const static float m_epsilon;

    /// x coordinate of the Vector2
    T m_x;

    /// y coordinate of the Vector2
    T m_y;
};

typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;


} /* namespace jumper */

#include "Vector.tcc"

#endif /* SRC_VECTOR_HPP_ */
