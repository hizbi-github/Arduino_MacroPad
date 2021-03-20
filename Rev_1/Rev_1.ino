
///// NO DEBOUNCING ISSUE WHATSOEVER /////
///// ABLE TO PRESS KEYS SIMULTANEOUSLY /////
///// millis() IS USED INSTEAD OF delay() /////
///// MACROPAD CONTAINS: /////
///// 3 ACTION BUTTONS, 1 LAYER-SELECTION BUTTON /////
///// 3 LEDs FOR SHOWING THE SELECTED LAYER /////
///// 1 VOLUME CONTROL KNOB /////
///// 1 SPST SWITCH FOR SWITCHING DISPLAYS /////


///// GitHub: @Hizbi-K /////



#include <Arduino.h>
#include "HID-Project.h"



void macroKeys();
void mediaKeys();
void textKeys();

void volumeCtrlPot();

void monitorSwitch();



int buttonMacroSelect = 2;
bool buttonstate_S = HIGH;
bool keyPressed_S = false;

int button_1 = 3;
bool buttonState_1 = HIGH;
bool keyPressed_1 = false;

int button_2 = 4;
bool buttonState_2 = HIGH;
bool keyPressed_2 = false;

int button_3 = 5;
bool buttonState_3 = HIGH;
bool keyPressed_3 = false;


int led_1 = 6;
int led_2 = 7;
int led_3 = 8;


int potPin = A0;
float currentPotVal = 0;
float previousPotVal = 0;


int monSwitch_1 = 14;
bool stateMon_1 = HIGH;
bool monPressed_1 = true;     /// This is set to 'true' to not detect the initial position of the toggle switch.
bool keyPressed_M1 = false;

int monSwitch_2 = 15;
bool stateMon_2 = HIGH;
bool monPressed_2 = false;
bool keyPressed_M2 = false;


unsigned long currentMillis = 0;
unsigned long previousMillis_S = 0;
unsigned long previousMillis_Def = 0;
unsigned long previousMillis_1 = 0;
unsigned long previousMillis_2 = 0;
unsigned long previousMillis_3 = 0;
unsigned long previousMillis_M_1 = 0;
unsigned long previousMillis_M_2 = 0;
unsigned long debounceInterval = 100;       /// Using two seperate 'debounces' as the 'button_3' (specifically) sent two pulses with an interval 
unsigned long debounceInterval_M = 350;     /// greater than 100ms, while to properly capture the toggle switch's input, an interval greater than atleast
                                            /// 140ms was required. 

int layers = 1;



void setup ()
{
  
  pinMode(buttonMacroSelect, INPUT_PULLUP);

  pinMode(button_1, INPUT_PULLUP);
  pinMode(button_2, INPUT_PULLUP); 
  pinMode(button_3, INPUT_PULLUP);

  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);

  pinMode(potPin, INPUT);

  pinMode(monSwitch_1, INPUT_PULLUP);
  pinMode(monSwitch_2, INPUT_PULLUP);


  digitalWrite(led_1, HIGH);      /// Initially activating Layer 1.
  digitalWrite(led_2, LOW);
  digitalWrite(led_3, LOW);


  previousPotVal = (analogRead(potPin) / 800.00) * 100.00;            /// Initializing the previous value beforehand. 


  Keyboard.begin();
  Consumer.begin();

  /*Serial.begin(9600);

  while(!Serial);

  Serial.println("Starting to print!");*/

}  
  


void loop ()
{
  
  
  currentMillis = millis();
  
  
  if (buttonstate_S != digitalRead(buttonMacroSelect))
  {
    buttonstate_S = digitalRead(buttonMacroSelect);

    previousMillis_Def = currentMillis;
  }
  

  if ((buttonstate_S == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    layers = layers + 1;

    keyPressed_S = true;
    buttonstate_S = HIGH;

    previousMillis_S = currentMillis;    
  } 


  if ((keyPressed_S == true) && ((currentMillis - previousMillis_S) > debounceInterval))
  {
    keyPressed_S = false;
  }



  if (layers == 2)
  {
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, HIGH);
    digitalWrite(led_3, LOW);
  }
  else if (layers == 3)
  {
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, HIGH);
  }
  else if (layers == 4)
  {
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
  }
  else if (layers == 5)
  {
    layers = 1;     /// Looping back in the selection circle.
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
  }

  
  switch (layers)
  {
    case 1:
    {
      macroKeys();
      break;
    }

    case 2:
    {
      mediaKeys();
      break;
    }

    case 3:
    {
      textKeys();
      break;
    }

    case 4:
    {
      volumeCtrlPot();
      break;
    }

  }



  monitorSwitch();

} 



