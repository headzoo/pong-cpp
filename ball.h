#pragma once

#include "vec2.hpp"
#include <SDL2/SDL.h>

using namespace std;

const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15;
const float BALL_SPEED = 0.25f;

enum class CollisionType { None, Top, Middle, Bottom, Left, Right };

struct Contact {
  CollisionType type;
  float penetration;
};

class Ball {
public:
  Ball(int _windowWidth, int _windowHeight, Vec2 _position, Vec2 _velocity)
      : windowWidth(_windowWidth), windowHeight(_windowHeight),
        position(_position), velocity(_velocity) {
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = BALL_WIDTH;
    rect.h = BALL_HEIGHT;
  }

  void Update(float dt) { position += velocity * dt; }

  void Draw(SDL_Renderer *renderer) {
    {
      rect.x = static_cast<int>(position.x);
      rect.y = static_cast<int>(position.y);

      SDL_RenderFillRect(renderer, &rect);
    }
  }

  void CollideWithPaddle(Contact const &contact) {
    position.x += contact.penetration;
    velocity.x = -velocity.x;

    if (contact.type == CollisionType::Top) {
      velocity.y = -.75f * BALL_SPEED;
    } else if (contact.type == CollisionType::Bottom) {
      velocity.y = 0.75f * BALL_SPEED;
    }
  }

  void CollideWithWall(Contact const &contact) {
    if ((contact.type == CollisionType::Top) ||
        (contact.type == CollisionType::Bottom)) {
      position.y += contact.penetration;
      velocity.y = -velocity.y;
    } else if (contact.type == CollisionType::Left) {
      position.x = windowWidth / 2.0f;
      position.y = windowHeight / 2.0f;
      velocity.x = BALL_SPEED;
      velocity.y = 0.75f * BALL_SPEED;
    } else if (contact.type == CollisionType::Right) {
      position.x = windowWidth / 2.0f;
      position.y = windowHeight / 2.0f;
      velocity.x = -BALL_SPEED;
      velocity.y = 0.75f * BALL_SPEED;
    }
  }

  int windowWidth;
  int windowHeight;
  Vec2 position;
  Vec2 velocity;
  SDL_Rect rect{};
};