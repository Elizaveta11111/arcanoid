#include "arcanoid.h"
#include <stdio.h>
#include <stdlib.h>

void Arcanoid::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  int i, j;
  char message[30];
  QPainter painter(this);
  if (lives == 0) {
    snprintf(message, sizeof(message), "fail");
    killTimer(timerId);
  }
  else if(briksleft == 0) {
    snprintf(message, sizeof(message), "win");
    killTimer(timerId);
  }
  else 
    snprintf(message, sizeof(message), "Points: %d\n Lives: %d", points, lives);
  painter.setPen(QColor("#FF8C00"));
  paddle->draw(&painter);
  if (!balls.empty())
    for (i = 0; i < balls.size(); i++)
      balls[i]->draw(&painter);
  if (!bonuses.empty())
    for (i = 0; i < bonuses.size(); i++)
      bonuses[i]->draw(&painter);
  if (!movingBricks.empty())
    for (i = 0; i < movingBricks.size(); i++)
      movingBricks[i]->draw(&painter);
  for (i = 0; i < columns; i++)
    for (j = 0; j < rows; j++)
      if (briks[i][j] != NULL)
        briks[i][j]->draw(&painter);
  if (!dno.empty()) {
    tick--;
    for (i = 0; i < dno.size(); i++)
      dno[i]->draw(&painter);
    if (tick == 0) {
      dno.clear();
    }
  }
  catchBonuses();
  QFont font("Courier", 15, QFont::DemiBold);
  QFontMetrics fm(font);
  painter.setFont(font);
  int textWidth = fm.width(QString(message));
  painter.translate(QPoint(textWidth, fm.height()));
  painter.drawText(-textWidth, 0, message);
}

Arcanoid::Arcanoid(QWidget* parent) {
  int i, j;
  timerId = startTimer(8);
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
  stickball = balls[0];
  brickPos = 0;
  bonuslives = 0;
  lives = 3;
  points = 0;
  stick = 1;
  briksleft = columns * rows - 4;
  repaint();
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
      if (balls[i] == stickball)
        balls[i]->setX(balls[i]->left() + paddle->getDx());
      else
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
      encounter(balls[i], balls[j]);
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
  if (ball->top() >= height) {
    int i;
    for (i = 0; i < balls.size(); i++)
      if (balls[i] == ball)
        break;
    delete ball;
    balls.erase(balls.begin() + i);
    if (balls.empty()) {
      lives--;
      if (lives > 0) {
        balls.push_back(new Ball(paddle->left() + (paddle->right() - paddle->left()) / 2 - ballD / 2, paddle->top() - ballD, ballD));
        stick = 1;
        stickball = balls[balls.size() - 1];
      }
    }
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
  if (ball->bottom() >= paddle->top()) {
    if (ball->left() >= l && ball->right() <= r) {
      if (ball->right() <= l + (r - l) / 3)
        ball->moveLeft();
      else if (ball->left() >= r - (r - l) / 3)
        ball->moveRight();
      else
        ball->changeXdir();
      ball->setY(paddle->top() - ballD);
      ball->changeYdir();
      if (stick && stickball == NULL)
        stickball = ball;
    }
    else if (bonuslives) {
      tick = 10;
      dno.push_back(new Paddle(ball->left(), paddle->top(), ballD, paddleh, width));
      repaint();
      ball->changeYdir();
      bonuslives--;
    }
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
          points++;
          lrTouch = true;
        }
        else if (touchedUD(ball, briks[i][j])) {
          dropBonus(i, j, briks[i][j]->loseLive(), ball);
          points++;
          udTouch = true;
        }
        if (!briks[i][j]->isAlive()) {
          briksleft--;
          delete briks[i][j];
          briks[i][j] = NULL;
        }
      }
    }
  if (!movingBricks.empty())
    for (i = 0; i < movingBricks.size(); i++) {
      if (touchedLR(ball, movingBricks[i])) {
        movingBricks[i]->loseLive();
        points++;
        lrTouch = true;
      }
      else if (touchedUD(ball, movingBricks[i])) {
        movingBricks[i]->loseLive();
        points++;
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
  else if (bonus == Drop::losepoints)
    points--;
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
      if (bonuses[i]->getType() == Drop::dropSpeed) {
        balls[balls.size() - 1]->speedUp();
      }
      if (bonuses[i]->getType() == Drop::shrink) {
        paddle->changeSize(-1);
      }
      if (bonuses[i]->getType() == Drop::expand) {
        paddle->changeSize(1);
      }
      if (bonuses[i]->getType() == Drop::live) {
        bonuslives++;
      }
      if (bonuses[i]->getType() == Drop::stick) {
        stick++;
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
    if (stickball != NULL) {
      stick--;
      stickball = NULL;
    }
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

void Arcanoid::encounter(Ball* a, Ball* b) {
  int xd = a->distanseX(b);
  int yd = a->distanseY(b);
  if (xd <= ballD && yd <= ballD) {
    a->swapSpeed(b);
    if (a != stickball && b != stickball)
      a->split(b);
    else {
      a->goUp();
      b->goUp();
    }
  }
}

int Ball::distanseX(Ball* b) {
  return abs(this->x - b->x);
}

int Ball::distanseY(Ball* b) {
  return abs(this->y - b->x);
}

void Ball::swapSpeed(Ball* b) {
    Ball* a = this;
    int tx, ty;
    tx = a->dx;
    ty = a->dy;
    a->dx = b->dx;
    a->dy = b->dy;
    b->dx = tx;
    b->dy = ty;
}

void Ball::split(Ball* b) {
  Ball* a = this;
  if (b->x < a->x)
    a->x = b->x + d;
  else
    b->x = a->x + d;
  if (b->y < a->y)
    a->y = b->y + d;
  else
    b->y = a->y + d;
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