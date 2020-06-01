#pragma once
#include <QPainter>
class Arcanoid;

enum class Drop { nothing, point, speed, newball, brick, dropSpeed, shrink, expand, live, stick, losepoints };

class Bonus {
public:
  Bonus(Drop Type, int X, int Y, QColor col) {
    type = Type;
    color = col;
    type = Type;
    x = X;
    y = Y;
  }
  ~Bonus() {}
  virtual void destroed(Arcanoid* a) = 0;
  Drop getType() {
    return type;
  }
  void draw(QPainter* qp) {
    qp->setBrush(color);
    qp->drawRect(x - w / 2, y, w, h);
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
  int x, y;
  int const w = 10;
  int const h = 10;
  int const dy = 1;
};

class NewBallBonus : public Bonus {
public:
  NewBallBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#00FFFF")) {}
  void destroed(Arcanoid* a);
};

class BrickBonus : public Bonus {
public:
  BrickBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#00008B")) {}
  void destroed(Arcanoid* a);
};

class SpeedBonus : public Bonus {
public:
  SpeedBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#ADFF2F")) {}
  void destroed(Arcanoid* a);
};

class ShrinkBonus : public Bonus {
public:
  ShrinkBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#00FA9A")) {}
  void destroed(Arcanoid* a);
};

class ExpandBonus : public Bonus {
public:
  ExpandBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#BDB76B")) {}
  void destroed(Arcanoid* a);
};

class LiveBonus : public Bonus {
public:
  LiveBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#FF00FF")) {}
  void destroed(Arcanoid* a);
};

class StickBonus : public Bonus {
public:
  StickBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#A0522D")) {}
  void destroed(Arcanoid* a);
};