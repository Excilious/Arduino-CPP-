/*
  [Name]: Arduino Wild West Game
  [About]: The code required to run the Wild West game. Please ensure the pins are in the correct place
  [Date]:
    -12/12/22
  [Logs]:
*/
//Libnaries
#include <IRremote.h>
#include <Wire.h>

//LED Needed - We place this as constant as it doesnt need to be changed
const int REDLED = 2;
const int YELLOWLED = 3;
const int GREENLED = 4;
const int BLUELED = 5;
const int TotalLEDHeld = 4; //Change this if you add more or less LED;

//IR Items
const int IRSEN1 = 6;
const int IRSEN2 = 7;
const int IRSEN3 = 8;
const int IRSEN4 = 9;
IRrecv REC1(IRSEN1);
IRrecv REC2(IRSEN2);
IRrecv REC3(IRSEN3);
IRrecv REC4(IRSEN4);
decode_results Results;

//Sounds
const int PiezoSound = 10;

//GameFunctions
int Target = 0;
bool Tracking = false;
bool Debug = false;
bool IsReload = false;
int Rounds = 0;
bool GameOver = false;

//Insight
char Title = "";
char Message = "";

void Insight(){
  //Not much is being done here. This would be to help make the terminal clean.
  if (not Debug){
    return;
  }
  Serial.println("[Insight]: "+Title + Message);
}

void Sound(){
  noTone(PiezoSound);
  if (IsReload == false){
    tone(PiezoSound,1000);
    delay(200);
    tone(PiezoSound,200);
    delay(200);
    noTone(PiezoSound);
  }
  else{
    for (int i = 2; i < 1000; i++){
      tone(PiezoSound,i);
      delay(1);
    }
  }
  //delay(200);
}

void TargetLED(){
  //Not much is needed here. We just need to pick a random pin to set value to high
  int RandomTarget = random(2,6);
  //Serial.print(RandomTarget);
  digitalWrite(RandomTarget,HIGH);
  Target = RandomTarget;
}

void TargetIR(){
  Serial.print(Target);
  //We are using a switch case function to check if target can be either 1,2,3 or 4
  if (Target == 1){
    if (REC1.decode(&Results)){
          //This is where you detect the shot
          //Serial.print(Results.value,HEX);
          digitalWrite(Target,LOW);
          REC1.resume();
          Tracking = true;
        }
  }
  else if (Target == 2){
    if (REC2.decode(&Results)){
          //This is where you detect the shot
          //Serial.print(Results.value,HEX);
          digitalWrite(Target,LOW);
          REC2.resume();
          Tracking = true;
        }
  }
  else if (Target == 3){
    if (REC3.decode(&Results)){
          //This is where you detect the shot
          //Serial.print(Results.value,HEX);
          digitalWrite(Target,LOW);
          REC3.resume();
          Tracking = true;
        }
  }
   else if (Target == 4){
   if (REC4.decode(&Results)){
          //This is where you detect the shot
          //Serial.print(Results.value,HEX);
          digitalWrite(Target,LOW);
          REC4.resume();
          Tracking = true;
        }
  }
  /*
  switch(Target){
    case 2:
      if (Tracking){
        if (REC1.decode(&Results)){
          //This is where you detect the shot
          //Serial.print(Results.value,HEX);
          digitalWrite(Target,LOW);
          REC1.resume();
          Tracking = true;
        }
      }
    case 3:
      if (not Tracking){
        if (REC2.decode(&Results)){
          //Serial.print(Results.value,HEX);
          //This is where you detect the shot
          digitalWrite(Target,LOW);
          REC2.resume();
          Tracking = true;
        }
      }

    case 4:
      if (not Tracking){
        if (REC3.decode(&Results)){
          //Serial.print(Results.value,HEX);
          //This is where you detect the shot
          digitalWrite(Target,LOW);
          REC3.resume();
          Tracking = true;
        }
      }

      case 5:
      if (not Tracking){
        //Serial.print(Results.value,HEX);
        if (REC4.decode(&Results)){
          //This is where you detect the shot
          digitalWrite(Target,LOW);
          REC4.resume();
          Tracking = true;
        }
      }
  }
  */
}

void Game(){
  while (Rounds < 20 || GameOver == false){
    if (Target == 0){
      TargetLED();
      TargetIR();
    };
  }
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  //LED - We need this to represent the target
  pinMode(REDLED,OUTPUT); //Red LED
  pinMode(YELLOWLED,OUTPUT); //Yellow LED
  pinMode(GREENLED,OUTPUT); //Green LED
  pinMode(BLUELED,OUTPUT); //Blue LED
  //Sounds - Might be annoying. We can change this within settings
  pinMode(PiezoSound,OUTPUT);
  //IR Sensors
  pinMode(IRSEN1,INPUT);
  pinMode(IRSEN2,INPUT);
  pinMode(IRSEN3,INPUT);
  pinMode(IRSEN4,INPUT);
}

void loop() {
  Game();
}
