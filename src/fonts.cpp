#include "../include/fonts.hpp"
#include <SDL2/SDL_ttf.h>
#include <stdexcept>

TTF_Font *scoreFont = nullptr;

TTF_Font *InitializeScoreFont() {
  if (scoreFont != nullptr) {
    return scoreFont;
  }

  if (TTF_Init() != 0) {
    throw std::runtime_error("Failed to initialize TTF: " +
                             std::string(TTF_GetError()));
  }

  scoreFont = TTF_OpenFont("fonts/DejaVuSansMono.ttf", 40);
  if (scoreFont == nullptr) {
    throw std::runtime_error("Failed to open score font: " +
                             std::string(TTF_GetError()));
  }

  return scoreFont;
}

void FreeScoreFont() {
  if (scoreFont != nullptr) {
    TTF_CloseFont(scoreFont);
    scoreFont = nullptr;
  }

  if (TTF_WasInit() != 0) {
    TTF_Quit();
  }
}