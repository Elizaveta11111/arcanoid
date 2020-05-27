#include "paddle.h"

Paddle::Paddle(int xPos, int yPos, int width, int height, int right) {
  x = xPos;
  y = yPos;
  w = width;
  h = height;
  maxRight = right;
  dx = 0;
}

void Paddle::changeSize(int sign) {
  w += sign * shrinkTo;
  x -= sign * shrinkTo / 2;
  if (x < 0) {
    x = 0;
  }
  if (x > maxRight - w)
    x = maxRight - w;
}

void Paddle::draw(QPainter* qp) {
  qp->setBrush(QColor("#800000"));
  qp->drawRect(x, y, w, h);
}

void Paddle::moveRight() {
  dx = 8;
}

void Paddle::moveLeft() {
  dx = -8;
}

int Paddle::getDx() {
  return dx;
}

void Paddle::move() {
  int s = x + dx;
  if (s >= 0 && s <= maxRight - w)
    x = s;
}

void Paddle::dontMove() {
  dx = 0;
}

int Paddle::top() {
  return y;
}

int Paddle::left() {
  return x;
}

int Paddle::right() {
  return x + w;
}