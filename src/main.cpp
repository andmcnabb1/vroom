#include <servo.h>
#include <Arduino.h>
#include <NewPing.h>
Servo myservo;
const int in_dist = 11;
const int out_dist = in_dist + 25;
const int irled = 2;
const int threshold = 600;
const int stopthreshold = 670;
const int hallpin = 3;
const int enamotor = 4;
const int throttle = 5;
const int tiltswitch = 6;
const int lowspeed = 5;
const int highspeed = 10;
const int steeringoffset = 0;
const int trigPin = 6;
const int echoPin = 7;
const int buttonpin = 12;
int distance = 100;
unsigned int pingSpeed = 100;
unsigned long pingTimer = 0;
NewPing sonar(trigPin, echoPin, 600);
bool finished = false;
bool butti = false;
int locator();
int pid(int setpoint, int measure, int P, int I, int D);
// void counter();
int distancer(int pingtime);
void powercontrol(int distance);
void echoCheck();
void setup()
{
    pinMode(4, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(buttonpin, INPUT_PULLUP);
    myservo.attach(2);
    Serial.begin(9600);
    digitalWrite(3, HIGH);
    digitalWrite(4, HIGH);
    pingTimer = millis();
}
void loop()
{
    static int lineloc = 0;

    lineloc = locator();
    myservo.write(pid(0, lineloc, 7, 0, 0) + 90 + steeringoffset);
    /*if (millis() >= pingTimer)
    {
        pingTimer += pingSpeed;
        sonar.ping_timer(echoCheck);
    }*/
    //distance=100; //this deactivates the ultrasonic sensor
    Serial.println(distance);
    powercontrol(distance);
    butti = digitalRead(buttonpin);
    if (!butti)
    {
        finished = false;
    }
}
int locator()
{
    int location = 0;

    int bright1 = analogRead(A1);
    int bright2 = analogRead(A2);
    int bright3 = analogRead(A3);
    int bright4 = analogRead(A4);

    if (bright1 < threshold)
    {
        location = -2;
    }
    else if (bright2 < threshold)
    {
        location = -1;
    }
    else if (bright3 < threshold)
    {
        location = 1;
    }
    else if (bright4 < threshold)
    {
        location = 2;
    }
    else
    {
        location = 0;
    }
    if ((bright1 < stopthreshold) && (bright2 < stopthreshold) && (bright3 < stopthreshold) && (bright4 < stopthreshold))
    {
        finished = true;
        //delay(200); // use if it doesn't stop correctly
    }
    return location;
}

int pid(int setpoint, int measure, int P, int I, int D)
{
    static int result = 0;
    if (measure != 0)
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
        result = P * proportional + I * integral + D * derivative;
        preverror = error;
        prevtime = time;
    }

    return result;
}
/* void counter()
{
    static long prevt = 0;
    static bool haller = true;
    static float speed = 0;
    bool prevhal = haller;
    haller = digitalRead(hallpin);
    if (haller == false && haller != prevhal)
    {
        long time = millis();
        float deltatime = (time - prevt) * 0.001;
        speed = (1 / deltatime) * 0.27018;
        prevt = time;
    }
} */

void powercontrol(int distance)
{

    if ((!finished) && butti)
    {
        analogWrite(throttle, 47);
    }
    else
    {
        analogWrite(throttle, 0);
    }
}
/*void echoCheck()
{
    if (sonar.check_timer())
    {
        distance = sonar.ping_result / US_ROUNDTRIP_CM;
    }
} */