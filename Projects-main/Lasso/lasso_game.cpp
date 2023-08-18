#include <simplecpp>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace simplecpp;


//movingObject.h
#ifndef _MOVINGOBJECT_INCLUDED_
#define _MOVINGOBJECT_INCLUDED_

#include <simplecpp>
#include <vector>
#include <composite.h>
#include <sprite.h>

using namespace simplecpp;

class MovingObject : public Sprite {
  vector<Sprite*> parts;
  double vx, vy;
  double ax, ay;
  bool paused;

  void initMO(double argvx, double argvy, double argax, double argay, bool argpaused=true) {
    vx=argvx; vy=argvy; ax=argax; ay=argay; paused=argpaused;
  }

public :
    bool hided = false;

 MovingObject(double argvx, double argvy, double argax, double argay, bool argpaused=true)
    : Sprite() {
    initMO(argvx, argvy, argax, argay, argpaused);
  }
 MovingObject(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : Sprite() {
   double angle_rad = angle_deg*PI/180.0;
   double argvx = speed*cos(angle_rad);
   double argvy = -speed*sin(angle_rad);
   initMO(argvx, argvy, argax, argay, argpaused);
  }
  void set_vx(double argvx) { vx = argvx; }
  void set_vy(double argvy) { vy = argvy; }
  void set_ax(double argax) { ax = argax; }
  void set_ay(double argay) { ay = argay; }
  double getXPos();
  double getYPos();
  void reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta);

  void pause() { paused = true; }
  void unpause() { paused = false; }
  bool isPaused() { return paused; }

  void addPart(Sprite* p) {
    parts.push_back(p);
  }

  void nextStep(double t);
  void getAttachedTo(MovingObject *m);

  void hide_all();
  void unhide_all();

};

#endif

//MovingObject.cpp

void MovingObject::nextStep(double t) {
  if(paused) { return; }
  //cerr << "x=" << getXPos() << ",y=" << getYPos() << endl;
  //cerr << "vx=" << vx << ",vy=" << vy << endl;
  //cerr << "ax=" << ax << ",ay=" << ay << endl;

  for(size_t i=0; i<parts.size(); i++){
    parts[i]->move(vx*t, vy*t);
  }
  vx += ax*t;
  vy += ay*t;
} // End MovingObject::nextStep()

double MovingObject::getXPos() {
  return (parts.size() > 0) ? parts[0]->getX() : -1;
}

double MovingObject::getYPos() {
  return (parts.size() > 0) ? parts[0]->getY() : -1;
}

void MovingObject::reset_all(double argx, double argy, double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) {
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(argx, argy);
  }

  double angle_rad = angle_deg*PI/180.0;
  double argvx = speed*cos(angle_rad);
  double argvy = -speed*sin(angle_rad);
  vx = argvx; vy = argvy; ax = argax; ay = argay; paused = argpaused;

} // End MovingObject::reset_all()

void MovingObject::getAttachedTo(MovingObject *m) {
  double xpos = m->getXPos();
  double ypos = m->getYPos();
  for(size_t i=0; i<parts.size(); i++){
    parts[i]->moveTo(xpos, ypos);
  }
  initMO(m->vx, m->vy, m->ax, m->ay, m->paused);
}

void MovingObject::hide_all() {
   for(size_t i=0; i<parts.size(); i++){
     parts[i]->hide();
   }
   hided=true;
}

void MovingObject::unhide_all() {
   for(size_t i=0; i<parts.size(); i++){
     parts[i]->show ();
   }
   hided = false;

}

//coin.h
#ifndef __COIN_H__
#define __COIN_H__


class Coin : public MovingObject {
public:
  double coin_start_x;
  double coin_start_y;
  double release_speed;
  double release_angle_deg;
  double coin_ax;
  double coin_ay;

  // Moving parts
  Circle coin_circle;
  Circle coin_boundary;

 public:
   bool comp_catched=false;

 Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin();
  }

  Coin(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta,bool comp) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    coin_ax = argax;
    coin_ay = argay;
    initCoin_comp();
  }

  void initCoin();
  void initCoin_comp();
  void resetCoin_t(double time);
}; // End class Coin

#endif

//bomb.h
#ifndef __BOMB_H__
#define __BOMB_H__

