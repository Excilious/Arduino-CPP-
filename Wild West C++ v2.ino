/*
  Copyright (C) 2021 Excilious
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
  randomSeed(analogRead(0));
  int RandomTarget = random(2,6);
  //Serial.print(RandomTarget);
  digitalWrite(RandomTarget,HIGH);
  Target = RandomTarget;
}

void TargetIR(){
  switch(Target){
    case 2:
      Serial.print("A1");
      while (not Tracking){
        if (REC1.decode()){
          Tracking = true;
          Serial.println("PIN1");
          Serial.println(REC1.decodedIRData.decodedRawData,HEX);
          delay(1500);
          REC1.resume();
          return;
        }
      }
      //break;
    case 3:
      Serial.print("A2");
      while (not Tracking){
        if (REC2.decode()){
          Tracking = true;
          Serial.println("PIN2");
          Serial.println(REC2.decodedIRData.decodedRawData,HEX);
          delay(1500);
          REC2.resume();
          return;
        }
      }
      //break;
    case 4:
      Serial.print("A3");
      while (not Tracking){
        if (REC3.decode()){
          Tracking = true;
          Serial.println("PIN3");
          Serial.println(REC3.decodedIRData.decodedRawData,HEX);
          delay(1500);
          REC3.resume();
          return;
        }
      }
      //break;
    case 5:
      Serial.print("A4");
      while (not Tracking){
        if (REC4.decode()){
          Tracking = true;
          Serial.println("PIN4");
          Serial.println(REC4.decodedIRData.decodedRawData,HEX);
          delay(1500);
          REC4.resume();
          return;
        }
      }
      //break;
    default:
      Serial.print("None");      
  }
}

void Game(){
  while (Rounds < 20 || Tracking == false){
    if (Target == 0){
      TargetLED();
      TargetIR();
      Tracking = false;
      digitalWrite(Target,LOW);
      Sound();
      delay(200);
      IsReload = true;
      Sound();
      IsReload = false;
      noTone(PiezoSound);
      Target = 0;
      Serial.print("Aefertyhtrfujdtyfjrdtudrtfh");
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
  REC1.enableIRIn();
  REC2.enableIRIn();
  REC3.enableIRIn();
  REC4.enableIRIn();
}

void loop() {
  Game();
}
