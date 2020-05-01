#pragma once
#include "SDL.h"
#include "../../lib/entt/entt.hpp"
#include "../components/lifetime.h"

void lifetimeSystem(float dt, entt::registry* registry) {
  auto view = registry->view<lifetime>();

  for (auto entity : view) {
    auto &l = view.get<lifetime>(entity);

    l.timer += dt;

    if (l.timer > l.time) {
      registry->destroy(entity);
    }
  }
}