#include "arcanoid.h"
#include <stdio.h>
#include <stdlib.h>

void Arcanoid::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QString message;
  QPainter painter(this);
  drawField(&painter);
  setMessage(&message);
  writeMessage(message, &painter);
}

void  Arcanoid::setMessage(QString* message) {
  if (lives == 0) {
    *message = "fail";
    killTimer(timerId);
  }
  else if (briksleft == 0) {
    *message = "win";
    killTimer(timerId);
  }
  else {
    QString spoints, slives;
    spoints.setNum(points);
    slives.setNum(lives);
    *message = "Points: ";
    message->append(spoints);
    message->append(" Lives: ");
    message->append(slives);
  }
}

void Arcanoid::writeMessage(const QString& message,  QPainter* qp) {
  QFont font("Courier", 15, QFont::DemiBold);
  QFontMetrics fm(font);
  qp->setFont(font);
  int textWidth = fm.width(message);
  qp->translate(QPoint(textWidth, fm.height()));
  qp->drawText(-textWidth, 0, message);
}

void Arcanoid::drawField(QPainter* qp) {
  qp->setPen(QColor("#FF8C00"));
  paddle->draw(qp);
  drawBalls(qp);
  drawBonuses(qp);
  drawMovingBricks(qp);
  drawBricks(qp);
  drawDno(qp);
  catchBonuses();
}

void Arcanoid::drawBalls(QPainter* qp) {
  if (!balls.empty())
    for (int i = 0; i < balls.size(); i++)
      balls[i]->draw(qp);
}

void Arcanoid::drawBonuses(QPainter* qp) {
  if (!bonuses.empty())
    for (int i = 0; i < bonuses.size(); i++)
      bonuses[i]->draw(qp);
}

void Arcanoid::drawMovingBricks (QPainter* qp) {
  if (!movingBricks.empty())
    for (int i = 0; i < movingBricks.size(); i++)
      movingBricks[i]->draw(qp);
}

void Arcanoid::drawBricks(QPainter* qp) {
  int i, j;
  for (i = 0; i < columns; i++)
    for (j = 0; j < rows; j++)
      if (briks[i][j] != NULL)
        briks[i][j]->draw(qp);
}

void Arcanoid::drawDno(QPainter* qp) {
  if (!dno.empty()) {
    tick--;
    for (int i = 0; i < dno.size(); i++)
      dno[i]->draw(qp);
    if (tick == 0) {
      dno.clear();
    }
  }
}

Arcanoid::Arcanoid(QWidget* parent) {
  static const int bottomSpace = 30;
  static const int numberOfUnbreakable = 4;
  timerId = startTimer(8);
  createBricks();
  balls.push_back(new Ball((width - ballD )/ 2, height - (bottomSpace + ballD), ballD));
  paddle = new Paddle((width-paddlew) / 2 , height - bottomSpace, paddlew, paddleh, width);
  touched = new Touching();
  stickball = balls[0];
  brickPos = 0;
  bonuslives = 0;
  lives = 3;
  points = 0;
  stick = 1;
  briksleft = columns * rows - numberOfUnbreakable;
}

