#pragma once

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include "ui_arcanoid.h"
#include <vector>
#include "brick.h"

const int rows = 8;
const int columns = 6;


class Ball {
public:
  Ball(int xPos, int yPos, int diametr) {
    x = xPos;
    y = yPos;
    dx = 1;
    dy = -1;
    d = diametr;
    speed = 1;
  }
  Ball(const Ball& ball) {
    x = ball.x;
    y = ball.y;
    dx = -ball.dx;
    dy = -ball.dy;
    d = ball.d;
    speed = 1;
  }
  void move() {
    x += dx * speed;
    y += dy * speed;
  }
  void speedUp() {
    speed++;
  }
  void draw(QPainter* qp) {
    qp->setBrush(QColor("#FF4500"));
    qp->drawEllipse(x, y, d, d);
  }
  void changeXdir() {
    dx = -dx;
  }
  void changeYdir() {
    dy = -dy;
  }
  void moveRight() {
    dx = 1;
  }
  void moveLeft() {
    dx = -1;
  }
  void goUp() {
    dy = -1;
  }
  void setX(int X) {
    x = X;
  }
  void setY(int Y) {
    y = Y;
  }
  int top() {
    return y;
  }
  int bottom() {
    return y + d;
  }
  int left() {
    return x;
  }
  int right() {
    return x + d;
  }
  void swapSpeed(Ball* b);
  void split(Ball* b);
  int distanseX(Ball* b);
  int distanseY(Ball* b);
private :
  int speed;
  int d;
  int x, y;
  int dx, dy;
};

class Paddle {
public:
  Paddle(int xPos, int yPos, int width, int height, int right) {
    x = xPos;
    y = yPos;
    w = width;
    h = height;
    maxRight = right;
    dx = 0;
  }
  void changeSize(int sign) {
    w += sign * shrinkTo;
    x -= sign * shrinkTo / 2;
    if (x < 0) {
      x = 0;
    }
    if (x > maxRight - w)
      x = maxRight - w;
  }
  void draw(QPainter* qp) {
    qp->setBrush(QColor("#800000"));
    qp->drawRect(x, y, w, h);
  }
  void moveRight() {
    dx = 8;
  }
  void moveLeft() {
    dx = -8;
  }
  int getDx() {
    return dx;
  }
  void move() {
    int s = x + dx;
    if (s >= 0 && s <= maxRight - w)
      x = s;
  }
  void dontMove() {
    dx = 0;
  }
  int top() {
    return y;
  }
  int left() {
    return x;
  }
  int right() {
    return x + w;
  }
private:
  int const shrinkTo = 20;
  int maxRight;
  int x, y;
  int w, h;
  int dx;
};

class Arcanoid : public QWidget {
public:
  Arcanoid(QWidget* parent = 0);
protected:
  void paintEvent(QPaintEvent* event);
  void timerEvent(QTimerEvent*);
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);
private:
  void catchBonuses();
  void dropBonus(int i, int j, Drop bonus, Ball* ball);
  Brick* briks[columns][rows];
  std::vector<Ball*> balls;
  std::vector<MovingBrick*> movingBricks;
  std::vector<Bonus*> bonuses;
  std::vector<Paddle*> dno;
  Paddle* paddle;
  int timerId;
  int bonuslives;
  int lives;
  int briksleft;
  void checkBrick(MovingBrick* brick);
  void checkEncounter();
  void checkColisions(Ball* ball);
  void touchedBricks(Ball* ball);
  bool bricksTouched(Brick* a, Brick* b);
  void touchedPaddle(Ball* ball);
  void touchedBox(Ball* ball);
  bool touchedLR(Ball* ball, Brick* brick);
  bool touchedUD(Ball* ball, Brick* brick);
  void encounter(Ball* a, Ball* b);
  Ball* stickball;
  int stick;
  int brickPos;
  int tick;
  int points;
  const int width = 500;
  const int height = 600;
  const int brickw = 50;
  const int brickh = 20;
  const int ballD = 16;
  const int paddleh = 10;
};