///////////////////////////////////////////////////////////////////////////////////////////



void macroKeys ()
{
  
  if (buttonState_1 != digitalRead(button_1))
  {
    buttonState_1 = digitalRead(button_1);

    previousMillis_Def = currentMillis;
  }

  if (buttonState_2 != digitalRead(button_2))
  {
    buttonState_2 = digitalRead(button_2);

    previousMillis_Def = currentMillis;
  }
  
  if (buttonState_3 != digitalRead(button_3))
  {
    buttonState_3 = digitalRead(button_3);

    previousMillis_Def = currentMillis;
  }

///////////////////////////////////////

  if ((buttonState_1 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    Keyboard.press(KEY_F14);
    keyPressed_1 = true;
    buttonState_1 = HIGH;

    previousMillis_1 = currentMillis;
  } 


  if ((keyPressed_1 == true) && ((currentMillis - previousMillis_1) > debounceInterval))
  {
    Keyboard.release(KEY_F14);
    keyPressed_1 = false;
  }

///////////////////////////////////////

  if ((buttonState_2 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_D);
    keyPressed_2 = true;
    buttonState_2 = HIGH;

    previousMillis_2 = currentMillis;
  }


  if ((keyPressed_2 == true) && ((currentMillis - previousMillis_2) > debounceInterval))
  {
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.release(KEY_D);
    Keyboard.write(KEY_ESC);
    keyPressed_2 = false;
  }

///////////////////////////////////////
  
  if ((buttonState_3 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    Keyboard.press(KEY_LEFT_WINDOWS);
    Keyboard.press(KEY_D);
    keyPressed_3 = true;
    buttonState_3 = HIGH;

    previousMillis_3 = currentMillis;
  }


  if ((keyPressed_3 == true) && ((currentMillis - previousMillis_3) > debounceInterval))
  {
    Keyboard.release(KEY_LEFT_WINDOWS);
    Keyboard.release(KEY_D);
    keyPressed_3 = false;
  }

///////////////////////////////////////

}



///////////////////////////////////////////////////////////////////////////////////////////



void mediaKeys ()
{
  
  if (buttonState_1 != digitalRead(button_1))
  {
    buttonState_1 = digitalRead(button_1);

    previousMillis_Def = currentMillis;
  }

  if (buttonState_2 != digitalRead(button_2))
  {
    buttonState_2 = digitalRead(button_2);

    previousMillis_Def = currentMillis;
  }
  
  if (buttonState_3 != digitalRead(button_3))
  {
    buttonState_3 = digitalRead(button_3);

    previousMillis_Def = currentMillis;
  }

///////////////////////////////////////

  if ((buttonState_1 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    Consumer.write(MEDIA_VOLUME_MUTE);
    keyPressed_1 = true;
    buttonState_1 = HIGH;

    previousMillis_1 = currentMillis;
  } 


  if ((keyPressed_1 == true) && ((currentMillis - previousMillis_1) > debounceInterval))
  {
    keyPressed_1 = false;
  }

///////////////////////////////////////

  if ((buttonState_2 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    Consumer.write(MEDIA_PLAY_PAUSE);
    keyPressed_2 = true;
    buttonState_2 = HIGH;

    previousMillis_2 = currentMillis;
  }


  if ((keyPressed_2 == true) && ((currentMillis - previousMillis_2) > debounceInterval))
  {
    keyPressed_2 = false;
  }

///////////////////////////////////////
  
  if ((buttonState_3 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    Keyboard.press(KEY_F13);
    keyPressed_3 = true;
    buttonState_3 = HIGH;

    previousMillis_3 = currentMillis;
  }


  if ((keyPressed_3 == true) && ((currentMillis - previousMillis_3) > debounceInterval))
  {
      Keyboard.release(KEY_F13);
      keyPressed_3 = false;
  }

///////////////////////////////////////

}



///////////////////////////////////////////////////////////////////////////////////////////



void textKeys ()
{
  
  if (buttonState_1 != digitalRead(button_1))
  {
    buttonState_1 = digitalRead(button_1);

    previousMillis_Def = currentMillis;
  }

  if (buttonState_2 != digitalRead(button_2))
  {
    buttonState_2 = digitalRead(button_2);

    previousMillis_Def = currentMillis;
  }
  
  if (buttonState_3 != digitalRead(button_3))
  {
    buttonState_3 = digitalRead(button_3);

    previousMillis_Def = currentMillis;
  }

///////////////////////////////////////

  if ((buttonState_1 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_A);
    keyPressed_1 = true;
    buttonState_1 = HIGH;

    previousMillis_1 = currentMillis;
  } 


  if ((keyPressed_1 == true) && ((currentMillis - previousMillis_1) > debounceInterval))
  {
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.release(KEY_A);
    keyPressed_1 = false;
  }

///////////////////////////////////////

  if ((buttonState_2 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_C);
    keyPressed_2 = true;
    buttonState_2 = HIGH;

    previousMillis_2 = currentMillis;
  }


  if ((keyPressed_2 == true) && ((currentMillis - previousMillis_2) > debounceInterval))
  {
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.release(KEY_C);
    keyPressed_2 = false;
  }

///////////////////////////////////////
  
  if ((buttonState_3 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval))
  {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_V);
    keyPressed_3 = true;
    buttonState_3 = HIGH;

    previousMillis_3 = currentMillis;
  }


  if ((keyPressed_3 == true) && ((currentMillis - previousMillis_3) > debounceInterval))
  {
    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.release(KEY_V);
    keyPressed_3 = false;
  }

///////////////////////////////////////

}



///////////////////////////////////////////////////////////////////////////////////////////



void volumeCtrlPot ()
{
 
  currentPotVal = (analogRead(potPin) / 800.00) * 100.00;
  
    
  if (currentPotVal != previousPotVal)
  {
    
    if ((currentPotVal - previousPotVal) >= 1.8)
    {
      Consumer.press(MEDIA_VOLUME_UP);
      previousPotVal = currentPotVal;
      Consumer.release(MEDIA_VOLUME_UP);
    }
    else if ((previousPotVal - currentPotVal) >= 2.2)
    {
      Consumer.press(MEDIA_VOLUME_DOWN);
      previousPotVal = currentPotVal;
      Consumer.release(MEDIA_VOLUME_DOWN);
    }

  }

 
}



///////////////////////////////////////////////////////////////////////////////////////////



void monitorSwitch()
{
  
  if ((digitalRead(monSwitch_1) == LOW) && (monPressed_1 == false))       /// Initially, the toggle switch will be at this position, but it won't be detected.
  {
    stateMon_1 = digitalRead(monSwitch_1);
    monPressed_1 = true;
    monPressed_2 = false;

    previousMillis_Def = currentMillis;
  }

  if ((digitalRead(monSwitch_2) == LOW) && (monPressed_2 == false))
  {
    stateMon_2 = digitalRead(monSwitch_2);
    monPressed_2 = true;
    monPressed_1 = false;

    previousMillis_Def = currentMillis;
  }

///////////////////////////////////////

  if ((stateMon_1 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval_M))
  {
    Keyboard.press(KEY_LEFT_WINDOWS);
    Keyboard.write(KEY_P);
    Keyboard.release(KEY_LEFT_WINDOWS);
    keyPressed_M1 = true;
    stateMon_1 = HIGH;

    previousMillis_M_1 = currentMillis;
  } 


  if ((keyPressed_M1 == true) && ((currentMillis - previousMillis_M_1) > debounceInterval_M))
  {
    Keyboard.write(KEY_P);
    Keyboard.write(KEY_P);
    Keyboard.write(KEY_ENTER);
    Keyboard.write(KEY_ESC);
    keyPressed_M1 = false;
  }

///////////////////////////////////////

  if ((stateMon_2 == LOW) && ((currentMillis - previousMillis_Def) > debounceInterval_M))
  {
    Keyboard.press(KEY_LEFT_WINDOWS);
    Keyboard.write(KEY_P);
    Keyboard.release(KEY_LEFT_WINDOWS);
    keyPressed_M2 = true;
    stateMon_2 = true;

    previousMillis_M_2 = currentMillis;
  }


  if ((keyPressed_M2 == true) && ((currentMillis - previousMillis_M_2) > debounceInterval_M))
  {  
    Keyboard.write(KEY_P);
    Keyboard.write(KEY_P);
    Keyboard.write(KEY_ENTER);
    Keyboard.write(KEY_ESC);
    keyPressed_M2 = false;
  }

///////////////////////////////////////

}



///////////////////////////////////////////////////////////////////////////////////////////
