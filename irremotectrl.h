#ifndef IRREMOTECTRL_H
#define IRREMOTECTRL_H
#include <linux/input.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include "config.h"

enum KEY_RELEASE_VAL{
    ZERO = 0,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NIGHT,
    GO_BACK,
    MENU,
    SLOW_DOWN,
    MOVE_DOWN,
    SPEED_UP,
    MOVE_LEFT,
    OK,
    MOVE_RIGHT,
    WAKE_UP,
    MOVE_UP,
    DEL
};

typedef void (*callBack)(int *pout/*,void *viewParam*/);

class IrRemoteCtrl//遥控器
{
public:
    IrRemoteCtrl();
    int key;
    static void *getIrKeyValLoop(void  *args);
    int start2getIrKeyVal(callBack viewFun/*, void *viewParam*/);
    int stopgetIrKeyVal();
private:
    static int irKeyValJudged(input_event evt);
};

#endif // IRREMOTECTRL_H
