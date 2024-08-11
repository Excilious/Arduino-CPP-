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
/*
  [𝗔𝘂𝘁𝗵𝗼𝗿]: Excilious (For NEA)
  [𝗔𝗯𝘀𝘁𝗿𝗮𝗰𝘁𝗶𝗼𝗻]: Code to run the Wild West game circuit.
  [𝗟𝗼𝗴𝘀]:
  {
    30.03.23 - Refreshed code and implemented display module.
    24.04.23 - Added multithreading functionality using millis(). Systems can now run at different times rather than relying on the last component to be activated.
  }
  [𝗡𝗼𝘁𝗲𝘀]:
  {
    𝗧𝗼𝗱𝗼 - Rewrite sound module. Attempt to fix any vunerabilities. Improve upon display code.
    𝗧𝗼𝗱𝗼 - Need to finish off main module. Would need to test the game. Also the IR sensor may need to be fixed.
    𝗪𝗮𝗿𝗻𝗶𝗻𝗴 - Code is untested. Furthermore, there may be some delay within the game as the arduino would need to fire signals yet recieve signals at the same time! (Tedious).
  }
*/

//Libraries//
#include <LiquidCrystal_I2C.h>
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
  int ButtonPin           = 10;
  int BlueEventInterval   = 2500;//These would be in order of priority within the system
  int YellowEventInterval = 5000;// 1000 = Highest Priority | 2500 = Lowest Priority
  int ButtonEventInterval = 1000;
  bool BlueEventState     = LOW;
  bool YellowEventState   = LOW;
  bool ButtonEventState   = LOW;
  bool PlayerWonEvent     = false;
  bool TargetActive       = false;
  bool RevolverEventFired = false;
  bool ReserveDisplay     = false;
  bool DebugMode          = false;
  uint32_t RandData       = 0xFF609F;
  uint8_t LengthData      = 32;
  char DisplayMode        = "NONDEFINED";
  char DisplayLineMode    = "NONDEFINED";
  char ModePriority       = "PLAY";
  char MenuEntered        = "DEFAULT";
  char GameplayPriority   = "EASY";
} GameVariables;

int ButtonState = 0;
int PrevButtonState = LOW;

int CurrentPin;
int PiezoPin = NULL;
int ButtonIdleTime = 0;
int ButtonReleaseTime = 0;
int ButtonHoldTime = 0;
int ButtonActionTime = 0;
int ButtonMultiplier = 0;
bool Debounce = false;
bool ActiveScan = false;
char DisplayCache[16]; //16 maximum bits.
unsigned long PreviousClockYellow = 0;
unsigned long PreviousClockBlue = 0;
unsigned long PreviousClockRevolver = 0;
int ClockFrequancy = LED_BUILTIN;

//PinLayout
int DataPins[4][4] = {
  //Within format ROW1[LED1,LED2,LED3] | ROW2[IR1,IR2,IR3]
  {5,6},
  {4,7}
};


LiquidCrystal_I2C Display(0x27, 16, 2);

//RecieverAccess//


//Functions//
class TargetGame
{
  public:

    TargetGame::Menu()
    { 
      Display.clear();
      if (GameVariables.MenuEntered == "DEFAULT"){
        Display.print("   Wild West!  ");
        Display.setCursor(0,1);
        if (GameVariables.ModePriority == "PLAY")
        {
          Display.print("[Play] ◄ [Mode]");
        }
        else if (GameVariables.ModePriority == "MODE")
        {
          Display.print("[Play] [Mode] ◄");
        }
      }
      else if (GameVariables.MenuEntered == "MODE") 
      {
        Display.print("   Choose Mode.  ");
        Display.setCursor(0,1);
        if (GameVariables.GameplayPriority == "EASY")
        {
          Display.print("[EASY] ◄ [HARD]");
        }
        else if (GameVariables.GameplayPriority == "HARD")
        {
          Display.print("[EASY] [HARD] ◄");
        }
      }
    }

    TargetGame::CheckMenuInput()
    {
      /*
      We would use the revolvers input and time how long the player presses the button
      */
      TargetGame SelfObject;
      if (digitalRead(GameVariables.ButtonPin) == HIGH)
      {
        if (ButtonMultiplier >= 2)
        {
          //We check if the button is pressed twice quickly.
          if (GameVariables.MenuEntered != "DEFAULT")
          {
            GameVariables.MenuEntered = "DEFAULT";
          }
          ButtonMultiplier = 0;
        }
        ButtonActionTime = millis();
        ButtonIdleTime = ButtonActionTime - ButtonReleaseTime;
        if (ButtonIdleTime >= 1000)
        {
          //The button was idle for more than half a second.Reset the multiplier.
          ButtonMultiplier = 0;
        } 
      }
      else
      {
        ButtonReleaseTime = millis();
        ButtonHoldTime =  ButtonReleaseTime - ButtonActionTime;
        if (ButtonHoldTime >= 500 && ButtonHoldTime < 1000)
        {
          //The button would be pressed for half a second; we register that as a menu input.
          ButtonMultiplier += 1;
          if (GameVariables.ModePriority == "PLAY")
          {
            GameVariables.ModePriority = "MODE";
          }
          else if (GameVariables.ModePriority == "MODE")
          {
            GameVariables.ModePriority = "PLAY";
          }
          SelfObject.Menu();
        }
        if (ButtonHoldTime >= 1000)
        {
          //If the user is holding the button for more than half a second; we register that as a choice input.
          if (GameVariables.MenuEntered == "DEFAULT")
          {
            GameVariables.MenuEntered = "MODE";
          }
          else if (GameVariables.MenuEntered == "MODE"){
            
          }

        }
      }
    }

