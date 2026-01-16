#pragma once

#include "vec2.hpp"
#include <SDL2/SDL.h>

using namespace std;

const int PADDLE_WIDTH = 10;
const int PADDLE_HEIGHT = 100;
const float PADDLE_SPEED = 0.25f;

class Paddle {
public:
  Paddle(int _windowHeight, int _windowWidth, Vec2 _position, Vec2 _velocity)
      : windowHeight(_windowHeight), windowWidth(_windowWidth),
        position(_position), velocity(_velocity) {
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = PADDLE_WIDTH;
    rect.h = PADDLE_HEIGHT;
  }

  void Update(float dt) {
    position += velocity * dt;

    if (position.y < 0) {
      // Restrict to top of the screen
      position.y = 0;
    } else if (position.y > (windowHeight - PADDLE_HEIGHT)) {
      // Restrict to bottom of the screen
      position.y = windowHeight - PADDLE_HEIGHT;
    }
  }

  void Draw(SDL_Renderer *renderer) {
    rect.y = static_cast<int>(position.y);

    SDL_RenderFillRect(renderer, &rect);
  }

  int windowHeight;
  int windowWidth;
  Vec2 position;
  Vec2 velocity;
  SDL_Rect rect{};
};
