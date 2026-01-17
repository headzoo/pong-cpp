#include "../include/ball.hpp"
#include "../include/buttons.hpp"
#include "../include/collision.hpp"
#include "../include/constants.hpp"
#include "../include/paddle.hpp"
#include "../include/score.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <iostream>

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  InitializeCollisionSounds();
  SDL_Window *window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH,
                                        WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  // Create the player score text fields
  PlayerScore playerOneScoreText(
      Vec2D(static_cast<float>(WINDOW_WIDTH / 4.0f), 20), renderer);
  PlayerScore playerTwoScoreText(
      Vec2D(static_cast<float>(3 * WINDOW_WIDTH / 4.0f), 20), renderer);

  // Create the ball
  Ball ball(Vec2D((WINDOW_WIDTH / 2.0f) - (BALL_WIDTH / 2.0f),
                  (WINDOW_HEIGHT / 2.0f) - (BALL_WIDTH / 2.0f)),
            Vec2D(BALL_SPEED, 0.0f));

  // Create the paddles
  Paddle paddleOne(
      Vec2D(50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f)),
      Vec2D(0.0f, 0.0f));
  Paddle paddleTwo(Vec2D(WINDOW_WIDTH - 50.0f,
                         (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f)),
                   Vec2D(0.0f, 0.0f));

  {
    std::array<bool, 4> buttons = {};
    int playerOneScore = 0;
    int playerTwoScore = 0;
    float deltaTime = 0.0f;
    bool running = true;

    while (running) {
      auto startTime = std::chrono::high_resolution_clock::now();

      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN &&
                                       event.key.keysym.sym == SDLK_ESCAPE)) {
          running = false;
        }

        CheckButtons(event, buttons);
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

      HandleCollision(ball, paddleOne, paddleTwo, playerOneScoreText,
                      playerTwoScoreText, playerOneScore, playerTwoScore);

      paddleOne.Update(deltaTime);
      paddleTwo.Update(deltaTime);
      ball.Update(deltaTime);

      // Clear the screen
      SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
      SDL_RenderClear(renderer);

      // Draw the middle line
      SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
      for (int y = 0; y < WINDOW_HEIGHT; ++y) {
        if (y % 5) {
          SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
        }
      }

      // Draw the objects
      ball.Draw(renderer);
      paddleOne.Draw(renderer);
      paddleTwo.Draw(renderer);
      playerOneScoreText.Draw();
      playerTwoScoreText.Draw();
      SDL_RenderPresent(renderer);

      auto stopTime = std::chrono::high_resolution_clock::now();
      deltaTime =
          std::chrono::duration<float, std::chrono::milliseconds::period>(
              stopTime - startTime)
              .count();
    }
  }

  FreeScoreFont();
  FreeCollisionSounds();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}