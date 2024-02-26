#pragma once

#include <math.h>
#include "globals.h"
#include "window.h"
#include "inputHandler.h"
#include "sceneManager.h"
#include "scene.h"
#include "scene/gameplay.h"
#include "timer.h"

class Game
{
public: Game() {
    InputHandler::Instance()->addButton(SDLK_w, BUTTON::UP);
    InputHandler::Instance()->addButton(SDLK_s, BUTTON::DOWN);
    InputHandler::Instance()->addButton(SDLK_a, BUTTON::LEFT);
    InputHandler::Instance()->addButton(SDLK_d, BUTTON::RIGHT);
    InputHandler::Instance()->addButton(SDLK_k, BUTTON::JUMP);
    InputHandler::Instance()->addButton(SDLK_j, BUTTON::ATTACK);
    InputHandler::Instance()->addButton(SDLK_p, BUTTON::MENU);

    Window window("Hello world", WINDOW_WIDTH, WINDOW_HEIGHT);
    SDL_Renderer* renderer = window.getRenderer();

    AssetManager::Instance()->init(renderer);

    SceneManager* _sceneManager = new SceneManager(renderer);
    Timer fpsTimer;

    GameplayScene* _gameplayScene = new GameplayScene(renderer, "new_test_map");
    _sceneManager->addScene("gameplay", _gameplayScene);

    _sceneManager->gotoScene("gameplay", Transition::NONE);

    while (!window.isClosed()) {
      LAST = NOW;
      NOW = SDL_GetPerformanceCounter();
      
      deltaTime = std::min<double>((double)((NOW - LAST)*1000 / SDL_GetPerformanceFrequency() ), MAX_DELTA_TIME);

      window.clear();

      SDL_Event event;
      ImguiLayer* imgui = ImguiLayer::Instance();

      while (SDL_PollEvent(&event)) {
        for(const auto &obj : *EntityManager::Instance()->getEntities()) {
          obj->handleEvent(&event);
        }

        if (event.key.keysym.sym == SDLK_LSHIFT && event.key.state == SDL_PRESSED) {
          imgui->toggleVisible();
        }

        window.pollEvent(event);
        InputHandler::Instance()->pollEvent(event);
        imgui->processEvents(&event);
      }

      // Old testing code to try out scene transition
      /*
      if (!_sceneManager->isUpdating() && InputHandler::Instance()->isHeld(BUTTON::MENU)) {
        if (_sceneManager->currentScene() == "gameplay") {
          _sceneManager->gotoScene("gameplay2", Transition::FADE);
        }
        else {
          _sceneManager->gotoScene("gameplay", Transition::FADE);
        }
      }
      */

      _sceneManager->update(deltaTime);
      _sceneManager->draw(renderer);

      imgui->drawBegin();
      imgui->debugEntities(EntityManager::Instance()->getEntities());
      // EntityManager::Instance()->imgui();
      imgui->drawEnd();

      SDL_RenderPresent(renderer);


      //if( (fpsTimer.elapsed() < 1000 / WINDOW_FPS)) {
        //Sleep the remaining frame time
        // comment out for now since we're using VSYNC
        //SDL_Delay( ( 1000 / WINDOW_FPS ) - fpsTimer.elapsed() );
      //}

    }
  }

  ~Game() {

  }

private:
  Uint64 NOW = SDL_GetPerformanceCounter();
  Uint64 LAST = 0;
  double deltaTime = 0;
};
