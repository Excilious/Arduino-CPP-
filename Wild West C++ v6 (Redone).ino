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
*/

#include <IRremote.h>

int CurrentPin;
bool ActiveScan = false;

int DataPins[4][4] = {
  {2,3,4,5},
  {6,7,8,9}
};

IRrecv RemoteSensor2(6);

class TargetGame
{
  public:
    TargetGame::SelectRandomPin()
    {
      int Length = TargetGame::GetListSize(DataPins);
      Serial.print(Length);
      int SelectedPin = random(2,Length+2);
      digitalWrite(SelectedPin,HIGH);
      Serial.print(SelectedPin);
      CurrentPin = SelectedPin;
    };

    TargetGame::GetListSize(int Array)
    {
      int Count = 0;
      Count = sizeof(Array);
      return Count;
    };

    TargetGame::ScanTargets()
    {
      

    }
};

void setup() {
  TargetGame NewObject;
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);
  // put your setup code here, to run once:
  Serial.begin(9600);
  RemoteSensor2.enableIRIn();
  NewObject.SelectRandomPin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