#define BOMB_SIZE 9
#define BOMB_SPEED 130
#define BOMB_G 30
#define BOMB_ANGLE_DEG 108

class Bomb : public MovingObject {
  double bomb_start_x;
  double bomb_start_y;
  double release_speed;
  double release_angle_deg;
  double bomb_ax;
  double bomb_ay;

  // Moving parts
  Circle bomb_circle_out;
  Circle bomb_circle_in;
  Text   bomb_text;

 public:
 Bomb(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    bomb_ax = argax;
    bomb_ay = argay;
    initBomb();
  }

  void initBomb();
  void resetBomb_t(double time);

}; // End class Bomb

#endif


//lasso.h
#ifndef __LASSO_H__
#define __LASSO_H__

//#define WINDOW_X 1200
//#define WINDOW_Y 960
#define WINDOW_X 800
#define WINDOW_Y 600

#define STEP_TIME 0.05

#define PLAY_X_START 100
#define PLAY_Y_START 0
#define PLAY_X_WIDTH (WINDOW_X-PLAY_X_START)
#define PLAY_Y_HEIGHT (WINDOW_Y-100)

#define LASSO_X_OFFSET 100
#define LASSO_Y_HEIGHT 100
#define LASSO_BAND_LENGTH LASSO_X_OFFSET
#define LASSO_THICKNESS 5

#define COIN_GAP 1

#define RELEASE_ANGLE_STEP_DEG 5
#define MIN_RELEASE_ANGLE_DEG 0
#define MAX_RELEASE_ANGLE_DEG (360-RELEASE_ANGLE_STEP_DEG)
#define INIT_RELEASE_ANGLE_DEG 45

#define RELEASE_SPEED_STEP 20
#define MIN_RELEASE_SPEED 0
#define MAX_RELEASE_SPEED 200
#define INIT_RELEASE_SPEED 100

#define COIN_SPEED 120
#define COIN_ANGLE_DEG 90

#define LASSO_G 30
#define COIN_G  30

#define LASSO_SIZE 10
#define LASSO_RADIUS 50
#define COIN_SIZE 5
#define COMP_COIN_SIZE 9


class Lasso : public MovingObject {
  double lasso_start_x;
  double lasso_start_y;
  double release_speed;
  double release_angle_deg;
  double lasso_ax;
  double lasso_ay;

  // Moving parts
  Circle lasso_circle;
  Circle lasso_loop;
  // Non-moving parts
  Line lasso_line;
  Line lasso_band;

  // State info
  bool lasso_looped;
  Coin *the_coin;
  Bomb *the_bomb;

  void initLasso();

 public:

    int num_coins;

 Lasso(double speed, double angle_deg, double argax, double argay, bool argpaused, bool rtheta) : MovingObject(speed, angle_deg, argax, argay, argpaused, rtheta) {
    release_speed = speed;
    release_angle_deg = angle_deg;
    lasso_ax = argax;
    lasso_ay = argay;
    initLasso();
  }

  void draw_lasso_band();
  void yank();
  void loopit();
  void addAngle(double angle_deg);
  void addSpeed(double speed);

  void nextStep(double t);
  void check_for_coin(Coin *coin);
  void check_for_bomb(Bomb *bomb);
  int getNumCoins() { return num_coins; }

}; // End class Lasso

#endif


//coin.h

void Coin::initCoin() {
  coin_start_x = (PLAY_X_START+WINDOW_X)/2;
  coin_start_y = PLAY_Y_HEIGHT;
  coin_circle.reset(coin_start_x, coin_start_y, COIN_SIZE);
  coin_circle.setColor(COLOR(255,255,0));
  coin_circle.setFill(true);
  addPart(&coin_circle);
}

void Coin::initCoin_comp() {
  coin_start_x = (PLAY_X_START+WINDOW_X-50)/2;
  coin_start_y = PLAY_Y_HEIGHT;

  coin_circle.reset(coin_start_x, coin_start_y, COMP_COIN_SIZE);
  coin_circle.setColor(COLOR(255,178,80));
  coin_circle.setFill(true);

  coin_boundary.reset(coin_start_x, coin_start_y, COMP_COIN_SIZE+1);
  coin_boundary.setColor(COLOR(255,0,127));
  coin_boundary.setFill(false);

  addPart(&coin_circle);
  addPart(&coin_boundary);
}

