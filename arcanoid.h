#pragma once

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include "ui_arcanoid.h"
#include <vector>

const int rows = 8;
const int columns = 6;
enum class Drop {nothing, point, speed, newball, brick};

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
    speed = 3;
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
  void encounter(Ball* b);
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
  int maxRight;
  int x, y;
  int w, h;
  int dx;
};

class Brick {
public:
  Brick(int Lives, int X, int Y, int W, int H) {
    lives = Lives;
    x = X;
    y = Y;
    w = W;
    h = H;
    bonus = Drop::nothing;
  }
  void _draw(QPainter* qp) {
    qp->setBrush(color);
    qp->drawRect(x, y, w, h);
  }
  virtual void draw(QPainter* qp) = 0;
  void setBonus(Drop b) {
    bonus = b;
  }
  Drop _loseLive() {
    Drop temp = bonus;
    bonus = Drop::nothing;
    lives--;
    changeColor();
    return temp;
  }
  virtual Drop loseLive() {
    return this->_loseLive();
  }
  void changeColor() {
    switch (lives) {
    case 1:
      color = QColor("#FA8072");
      break;
    case 2:
      color = QColor("#FF0000");
      break;
    case 3:
      color = QColor("#B22222");
      break;
    case 4:
      color = QColor("#8B0000");
      break;
    default:
      break;
    }
  }
  void setColor(QColor col) {
    color = col;
  }
  void setColor() {
    changeColor();
  }
  void setX(int X) {
    x = X;
  }
  void changePos(int s) {
    x += s;
  }
  virtual bool isAlive() {
    return lives;
  }
  int top() {
    return y;
  }
  int bottom() {
    return y + h;
  }
  int left() {
    return x;
  }
  int right() {
    return x + w;
  }
private:
  int x, y;
  int w, h;
  QColor color;
  int lives;
  Drop bonus;
};

class UnbreakableBrick : public Brick {
public:
  UnbreakableBrick(int lives, int x, int y, int w, int h) : Brick(lives, x, y, w, h) {
    this->setColor();
    this->setBonus(Drop::nothing);
  }
  bool isAlive() {
    return true;
  }
  void draw(QPainter* qp) {
    this->_draw(qp);
  }
};

class SimpleBrick : public Brick {
public:
  SimpleBrick(int lives, int x, int y, int w, int h) : Brick(lives, x, y, w, h) {
    this->setColor();
    switch (rand() % 8) {
    case 1:
      this->setBonus(Drop::brick);
      //this->setColor(QColor("blue"));
      break;
    case 2:
      this->setBonus(Drop::newball);
      //this->setColor(QColor("green"));
      break;
    default:
      this->setBonus(Drop::nothing);
      break;
    }
  }
  void draw(QPainter* qp) {
    qp->setPen(QColor("#FF8C00"));
    this->_draw(qp);
  }
};

class SpeedBrick : public Brick {
public:
  SpeedBrick(int lives, int x, int y, int w, int h) : Brick(lives, x, y, w, h) { 
    this->setBonus(Drop::speed);
  }
  void draw(QPainter* qp) {
    setColor(QColor("#FF1493"));
    this->_draw(qp);
  }
  virtual Drop loseLive() {
    this->_loseLive();
    setBonus(Drop::speed);
    return Drop::speed;
  }
};

class MovingBrick : public Brick {
public:
  MovingBrick(int x, int y, int w, int h, int Pos) : Brick(3, x, y, w, h) {
    this->setColor();
    dx = 1;
    pos = Pos;
  }
  void move() {
    this->changePos(dx);
  }
  void goBack() {
    dx = -dx;
  }
  int getPos() {
    return pos;
  }
  void draw(QPainter* qp) {
    this->_draw(qp);
  }
private:
  int dx;
  int pos;
};

class Bonus {
public:
  Bonus(Drop Type, int X, int Y) {
    type = Type;
    if (type == Drop::newball) 
      color = QColor("green");
    else if (type == Drop::brick)
      color = QColor("red");
    type = Type;
    x = X;
    y = Y;
  }
  Drop getType() {
    return type;
  }
  void draw(QPainter* qp) {
    qp->setBrush(color);
    qp->drawRect(x - w/2, y, w, h);
  }
  void move() {
    y += dy;
  }
  int top() {
    return y;
  }
  int bottom() {
    return y + h;
  }
  int left() {
    return x;
  }
  int right() {
    return x + w;
  }
private:
  Drop type;
  QColor color;
  int const w = 10;
  int const h = 10;
  int const dy = 1;
  int x, y;
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
  Paddle* paddle;
  int timerId;
  void checkBrick(MovingBrick* brick);
  void checkEncounter();
  void checkColisions(Ball* ball);
  void touchedBricks(Ball* ball);
  bool bricksTouched(Brick* a, Brick* b);
  void touchedPaddle(Ball* ball);
  void touchedBox(Ball* ball);
  bool touchedLR(Ball* ball, Brick* brick);
  bool touchedUD(Ball* ball, Brick* brick);
  void startGame();
  int brickPos;
  const int width = 500;
  const int height = 600;
  const int brickw = 50;
  const int brickh = 20;
  const int ballD = 16;
};