void Arcanoid::createBricks() {
  int i, j;
  static const int firstLayer = 2, secondLayer = 4, thirdLayer = 6, fourthLayer = 8;
  static const int WeakBrick = 1, MediumBrick = 2, HardBrick = 3, HardestBrick = 4;
  static const int UnbreakableFirsRow = 2, UnbreakableLastRow = 6, SpeedRow = rows - 1;
  static const int UnbreakableOneX = 5, UnbreakableTwoX = 0, UnbreakableTreeX = 4, UnbreakableFourX = 1;
  static const int SpeedOneX = 5, SpeedTwoX = 0;
  for (i = 0; i < columns; i++) {
    for (j = 0; j < firstLayer; j++) {
      briks[i][j] = new SimpleBrick(WeakBrick, brickw * (leftspace + i), brickh * (topspace + j), brickw, brickh);
    }
    for (j = firstLayer; j < secondLayer; j++) {
      if ((i == UnbreakableOneX || i == UnbreakableTwoX) && j == UnbreakableFirsRow)
        briks[i][j] = new UnbreakableBrick(MediumBrick, brickw * (leftspace  + i), brickh * (topspace + j), brickw, brickh);
      else
        briks[i][j] = new SimpleBrick(MediumBrick, brickw * (leftspace + i), brickh * (topspace + j), brickw, brickh);
    }
    for (j = secondLayer; j < thirdLayer; j++)
      briks[i][j] = new SimpleBrick(HardBrick, brickw * (leftspace + i), brickh * (topspace + j), brickw, brickh);
    for (j = thirdLayer; j < fourthLayer; j++) {
      if ((i == UnbreakableTreeX || i == UnbreakableFourX) && j == UnbreakableLastRow)
        briks[i][j] = new UnbreakableBrick(HardestBrick, brickw * (leftspace + i), brickh * (topspace + j), brickw, brickh);
      else if ((i == SpeedOneX || i == SpeedTwoX) && j == SpeedRow)
        briks[i][j] = new SpeedBrick(MediumBrick, brickw * (leftspace + i), brickh * (topspace + j), brickw, brickh);
      else
        briks[i][j] = new SimpleBrick(HardestBrick, brickw * (leftspace + i), brickh * (topspace + j), brickw, brickh);
    }
  }
}

Arcanoid::~Arcanoid() {
  int i;
  deleteBricks();
  if (!balls.empty()) {
    for (i = 0; i < balls.size(); i++)
      delete balls[i] ;
    balls.clear();
  }
  if (!movingBricks.empty()) {
    for (i = 0; i < movingBricks.size(); i++)
      delete movingBricks[i];
    movingBricks.clear();
  }
  if (!bonuses.empty()) {
    for (i = 0; i < bonuses.size(); i++)
      delete bonuses[i];
    bonuses.clear();
  }
  if (!dno.empty()) {
    for (i = 0; i < dno.size(); i++)
      delete dno[i];
    dno.clear();
  }
  delete paddle;
}

void Arcanoid::deleteBricks() {
  int i, j;
  for (i = 0; i < columns; i++)
    for (j = 0; j < rows; j++) {
      if (briks[i][j] != NULL)
        delete briks[i][j];
    }
}

void Arcanoid::timerEvent(QTimerEvent* e) {

  Q_UNUSED(e);

  paddle->move();
  moveBonuses();
  moveBricks();
  moveBalls();
  repaint();
}

void Arcanoid::moveBonuses() {
  if (!bonuses.empty())
    for (int i = 0; i < bonuses.size(); i++)
      bonuses[i]->move();
}

void Arcanoid::moveBricks() {
  if (!movingBricks.empty())
    for (int i = 0; i < movingBricks.size(); i++) {
      checkBrick(movingBricks[i]);
      movingBricks[i]->move();
    }
}

void Arcanoid::moveBalls() {
  if (!balls.empty()) {
    for (int i = 0; i < balls.size(); i++) {
      if (balls[i] == stickball)
        balls[i]->setX(balls[i]->left() + paddle->getDx());
      else
        balls[i]->move();
      checkColisions(balls[i]);
    }
    checkEncounter();
  }
}

void Arcanoid::checkEncounter() {
  int i, j;
  for (i = 0; i < (int)balls.size() - 1; i++)
    for (j = i + 1; j < (int)balls.size(); j++)
      touched->encounter(balls[i], balls[j], this);
}

void Arcanoid::checkColisions(Ball* ball) {
  touched->touchedBox(ball, this);
  touched->touchedPaddle(ball, paddle, this);
  touched->touchedBricks(ball, this);
}

void Arcanoid::bonusPaddle(Ball* ball) {
  tick = 10;
  dno.push_back(new Paddle(ball->left(), paddle->top(), ballD, paddleh, width));
  repaint();
  ball->changeYdir();
  bonuslives--;
}

