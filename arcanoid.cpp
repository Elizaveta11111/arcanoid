#include "arcanoid.h"

Arcanoid::Arcanoid(QWidget* parent) {
  startGame();
  repaint();
}

void Arcanoid::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  int i, j;
  QPainter painter(this);
  painter.setPen(QColor("#FF8C00"));
  paddle->draw(&painter);
  if (!balls.empty())
    for (int i = 0; i < balls.size(); i++)
      balls[i]->draw(&painter);
  if (!bonuses.empty())
    for (i = 0; i < bonuses.size(); i++)
      bonuses[i]->draw(&painter);
  if (!movingBricks.empty())
    for (i = 0; i < movingBricks.size(); i++)
      movingBricks[i]->draw(&painter);
  for (i = 0; i < columns; i++)
    for (j = 0; j < rows; j++)
      if(briks[i][j] != NULL)
      briks[i][j]->draw(&painter);
  catchBonuses();
}

void Arcanoid::startGame() {
  int i, j;
  for (i = 0; i < columns; i++) {
    for (j = 0; j < 2; j++) {
        briks[i][j] = new SimpleBrick(1, brickw * (2 + i), brickh * (4 + j), brickw, brickh);
    }
    for (j = 2; j < 4; j++) {
      if ((i == 5 || i == 0) && j == 2)
        briks[i][j] = new UnbreakableBrick(2, brickw * (2 + i), brickh * (4 + j), brickw, brickh);
      else
        briks[i][j] = new SimpleBrick(2, brickw * (2 + i), brickh * (4 + j), brickw, brickh);
    }
    for (j = 4; j < 6; j++)
      briks[i][j] = new SimpleBrick(3, brickw * (2 + i), brickh * (4 + j), brickw, brickh);
    for (j = 6; j < rows; j++) {
      if ((i == 4 || i == 1) && j == 6)
        briks[i][j] = new UnbreakableBrick(4, brickw * (2 + i), brickh * (4 + j), brickw, brickh);
      else if ((i == 5 || i == 0) && j == rows - 1)
        briks[i][j] = new SpeedBrick(2, brickw * (2 + i), brickh * (4 + j), brickw, brickh);
      else
        briks[i][j] = new SimpleBrick(4, brickw * (2 + i), brickh * (4 + j), brickw, brickh);
    }
  }
  balls.push_back(new Ball(width / 2 - ballD/2, height - 46, ballD));
  paddle = new Paddle(width / 2 - 60, height - 30, 120, 10, width);
  brickPos = 0;
}

void Arcanoid::timerEvent(QTimerEvent* e) {

  Q_UNUSED(e);

  paddle->move();
  if (!bonuses.empty())
    for (int i = 0; i < bonuses.size(); i++)
      bonuses[i]->move();
  if (!movingBricks.empty())
    for (int i = 0; i < movingBricks.size(); i++) {
      checkBrick(movingBricks[i]);
      movingBricks[i]->move();
    }
  if (!balls.empty()) {
    for (int i = 0; i < balls.size(); i++) {
      balls[i]->move();
      checkColisions(balls[i]);
    }
    checkEncounter();
  }
  repaint();
}

void Arcanoid::checkEncounter() {
  int i, j;
  for (i = 0; i < balls.size() - 1; i++)
    for (j = i + 1; j < balls.size(); j++)
      balls[i]->encounter(balls[j]);
}

void Arcanoid::checkColisions(Ball* ball) {
  touchedBox(ball);
  touchedPaddle(ball);
  touchedBricks(ball);
}

void Arcanoid::touchedBox(Ball* ball) {
  if (ball->top() <= 0) {
    ball->setY(0);
    ball->changeYdir();
  }
  if (ball->bottom() >= height) {
    ball->changeYdir();
  }
  if ((ball->left() <= 0)) {
    ball->setX(0);
    ball->changeXdir();
  }
  if (ball->right() >= width) {
    ball->setX(width - ballD);
    ball->changeXdir();
  }
}

void Arcanoid::touchedPaddle(Ball* ball) {
  int r = paddle->right();
  int l = paddle->left();
  if (ball->bottom() >= paddle->top() && ball->left() >= l && ball->right() <= r) {
    if (ball->right() <= l + (r - l) / 3)
      ball->moveLeft();
    else if (ball->left() >= r - (r - l) / 3)
      ball->moveRight();
    else
      ball->changeXdir();
    ball->setY(paddle->top() - ballD);
    ball->changeYdir();
  }
}

