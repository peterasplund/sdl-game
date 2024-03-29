#pragma once

#include "../abstractGameobject.h"
#include "../animator.h"
#include "../assetManager.h"
#include "../inputHandler.h"
#include "../renderer.h"

namespace obj {
  enum class State {
    IDLE,
    CROUCH,
    RUN,
    JUMP,
    ATTACK,
    HURT,
    SLIDE,
    CLIMBING,
  };

  class Player : public AbstractGameObject {
    public:
      virtual ~Player() { }
      void init() override;
      void update(float dt) override;
      void performJump();
      void jump();
      void attack();
      void slide();
      TileData* tileAt(RectF rect, std::string property);
      void onInputPressed(int button) override;
      void onInputReleased(int button) override;
      void draw(Renderer* renderer) override;

    protected:
      /// How high the initial jump should be
      const float JUMP_POWER = 0.4f;
      /// How much higher the player should go if they hold the jump button (lower value means higher jump).
      float JUMP_SHORT_GRAVITY = 0.006f;
      /// How fast to get to max run speed
      const float RUN_ACCELERATION = 0.015f;
      /// How fast to get to a stop
      const float RUN_DEACCELERATION = 0.02f;
      /// How fast to get to max run speed in air
      const float AIR_ACCELERATION = 0.01f;
      /// How fast to get to a stop in air
      const float AIR_DEACCELERATION = 0.004f;
      /// Max run speed
      const float RUN_SPEED = 0.2f;
      /// How long to slide
      const float SLIDE_POWER = 0.36f;
      /// How fast to get to a stop
      const float SLIDE_DEACCELERATION = 0.004f;
      /// Jump buffer window in MS
      const int JUMP_BUFFER_WINDOW = 100; 
      /// Error margin to land on one-way platform. How many pixels below we can be and still snap on top of it.
      const int ONEWAY_PLATFORM_GRACE = 3;
      
      // How close we need to be to the ladder to climb it.
      const int LADDER_X_DEADZONE = 5;

      /// When couching and pressing jump on top of a one-way platform we let the player go through it.
      /// This is how long we should act like it's not solid for.
      const int ONE_WAY_PLATFORM_FALLTHROUGH_WINDOW = 100;

      GAME_OBJECT _type = GAME_OBJECT::PLAYER;
      Animator _animator;
      std::string direction = "right";
      State state = State::IDLE;
      Timer attackTimer;
      Timer slideTimer;
      Timer hurtTimer;
      Timer jumpBufferTimer;
      Timer onwayPlatformFallThroughTimer;
      float _normalGravity;
      bool _jumpHold = false;
      bool _justJumped = false;
      float backDashSpeed = 1.5f;
      float attackDelay = 280.0f;
      float slideDelay = 400.0f;
      bool isBackDashing = false;
      bool jumpBuffered = false;
      bool previouslyOnFloor = false;
      bool isMoving = false;
      bool onOneWayPlatform = false;
      bool dead = false;
  };
}
