#pragma once
#include <SDL2/SDL.h>
#include "assetManager.h"
#include "object.h"
#include "animation.h"
#include "animator.h"
#include "inputHandler.h"
#include "camera.h"

enum State {
  S_IDLE,
  S_RUN,
  S_JUMP,
  S_ATTACK,
};

class Player : public Object
{
private:
  Animator animator;
  InputHandler* _inputHandler;
  bool onFloor = false;

  SDL_Rect leftRect;
  SDL_Rect rightRect;
  SDL_Rect topRect;
  SDL_Rect bottomRect;

  float _gravity = 0.035f;
  float _jumpPower = 2.4f;
  float _maxFallSpeed = 15.0f;
  float friction = 0.08f;
  float backDashSpeed = 1.5f;
  bool isBackDashing = false;
  std::string direction = "right";
  State _state = State::S_IDLE;

  Timer _timer;
public:

  Player(SDL_Renderer* renderer) : Object() {
    size = { 48.0f, 48.0f };
    hitBox = { (int)14.0f, (int)14.0f, (int)22.0f, (int)34.0f };
    speed = 1.0;
    velocity = { 0, 0 };
    id = "player";

    _timer = Timer();
    _inputHandler = InputHandler::Instance();
    SDL_Texture* texture = AssetManager::Instance(renderer)->getTexture("sprites/LightBandit_Spritesheet.png");

    int tw = 48;
    int th = 48;

    Animation* animIdle = new Animation(texture);
    Animation* animRun = new Animation(texture);
    Animation* animAttack = new Animation(texture, false);
    Animation* animJump = new Animation(texture, false);
    Animation* animFall = new Animation(texture, false);
    Animation* animBackDash = new Animation(texture, false);

    animIdle->addFrame({ tw * 0, th * 0, tw, th }, 500);
    animIdle->addFrame({ tw * 1, th * 0, tw, th }, 500);
    animIdle->addFrame({ tw * 2, th * 0, tw, th }, 500);
    animIdle->addFrame({ tw * 3, th * 0, tw, th }, 500);

    animRun->addFrame({ tw * 0, th * 1, tw, th }, 100);
    animRun->addFrame({ tw * 1, th * 1, tw, th }, 100);
    animRun->addFrame({ tw * 2, th * 1, tw, th }, 100);
    animRun->addFrame({ tw * 3, th * 1, tw, th }, 100);
    animRun->addFrame({ tw * 4, th * 1, tw, th }, 100);

    animAttack->addFrame({ tw * 3, th * 2, tw, th }, 100);
    animAttack->addFrame({ tw * 4, th * 2, tw, th }, 100);
    animAttack->addFrame({ tw * 5, th * 2, tw, th }, 100);
    animAttack->addFrame({ tw * 6, th * 2, tw, th }, 100);
    animAttack->addFrame({ tw * 7, th * 2, tw, th }, 100);

    animJump->addFrame({ tw * 5, th * 4, tw, th }, 100);

    animFall->addFrame({ tw * 5, th * 4, tw, th }, 100);

    animBackDash->addFrame({ tw * 1, th * 4, tw, th }, 100);


    animator.addAnimation("idle", animIdle);
    animator.addAnimation("run", animRun);
    animator.addAnimation("attack", animAttack);
    animator.addAnimation("jump", animJump);
    animator.addAnimation("fall", animFall);
    animator.addAnimation("backDash", animBackDash);

    animator.setAnimation("fall");

    animator.play();
  }

