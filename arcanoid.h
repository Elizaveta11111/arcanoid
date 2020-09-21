#pragma once

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_arcanoid.h"
#include <vector>
#include "brick.h"
#include "ball.h"
#include "paddle.h"
#include "touching.h"

const int rows = 8;
const int columns = 6;

class Arcanoid : public QWidget {
public:
  Arcanoid(QWidget* parent = 0);
  void CreateNewBall();
  void CreateMovingBrick();
  void BallSpeedUp();
  void ShrinkPaddle();
  void ExpandPaddle();
  void AddLives();
  void StickToPaddle();
  void AddPoint();
  ~Arcanoid();
  friend class Touching;
protected:
  void paintEvent(QPaintEvent* event);
  void timerEvent(QTimerEvent*);
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);
private:
  void setMessage(QString* message);
  void writeMessage(const QString& message, QPainter* qp);
  void drawField(QPainter* qp);
  void drawBalls(QPainter* qp);
  void drawBonuses(QPainter* qp);
  void drawMovingBricks(QPainter* qp);
  void drawBricks(QPainter* qp);
  void drawDno(QPainter* qp);
  void createBricks();
  void deleteBricks();
  void moveBonuses();
  void moveBricks();
  void moveBalls();
  void bonusPaddle(Ball*);
  void catchBonuses();
  void dropBonus(int i, int j, Drop bonus, Ball* ball);
  Brick* briks[columns][rows];
  std::vector<Ball*> balls;
  std::vector<MovingBrick*> movingBricks;
  std::vector<Bonus*> bonuses;
  std::vector<Paddle*> dno;
  Paddle* paddle;
  int timerId;
  int bonuslives;
  int lives;
  int briksleft;
  void checkBrick(MovingBrick* brick);
  void checkEncounter();
  void checkColisions(Ball* ball);
  bool bricksTouched(Brick* a, Brick* b);
  Ball* stickball;
  Touching* touched;
  int stick;
  int brickPos;
  int tick;
  int points;
  const int width = 500;
  const int height = 600;
  const int brickw = 50;
  const int brickh = 20;
  const int ballD = 16;
  const int paddleh = 10;
};

