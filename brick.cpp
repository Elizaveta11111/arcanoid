#include "brick.h"

Brick::Brick(int Lives, int X, int Y, int W, int H) {
  lives = Lives;
  x = X;
  y = Y;
  w = W;
  h = H;
  bonus = Drop::nothing;
}

void Brick::_draw(QPainter* qp) {
  qp->setBrush(color);
  qp->drawRect(x, y, w, h);
}

void Brick::setBonus(Drop b) {
  bonus = b;
}

Drop Brick::_loseLive() {
  Drop temp = bonus;
  bonus = Drop::nothing;
  lives--;
  changeColor();
  return temp;
}

Drop Brick::loseLive() {
  return this->_loseLive();
}

void Brick::changeColor() {
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

void Brick::setColor(QColor col) {
  color = col;
}

void Brick::setColor() {
  changeColor();
}

void Brick::setX(int X) {
  x = X;
}

void Brick::changePos(int s) {
  x += s;
}

bool Brick::isAlive() {
  return lives;
}

int Brick::top() {
  return y;
}

int Brick::bottom() {
  return y + h;
}

int Brick::left() {
  return x;
}

int Brick::right() {
  return x + w;
}

SimpleBrick::SimpleBrick(int lives, int x, int y, int w, int h) : Brick(lives, x, y, w, h) {
  this->setColor();
  switch (rand() % 10) {
  case 1:
    this->setBonus(Drop::brick);
    break;
  case 2:
    this->setBonus(Drop::newball);
    break;
  case 3:
    this->setBonus(Drop::dropSpeed);
    break;
  case 4:
    this->setBonus(Drop::expand);
    break;
  case 5:
    this->setBonus(Drop::shrink);
    break;
  case 6:
    this->setBonus(Drop::live);
    break;
  case 7:
    this->setBonus(Drop::stick);
    //this->setColor(QColor("green"));
    break;
  default:
    this->setBonus(Drop::nothing);
    break;
  }
}

void SimpleBrick::draw(QPainter* qp) {
  qp->setPen(QColor("#FF8C00"));
  this->_draw(qp);
}

UnbreakableBrick::UnbreakableBrick(int lives, int x, int y, int w, int h) : Brick(lives, x, y, w, h) {
  this->setColor();
  this->setBonus(Drop::losepoints);
}

bool UnbreakableBrick::isAlive() {
  return true;
}

void UnbreakableBrick::draw(QPainter* qp) {
  this->_draw(qp);
}

Drop UnbreakableBrick::loseLive() {
  this->_loseLive();
  return Drop::losepoints;
}

SpeedBrick::SpeedBrick(int lives, int x, int y, int w, int h) : Brick(lives, x, y, w, h) {
  this->setBonus(Drop::speed);
}

void SpeedBrick::draw(QPainter* qp) {
  setColor(QColor("#FF1493"));
  this->_draw(qp);
}

Drop SpeedBrick::loseLive() {
  this->_loseLive();
  setBonus(Drop::speed);
  return Drop::speed;
}

MovingBrick::MovingBrick(int x, int y, int w, int h, int Pos) : Brick(3, x, y, w, h) {
  this->setColor();
  dx = 1;
  pos = Pos;
}

void MovingBrick::move() {
  this->changePos(dx);
}

void MovingBrick::goBack() {
  dx = -dx;
}

int MovingBrick::getPos() {
  return pos;
}

void MovingBrick::draw(QPainter* qp) {
  this->_draw(qp);
}