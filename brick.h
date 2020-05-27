#pragma once
#include "bonus.h"

class Brick {
public:
  Brick(int Lives, int X, int Y, int W, int H);
  ~Brick() {}
  void _draw(QPainter* qp);
  virtual void draw(QPainter* qp) = 0;
  void setBonus(Drop b);
  Drop _loseLive();
  virtual Drop loseLive();
  void changeColor();
  void setColor(QColor col);
  void setColor();
  void setX(int X);
  void changePos(int s);
  virtual bool isAlive();
  int top();
  int bottom();
  int left();
  int right();
private:
  int x, y;
  int w, h;
  QColor color;
  int lives;
  Drop bonus;
};

class SimpleBrick : public Brick {
public:
  SimpleBrick(int lives, int x, int y, int w, int h);
  ~SimpleBrick() {}
  void draw(QPainter* qp);
};

class UnbreakableBrick : public Brick {
public:
  UnbreakableBrick(int lives, int x, int y, int w, int h);
  ~UnbreakableBrick() {}
  bool isAlive();
  void draw(QPainter* qp);
  Drop loseLive();
};

class SpeedBrick : public Brick {
public:
  SpeedBrick(int lives, int x, int y, int w, int h);
  ~SpeedBrick() {}
  void draw(QPainter* qp);
  Drop loseLive();
};

class MovingBrick : public Brick {
public:
  MovingBrick(int x, int y, int w, int h, int Pos);
  ~MovingBrick() {}
  void move();
  void goBack();
  int getPos();
  void draw(QPainter* qp);
private:
  int dx;
  int pos;
};
