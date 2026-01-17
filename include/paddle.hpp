#pragma once

#include "./constants.hpp"
#include "./vector.hpp"
#include <SDL2/SDL.h>

using namespace std;

/**
 * @brief The paddle in the game.
 */
class Paddle {
public:
  /**
   * @brief Constructs a new paddle.
   *
   * @param _position The position of the paddle.
   * @param _velocity The velocity of the paddle.
   */
  Paddle(Vec2D _position, Vec2D _velocity)
      : position(_position), velocity(_velocity) {
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = PADDLE_WIDTH;
    rect.h = PADDLE_HEIGHT;
  }

  /**
   * @brief Updates the paddle.
   *
   * @param dt The delta time.
   */
  void Update(float dt) {
    position += velocity * dt;

    if (position.y < 0) {
      // Restrict to top of the screen
      position.y = 0;
    } else if (position.y > (WINDOW_HEIGHT - PADDLE_HEIGHT)) {
      // Restrict to bottom of the screen
      position.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
    }
  }

  /**
   * @brief Draws the paddle.
   *
   * @param renderer The renderer to draw the paddle.
   */
  void Draw(SDL_Renderer *renderer) {
    rect.y = static_cast<int>(position.y);
    SDL_RenderFillRect(renderer, &rect);
  }

  Vec2D position;
  Vec2D velocity;
  SDL_Rect rect{};
};
