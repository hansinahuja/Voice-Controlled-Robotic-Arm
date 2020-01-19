#include<Servo.h>
Servo gripper;
Servo wrist;
Servo claw;
Servo barrel;
String s;
void setup(){
  
  s="Z";
  gripper.attach(8);
  claw.attach(9);
  wrist.attach(10);
  barrel.attach(11);
  gripper.write(140);
  claw.write(40);
  wrist.write(110);
  barrel.write(90);
  Serial.begin(115200);
}
void setNorm(){
    gripper.write(140);
    claw.write(40);
    wrist.write(110);
    barrel.write(90);
}
void loop(){
  String s;
  int data;
  if(Serial.available()>0){
    String tests=Serial.readString();
    Serial.println(tests);
    if(tests[0]=='S'||tests[0]=='Z'||tests[0]=='D'||tests[0]=='M'||tests[0]=='R')s=tests;
    else s="123";
  }
    char ch=s[0];
    switch(ch){
      case 'Z':
                setNorm();
                delay(100);
                break;
      case 'S':
                data=(s.substring(1,s.length())).toInt();
                gripper.write(data);
                delay(100);
                break;
      case 'D':
                data=(s.substring(1,s.length())).toInt();
                claw.write(data);
                delay(100);
                break;
      case 'M':
                data=(s.substring(1,s.length())).toInt();
                wrist.write(data);
                delay(100);
                break;
      case 'R':
                data=(s.substring(1,s.length())).toInt();
                barrel.write(120);
                delay(data*1000);
                barrel.write(90);
                break;
    }
  
}