    TargetGame::BlueIntervalTarget()
    {
      unsigned long CurrentClock = millis();
      if (CurrentClock - PreviousClockBlue >= GameVariables.BlueEventInterval)
      {
        PreviousClockBlue = CurrentClock;
        if (GameVariables.DebugMode)//Going to use debug mode to show the functionality of the multithreading.
        {
          if (GameVariables.BlueEventState == LOW)
          {
            GameVariables.BlueEventState = HIGH;
          }
          else
          {
            GameVariables.BlueEventState = LOW;
          }
          digitalWrite(4,GameVariables.BlueEventState);
        }
      }
    }

    TargetGame::YellowIntervalTarget()
    {
      //We can now run the targets at different intervals to get multitasking on arduino.
      unsigned long CurrentClock = millis();
      if (CurrentClock - PreviousClockYellow >= GameVariables.YellowEventInterval)
      {
        if (GameVariables.DebugMode)
        {
          PreviousClockYellow = CurrentClock;
          if (GameVariables.YellowEventState == LOW)
          {
            GameVariables.YellowEventState = HIGH;
          }
          else
          {
            GameVariables.YellowEventState = LOW;
          }
          digitalWrite(5,GameVariables.YellowEventState);
        }
      }
    }

    TargetGame::RevolverIntervalTarget()
    {
      unsigned long CurrentClock = millis();
      if (CurrentClock - PreviousClockRevolver >= GameVariables.ButtonEventInterval)
      {
        PreviousClockRevolver = CurrentClock;
        ButtonState = digitalRead(GameVariables.ButtonPin);
        if ((ButtonState != PrevButtonState))
        {
          if (not Debounce)
          {
            Debounce = true;
            Serial.println("Revolver Event Fired.");
            delay(200);
            Debounce = false;
          }
        }
        PrevButtonState = ButtonState;
      }
    }

    TargetGame::Sound()
    {
      return;
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
        Serial.print("Player did not win the game!");
      }
    }

    TargetGame::SelectRandomPin()
    {
      randomSeed(analogRead(random(1,1000000)));
      //int Length = TargetGame::GetListSize(DataPins);
      //int Random = random(1,Length);
      //int SelectedPin;
      //Serial.print(SelectedPin-1);
      digitalWrite(5,HIGH);
      delay(1000);
      digitalWrite(5,LOW);
      //Serial.print(SelectedPin);
      CurrentPin = 5;
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
      /*
      Used to use a library 'IRremote.h' to access the IR sensor.
      This would create too many problems and would be useless as we are
      currently not reading the data. Referring the data as HIGH/LOW inputs.
      */
      {
        case 5:
          //Record the shot.
          while (not ActiveScan)
          {
            if (digitalRead(DataPins[1][0]) == 1 && GameVariables.RevolverEventFired)
            {
              Serial.print("Fired!");
            }
          }

        case 6:
          //Record the shot.
          while (not ActiveScan)
          {
            if (digitalRead(DataPins[1][0]) == 1)
            {
              ActiveScan = true;
              digitalWrite(CurrentPin,LOW);
              delay(GameVariables.Velocity);
            }
          }
      }
    }
};

void setup() {
  TargetGame NewObject; //Accessing the public class
  Serial.begin(9600); 
  randomSeed(analogRead(random(1,1000000)));
  //[Re-edited]: IR sensors would not work unless being referanced as "input devices". 
  //LEDs
  pinMode(DataPins[0][0],OUTPUT);
  pinMode(DataPins[0][1],OUTPUT);
  //IRs
  pinMode(DataPins[1][0],INPUT);
  pinMode(DataPins[1][1],INPUT);
  pinMode(10,INPUT);
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  //pinMode(PiezoPin,OUTPUT);
  Display.init();
  Display.backlight();
  Display.clear();
  digitalWrite(5,HIGH);
  digitalWrite(4,HIGH);
}

void loop() {
  TargetGame SelfObject;
  SelfObject.YellowIntervalTarget();
  SelfObject.BlueIntervalTarget();
  SelfObject.RevolverIntervalTarget();
}
