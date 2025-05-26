#include <servo.h>
#include <Arduino.h>
Servo myservo;
const int in_dist = 11;
const int out_dist = in_dist + 25;
const int irled = 2;
const int threshold = 200;
const int hallpin = 3;
const int enamotor = 4;
const int throttle = 5;
const int tiltswitch = 6;
const int lowspeed = 5;
const int highspeed = 10;
long speed = 0;

bool finished = false;
int locator();
int pid(int setpoint, int measure,int P,int I,int D);
void counter();
void powercontrol();
void setup()
{
    attachInterrupt(digitalPinToInterrupt(hallpin), counter, RISING);
    pinMode(2, OUTPUT);
    myservo.attach(2);
}
void loop()
{
    static int lineloc = 0;
    lineloc = locator();
    myservo.write(pid(0, lineloc,1,0,0));
    powercontrol();
}
int locator()
{
    int prevloc = 0;
    int bright1 = 1023 - analogRead(A1);
    int bright2 = 1023 - analogRead(A2);
    int bright3 = 1023 - analogRead(A3);
    int bright4 = 1023 - analogRead(A4);
    if ((bright1 + bright2 + bright3 + bright4) > threshold)
    {
        finished = true;
        return 0;
    }
    else
    {
        int location = (bright1 * out_dist + bright2 * in_dist - bright3 * in_dist - bright4 * out_dist) / (bright1 + bright2 + bright3 + bright4);
        if (prevloc > 10 and location == 0)

            prevloc = location;
        return location;
    }
}
int pid(int setpoint, int measure,int P,int I,int D)
{
    int preverror = 0;
    int prevtime = 0;
    static int integral = 0;
    long time = millis();
    long delta = prevtime - time;
    int error = setpoint - measure;
    int proportional = error;
    integral = integral + error * delta;
    int derivative = (error - preverror) / delta;
    int result = P * proportional + I * integral + D * derivative;
    preverror = error;
    prevtime = time;
    return result;
}
void counter()
{
    static long prevt = 0;
    long time = micros();
    long deltatime = (time - prevt) * 10 ^ -6;
    speed = (1 / deltatime) * 27.018;
    prevt = time;
}
void powercontrol()
{
    int setspeed = 0;
    if (digitalRead(tiltswitch))
    {
        setspeed = lowspeed;
    }
    else
    {
        setspeed = highspeed;
    }
    if (setspeed < 1)
    {
        digitalWrite(enamotor, LOW);
    }
    else
    {
       digitalWrite(enamotor,HIGH);
    }
    
    analogWrite( throttle , pid(setspeed, speed,1,0,0));
}