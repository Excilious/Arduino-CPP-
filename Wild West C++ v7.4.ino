/*
  [𝗔𝘂𝘁𝗵𝗼𝗿]: Excilious (For NEA)
  [𝗔𝗯𝘀𝘁𝗿𝗮𝗰𝘁𝗶𝗼𝗻]: Code to run the Wild West game circuit.
  [𝗟𝗼𝗴𝘀]:
  {
    30.03.23 - Refreshed code and implemented display module.
  }
  [𝗡𝗼𝘁𝗲𝘀]:
  {
    𝗧𝗼𝗱𝗼 - Rewrite sound module. Attempt to fix any vunerabilities. Improve upon display code.
    𝗧𝗼𝗱𝗼 - Create a multithreaded task within the code. You could use the <Coroutines> library to help. You could also use <BLINKWITHOUTDELAY> to iterate through different 
    modules of the code (However, these methods would place a strain towards the arduino as arduino would use a singlethreaded processor + the limitations of memory)
    𝗧𝗼𝗱𝗼 - Need to finish off main module. Would need to test the game.
    𝗪𝗮𝗿𝗻𝗶𝗻𝗴 - Code is untested. Furthermore, there may be some delay within the game as the arduino would need to fire signals yet recieve signals at the same time! (Tedious).
  }
*/

//Libraries//
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>
#include <Wire.h>



//Configurations//
struct 
{
  int Velocity            = 1000;
  int ReloadDelay         = 1000; 
  int TotalScore          = 0;
  int DisplayDuration     = 0;
  int TotalDuration       = 0;
  int DisplayRefresh      = 200;
  bool PlayerWonEvent     = false;
  bool TargetActive       = false;
  bool RevolverEventFired = false;
  bool ReserveDisplay     = false;
  uint32_t RandData       = 0xFF609F;
  uint8_t LengthData      = 32;
  char DisplayMode        = "NONDEFINED";
  char DisplayLineMode    = "NONDEFINED";
} GameVariables;

struct
{} CacheStruct;

int CurrentPin;
int PiezoPin = NULL;
bool ActiveScan = false;
char DisplayCache[16]; //16 maximum bits.

//PinLayout
int DataPins[4][4] = {
  {2,3,4,5},
  {6,7,8,9}
};


LiquidCrystal_I2C Display(0x27, 16, 2);

//RecieverAccess//
IRsend Sender;
IRrecv RemoteSensor(DataPins[1][0]);


//Functions//
class TargetGame
{
  public:
    TargetGame::Display(char Text)
    {
      Display.clear();
      if (GameVariables.DisplayMode == "NONDEFINED" & GameVariables.DisplayDuration == NULL)
      {
        //We would also need to assume that the maximum display duration would also be NULL;
        GameVariables.CurrentDisplayOrder = "STATIC";
        Display.print(Text);
      }
      else if (GameVariables.DisplayMode == "DYNAMIC")
      {
        for (int DisplayTick = GameVariables.DisplayDuration; DisplayTick >= GameVariables.TotalDuration; DisplayTick--)
        {
          //We would assume that the value needed to show would not exceed 2 digits.
          sprintf(DisplayCache,Text+" [%0.2d]",DisplayTick)
          Display.print(DisplayCache);
          delay(GameVariables.DisplayRefresh);
        }
      }
    }

    TargetGame::Sound()
    {
      
    }

    TargetGame::Initilise()
    {
      /* A one time intilisation sequance in which would set up
      the arduino per reset of said arduino */
      CacheStruct = GameVariables;
      TargetGame SelfObject;
      SelfObject.Main();
    }

    TargetGame::Main()
    {
      if (not GameVariables.TargetActive)
      {
      //We declare the object after we checked if the process is used. Memory efficant!
      TargetGame SelfObject;
      SelfObject.SelectRandomPin();
      SelfObject.ScanTargets();
      }
    }

    TargetGame::EndConditions()
    {
      if (GameVariables.PlayerWonEvent)
      {
        Serial.print("Player has won the game.");
      }
      else
      {
        Serial.print("Player did not win the game!")
      }
    }

    TargetGame::ScanLaserInput()
    {
      if (not GameVariables.RevolverEventFired){
        GameVariables.RevolverEventFired = true;
        Sender.sendNEC(GameVariables.RandData,GameVariables.LengthData);
        //Add Sound!
        delay(GameVariables.ReloadDelay);
        GameVariables.RevolverEventFired = false;
      }
    }

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
      switch(CurrentPin)
      {
        case 2:
          //Record the shot.
          while (not ActiveScan)
          {
            if (RemoteSensor.decode())
            {
              ActiveScan = true;
              digitalWrite(CurrentPin,LOW);
              delay(GameVariables.Velocity);
              RemoteSensor.resume();
            }
          }

        case 3:
          //Record the shot.
          while (not ActiveScan)
          {
            if (RemoteSensor.decode())
            {
              ActiveScan = true;
              digitalWrite(CurrentPin,LOW);
              delay(GameVariables.Velocity);
              RemoteSensor.resume();
            }
          }
      }
    }
};


//DefaultInit//
void setup() {
  TargetGame NewObject;
  Serial.begin(9600);
  randomSeed(analogRead(random(1,math.fmax())));
  pinMode(DataPins[0][0],OUTPUT);
  pinMode(DataPins[0][1],OUTPUT);
  pinMode(PiezoPin,OUTPUT);
  Display.init();
  Display.backlight();
  Display.clear();
  RemoteSensor.enableIRIn();
  NewObject.SelectRandomPin();
}

void loop() {
  // put your main code here, to run repeatedly:
}
