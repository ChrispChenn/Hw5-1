#include "mbed.h"
#include "bbcar.h"

DigitalOut led1(LED1);
Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);

EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;
Thread t2;
DigitalInOut pin8(D8);

BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

int mode;

parallax_ping  ping1(pin8);

void print()
{
    mode++;
//    printf ("%d\n\r", mode);
}

void rotation() {
    t2.start(callback(&queue, &EventQueue::dispatch_forever));
    queue.call(print);
    car.goStraight(-55);
    wait_us(2000000);
    //car.stop();
    queue.call(print);
    car.goStraight(55);
    wait_us(2050000);
    car.stop();
    queue.call(print);
}

int main() {
    int temp, temp2, angle1, angle2, A, B;
    float cm[2] = {99.0, 99.0};
   // parallax_laserping  ping1(pin8);
    t.start(rotation);
    int next = 0;
    while (mode != 3) {
        switch (mode) { 
            case 1:
                temp = (float) ping1;
            //    printf ("%d\n\r", angle1);
                angle1 = car.servo0.angle; 

                if (temp < 100 && temp < cm[0] && !next) {
                    cm[0] = temp;
                    A = angle1;
                }
                else if (temp > 100)
                    next = 1;
                else if (temp < 100 && temp < cm[1] && next) {
                    cm[1] = temp;
                    B = angle1;
                }
                
            //    printf ("%f\n\r", cm[0]);
            //    printf ("%d\n\r", angle1);
                ThisThread::sleep_for(10ms);
            break;
  /*          case 2:
                temp2 = (float) ping1;
                printf ("%d\n\r", temp2);
                angle2 = car.servo0.angle;
                if (temp2 < cm[1]) {
                    cm[1] = temp2;
                    
                }
            //    printf ("%f\n\r", cm[1]);
            //    printf ("%d\n\r", angle2);
                ThisThread::sleep_for(10ms);
            break; 
            case 3:
                printf ("%f\n\r", cm[0]);
                printf ("%f\n\r", cm[1]);
            break; */
            default: 
            break;
        } 
    }
    
    printf ("d1 = %f\n\r", cm[0]);
    printf ("d2 = %f\n\r", cm[1]);
    printf ("servo angle1 : %d\n\r", A);
    printf ("servo angle2 : %d\n\r", B);
    float c;
    c=sqrt(cm[0]*cm[0] + cm[1]*cm[1] -2*cm[0]*cm[1]*cos((-B+A)/(1.65*57.29578)));
    printf ("w = %f\n\n\r", c);
    
}