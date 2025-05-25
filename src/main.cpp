#include <servo.h>
#include <Arduino.h>
Servo myservo;
const int in_dist = 11;
const int out_dist = in_dist + 25;
const int irled = 2;
const int threshold = 200;
const int hallpin = 3;
int preverror=0;
int integral=0;
int prevtime=0;
int delta=0;
long speed=0;

bool finished = false; 
int locator();
void setup()
{
    attachInterrupt(digitalPinToInterrupt(hallpin), counter, RISING);
    pinMode(2, OUTPUT);
    myservo.attach(2);
}
void loop()
{
static int lineloc=0;
lineloc=locator();
myservo.write(pid(0, lineloc));
}
int locator()
{
    int prevloc=0;
    int bright1 = 1023 - analogRead(A1);
    int bright2 = 1023 - analogRead(A2);
    int bright3 = 1023 - analogRead(A3);
    int bright4 = 1023 - analogRead(A4);
    if ((bright1 + bright2 + bright3 + bright4) > threshold)
    {
        finished = true;
    }
    else
    {
        int location = (bright1 * out_dist + bright2 * in_dist - bright3 * in_dist - bright4 * out_dist) / (bright1 + bright2 + bright3 + bright4);
        if (prevloc>10 and location==0)
        
        
        prevloc=location;
        return location;
    }
}
int pid(int setpoint,int measure)
{
    const int P=1;
    const int I=0;
    const int D=0;
    int error=setpoint-measure;
    int proportional=error;
    integral=integral+error*delta;
    int derivative=(error-preverror)/delta;
    int result=P*proportional+I*integral+D*derivative;
    preverror=error;
    return result;
} 
void counter()
{
    static long prevt =0;
    long time =micros();
    long deltatime =(time-prevt)*10^-6;
    speed=(1/deltatime)*27.018;
    prevt=time;

}
