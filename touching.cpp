#include "touching.h"


void Touching::touchedBox(Ball* ball, Arcanoid* arcanoid) {
  touchedTop(ball);
  touchedLeft(ball);
  touchedRight(ball, arcanoid);
  touchedButtom(ball, arcanoid);
}

void Touching::touchedTop(Ball* ball) {
  if (ball->top() <= 0) {
    ball->setY(0);
    ball->changeYdir();
  }
}

void Touching::touchedLeft(Ball* ball) {
  if ((ball->left() <= 0)) {
    ball->setX(0);
    ball->changeXdir();
  }
}

void Touching::touchedRight(Ball* ball, Arcanoid* arcanoid) {
  if (ball->right() >= arcanoid->width) {
    ball->setX(arcanoid->width - arcanoid->ballD);
    ball->changeXdir();
  }
}

void Touching::touchedButtom(Ball* ball, Arcanoid* arcanoid) {
  if (ball->top() >= arcanoid->height) {
    int i;
    for (i = 0; i < arcanoid->balls.size(); i++)
      if (arcanoid->balls[i] == ball)
        break;
    delete ball;
    arcanoid->balls.erase(arcanoid->balls.begin() + i);
    if (arcanoid->balls.empty()) {
      arcanoid->lives--;
      if (arcanoid->lives > 0) {
        arcanoid->balls.push_back(new Ball(arcanoid->paddle->left() + (arcanoid->paddle->right() - arcanoid->paddle->left()) / 2 - arcanoid->ballD / 2, arcanoid->paddle->top() - arcanoid->ballD, arcanoid->ballD));
        arcanoid->stick = 1;
        arcanoid->stickball = arcanoid->balls[arcanoid->balls.size() - 1];
      }
    }
  }
}

void Touching::touchedPaddle(Ball* ball, Paddle* paddle, Arcanoid* arcanoid) {
  int r = paddle->right();
  int l = paddle->left();
  if (ball->bottom() >= paddle->top()) {
    if (ball->left() >= l && ball->right() <= r)
      bounce(ball, l, r, arcanoid);
    else if (arcanoid->bonuslives)
      arcanoid->bonusPaddle(ball);
  }
}

void Touching::bounce(Ball* ball, int l, int r, Arcanoid* arcanoid) {
  int leftPaddleEdge = l + (r - l) / 3;
  int rightPaddleEdge = r - (r - l) / 3;
  if (ball->right() <= leftPaddleEdge)
    ball->moveLeft();
  else if (ball->left() >= rightPaddleEdge)
    ball->moveRight();
  else
    ball->changeXdir();
  ball->setY(arcanoid->paddle->top() - arcanoid->ballD);
  ball->changeYdir();
  if (arcanoid->stick && arcanoid->stickball == NULL)
    arcanoid->stickball = ball;
}

void Touching::touchedBricks(Ball* ball, Arcanoid* arcanoid) {
  bool lrTouch = false;
  bool udTouch = false;
  touchedStaticBricks(ball, &lrTouch, &udTouch, arcanoid);
  touchedMovingBricks(ball, &lrTouch, &udTouch, arcanoid);
  if (lrTouch)
    ball->changeXdir();
  else if (udTouch)
    ball->changeYdir();
}

void Touching::touchedStaticBricks(Ball* ball, bool* lrTouch, bool* udTouch, Arcanoid* arcanoid) {
  int i, j;
  for (i = 0; i < columns; i++)
    for (j = 0; j < rows; j++) {
      if (arcanoid->briks[i][j] != NULL) {
        if (touchedLR(ball, arcanoid->briks[i][j], arcanoid)) {
          arcanoid->dropBonus(i, j, arcanoid->briks[i][j]->touchedBall(ball, arcanoid), ball);
          *lrTouch = true;
        }
        else if (touchedUD(ball, arcanoid->briks[i][j], arcanoid)) {
          arcanoid->dropBonus(i, j, arcanoid->briks[i][j]->touchedBall(ball, arcanoid), ball);
          *udTouch = true;
        }
        if (!arcanoid->briks[i][j]->isAlive()) {
          arcanoid->briksleft--;
          delete arcanoid->briks[i][j];
          arcanoid->briks[i][j] = NULL;
        }
      }
    }
}