  void update(float dt, std::vector<Object> tiles) {
    leftRect = getHitBox();
    leftRect.w = 5;
    leftRect.h = leftRect.h / 2;
    leftRect.x = leftRect.x;
    leftRect.y = leftRect.y + (leftRect.h / 2);

    rightRect = getHitBox();
    rightRect.w = 5;
    rightRect.h = rightRect.h / 2;
    rightRect.x = rightRect.x + getHitBox().w - 5;
    rightRect.y = rightRect.y + (rightRect.h / 2);

    topRect = getHitBox();
    topRect.w = topRect.w / 2;
    topRect.h = 5;
    topRect.x = topRect.x + (topRect.w / 2);
    topRect.y = topRect.y;

    bottomRect = getHitBox();
    bottomRect.w = bottomRect.w / 2;
    bottomRect.h = 5;
    bottomRect.x = bottomRect.x + (bottomRect.w / 2);
    bottomRect.y = bottomRect.y + getHitBox().h - 6;

    onFloor = false;

    //resolve collisions
    for (int j = 0; j < tiles.size(); j ++) {
      if (!tiles[j].getSolid()) {
        continue;
      }

      // collision
      if (Object::intersectsWith(&tiles[j])) {
        if (tiles[j].intersectsWithRect(&bottomRect) && velocity.y > 0) {
          // handle landing collision
          position.y = tiles[j].getPosition().y - size.y + 2;
          onFloor = true;
          velocity.y = 0.0f;
        } else if (tiles[j].intersectsWithRect(&topRect) && velocity.y < 0) {
          // handle top collision
          position.y = tiles[j].getPosition().y + tiles[j].getSize().y - hitBox.y;
          velocity.y = 0.0f;
        } else if (tiles[j].intersectsWithRect(&leftRect)) {
          // handle left collision
          position.x = tiles[j].getRect().x + tiles[j].getRect().w - hitBox.x;
          velocity.x = 0.0f;
        } else if (tiles[j].intersectsWithRect(&rightRect)) {
          // handle right collision
          position.x = tiles[j].getRect().x - hitBox.w - hitBox.x + 1;
          velocity.x = 0.0f;
        }
      }
    }

    Object::update(dt);

    if (velocity.x > 0.2) {
      velocity.x -= friction;
    } else if (velocity.x < -0.2) {
      velocity.x += friction;
    } else {
      velocity.x = 0;
    }

    if (_state != State::S_ATTACK) {
      if (_inputHandler->isPressed(BUTTON::LEFT)) {
        direction = "left";
        velocity.x = -1.0f;
        textureFlip = SDL_FLIP_NONE;
      }

      if (_inputHandler->isPressed(BUTTON::RIGHT)) {
        direction = "right";
        velocity.x = 1.0f;
        textureFlip = SDL_FLIP_HORIZONTAL;
      }

      if (_inputHandler->isPressed(BUTTON::UP) && onFloor) {
        velocity.y = -_jumpPower;
        onFloor = false;
      }
    }

    if (!onFloor) {
      velocity.y += _gravity;
    }

    if (velocity.y > _maxFallSpeed) {
      velocity.y = _maxFallSpeed;
    }

    if (_inputHandler->isPressed(BUTTON::ATTACK) && onFloor && _state == State::S_IDLE) {
      printf("ATTACK\n");
      _state = State::S_ATTACK;
      animator.setAnimation("attack");
      animator.reset();
      _timer.reset();
    }

    // back dash after attack
    if (_state == State::S_ATTACK && _timer.elapsed() > 300) {
      if (direction == "right" && _inputHandler->isPressed(BUTTON::LEFT)) {
        isBackDashing = true;
        velocity.x = -backDashSpeed;
        animator.setAnimation("backDash");
        animator.reset();
      } else if (direction == "left" && _inputHandler->isPressed(BUTTON::RIGHT)) {
        isBackDashing = true;
        velocity.x = backDashSpeed;
        animator.setAnimation("backDash");
        animator.reset();
      }
    }

    // 500 = 100ms on each frame of attack animation
    if (_state == State::S_ATTACK && _timer.elapsed() > 500) {
      isBackDashing = false;
      _state = State::S_IDLE;
    }

    if (_state != State::S_ATTACK) {
      if (velocity.y > .4f && !onFloor) {
        if (animator.getCurrent() != "fall") {
          animator.reset();
          animator.setAnimation("fall");
        }
      } else if (velocity.y < 0 && !onFloor) {
        if (animator.getCurrent() != "jump") {
          animator.reset();
          animator.setAnimation("jump");
        }
      } else if (velocity.x != 0 && onFloor) {
        if (animator.getCurrent() != "run") {
          animator.setAnimation("run");
        }
      } else if (onFloor) {
        if (animator.getCurrent() != "idle") {
          printf("setting idle\n");
          animator.setAnimation("idle");
        }
      }
    }

  }


  virtual void draw(SDL_Renderer* renderer, SDL_Rect origin) {
    //printf("animation: %s\n", animator.getCurrent().c_str());
    printf("x: %f\n", position.x);
    textureRect = animator.getFrame();
    setTexture(animator.getTexture());

    Object::draw(renderer, origin);

    // debug collision rectangles
    /*
    SDL_SetRenderDrawColor(renderer, 20, 255, 20, 255);
    SDL_RenderFillRect(renderer, &leftRect);
    SDL_SetRenderDrawColor(renderer, 20, 20, 255, 255);
    SDL_RenderFillRect(renderer, &rightRect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 20, 255);
    SDL_RenderFillRect(renderer, &topRect);
    SDL_SetRenderDrawColor(renderer, 255, 20, 20, 255);
    SDL_RenderFillRect(renderer, &bottomRect);
    */
  }
};
