
#pragma once

#include "../abstractGameobject.h"
#include "../animator.h"
#include "../assetManager.h"
#include "../renderer.h"

namespace obj {
  class Bat : public AbstractGameObject {
    public:
      void init() override {
        AbstractGameObject::init();
        _type = GAME_OBJECT::ENEMY;

        this->_collidable.boundingBox = { 
          0, 0, 
          16,
          16,
        };

        int tw = 16;
        int th = 16;

        SDL_Texture* texture = AssetManager::Instance()->getTexture("assets/sprites/bat.png");

        Animation* animIdle = new Animation(texture);
        // 13 frames
        animIdle->addFrame({ tw * 0, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 1, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 2, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 3, th * 0, tw, th }, 100);
        animIdle->addFrame({ tw * 4, th * 0, tw, th }, 100);

        this->_animator = Animator();
        _animator.addAnimation("idle", animIdle);
        _animator.setAnimation("idle");

        this->_renderable.texture = texture;
        _position.x = 255;
        _position.y = 198;
      }

      void update(float dt) override {
        _position.x -= 0.3f;
        _position.y = AMPLITUDE * cos(SPEED / 2 * elapsedTime) + 50.0f;
        AbstractGameObject::update(dt);
        elapsedTime += dt;
        
        if (_timer.elapsed() > 350) {
          hurt = false;
        }
      }

      void draw(Renderer* renderer) override {
        _renderable.textureRect = _animator.getFrame();
        _renderable.texture = _animator.getTexture();
        _renderable.textureFlip = SDL_FLIP_HORIZONTAL;
        
        if (hurt) {
          hurtTimer += 0.1;
        }

        if (!hurt || (int)hurtTimer % 2) {
          AbstractGameObject::draw(renderer);
        }
      }

    virtual void damage(int dmg) override {
      if (!hurt) {
        hp -= dmg;
        hurt = true;
        _timer.reset();
      }
    }

    protected:
      float SPEED = 0.005f;
      float AMPLITUDE = 15.0f;
      float elapsedTime = 0.0f;
      Animator _animator;

      Timer _timer;
      int hp = 3000;
      float hurtTimer;
  };
}
