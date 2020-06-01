#include "bonus.h"
#include "arcanoid.h"

Bonus::Bonus(Drop Type, int X, int Y, QColor col) {
  type = Type;
  color = col;
  type = Type;
  x = X;
  y = Y;
}

Drop Bonus::getType() {
  return type;
}

void Bonus::draw(QPainter* qp) {
  qp->setBrush(color);
  qp->drawRect(x - w / 2, y, w, h);
}

void Bonus::move() {
  y += dy;
}

int Bonus::top() {
  return y;
}

int Bonus::bottom() {
  return y + h;
}

int Bonus::left() {
  return x;
}

int Bonus::right() {
  return x + w;
}

void NewBallBonus::destroed(Arcanoid* a) {
  a->CreateNewBall();
}

void BrickBonus::destroed(Arcanoid* a) {
  a->CreateMovingBrick();
}

void SpeedBonus::destroed(Arcanoid* a) {
  a->BallSpeedUp();
}

void ShrinkBonus::destroed(Arcanoid* a) {
  a->ShrinkPaddle();
}

void ExpandBonus::destroed(Arcanoid* a) {
  a->ExpandPaddle();
}

void LiveBonus::destroed(Arcanoid* a) {
  a->AddLives();
}

void StickBonus::destroed(Arcanoid* a) {
  a->StickToPaddle();
}