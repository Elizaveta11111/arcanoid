#include "bonus.h"
#include "arcanoid.h"
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