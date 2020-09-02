#pragma once
#include <QPainter>
class Arcanoid;

enum class Drop { nothing, speed, newball, brick, shrink, expand, live, stick};

class Bonus {
public:
  Bonus(Drop Type, int X, int Y, QColor col);
  virtual ~Bonus() {}
  virtual void destroed(Arcanoid* a) = 0;
  Drop getType();
  void draw(QPainter* qp);
  void move();
  int top();
  int bottom();
  int left();
  int right();
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
  ~NewBallBonus() {}
};

class BrickBonus : public Bonus {
public:
  BrickBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#00008B")) {}
  void destroed(Arcanoid* a);
  ~BrickBonus() {}
};

class SpeedBonus : public Bonus {
public:
  SpeedBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#ADFF2F")) {}
  void destroed(Arcanoid* a);
  ~SpeedBonus() {}
};

class ShrinkBonus : public Bonus {
public:
  ShrinkBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#00FA9A")) {}
  void destroed(Arcanoid* a);
  ~ShrinkBonus() {}
};

class ExpandBonus : public Bonus {
public:
  ExpandBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#BDB76B")) {}
  void destroed(Arcanoid* a);
  ~ExpandBonus() {}
};

class LiveBonus : public Bonus {
public:
  LiveBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#FF00FF")) {}
  void destroed(Arcanoid* a);
  ~LiveBonus() {}
};

class StickBonus : public Bonus {
public:
  StickBonus(Drop Type, int X, int Y) : Bonus(Type, X, Y, QColor("#A0522D")) {}
  void destroed(Arcanoid* a);
  ~StickBonus() {}
};