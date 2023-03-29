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
