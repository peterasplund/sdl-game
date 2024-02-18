#pragma once

#include "components/renderable.h"
#include "components/velocity.h"
#include "components/gravity.h"
#include "components/collidable.h"
#include "globals.h"
#include "math.h"

// class collidable;

class AbstractGameObject {
  public:
    collidable _collidable;
    v2 _position;
    velocity _velocity;
    gravity _gravity;

    virtual void init(SDL_Renderer* renderer);
    virtual void update(float dt);
    virtual void draw(SDL_Renderer* renderer, v2 offset);
    virtual ~AbstractGameObject() {
    }
    virtual void handleEvent(SDL_Event* event) {};
    void setListenForCollisions();
    virtual bool contains(Rect other);
    virtual bool contains(AbstractGameObject other);
    virtual void onCollision(AbstractGameObject* other) {}; // @TODO: connect this
    Rect getRect();
    Rect* getRectPointer();
    v2 getPosition();
    v2* getPositionPointer();
    bool getListensForCollisions();
    GAME_OBJECT getType();
    OBJECT_TAG getTag();
  protected:
    GAME_OBJECT _type;
    OBJECT_TAG _tag;
    /// Will be able to listen to other collisions.
    bool _listenForCollisions = false;
    Renderable _renderable;
};
