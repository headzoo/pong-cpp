#pragma once

#include <SDL2/SDL.h>
#include <array>

enum Buttons {
  PaddleOneUp = 0,
  PaddleOneDown,
  PaddleTwoUp,
  PaddleTwoDown,
};

/**
 * @brief Checks which buttons are being pressed.
 *
 * @param event The event to check.
 * @param buttons The buttons to check.
 */
void CheckButtons(SDL_Event &event, std::array<bool, 4> &buttons);