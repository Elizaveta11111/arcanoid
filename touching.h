#pragma once
#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "arcanoid.h"

class Touching {
public:
  void touchedTop(Ball* ball);
  void touchedLeft(Ball* ball);
  void touchedRight(Ball* ball, Arcanoid* arcanoid);
  void touchedButtom(Ball* ball, Arcanoid* arcanoid);
  void touchedPaddle(Ball* ball, Paddle* paddle, Arcanoid* arcanoid);
  void bounce(Ball* ball, int l, int r, Arcanoid* arcanoid);
  void touchedBox(Ball* ball, Arcanoid* arcanoid);
  void touchedBricks(Ball* ball, Arcanoid* arcanoid);
  void touchedStaticBricks(Ball* ball, bool* lrTouch, bool* udTouch, Arcanoid* arcanoid);
  void touchedMovingBricks(Ball* ball, bool* lrTouch, bool* udTouch, Arcanoid* arcanoid);
  bool touchedLR(Ball* ball, Brick* brick, Arcanoid* arcanoid);
  bool touchedUD(Ball* ball, Brick* brick, Arcanoid* arcanoid);
  void encounter(Ball* a, Ball* b, Arcanoid* arcanoid);
  void touchedMovingBrick(MovingBrick* brick, Arcanoid* arcanoid);
  void touchedSimpleBrick(MovingBrick* brick, Arcanoid* arcanoid);
  void touchedLeft(MovingBrick* brick);
  void touchedRight(MovingBrick* brick, Arcanoid* arcanoid);
};