void Coin::resetCoin_t(double time) {
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = 90 - fabs(30*sine(2000*time));// randomising the release angle at various times

  coin_ax = 0;
  coin_ay = COIN_G;
  bool paused = true, rtheta = true;
  reset_all(coin_start_x, coin_start_y, coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
}


//bomb.h

void Bomb::initBomb() {
  bomb_start_x = (PLAY_X_START+WINDOW_X+100)/2;
  bomb_start_y = PLAY_Y_HEIGHT;
  bomb_circle_in.reset(bomb_start_x, bomb_start_y, BOMB_SIZE);
  bomb_circle_in.setFill(true);
  bomb_circle_out.reset(bomb_start_x, bomb_start_y,1.5*BOMB_SIZE);
  bomb_text.reset(bomb_start_x, bomb_start_y,"B");
  addPart(&bomb_circle_in);
  addPart(&bomb_circle_out);
  addPart(&bomb_text);
}

void Bomb::resetBomb_t(double time) {
  double bomb_speed = BOMB_SPEED;
  double bomb_angle_deg = 90-fabs(30*cosine(1000*time));//randomising release angle
  bomb_ax = 0;
  bomb_ay = BOMB_G;
  bool paused = true, rtheta = true;
  reset_all(bomb_start_x, bomb_start_y, bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);
}


//lasso.cpp

void Lasso::draw_lasso_band() {
  double len = (release_speed/MAX_RELEASE_SPEED)*LASSO_BAND_LENGTH;
  double arad = release_angle_deg*PI/180.0;
  double xlen = len*cos(arad);
  double ylen = len*sin(arad);
  lasso_band.reset(lasso_start_x, lasso_start_y, (lasso_start_x-xlen), (lasso_start_y+ylen));
  lasso_band.setThickness(LASSO_THICKNESS);
  lasso_band.setColor(COLOR(0,255,255));
} // End Lasso::draw_lasso_band()

void Lasso::initLasso() {
  lasso_start_x = (PLAY_X_START+LASSO_X_OFFSET);
  lasso_start_y = (PLAY_Y_HEIGHT-LASSO_Y_HEIGHT);
  lasso_circle.reset(lasso_start_x, lasso_start_y, LASSO_SIZE);
  lasso_circle.setColor(COLOR("red"));
  lasso_circle.setFill(true);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setColor(COLOR(178,102,255));
  lasso_loop.setFill(true);
  addPart(&lasso_circle);
  addPart(&lasso_loop);
  lasso_looped = false;
  the_coin = NULL;
  num_coins = 0;

  lasso_line.reset(lasso_start_x, lasso_start_y, lasso_start_x, lasso_start_y);
  lasso_line.setColor(COLOR("green"));

  lasso_band.setColor(COLOR(0,255,255));
  draw_lasso_band();

} // End Lasso::initLasso()

void Lasso::yank() {
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
  lasso_loop.reset(lasso_start_x, lasso_start_y, LASSO_SIZE/2);
  lasso_loop.setFill(true);
  lasso_looped = false;

} // End Lasso::yank()

void Lasso::loopit() {
  if(lasso_looped) { return; } // Already looped
  lasso_loop.reset(getXPos(), getYPos(), LASSO_RADIUS);
  lasso_loop.setFill(false);
  lasso_looped = true;
} // End Lasso::loopit()

void Lasso::addAngle(double angle_deg) {
  release_angle_deg += angle_deg;
  if(release_angle_deg < MIN_RELEASE_ANGLE_DEG) { release_angle_deg = MIN_RELEASE_ANGLE_DEG; }
  if(release_angle_deg > MAX_RELEASE_ANGLE_DEG) { release_angle_deg = MAX_RELEASE_ANGLE_DEG; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addAngle()

void Lasso::addSpeed(double speed) {
  release_speed += speed;
  if(release_speed < MIN_RELEASE_SPEED) { release_speed = MIN_RELEASE_SPEED; }
  if(release_speed > MAX_RELEASE_SPEED) { release_speed = MAX_RELEASE_SPEED; }
  bool paused = true, rtheta = true;
  reset_all(lasso_start_x, lasso_start_y, release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);
} // End Lasso::addSpeed()

void Lasso::nextStep(double stepTime) {
  draw_lasso_band();
  MovingObject::nextStep(stepTime);
  if(getYPos() > PLAY_Y_HEIGHT) { yank(); }
  lasso_line.reset(lasso_start_x, lasso_start_y, getXPos(), getYPos());
} // End Lasso::nextStep()

void Lasso::check_for_coin(Coin *coinPtr) {
 if(coinPtr->hided) return;
   double lasso_x = getXPos();
   double lasso_y = getYPos();
   double coin_x = coinPtr->getXPos();
   double coin_y = coinPtr->getYPos();
   double xdiff = (lasso_x - coin_x);
   double ydiff = (lasso_y - coin_y);
   double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
   if(distance <= LASSO_RADIUS) {
     the_coin = coinPtr;
     the_coin->comp_catched=true;
     the_coin->getAttachedTo(this);
    num_coins ++;
  }
} // End Lasso::check_for_coin()


void Lasso::check_for_bomb(Bomb *bombPtr) {
 if(bombPtr->hided) return;
   double lasso_x = getXPos();
   double lasso_y = getYPos();
   double bomb_x = bombPtr->getXPos();
   double bomb_y = bombPtr->getYPos();
   double xdiff = (lasso_x - bomb_x);
   double ydiff = (lasso_y - bomb_y);
   double distance = sqrt((xdiff*xdiff)+(ydiff*ydiff));
   if(distance <= LASSO_RADIUS) {
     the_bomb = bombPtr;
     the_bomb->getAttachedTo(this);
     wait(0.3);

     the_bomb->hide_all();
     for(int i=0;i<20;i++){
        Circle co(getXPos(),getYPos(),10*i);
        Circle ci(getXPos(),getYPos(),7*i);
        Circle cii(getXPos(),getYPos(),5*i);

         wait(0.04);
       }

    num_coins-- ;
 }

} // End Lasso::check_for_bomb();



// main program starts
main_program {
  initCanvas("Lasso", WINDOW_X+400, WINDOW_Y);

  int stepCount = 0;
  float stepTime = STEP_TIME;
  float runTime = -1; // sec; -ve means infinite
  float currTime = 0;

  // Draw lasso at start position
  double release_speed = INIT_RELEASE_SPEED; // m/s
  double release_angle_deg = INIT_RELEASE_ANGLE_DEG; // degrees
  double lasso_ax = 0;
  double lasso_ay = LASSO_G;
  bool paused = true;
  bool rtheta = true;
  Lasso lasso(release_speed, release_angle_deg, lasso_ax, lasso_ay, paused, rtheta);

  Line b1(0, PLAY_Y_HEIGHT, WINDOW_X+400, PLAY_Y_HEIGHT);
  b1.setColor(COLOR("blue"));
  Line b2(PLAY_X_START, 0, PLAY_X_START, WINDOW_Y);
  b2.setColor(COLOR("blue"));

  string msg("Cmd: _");
  Text charPressed(PLAY_X_START+50, PLAY_Y_HEIGHT+20, msg);
  char coinScoreStr[256];
  sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
  Text coinScore(PLAY_X_START+50, PLAY_Y_HEIGHT+50, coinScoreStr);

  //creating all objects to be catched by lasso
  paused = true; rtheta = true;
  double coin_speed = COIN_SPEED;
  double coin_angle_deg = COIN_ANGLE_DEG;
  double coin_ax = 0;
  double coin_ay = COIN_G;

  double bomb_speed = BOMB_SPEED;
  double bomb_angle_deg = BOMB_ANGLE_DEG;
  double bomb_ax = 0;
  double bomb_ay = BOMB_G;


  Coin coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  Coin coin1(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  Coin coin2(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta);
  Coin comp_coin(coin_speed, coin_angle_deg, coin_ax, coin_ay, paused, rtheta,true);
  Bomb bomb(bomb_speed, bomb_angle_deg, bomb_ax, bomb_ay, paused, rtheta);

  // After every COIN_GAP sec, make the coin jump
  double last_coin_jump_end = 0;
  double last_bomb_jump_end = 0;
  double last_comp_coin_jump_end = 0;
  double last_coin1_jump_end = 0;
  double last_coin2_jump_end = 0;

  // When t is pressed, throw lasso
  // If lasso within range, make coin stick
  // When y is pressed, yank lasso
  // When l is pressed, loop lasso
  // When q is pressed, quit

  for(;;) {
    if((runTime > 0) && (currTime > runTime)) { break; }

    XEvent e;
    bool pendingEv = checkEvent(e);
    if(pendingEv) {
      char c = charFromEvent(e);
      msg[msg.length()-1] = c;
      charPressed.setMessage(msg);
      switch(c) {
      case 't':
	lasso.unpause();
	break;
      case 'y':
	lasso.yank();
	break;
      case 'l':
	lasso.loopit();
	lasso.check_for_coin(&coin);
	lasso.check_for_bomb(&bomb);
	lasso.check_for_coin(&coin1);
	lasso.check_for_coin(&coin2);
	lasso.check_for_coin(&comp_coin);

	wait(STEP_TIME*5);
	break;
      case '[':
	if(lasso.isPaused()) { lasso.addAngle(-RELEASE_ANGLE_STEP_DEG);	}
	break;
      case ']':
	if(lasso.isPaused()) { lasso.addAngle(+RELEASE_ANGLE_STEP_DEG); }
	break;
      case '-':
	if(lasso.isPaused()) { lasso.addSpeed(-RELEASE_SPEED_STEP); }
	break;
      case '=':
	if(lasso.isPaused()) { lasso.addSpeed(+RELEASE_SPEED_STEP); }
	break;
      case 'q':
	exit(0);
      default:
	break;
      }
    }

// moving the objects by one step
    lasso.nextStep(stepTime);
    bomb.nextStep(stepTime);
    coin.nextStep(stepTime);
    coin1.nextStep(stepTime);
    coin2.nextStep(stepTime);
    comp_coin.nextStep(stepTime);

 //releasing each object after a COIN_GAP interval
    if(coin.isPaused()) {
      if((currTime-last_coin_jump_end) >= COIN_GAP) {
	coin.unpause();
      }
    }

    if(bomb.isPaused()) {
      if((currTime-last_bomb_jump_end) >= COIN_GAP) {
	bomb.unpause();
      }
    }

    if(coin1.isPaused()) {
      if((currTime-last_coin1_jump_end) >= COIN_GAP) {
	coin1.unpause();
      }
    }

    if(coin2.isPaused()) {
      if((currTime-last_coin2_jump_end) >= COIN_GAP) {
	coin2.unpause();
      }
    }

    if(comp_coin.isPaused()) {
      if((currTime-last_comp_coin_jump_end) >= COIN_GAP) {
	comp_coin.unpause();
      }
    }

//resetting the objects if they have crossed x axis

    if(coin.getYPos() > PLAY_Y_HEIGHT) {
      coin.resetCoin_t(currTime);
      last_coin_jump_end = currTime;
    }

     if(bomb.getYPos() > PLAY_Y_HEIGHT) {
      bomb.resetBomb_t(currTime);
      last_bomb_jump_end = currTime;
      bomb.unhide_all();
    }

    if(coin1.getYPos() > PLAY_Y_HEIGHT) {
      coin1.resetCoin_t(currTime+2);
      last_coin1_jump_end = currTime;
    }

    if(coin2.getYPos() > PLAY_Y_HEIGHT) {
      coin2.resetCoin_t(currTime+1);
      last_coin2_jump_end = currTime;
    }
    if(comp_coin.getYPos() > PLAY_Y_HEIGHT) {
     if(!comp_coin.comp_catched){
          lasso.num_coins--;
       Rectangle R(600,300,1200,600);
       R.setColor(COLOR("red"));
       R.setFill(1);
       wait(0.2);
    }
      comp_coin.resetCoin_t(currTime+4);
      last_comp_coin_jump_end = currTime;
      comp_coin.comp_catched=false;
    }

//updating the no:of coins message
    sprintf(coinScoreStr, "Coins: %d", lasso.getNumCoins());
    coinScore.setMessage(coinScoreStr);

    stepCount++;
    currTime += stepTime;
    wait(stepTime);
  } // End for(;;)

  wait(3);
} // End main_program
