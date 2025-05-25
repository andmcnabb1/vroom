#include <servo.h>
#include <Arduino.h>
Servo myservo;
const int in_dist = 11;
const int out_dist = in_dist + 25;
const int irled = 2;
const int threshold = 200;
int preverror=0;
int integral=0;
int prevtime=0;
int delta=0;
int bright1base = 0;
int bright2base = 0;
int bright3base = 0;
int bright4base = 0;
int bright1 = 0;
int bright2 = 0;
int bright3 = 0;
int bright4 = 0;
bool finished = false;
int locator();
void setup()
{
    Serial.begin(9600);
    pinMode(2, OUTPUT);
    myservo.attach(2);
}
void loop()
{
}
int locator()
{

    bright1 = 1023 - analogRead(A1);
    bright2 = 1023 - analogRead(A2);
    bright3 = 1023 - analogRead(A3);
    bright4 = 1023 - analogRead(A4);
    if ((bright1 + bright2 + bright3 + bright4) > threshold)
    {
        finished = true;
    }
    else
    {
        int location = (bright1 * out_dist + bright2 * in_dist - bright3 * in_dist - bright4 * out_dist) / (bright1 + bright2 + bright3 + bright4);
        return location;
    }
}
int pid(int setpoint,int measure)
{
    const int P=1;
    const int I=1;
    const int D=1;

    int error=setpoint-measure;
    int proportional=error;
    integral=integral+error*delta;
    int derivative=(error-preverror)/delta;
    int result=P*proportional+I*integral+D*derivative;
    preverror=error;
    return result;
} 