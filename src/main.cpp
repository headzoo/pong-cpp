#include "../include/ball.hpp"
#include "../include/buttons.hpp"
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
  if (TTF_Init() != 0) {
    std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
    return 1;
  }
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
    std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
    return 1;
  }

  Mix_Chunk *wallHitSound = Mix_LoadWAV("sounds/WallHit.wav");
  Mix_Chunk *paddleHitSound = Mix_LoadWAV("sounds/PaddleHit.wav");
  TTF_Font *scoreFont = TTF_OpenFont("fonts/DejaVuSansMono.ttf", 40);
  SDL_Window *window = SDL_CreateWindow("Pong", 0, 0, WINDOW_WIDTH,
                                        WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

  // Create the player score text fields
  PlayerScore playerOneScoreText(
      Vec2D(static_cast<float>(WINDOW_WIDTH / 4.0f), 20), renderer, scoreFont);
  PlayerScore playerTwoScoreText(
      Vec2D(static_cast<float>(3 * WINDOW_WIDTH / 4.0f), 20), renderer,
      scoreFont);

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
    bool running = true;
    float dt = 0.0f;

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