#pragma once

#include "../globals.h"
#include "../math.h"

struct camera {
  v2i viewport; // viewport size. In this case same as window size. Make this less tall later when implementing the HUD.
  SDL_Rect bounds = { 0, 0, 2000, 352 }; // clamping bounds. Limit camera position within
  v2f pos = { 0, 0 }; // viewport size. In this case same as window size. Make this less tall when implementing the HUD.
  RectF* following; // entity to follow, eg. the player
  float zoom = 1.0f; // camera zoom

  camera() {
    int x = std::round(WINDOW_WIDTH / WINDOW_ZOOM);
    int y = std::round(WINDOW_HEIGHT / WINDOW_ZOOM);
    viewport = { x, y };
  }

  void update() {
    if (following == nullptr) {
      return;
    }

    float px, py, pw, ph;

    pw = following->w;
    ph = following->h;
    px = following->x;
    py = following->y;

    pos.x = round(px + (pw / 2) - (float)viewport.x / 2);
    pos.y = round(py + (ph / 2) - (float)viewport.y / 2);

    // clamp within bounds
    if (pos.x < bounds.x) { pos.x = bounds.x; }
    if (pos.x + viewport.x > bounds.w) { pos.x = bounds.w - viewport.x; }
    if (pos.y < bounds.y) { pos.y = bounds.y; }
    if (pos.y + viewport.y > bounds.h) { pos.y = bounds.h - viewport.y; }
  }

  void setBounds(v2i bounds) {
    this->bounds.w = bounds.x;
    this->bounds.h = bounds.y;
  }

  void follow(RectF* subject) {
    following = subject;
  }

  RectF getRect() {
    return {
      pos.x,
      pos.y,
      (float)viewport.x,
      (float)viewport.y
    };
  }
};
