#pragma once

/**
 * @brief A 2D vector.
 */
class Vec2D {
public:
  /**
   * @brief Constructs a new vector.
   */
  Vec2D() : x(0.0f), y(0.0f) {}

  /**
   * @brief Constructs a new vector.
   *
   * @param x The x component of the vector.
   * @param y The y component of the vector.
   */
  Vec2D(float x, float y) : x(x), y(y) {}

  /**
   * @brief Adds two vectors.
   *
   * @param rhs The right-hand side vector.
   * @return The sum of the two vectors.
   */
  Vec2D operator+(Vec2D const &rhs) { return Vec2D(x + rhs.x, y + rhs.y); }

  /**
   * @brief Adds two vectors and assigns the result to the left-hand side
   * vector.
   *
   * @param rhs The right-hand side vector.
   * @return The left-hand side vector.
   */
  Vec2D operator+=(Vec2D const &rhs) {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  /**
   * @brief Multiplies a vector by a scalar.
   *
   * @param rhs The scalar to multiply the vector by.
   * @return The product of the vector and the scalar.
   */
  Vec2D operator*(float rhs) { return Vec2D(x * rhs, y * rhs); }

  float x, y;
};