#pragma once

#include "./fonts.hpp"
#include "./vector.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

using namespace std;

/**
 * @brief The score of a player.
 */
class PlayerScore {
public:
  /**
   * @brief Constructs a new player score.
   *
   * @param position The position of the player score.
   * @param renderer The renderer to draw the player score.
   */
  PlayerScore(Vec2D position, SDL_Renderer *renderer)
      : renderer(renderer), font(InitializeScoreFont()) {
    surface = TTF_RenderText_Solid(font, "0", {0xFF, 0xFF, 0xFF, 0xFF});
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    rect.w = width;
    rect.h = height;
  }

  /**
   * @brief Destroys the player score.
   */
  ~PlayerScore() {
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
  }

  /**
   * @brief Sets the score of the player.
   *
   * @param score The score to set.
   */
  void SetScore(int score) {
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    surface = TTF_RenderText_Solid(font, std::to_string(score).c_str(),
                                   {0xFF, 0xFF, 0xFF, 0xFF});
    texture = SDL_CreateTextureFromSurface(renderer, surface);

    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    rect.w = width;
    rect.h = height;
  }

  /**
   * @brief Draws the player score.
   */
  void Draw() { SDL_RenderCopy(renderer, texture, nullptr, &rect); }

private:
  SDL_Renderer *renderer;
  TTF_Font *font;
  SDL_Surface *surface{};
  SDL_Texture *texture{};
  SDL_Rect rect{};
};
