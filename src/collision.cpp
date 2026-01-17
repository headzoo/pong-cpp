#include "../include/collision.hpp"
#include "../include/ball.hpp"
#include "../include/score.hpp"
#include <SDL2/SDL_mixer.h>
#include <stdexcept>

Mix_Chunk *wallHitSound;
Mix_Chunk *paddleHitSound;

void InitializeCollisionSounds() {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
    throw std::runtime_error("Failed to open audio: " +
                             std::string(Mix_GetError()));
  }

  wallHitSound = Mix_LoadWAV("sounds/WallHit.wav");
  paddleHitSound = Mix_LoadWAV("sounds/PaddleHit.wav");
}

void FreeCollisionSounds() {
  Mix_FreeChunk(wallHitSound);
  Mix_FreeChunk(paddleHitSound);
  Mix_CloseAudio();
}

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

void HandleCollision(Ball &ball, Paddle &paddleOne, Paddle &paddleTwo,
                     PlayerScore &playerOneScoreText,
                     PlayerScore &playerTwoScoreText, int &playerTwoScore,
                     int &playerOneScore) {
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
}