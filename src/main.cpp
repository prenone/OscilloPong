#include <Arduino.h>
#include "Potentiometer.h"
#include "Screen.h"

#define BALL_RADIUS 0.05
#define PLAYER_LENGTH 0.2
#define PLAYER_WIDTH 0.025
#define PLAYER_MARGIN 0.02

#define TOP 0.9
#define BOTTOM 0.1

#define USEC_PER_SEC 1000000

//#define SINGLEPLAYER1
//#define SINGLEPLAYER2

Potentiometer P1(2, A0); // blue Player 1
Potentiometer P2(3, A1); // red Player 2
Screen S1(DAC0, DAC1, 3, 3276, 3276);

int lastFrame = 0;

double xp, yp;
double lvxp, vxp, vyp;

int score1 = 0;
int score2 = 0;

void ResetPlay()
{
  xp = 0.5, yp = (TOP + BOTTOM) / 2;

  double theta = drand48() * 2 * M_PI;
  vxp = 0.2 * cos(theta);
  vyp = 0.2 * sin(theta);
}

void setup()
{
  Serial.begin(250000);
  pinMode(4, OUTPUT); // trigger

  analogReadResolution(12);
  analogWriteResolution(12);

  Serial.println("Calibrating P1");
  P1.Calibrate();
  Serial.println("Calibrating P2");
  P2.Calibrate();

  Serial.println("End of calibration");

  ResetPlay();
  lastFrame = micros();

  delay(1000);
}

void loop()
{
  int start = micros();

  // digitalWrite(4, HIGH);
  // delayMicroseconds(50); // may need to be removed!
  // digitalWrite(4, LOW);

  // Frame
  S1.DrawRectangle(0, BOTTOM, 1, TOP);

  // Ball
  int currentFrame = micros();
  xp += vxp * (currentFrame - lastFrame) / USEC_PER_SEC;
  yp += vyp * (currentFrame - lastFrame) / USEC_PER_SEC;
  lastFrame = currentFrame;

  lvxp = vxp;

  S1.DrawCircle(xp, yp, BALL_RADIUS);

  // Players
  double y1 = P1.Read();
  double y2 = P2.Read();

  y1 = BOTTOM + PLAYER_LENGTH / 2 + (y1 * (TOP - BOTTOM - PLAYER_LENGTH));
  y2 = BOTTOM + PLAYER_LENGTH / 2 + (y2 * (TOP - BOTTOM - PLAYER_LENGTH));

#ifdef SINGLEPLAYER1
  y1 = yp;
#endif
#ifdef SINGLEPLAYER2
  y2 = yp;
#endif

  S1.DrawRectangle(PLAYER_MARGIN, y1 - PLAYER_LENGTH / 2, PLAYER_MARGIN + PLAYER_WIDTH, y1 + PLAYER_LENGTH / 2);
  S1.DrawRectangle(1 - PLAYER_MARGIN, y2 - PLAYER_LENGTH / 2, 1 - (PLAYER_MARGIN + PLAYER_WIDTH), y2 + PLAYER_LENGTH / 2);

  // Scoreboard
  for (int i = 0; i < score1; i++)
    S1.DrawVerticalLine(0.15 + i * 0.02, TOP - 0.08, TOP - 0.03);
  for (int i = 0; i < score2; i++)
    S1.DrawVerticalLine(0.85 - i * 0.02, TOP - 0.08, TOP - 0.03);

  if (xp - BALL_RADIUS < PLAYER_MARGIN + PLAYER_WIDTH) // Player 1 collision
  {
    if (xp - BALL_RADIUS < 0)
    { // Player 1 loses
      score2++;
      ResetPlay();
    }

    if (abs(y1 - yp) < PLAYER_LENGTH / 2 + BALL_RADIUS)
      vxp = abs(vxp);
  }

  if (xp + BALL_RADIUS > 1 - (PLAYER_MARGIN + PLAYER_WIDTH)) // Player 2 collision
  {
    if (xp + BALL_RADIUS > 1)
    { // Player 2 loses
      score1++;
      ResetPlay();
    }

    if (abs(y2 - yp) < PLAYER_LENGTH / 2 + BALL_RADIUS)
      vxp = -abs(vxp);
  }

  if (yp - BALL_RADIUS < BOTTOM || yp + BALL_RADIUS > TOP)
  {
    vyp = -vyp;
  }

  if ((lvxp > 0 && vxp < 0) || (lvxp < 0 && vxp > 0))
  {
    vxp = vxp + ((vxp > 0) - (vxp < 0)) * 0.05;
    vyp = vyp + ((vyp > 0) - (vyp < 0)) * 0.05;
  }

  if (score1 > 5 || score2 > 5)
  {
    score1 = 0;
    score2 = 0;
    ResetPlay();
  }

  int stop = micros();
  //Serial.println(String(stop - start));
}