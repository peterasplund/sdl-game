#include "abstractGameobject.h"

void AbstractGameObject::init() {
  _collidable = collidable(_position, _renderable.textureRect);
}

void AbstractGameObject::update(double dt) {
  if (_gravity.entityGravity != 0.0f) {
    _gravity.update(&_position, &_velocity, dt);
  }

  _collidable.update(_position);
}

void AbstractGameObject::draw(Renderer *renderer) {
  // @TODO: add vectors instead
  _renderable.render(renderer, this->_position);
}

bool AbstractGameObject::contains(RectF other) {
  return _collidable.rect.hasIntersection(&other);
}

bool AbstractGameObject::contains(AbstractGameObject other) {
  return false;
  // return _collidable.rect.hasIntersection(&other._collidable.rect);
}

Rect AbstractGameObject::getRect() {
  return {
      (int)_position.x,
      (int)_position.y,
      (int)_collidable.rect.w,
      (int)_collidable.rect.h,
  };
}

RectF AbstractGameObject::getRectFloat() {
  return {
      _position.x,
      _position.y,
      _collidable.rect.w,
      _collidable.rect.h,
  };
}

RectF *AbstractGameObject::getRectPointer() { return &this->_collidable.rect; }

v2f AbstractGameObject::getPosition() { return this->_position; }

v2f *AbstractGameObject::getPositionPointer() { return &this->_position; }

GAME_OBJECT AbstractGameObject::getType() { return _type; }

OBJECT_TAG AbstractGameObject::getTag() { return _tag; }

void AbstractGameObject::setPosition(v2f p) { 
  this->_position = p;
}
