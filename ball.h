#pragma once
#include <QPainter>

class Ball {
public:
  Ball(int xPos, int yPos, int diametr);
  ~Ball() {}
  Ball(const Ball& ball);
  void move();
  void speedUp();
  void draw(QPainter* qp);
  void changeXdir();
  void changeYdir();
  void moveRight();
  void moveLeft();
  void goUp();
  void setX(int X);
  void setY(int Y);
  int top();
  int bottom();
  int left();
  int right();
  void swapSpeed(Ball* b);
  void split(Ball* b);
  int distanseX(Ball* b);
  int distanseY(Ball* b);
private:
  int speed;
  int d;
  int x, y;
  int dx, dy;
};
