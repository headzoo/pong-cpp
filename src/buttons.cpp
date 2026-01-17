#include "../include/buttons.hpp"

void CheckButtons(SDL_Event &event, std::array<bool, 4> &buttons) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_w) {
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
}