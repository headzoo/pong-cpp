#pragma once

#include "./collision.hpp"
#include "./constants.hpp"
#include "./vector.hpp"
#include <SDL2/SDL.h>

using namespace std;

/**
 * @brief The ball in the game.
 */
class Ball {
public:
  /**
   * @brief Constructs a new ball.
   *
   * @param _position The position of the ball.
   * @param _velocity The velocity of the ball.
   */
  Ball(Vec2D _position, Vec2D _velocity)
      : position(_position), velocity(_velocity) {
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = BALL_WIDTH;
    rect.h = BALL_HEIGHT;
  }

  /**
   * @brief Updates the ball.
   *
   * @param dt The delta time.
   */
  void Update(float dt) { position += velocity * dt; }

  /**
   * @brief Draws the ball.
   *
   * @param renderer The renderer to draw the ball.
   */
  void Draw(SDL_Renderer *renderer) {
    {
      rect.x = static_cast<int>(position.x);
      rect.y = static_cast<int>(position.y);

      SDL_RenderFillRect(renderer, &rect);
    }
  }

  /**
   * @brief Collides the ball with the paddle.
   *
   * @param contact The contact information of the collision.
   */
  void CollideWithPaddle(Contact const &contact) {
    position.x += contact.penetration;
    velocity.x = -velocity.x;

    if (contact.type == CollisionType::Top) {
      velocity.y = -.75f * BALL_SPEED;
    } else if (contact.type == CollisionType::Bottom) {
      velocity.y = 0.75f * BALL_SPEED;
    }
  }

  /**
   * @brief Collides the ball with the wall.
   *
   * @param contact The contact information of the collision.
   */
  void CollideWithWall(Contact const &contact) {
    if ((contact.type == CollisionType::Top) ||
        (contact.type == CollisionType::Bottom)) {
      position.y += contact.penetration;
      velocity.y = -velocity.y;
    } else if (contact.type == CollisionType::Left) {
      position.x = WINDOW_WIDTH / 2.0f;
      position.y = WINDOW_HEIGHT / 2.0f;
      velocity.x = BALL_SPEED;
      velocity.y = 0.75f * BALL_SPEED;
    } else if (contact.type == CollisionType::Right) {
      position.x = WINDOW_WIDTH / 2.0f;
      position.y = WINDOW_HEIGHT / 2.0f;
      velocity.x = -BALL_SPEED;
      velocity.y = 0.75f * BALL_SPEED;
    }
  }

  Vec2D position;
  Vec2D velocity;
  SDL_Rect rect{};
};