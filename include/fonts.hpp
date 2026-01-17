#pragma once

#include <SDL2/SDL_ttf.h>

/**
 * @brief Initializes the fonts.
 */
TTF_Font *InitializeScoreFont();

/**
 * @brief Frees the fonts.
 */
void FreeScoreFont();