#pragma once

#include "../include/paddle.hpp"
#include "../include/score.hpp"

class Ball;

/**
 * @brief Initializes the collision sounds.
 */
void InitializeCollisionSounds();

/**
 * @brief Frees the collision sounds.
 */
void FreeCollisionSounds();

/**
 * @brief The type of collision.
 */
enum class CollisionType { None, Top, Middle, Bottom, Left, Right };

/**
 * @brief The contact information of the collision.
 */
struct Contact {
  /**
   * @brief The type of collision.
   */
  CollisionType type;

  /**
   * @brief The amount of penetration of the collision.
   */
  float penetration;
};

/**
 * @brief Checks if the ball has collided with the paddle.
 *
 * @param ball The ball to check for collision.
 * @param paddle The paddle to check for collision.
 * @return The contact information of the collision.
 */
Contact CheckPaddleCollision(Ball const &ball, Paddle const &paddle);

/**
 * @brief Checks if the ball has collided with the wall.
 *
 * @param ball The ball to check for collision.
 * @return The contact information of the collision.
 */
Contact CheckWallCollision(Ball const &ball);

void HandleCollision(Ball &ball, Paddle &paddleOne, Paddle &paddleTwo,
                     PlayerScore &playerOneScoreText,
                     PlayerScore &playerTwoScoreText, int &playerTwoScore,
                     int &playerOneScore);