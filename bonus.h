#pragma once
#include <QPainter>

enum class Drop { nothing, point, speed, newball, brick, dropSpeed, shrink, expand, live, stick, losepoints };

class Bonus {
public:
  Bonus(Drop Type, int X, int Y) {
    type = Type;
    color = QColor("red");
    type = Type;
    x = X;
    y = Y;
  }
  ~Bonus() {}
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
