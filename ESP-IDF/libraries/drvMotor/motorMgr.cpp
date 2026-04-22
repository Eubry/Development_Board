#include "motorMgr.h"

motMgr::motMgr(pinL293D leftMotor, pinL293D rightMotor){
    driver = "L293D";
    pMot.l = leftMotor;
    pMot.r = rightMotor;
}
motMgr::motMgr(pindrv8833 leftMotor, pindrv8833 rightMotor) {
    pMotDrv.l = leftMotor;
    pMotDrv.r = rightMotor;
    driver="drv8833";
}
void motMgr::begin() {
    if(driver == "l293d"){
        setMotpins(pMot.l,'l');
        setMotpins(pMot.r,'r');
    } else if(driver == "drv8833"){
        setMotpins(pMotDrv.l,'l');
        setMotpins(pMotDrv.r,'r');
    }
}
void motMgr::setMotpins(pinL293D pinMot, char side) {
    pin.pwmPin(std::string("en")+side, pinMot.en);
    pin.digitalPin(std::string("inA")+side, pinMot.inA, GPIO_MODE_OUTPUT);
    pin.digitalPin(std::string("inB")+side, pinMot.inB, GPIO_MODE_OUTPUT);
}
void motMgr::setMotpins(pindrv8833 pinMot, char side) {
    pin.pwmPin(std::string("inA")+side, pinMot.inA);
    pin.pwmPin(std::string("inB")+side, pinMot.inB);
}
void motMgr::moveSide(char side,pinL293D pMot, int16_t speed) {
  if (speed > 0) {
    pin.setPwmDutyPercent(std::string("en")+side, speed);
    pin.digitalWrite(std::string("inA")+side, HIGH);
    pin.digitalWrite(std::string("inB")+side, LOW);
    //analogWrite(pMot.en, speed);
  } else if (speed < 0) {
    //analogWrite(pMot.en, -speed);
    pin.digitalWrite(std::string("inA")+side, LOW);
    pin.digitalWrite(std::string("inB")+side, HIGH);
  } else {
    analogWrite(pMot.en, 0);
    digitalWrite(pMot.inA, LOW);
    digitalWrite(pMot.inB, LOW);
  }
}
void motMgr::moveSide(char side,pindrv8833 pinMot, int16_t speed) {
  if (speed > 0) {
    analogWrite(pinMot.inA, speed);
    digitalWrite(pinMot.inB, LOW);
  } else if (speed < 0) {
    digitalWrite(pinMot.inA, LOW);
    analogWrite(pinMot.inB, -speed);
  } else {
    digitalWrite(pinMot.inA, LOW);
    digitalWrite(pinMot.inB, LOW);
  }
}
void motMgr::setSpeed(char side,pinL293D pinMot,state &speed, int16_t speedValue){
    speed.curr = speedValue;
    if(speed.curr != speed.prev){
        speed.prev = speed.curr;
        moveSide(side, pinMot, speed.curr);
    }
}
void motMgr::setSpeed(char side,pindrv8833 pinMot,state &speed, int16_t speedValue){
    speed.curr = speedValue;
    if(speed.curr != speed.prev){
        speed.prev = speed.curr;
        moveSide(side, pinMot, speed.curr);
    }
}
void motMgr::move(int16_t speedL, int16_t speedR) {
    if(driver == "l293d"){
        setSpeed('l', pMot.l, speedStat.l, speedL);
        setSpeed('r', pMot.r, speedStat.r, speedR);
    }else if(driver == "drv8833"){
        setSpeed('l', pMotDrv.l, speedStat.l, speedL);
        setSpeed('r', pMotDrv.r, speedStat.r, speedR);
    }
}