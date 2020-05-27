#pragma once
#include <QPainter>
class Paddle {
public:
  Paddle(int xPos, int yPos, int width, int height, int right);
  ~Paddle() {}
  void changeSize(int sign);
  void draw(QPainter* qp);
  void moveRight();
  void moveLeft();
  int getDx();
  void move();
  void dontMove();
  int top();
  int left();
  int right();
private:
  int const shrinkTo = 20;
  int maxRight;
  int x, y;
  int w, h;
  int dx;
};