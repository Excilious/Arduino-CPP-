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

#include <Adafruit_LiquidCrystal.h>
#include <Servo.h>

Adafruit_LiquidCrystal lcd_1(0);

//Level system + speed
const int MAX_LEVEL = 100;
int sequence = 0;
int your_sequence = 0;
int level = 1;
int velocity = 1000;
//Rounds
int currentround = 0;
int maxround = 5;
int loadround = 0;
//Servo
Servo AServo;

//Setup
void setup() 
{
  	lcd_1.begin(16, 2);
  	lcd_1.setBacklight(1);
  	Serial.begin(9600);
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	pinMode(A3, INPUT);

	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);

	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);
  	AServo.attach(6, 500, 2500);
}

void display()
{
  	lcd_1.begin(16, 2);
  	lcd_1.setBacklight(1);
	lcd_1.print("hello world");

}

//Check what to do
void loop()
{	
	if (level == 1)
		generate_sequence();
	if (digitalRead(A4) == LOW || level != 1 & currentround < maxround) 
	{
		show_sequence();    
		get_sequence();
    }
  	else if (currentround >= maxround)
    {
    	AServo.write(0);
    }
}

//Show the sequance
void show_sequence()
{
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);

	for (int i = 0; i < level; i++)
	{
		digitalWrite(sequence, HIGH);
		delay(velocity);
	}
}

//Get the sequance from the player
void get_sequence()
{
	int flag = 0; 
	for (int i = 0; i < level; i++)
	{
		flag = 0;
		while(flag == 0 & loadround <= 10)
		{
          	if (loadround >= 10){
          		return;
          	}
			if (digitalRead(A0) == LOW)
			{
				//digitalWrite(5, HIGH);
				your_sequence = 5;
				flag = 1;
              	loadround += 1;
				delay(200);
				if (your_sequence != sequence)
				{
					wrong_sequence();
					return;
				}
              	int sequence = 0;
              	int your_sequence= 0;
				digitalWrite(5, LOW);
			}

			if (digitalRead(A1) == LOW)
			{
				//digitalWrite(4, HIGH);
				your_sequence = 4;
				flag = 1;
              	loadround += 1;
				delay(200);
				if (your_sequence != sequence)
				{
					wrong_sequence();
					return;
				}
              	int sequence = 0;
              	int your_sequence= 0;
				digitalWrite(4, LOW);
			}

			if (digitalRead(A2) == LOW)
			{
				//digitalWrite(3, HIGH);
				your_sequence = 3;
				flag = 1;
              	loadround += 1;
				delay(200);
				if (your_sequence != sequence)
				{
					wrong_sequence();
					return;
				}
              	int sequence = 0;
              	int your_sequence= 0;
				digitalWrite(3, LOW);
			}

			if (digitalRead(A3) == LOW)
			{
				//digitalWrite(2, HIGH);
				your_sequence = 2;
				flag = 1;
              	loadround += 1;
				delay(200);
				if (your_sequence != sequence)
				{
					wrong_sequence();
					return;
				}
              	int sequence = 0;
              	int your_sequence= 0;
				digitalWrite(2, LOW);
			}
		}
      Serial.println(loadround);
      generate_sequence();
  	  show_sequence();    
	  get_sequence();
	}
Serial.print("Won");
right_sequence();
return;
}

//Generate and add to current sequance
void generate_sequence()
{
	randomSeed(millis());
	sequence = random(2,6);
}

//If wrong
void wrong_sequence()
{
	for (int i = 0; i < 3; i++)
	{
		digitalWrite(2, HIGH);
		digitalWrite(3, HIGH);
		digitalWrite(4, HIGH);
		digitalWrite(5, HIGH);
		delay(250);
		digitalWrite(2, LOW);
		digitalWrite(3, LOW);
		digitalWrite(4, LOW);
		digitalWrite(5, LOW);
		delay(250);
	}
	level = 1;
	velocity = 1000;
  	currentround = 0;
}

//if right
void right_sequence()
{
  	
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);
	delay(250);

	digitalWrite(2, HIGH);
	digitalWrite(3, HIGH);
	digitalWrite(4, HIGH);
	digitalWrite(5, HIGH);
	delay(500);
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
	digitalWrite(4, LOW);
	digitalWrite(5, LOW);
	delay(500);

	velocity -= 50;
  	currentround = 1;
  	AServo.write(0);
}
