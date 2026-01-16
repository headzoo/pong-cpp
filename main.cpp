#include "ball.h"
#include "paddle.hpp"
#include "score.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <iostream>

enum Buttons {
  PaddleOneUp = 0,
  PaddleOneDown,
  PaddleTwoUp,
  PaddleTwoDown,
};

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

Contact CheckPaddleCollision(Ball const &ball, Paddle const &paddle) {
  float ballLeft = ball.position.x;
  float ballRight = ball.position.x + BALL_WIDTH;
  float ballTop = ball.position.y;
  float ballBottom = ball.position.y + BALL_HEIGHT;

  float paddleLeft = paddle.position.x;
  float paddleRight = paddle.position.x + PADDLE_WIDTH;
  float paddleTop = paddle.position.y;
  float paddleBottom = paddle.position.y + PADDLE_HEIGHT;

  Contact contact{};

  if (ballLeft >= paddleRight) {
    return contact;
  }

  if (ballRight <= paddleLeft) {
    return contact;
  }

  if (ballTop >= paddleBottom) {
    return contact;
  }

  if (ballBottom <= paddleTop) {
    return contact;
  }

  float paddleRangeUpper = paddleBottom - (2.0f * PADDLE_HEIGHT / 3.0f);
  float paddleRangeMiddle = paddleBottom - (PADDLE_HEIGHT / 3.0f);

  if (ball.velocity.x < 0) {
    // Left paddle
    contact.penetration = paddleRight - ballLeft;
  } else if (ball.velocity.x > 0) {
    // Right paddle
    contact.penetration = paddleLeft - ballRight;
  }

  if ((ballBottom > paddleTop) && (ballBottom < paddleRangeUpper)) {
    contact.type = CollisionType::Top;
  } else if ((ballBottom > paddleRangeUpper) &&
             (ballBottom < paddleRangeMiddle)) {
    contact.type = CollisionType::Middle;
  } else {
    contact.type = CollisionType::Bottom;
  }

  return contact;
}

