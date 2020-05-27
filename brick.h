#pragma once
enum class Drop { nothing, point, speed, newball, brick, dropSpeed, shrink, expand, live, stick, losepoints};

class Bonus {
public:
  Bonus(Drop Type, int X, int Y) {
    type = Type;
    color = QColor("red");
    type = Type;
    x = X;
    y = Y;
  }
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
  int const w = 10;
  int const h = 10;
  int const dy = 1;
  int x, y;
};

class Brick {
public:
  Brick(int Lives, int X, int Y, int W, int H) {
    lives = Lives;
    x = X;
    y = Y;
    w = W;
    h = H;
    bonus = Drop::nothing;
  }
  void _draw(QPainter* qp) {
    qp->setBrush(color);
    qp->drawRect(x, y, w, h);
  }
  virtual void draw(QPainter* qp) = 0;
  void setBonus(Drop b) {
    bonus = b;
  }
  Drop _loseLive() {
    Drop temp = bonus;
    bonus = Drop::nothing;
    lives--;
    changeColor();
    return temp;
  }
  virtual Drop loseLive() {
    return this->_loseLive();
  }
  void changeColor() {
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
  void setColor(QColor col) {
    color = col;
  }
  void setColor() {
    changeColor();
  }
  void setX(int X) {
    x = X;
  }
  void changePos(int s) {
    x += s;
  }
  virtual bool isAlive() {
    return lives;
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
  int x, y;
  int w, h;
  QColor color;
  int lives;
  Drop bonus;
};

class SimpleBrick : public Brick {
public:
  SimpleBrick(int lives, int x, int y, int w, int h) : Brick(lives, x, y, w, h) {
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
  void draw(QPainter* qp) {
    qp->setPen(QColor("#FF8C00"));
    this->_draw(qp);
  }
};

class UnbreakableBrick : public Brick {
public:
  UnbreakableBrick(int lives, int x, int y, int w, int h) : Brick(lives, x, y, w, h) {
    this->setColor();
    this->setBonus(Drop::losepoints);
  }
  bool isAlive() {
    return true;
  }
  void draw(QPainter* qp) {
    this->_draw(qp);
  }
  Drop loseLive() {
    this->_loseLive();
    return Drop::losepoints;
  }
};

class SpeedBrick : public Brick {
public:
  SpeedBrick(int lives, int x, int y, int w, int h) : Brick(lives, x, y, w, h) {
    this->setBonus(Drop::speed);
  }
  void draw(QPainter* qp) {
    setColor(QColor("#FF1493"));
    this->_draw(qp);
  }
  virtual Drop loseLive() {
    this->_loseLive();
    setBonus(Drop::speed);
    return Drop::speed;
  }
};

class MovingBrick : public Brick {
public:
  MovingBrick(int x, int y, int w, int h, int Pos) : Brick(3, x, y, w, h) {
    this->setColor();
    dx = 1;
    pos = Pos;
  }
  void move() {
    this->changePos(dx);
  }
  void goBack() {
    dx = -dx;
  }
  int getPos() {
    return pos;
  }
  void draw(QPainter* qp) {
    this->_draw(qp);
  }
private:
  int dx;
  int pos;
};