void Touching::touchedMovingBricks(Ball* ball, bool* lrTouch, bool* udTouch, Arcanoid* arcanoid) {
  int i, j;
  if (!arcanoid->movingBricks.empty())
    for (i = 0; i < arcanoid->movingBricks.size(); i++) {
      if (touchedLR(ball, arcanoid->movingBricks[i], arcanoid)) {
        arcanoid->movingBricks[i]->touchedBall(ball, arcanoid);
        *lrTouch = true;
      }
      else if (touchedUD(ball, arcanoid->movingBricks[i], arcanoid)) {
        arcanoid->movingBricks[i]->touchedBall(ball, arcanoid);
        *udTouch = true;
      }
      if (!arcanoid->movingBricks[i]->isAlive()) {
        delete arcanoid->movingBricks[i];
        arcanoid->movingBricks.erase(arcanoid->movingBricks.begin() + i--);
      }
    }
}

bool Touching::touchedLR(Ball* ball, Brick* brick, Arcanoid* arcanoid) {
  if (ball->bottom() >= brick->top() && ball->top() <= brick->bottom()) {
    if (ball->left() <= brick->right() && ball->right() >= brick->right()) {
      ball->setX(brick->right());
      return true;
    }
    else if (ball->right() >= brick->left() && ball->left() <= brick->left()) {
      ball->setX(brick->left() - arcanoid->ballD);
      return true;
    }
  }
  return false;
}

bool Touching::touchedUD(Ball* ball, Brick* brick, Arcanoid* arcanoid) {
  if (ball->left() <= brick->right() && ball->right() >= brick->left()) {
    if (ball->top() <= brick->bottom() && ball->bottom() >= brick->bottom()) {
      ball->setY(brick->bottom());
      return true;
    }
    else if (ball->bottom() >= brick->top() && ball->top() <= brick->top()) {
      ball->setY(brick->top() - arcanoid->ballD);
      return true;
    }
  }
  return false;
}

void Touching::encounter(Ball* a, Ball* b, Arcanoid* arcanoid) {
  int xd = a->distanseX(b);
  int yd = a->distanseY(b);
  if (xd <= arcanoid->ballD && yd <= arcanoid->ballD) {
    a->swapSpeed(b);
    if (a != arcanoid->stickball && b != arcanoid->stickball)
      a->split(b);
    else {
      a->goUp();
      b->goUp();
    }
  }
}

void Touching::touchedMovingBrick(MovingBrick* brick, Arcanoid* arcanoid) {
  for (int i = 0; i < arcanoid->movingBricks.size(); i++) {
    if (brick->getPos() % columns == arcanoid->movingBricks[i]->getPos() % columns)
      if (arcanoid->bricksTouched(brick, arcanoid->movingBricks[i]))
        brick->goBack();
  }
}

void Touching::touchedSimpleBrick(MovingBrick* brick, Arcanoid* arcanoid) {
  int y = brick->getPos() % columns;
  for (int i = 0; i < columns; i++) {
    if (arcanoid->briks[i][y] != NULL) {
      if (arcanoid->bricksTouched(brick, arcanoid->briks[i][y]))
        brick->goBack();
    }
  }
}

void Touching::touchedLeft(MovingBrick* brick) {
  if (brick->left() < 0) {
    brick->setX(0);
    brick->goBack();
  }
}

void Touching::touchedRight(MovingBrick* brick, Arcanoid* arcanoid) {
  if (brick->right() > arcanoid->width) {
    brick->setX(arcanoid->width - arcanoid->brickw);
    brick->goBack();
  }
}

