#include <Arduino.h>
int in_dist = 11;
int out_dist = in_dist + 25;
int irled = 2;
int bright1base = 0;
int bright2base = 0;
int bright3base = 0;
int bright4base = 0;
int bright1 = 0;
int bright2 = 0;
int bright3 = 0;
int bright4 = 0;
int location = 0;
void setup()
{
    Serial.begin(9600);
    pinMode(2, OUTPUT);
}
void loop()
{

    bright1 = 1023 - analogRead(A1);
    bright2 = 1023 - analogRead(A2);
    bright3 = 1023 - analogRead(A3);
    bright4 = 1023 - analogRead(A4);
    
    location = (bright1 * out_dist + bright2 * in_dist - bright2 * in_dist - bright3 * out_dist) / (bright1 + bright2 + bright3 + bright4);
    Serial.print(">bright:");
    Serial.println(location);
}