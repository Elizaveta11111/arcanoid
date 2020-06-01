#pragma once

#include <QtWidgets/QMainWindow>
#include <QKeyEvent>
#include "ui_arcanoid.h"
#include <vector>
#include "brick.h"
#include "ball.h"
#include "paddle.h"

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
protected:
  void paintEvent(QPaintEvent* event);
  void timerEvent(QTimerEvent*);
  void keyPressEvent(QKeyEvent*);
  void keyReleaseEvent(QKeyEvent*);
private:
  void setMessage(char* message, int size);
  void writeMessage(char* message, int size, QPainter* qp);
  void touchedStaticBricks(Ball* ball, bool* lrTouch, bool* udTouch);
  void touchedMovingBricks(Ball* ball, bool* lrTouch, bool* udTouch);
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
  void touchedTop(Ball* ball);
  void touchedLeft(Ball* ball);
  void touchedRight(Ball* ball);
  void touchedButtom(Ball* ball);
  void touchedLeft(MovingBrick* brick);
  void touchedRight(MovingBrick* brick);
  void touchedSimpleBrick(MovingBrick* brick);
  void touchedMovingBrick(MovingBrick* brick);
  void bounce(Ball* ball, int l, int r);
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
  void touchedBricks(Ball* ball);
  bool bricksTouched(Brick* a, Brick* b);
  void touchedPaddle(Ball* ball);
  void touchedBox(Ball* ball);
  bool touchedLR(Ball* ball, Brick* brick);
  bool touchedUD(Ball* ball, Brick* brick);
  void encounter(Ball* a, Ball* b);
  Ball* stickball;
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

