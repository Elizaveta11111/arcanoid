#include "ball.h"

Ball::Ball(int xPos, int yPos, int diametr) {
  x = xPos;
  y = yPos;
  dx = 1;
  dy = -1;
  d = diametr;
  speed = 2;
}

Ball::Ball(const Ball& ball) {
  x = ball.x;
  y = ball.y;
  dx = -ball.dx;
  dy = -ball.dy;
  d = ball.d;
  speed = 2;
}

void Ball::move() {
  x += dx * speed;
  y += dy * speed;
}

void Ball::speedUp() {
  speed++;
}

void Ball::draw(QPainter* qp) {
  qp->setBrush(QColor("#FF4500"));
  qp->drawEllipse(x, y, d, d);
}

void Ball::changeXdir() {
  dx = -dx;
}

void Ball::changeYdir() {
  dy = -dy;
}

void Ball::moveRight() {
  dx = 1;
}

void Ball::moveLeft() {
  dx = -1;
}

void Ball::goUp() {
  dy = -1;
}

void Ball::setX(int X) {
  x = X;
}

void Ball::setY(int Y) {
  y = Y;
}

int Ball::top() {
  return y;
}

int Ball::bottom() {
  return y + d;
}

int Ball::left() {
  return x;
}

int Ball::right() {
  return x + d;
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