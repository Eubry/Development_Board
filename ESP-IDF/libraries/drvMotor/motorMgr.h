#pragma once
#include <string>
#include "pinManager.h"
struct speedValue{
  int l=0;
  int r=0;
  speedValue(int speedL=0, int speedR=0): l(speedL), r(speedR) {}
};
class motMgr{
    public:
        struct pinL293D{
            int en;
            int inA;
            int inB;
        };
        struct pindrv8833{
            int inA;
            int inB;
        };
        struct l293d{
            pinL293D l;
            pinL293D r;
        };
        struct drv8833{
            pindrv8833 l;
            pindrv8833 r;
        };
        motMgr(pinL293D leftMotor, pinL293D rightMotor);
        motMgr(pindrv8833 leftMotor, pindrv8833 rightMotor);
        void begin();
        void move(int16_t speedL, int16_t speedR);
    private:
        pinManager pin;
        std::string driver = "-";
        struct state{
            int prev=0;
            int curr=0;
        };
        struct stateSpeed{
            state l;
            state r;
        };
        stateSpeed speedStat;
        void setMotpins(pinL293D pinMot, char side);
        void setMotpins(pindrv8833 pinMot, char side);
        void setSpeed(char side,pinL293D pMot,state &speed, int16_t speedValue);
        void setSpeed(char side,pindrv8833 pMot,state &speed, int16_t speedValue);
        void moveSide(char side,pinL293D pMot, int16_t speed);
        void moveSide(char side,pindrv8833 pMot, int16_t speed);
        l293d pMot;
        drv8833 pMotDrv;
};