Contact CheckWallCollision(Ball const &ball) {
  float ballLeft = ball.position.x;
  float ballRight = ball.position.x + BALL_WIDTH;
  float ballTop = ball.position.y;
  float ballBottom = ball.position.y + BALL_HEIGHT;

  Contact contact{};

  if (ballLeft < 0.0f) {
    contact.type = CollisionType::Left;
  } else if (ballRight > WINDOW_WIDTH) {
    contact.type = CollisionType::Right;
  } else if (ballTop < 0.0f) {
    contact.type = CollisionType::Top;
    contact.penetration = -ballTop;
  } else if (ballBottom > WINDOW_HEIGHT) {
    contact.type = CollisionType::Bottom;
    contact.penetration = WINDOW_HEIGHT - ballBottom;
  }

  return contact;
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  TTF_Init();
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  Mix_Chunk *wallHitSound = Mix_LoadWAV("WallHit.wav");
  Mix_Chunk *paddleHitSound = Mix_LoadWAV("PaddleHit.wav");

  SDL_Window *window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH,
                                        WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  TTF_Font *scoreFont = TTF_OpenFont("DejaVuSansMono.ttf", 40);

  // Create the player score text fields
  PlayerScore playerOneScoreText(
      Vec2(static_cast<float>(WINDOW_WIDTH / 4.0f), 20), renderer, scoreFont);

  PlayerScore playerTwoScoreText(
      Vec2(static_cast<float>(3 * WINDOW_WIDTH / 4.0f), 20), renderer,
      scoreFont);

  // Create the ball
  Ball ball(WINDOW_WIDTH, WINDOW_HEIGHT,
            Vec2((WINDOW_WIDTH / 2.0f) - (BALL_WIDTH / 2.0f),
                 (WINDOW_HEIGHT / 2.0f) - (BALL_WIDTH / 2.0f)),
            Vec2(BALL_SPEED, 0.0f));

  // Create the paddles
  Paddle paddleOne(WINDOW_HEIGHT, WINDOW_WIDTH,
                   Vec2(50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f)),
                   Vec2(0.0f, 0.0f));
  Paddle paddleTwo(WINDOW_HEIGHT, WINDOW_WIDTH,
                   Vec2(WINDOW_WIDTH - 50.0f,
                        (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f)),
                   Vec2(0.0f, 0.0f));

  {
    int playerOneScore = 0;
    int playerTwoScore = 0;
    bool running = true;
    bool buttons[4] = {};
    float dt = 0.0f;

    while (running) {
      auto startTime = std::chrono::high_resolution_clock::now();

      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          running = false;
        } else if (event.type == SDL_KEYDOWN) {
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            running = false;
          } else if (event.key.keysym.sym == SDLK_w) {
            buttons[Buttons::PaddleOneUp] = true;
          } else if (event.key.keysym.sym == SDLK_s) {
            buttons[Buttons::PaddleOneDown] = true;
          } else if (event.key.keysym.sym == SDLK_UP) {
            buttons[Buttons::PaddleTwoUp] = true;
          } else if (event.key.keysym.sym == SDLK_DOWN) {
            buttons[Buttons::PaddleTwoDown] = true;
          }
        } else if (event.type == SDL_KEYUP) {
          if (event.key.keysym.sym == SDLK_w) {
            buttons[Buttons::PaddleOneUp] = false;
          } else if (event.key.keysym.sym == SDLK_s) {
            buttons[Buttons::PaddleOneDown] = false;
          } else if (event.key.keysym.sym == SDLK_UP) {
            buttons[Buttons::PaddleTwoUp] = false;
          } else if (event.key.keysym.sym == SDLK_DOWN) {
            buttons[Buttons::PaddleTwoDown] = false;
          }
        }

        if (buttons[Buttons::PaddleOneUp]) {
          paddleOne.velocity.y = -PADDLE_SPEED;
        } else if (buttons[Buttons::PaddleOneDown]) {
          paddleOne.velocity.y = PADDLE_SPEED;
        } else {
          paddleOne.velocity.y = 0.0f;
        }

        if (buttons[Buttons::PaddleTwoUp]) {
          paddleTwo.velocity.y = -PADDLE_SPEED;
        } else if (buttons[Buttons::PaddleTwoDown]) {
          paddleTwo.velocity.y = PADDLE_SPEED;
        } else {
          paddleTwo.velocity.y = 0.0f;
        }
      }

      paddleOne.Update(dt);
      paddleTwo.Update(dt);
      ball.Update(dt);

      if (Contact contact = CheckPaddleCollision(ball, paddleOne);
          contact.type != CollisionType::None) {
        ball.CollideWithPaddle(contact);
        Mix_PlayChannel(-1, paddleHitSound, 0);
      } else if (contact = CheckPaddleCollision(ball, paddleTwo);
                 contact.type != CollisionType::None) {
        ball.CollideWithPaddle(contact);
        Mix_PlayChannel(-1, paddleHitSound, 0);
      } else if (contact = CheckWallCollision(ball);
                 contact.type != CollisionType::None) {
        ball.CollideWithWall(contact);

        if (contact.type == CollisionType::Left) {
          ++playerTwoScore;

          playerTwoScoreText.SetScore(playerTwoScore);
        } else if (contact.type == CollisionType::Right) {
          ++playerOneScore;

          playerOneScoreText.SetScore(playerOneScore);
        } else {
          Mix_PlayChannel(-1, wallHitSound, 0);
        }
      }

      SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
      SDL_RenderClear(renderer);
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        if (y % 5) {
          SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
        }
      }

      ball.Draw(renderer);
      paddleOne.Draw(renderer);
      paddleTwo.Draw(renderer);
      playerOneScoreText.Draw();
      playerTwoScoreText.Draw();

      SDL_RenderPresent(renderer);

      auto stopTime = std::chrono::high_resolution_clock::now();
      dt = std::chrono::duration<float, std::chrono::milliseconds::period>(
               stopTime - startTime)
               .count();
    }
  }

  Mix_FreeChunk(wallHitSound);
  Mix_FreeChunk(paddleHitSound);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(scoreFont);
  Mix_Quit();
  TTF_Quit();
  SDL_Quit();

  return 0;
}