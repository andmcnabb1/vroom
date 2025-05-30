#include <servo.h>
#include <Arduino.h>
Servo myservo;
const int in_dist = 11;
const int out_dist = in_dist + 25;
const int irled = 2;
const int threshold = 400;
const int hallpin = 3;
const int enamotor = 4;
const int throttle = 5;
const int tiltswitch = 6;
const int lowspeed = 5;
const int highspeed = 10;
const int steeringoffset = 0;

bool finished = false;
int locator();
int pid(int setpoint, int measure, int P, int I, int D);
void counter();
void powercontrol();
void setup()
{
    pinMode(2, OUTPUT);
    myservo.attach(2);
    Serial.begin(9600);
}
void loop()
{
    static int lineloc = 0;
    lineloc = locator();
    myservo.write(pid(0, lineloc, 10, 0, 0) + 90 + steeringoffset);
    delay(10);
    powercontrol();
    counter();
}
int locator()
{
    int location = 0;

    bool bright1 = analogRead(A1) < threshold;
    bool bright2 = analogRead(A2) < threshold;
    bool bright3 = analogRead(A3) < threshold;
    bool bright4 = analogRead(A4) < threshold;

    if (bright1)
    {
        location = -2;
    }
    else if (bright2)
    {
        location = -1;
    }
    else if (bright3)
    {
        location = 1;
    }
    else if (bright4)
    {
        location = 2;
    }
    else
    {
        location = 0;
    }
    if (bright1 && bright2 && bright3 && bright4)
    {
        finished == true;
    }
    return location;
}

int pid(int setpoint, int measure, int P, int I, int D)
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
    static bool haller = true;
    static float speed = 0;
    bool prevhal = haller;
    haller = digitalRead(hallpin);
    if (haller == false && haller != prevhal)
    {
        long time = millis();
        float deltatime = (time - prevt) *0.001;
        speed = (1 / deltatime) * 0.27018;
        prevt = time;
    }
    Serial.println(speed);
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
        digitalWrite(enamotor, HIGH);
    }

    analogWrite(throttle, pid(setspeed, 0, 1, 0, 0));
}