void Arcanoid::dropBonus(int i, int j, Drop bonus, Ball* ball) {
  switch (bonus) {
  case Drop::newball:
    bonuses.push_back(new NewBallBonus(bonus, (leftspace + i) * brickw, (topspace + j) * brickh));
    break;
  case Drop::brick:
    bonuses.push_back(new BrickBonus(bonus, (leftspace + i) * brickw, (topspace + j) * brickh));
    break;
  case Drop::speed:
    bonuses.push_back(new SpeedBonus(bonus, (leftspace + i) * brickw, (topspace + j) * brickh));
    break;
  case Drop::shrink:
    bonuses.push_back(new ShrinkBonus(bonus, (leftspace + i) * brickw, (topspace + j) * brickh));
    break;
  case Drop::expand:
    bonuses.push_back(new ExpandBonus(bonus, (leftspace + i) * brickw, (topspace + j) * brickh));
    break;
  case Drop::live:
    bonuses.push_back(new LiveBonus(bonus, (leftspace + i) * brickw, (topspace + j) * brickh));
    break;
  case Drop::stick:
    bonuses.push_back(new StickBonus(bonus, (leftspace + i) * brickw, (topspace + j) * brickh));
    break;
  default:
    break;
  } 
}

void Arcanoid::catchBonuses() {
  for (int i = 0; i < bonuses.size(); i++) {
    if ((bonuses[i]->bottom() >= paddle->top() && bonuses[i]->left() <= paddle->right() && 
                  bonuses[i]->right() >= paddle->left())) {
      bonuses[i]->destroed(this);
      delete bonuses[i];
      bonuses.erase(bonuses.begin() + i--);
    }
    else if (bonuses[i]->top() > height) {
      delete bonuses[i];
      bonuses.erase(bonuses.begin() + i--);
    }
  }
}

void Arcanoid::keyPressEvent(QKeyEvent* e) {

  switch (e->key()) {
  case Qt::Key_Space:
    if (stickball != NULL) {
      stick--;
      stickball = NULL;
    }
    break;
  case Qt::Key_Left:
    paddle->moveLeft();
    break;
  case Qt::Key_Right:
    paddle->moveRight();
    break;
  }
}

void Arcanoid::keyReleaseEvent(QKeyEvent * e) {

  switch (e->key()) {
  case Qt::Key_Left:
  case Qt::Key_Right:
    paddle->dontMove();
    break;
  }
}

void Arcanoid::checkBrick(MovingBrick* brick) {
  touched->touchedLeft(brick);
  touched->touchedRight(brick, this);
  touched->touchedSimpleBrick(brick, this);
  touched->touchedMovingBrick(brick, this);
}

bool Arcanoid::bricksTouched(Brick* a, Brick* b) {
  if (a == b)
    return false;
  return (a->right() >= b->left() && a->right() < b->right()) || (a->left() <= b->right() && a->left() >= b->left());
}

void Arcanoid::CreateNewBall() {
  balls.push_back(new Ball(*balls[0]));
}

void Arcanoid::CreateMovingBrick() {
  int x, y;
  x = brickPos / columns;
  if (x == 0)
    x = 0;
  else
    x = width - brickw;
  y = topspace + brickPos % columns;
  movingBricks.push_back(new MovingBrick(x, y * brickh, brickw, brickh, brickPos));
  brickPos++;
}

void Arcanoid::BallSpeedUp() {
  balls[balls.size() - 1]->speedUp();
}

void Arcanoid::ShrinkPaddle() {
  paddle->changeSize(-1);
}

void Arcanoid::ExpandPaddle() {
  paddle->changeSize(1);
}

void Arcanoid::AddLives() {
  bonuslives++;
}

void Arcanoid::StickToPaddle() {
  stick++;
}

void Arcanoid::AddPoint() {
  points++;
}