void Arcanoid::touchedBricks(Ball* ball) {
  bool lrTouch = false;
  bool udTouch = false;
  int i, j;
  for (i = 0; i < columns; i++)
    for (j = 0; j < rows; j++) {
      if (briks[i][j] != NULL) {
        if (touchedLR(ball, briks[i][j])) {
          dropBonus(i, j, briks[i][j]->loseLive(), ball);
          lrTouch = true;
        }
        else if (touchedUD(ball, briks[i][j])) {
          dropBonus(i, j, briks[i][j]->loseLive(), ball);
          udTouch = true;
        }
        if (!briks[i][j]->isAlive()) {
          delete briks[i][j];
          briks[i][j] = NULL;
        }
      }
    }
  if (!movingBricks.empty())
    for (i = 0; i < movingBricks.size(); i++) {
      if (touchedLR(ball, movingBricks[i])) {
        movingBricks[i]->loseLive();
        lrTouch = true;
      }
      else if (touchedUD(ball, movingBricks[i])) {
        movingBricks[i]->loseLive();
        udTouch = true;
      }
      if (!movingBricks[i]->isAlive()) {
        delete movingBricks[i];
        movingBricks.erase(movingBricks.begin() + i--);
      }
    }
  if (lrTouch)
    ball->changeXdir();
  else if (udTouch)
    ball->changeYdir();
}

void Arcanoid::dropBonus(int i, int j, Drop bonus, Ball* ball) {
  if (bonus == Drop::nothing)
    return;
  else if (bonus == Drop::speed)
    ball->speedUp();
  else
    bonuses.push_back(new Bonus(bonus, (2 + i) * brickw, (4 + j) * brickh));
}

void Arcanoid::catchBonuses() {
  for (int i = 0; i < bonuses.size(); i++) {
    if ((bonuses[i]->bottom() >= paddle->top() && bonuses[i]->left() <= paddle->right() && 
                  bonuses[i]->right() >= paddle->left())) {
      if (bonuses[i]->getType() == Drop::newball) 
        balls.push_back(new Ball(*balls[0]));
      if (bonuses[i]->getType() == Drop::brick) {
        int x, y;
        x = brickPos / columns;
        if (x == 0)
          x = 0;
        else
          x = width - brickw;
        y = 4 + brickPos % columns;
        movingBricks.push_back(new MovingBrick(x, y * brickh, brickw, brickh, brickPos));
        brickPos++;
      }
      delete bonuses[i];
      bonuses.erase(bonuses.begin() + i--);
    }
    else if (bonuses[i]->top() > height) {
      delete bonuses[i];
      bonuses.erase(bonuses.begin() + i--);
    }
  }
}

bool Arcanoid::touchedLR(Ball* ball, Brick* brick) {
  if (ball->bottom() >= brick->top() && ball->top() <= brick->bottom()) {
    if (ball->left() <= brick->right() && ball->right() >= brick->right()) {
      ball->setX(brick->right());
      return true;
    }
    else if (ball->right() >= brick->left() && ball->left() <= brick->left()) {
      ball->setX(brick->left() - ballD);
      return true;
    }
  }
  return false;
}

bool Arcanoid::touchedUD(Ball* ball, Brick* brick) {
  if (ball->left() <= brick->right() && ball->right() >= brick->left()) {
    if (ball->top() <= brick->bottom() && ball->bottom() >= brick->bottom()) {
      ball->setY(brick->bottom());
      return true;
    }
    else if (ball->bottom() >= brick->top() && ball->top() <= brick->top()) {
      ball->setY(brick->top() - ballD);
      return true;
    }
  }
  return false;
}

void Arcanoid::keyPressEvent(QKeyEvent* e) {

  switch (e->key()) {
  case Qt::Key_Space:
    timerId = startTimer(8);
    break;
  case Qt::Key_Left:
    paddle->moveLeft();
    break;
  case Qt::Key_Right:
    paddle->moveRight();
    break;
  }
}

void Arcanoid::keyReleaseEvent(QKeyEvent * e) {

  switch (e->key()) {
  case Qt::Key_Left:
  case Qt::Key_Right:
    paddle->dontMove();
    break;
  }
}

void Ball::encounter(Ball* b) {
  Ball* a = this;
  int xd = abs(a->x - b->x);
  int yd = abs(a->y - b->y);
  int tx, ty;
  if (xd <= d && yd <= d) {
    tx = a->dx;
    ty = a->dy;
    a->dx = b->dx;
    a->dy = b->dy;
    b->dx = tx;
    b->dy = ty;
    if (b->x < a->x)
      a->x = b->x + d;
    else
      b->x = a->x + d;
    if (b->y < a->y)
      a->y = b->y + d;
    else
      b->y = a->y + d;
  }
}

void Arcanoid::checkBrick(MovingBrick* brick) {
  int i, j;
  int y = brick->getPos() % columns;
  if (brick->left() < 0) {
    brick->setX(0);
    brick->goBack();
  }
  if (brick->right() > width) {
    brick->setX(width - brickw);
    brick->goBack();
  }
  for (i = 0; i < columns; i++) {
      if (briks[i][y] != NULL) {
        if (bricksTouched(brick, briks[i][y]))
          brick->goBack();
      }
  } 
  for (int i = 0; i < movingBricks.size(); i++) {
    if(brick->getPos() % columns == movingBricks[i]->getPos() % columns)
      if (bricksTouched(brick, movingBricks[i]))
        brick->goBack();
  }
}

bool Arcanoid::bricksTouched(Brick* a, Brick* b) {
  if (a == b)
    return false;
  return (a->right() >= b->left() && a->right() < b->right()) || (a->left() <= b->right() && a->left() >= b->left());
}