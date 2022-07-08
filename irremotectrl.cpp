#include "irremotectrl.h"

typedef struct {
    int fd;
    callBack cb;
    void *viewParam;
}Info;
Info info;
pthread_t thr1;
pthread_mutex_t mut;
int fd = -1;
int isIrCtrlStop = -1;
IrRemoteCtrl::IrRemoteCtrl()
{
    printf("IrRemoteCtrl init\n");
    isIrCtrlStop = 0;
}

int IrRemoteCtrl::start2getIrKeyVal(callBack viewFun)
{
    isIrCtrlStop = 0;
    info.fd = 0;
    info.cb = viewFun;
    if(pthread_create(&thr1,NULL,getIrKeyValLoop,&info)!=0)
    {
        printf("create thread failed!\n");
    }
    else
    {
//        pthread_mutex_lock(&mut);
    }
}

int IrRemoteCtrl::stopgetIrKeyVal()
{
     isIrCtrlStop = 1;
//     pthread_mutex_unlock(&mut);
     pthread_join(thr1,NULL);
     close(fd);
     printf("stop get IrKey Val !\n");
     sleep(1);
}

void *IrRemoteCtrl::getIrKeyValLoop(void *args)
{
    printf("getIrKeyValLoop  statrt!\n");
    Info *info = (Info *)args;
    callBack cb = info->cb;
    int i;
    int rd,mykey=-1;
    struct input_event evt[64];
    char evt_path[32];
    char pre_key[64];
    char buf[64];
    memset( evt, 0, sizeof(struct input_event) );
    memset( evt_path, 0, sizeof(evt_path) );
    memset( pre_key, 0, sizeof(pre_key));
    memset( buf, 0, sizeof(buf) );

    strncpy(evt_path, "/dev/input/event2", 32);
    printf("[%s][%d] ==========evt_path=%s \n",__func__,__LINE__,evt_path);

    if ((fd = open(evt_path, O_RDONLY)) < 0)
    {
            printf("can't open %s(%s)\n", evt_path, strerror(errno));
            goto err;
    }

    while (!isIrCtrlStop)
    {
        rd = read(fd, evt, sizeof(struct input_event) * 64);
        if (rd < (int)sizeof(struct input_event))
        {
            printf("error in reading\n");
            goto err;
        }

        for (i = 0; i < rd / sizeof(struct input_event); i++)
        {
            if (evt[i].type == EV_SYN)
            {
                ;
            }
            else if (evt[i].type == EV_MSC && (evt[i].code == MSC_RAW || evt[i].code == MSC_SCAN))
            {
                if (!evt[i].value)
                    continue;

                if(evt[i].value >> 24) // 0001 0000 0000 0000 0000 0000 0000 0x1ffffff
                {
                        printf("[IR1_down]: %d code =%d, value[%d] =%02x \n",rd / sizeof(struct input_event), evt[i].code, i,evt[i].value );
                }
                else
                {
                        printf("[IR1_up]: code =%d, value[%d] =%02x \n", evt[i].code, i,evt[i].value );
                        mykey = irKeyValJudged(evt[i]);
                        cb(&mykey);
                        usleep(5000);
                        printf("[IR1_up] end  callback\n");
                }
            }
            else
            {
                printf("[IR2]: code =%d, value =%02x \n", evt[i].code, evt[i].value );
                if (!evt[i].value)
                    continue;
            }
        }
    }
err:
    return NULL;
}

int IrRemoteCtrl::irKeyValJudged(struct input_event evt)
{
    struct input_event inputevt = evt;
    int val = inputevt.value;
    int iKey = -1;
    printf("irKeyValJudged value =%02x val == \n",evt.value ,val);
    switch (val) {
    case 0xff0000:
        iKey = WAKE_UP;
        break;
    case 0xff0001:
        iKey = MOVE_UP;
        break;
    case 0xff0002:
        iKey = DEL;
        break;
    case 0xff0004:
        iKey = MOVE_LEFT;
        break;
    case 0xff0005:
        iKey = OK;
        break;
    case 0xff0006:
        iKey = MOVE_RIGHT;
        break;
    case 0xff0008:
        iKey = SLOW_DOWN;
        break;
    case 0xff0009:
        iKey = MOVE_DOWN;
        break;
    case 0xff000a:
        iKey = SPEED_UP;
        break;
    case 0xff000c:
        iKey = 0;
        break;
    case 0xff000d:
        iKey = GO_BACK;
        break;
    case 0xff000e:
        iKey = MENU;
        break;
    case 0xff0010:
        iKey = 1;
        break;
    case 0xff0011:
        iKey = 2;
        break;
    case 0xff0012:
        iKey = 3;
        break;
    case 0xff0014:
        iKey = 4;
        break;
    case 0xff0015:
        iKey = 5;
        break;
    case 0xff0016:
        iKey = 6;
        break;
    case 0xff0018:
        iKey = 7;
        break;
    case 0xff0019:
        iKey = 8;
        break;
    case 0xff001a:
        iKey = 9;
        break;
    default:
        break;
    }
    return iKey;
}


