#pragma once
#include "../../lib/entt/entt.hpp"
#include "../components/camera.h"
#include "../components/characterController.h"

entt::entity createCamera(entt::registry* registry) {
  auto entity = registry->create();
  registry->emplace<camera>(entity);

  return entity;
}
