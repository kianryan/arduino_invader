#include "LedControl.h"

LedControl lc=LedControl(10,8,9,1);  // Pins: DIN,CLK,CS, # of Display connected

const int left = 4;
const int right = 3;

int position;

int rows[] = {-1, -1, -1, -1, -1, -1, -1};

int speed;
int spawn;
int move = 0;

int caught = 0;
const int levelup = 5;

void setup()
{
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  
  lc.shutdown(0,false);  // Wake up displays
  lc.setIntensity(0,5);  // Set intensity levels
  lc.clearDisplay(0);  // Clear Displays

  reset_vars();
}

void loop()
{
  
  if (speed == move) {
    /* Test colision */
    if (rows[0] != -1) {
      if (rows[0] == position) {
        caught = caught + 1;  
  
        if ((caught % levelup) == 0) {
          speed = speed - 1;
        }
      } else {
        // Fail
        lc.clearDisplay(0);
        delay(2000);
        reset_vars();
      }
    }
    update_invaders();
    move = 0;
  } else {
    move = move + 1;
  }

  read_position();
}

void reset_vars()
{
  speed = 7;
  spawn = 0;
  move = 0;

  position = 4;

  for(int i = 0; i < 7; i++) {
    rows[i] = -1;  
  }

  caught = 0;
}

void update_invaders()
{
  /* shift all rows down... */
  for (int i = 0; i < 6; i++) {
    lc.setLed(0, rows[i], i + 1, false);
    rows[i] = rows[i+1];
    lc.setLed(0, rows[i], i + 1, true);
  } 

  if (spawn >= speed) {
    /* Generate new row */ 
    int pos = random(8);
    rows[6] = pos; 
    spawn = 0;
  } else {
    /* Add blank row */  
    rows[6] = -1;
    spawn = spawn + 1;
  }

  lc.setLed(0, rows[5], 7, false);
  lc.setLed(0, rows[6], 7, true);
}

void read_position()
{
  /* Read position */
  int move_l = digitalRead(left);
  int move_r = digitalRead(right);

  if (move_l != move_r) {
    if (move_l == LOW) {
      position = position > 0 ? position - 1 : 0;  
    } else {
      position = position < 7 ? position + 1 : 7;
    }
  }

  lc.setColumn(0, 0, B00000000);
  lc.setLed(0, position, 0, true);
  
  delay(50);
}

