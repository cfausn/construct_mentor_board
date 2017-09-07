//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define NUM_MENTORS 7
#define NUM_STATUSES 3

#define STATUS_PIN_UP 0
#define STATUS_PIN_DOWN 1
#define NAME_PIN_UP 2
#define NAME_PIN_DOWN 3

String mentors[NUM_MENTORS]  = {"     Andrew       ","     Colin      ","     Esther     ","     Mark       ","     Matt       ","     Michelle   ","     Myles      "};
String statuses[NUM_STATUSES] = {"Unavailable     ","Available       ","Helping Someone "};


int mentor_index = 0;
int status_index = 1;

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
int last_btn    = 5;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
*/


 return btnNONE;  // when all others fail, return this...
}

void setup()
{
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print(mentors[mentor_index]); // print a simple message
 lcd.setCursor(0,1);
 lcd.print(statuses[status_index]);
 pinMode(STATUS_PIN_UP, OUTPUT);
 pinMode(STATUS_PIN_DOWN, OUTPUT);
 pinMode(NAME_PIN_UP, OUTPUT);
 pinMode(NAME_PIN_DOWN, OUTPUT);
 digitalWrite(STATUS_PIN_DOWN,LOW);
 }
 
void loop()
{
 
 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     last_btn = btnRIGHT;
     break;
     }
   case btnLEFT:
     {
     last_btn = btnLEFT;
     break;
     }
   case btnUP:
     {
     last_btn = btnUP;
     break;
     }
   case btnDOWN:
     {
     last_btn = btnDOWN;
     break;
     }
   case btnSELECT:
     {
     last_btn = btnSELECT;
     break;
     }
     case btnNONE:
     {
     switch(last_btn){
      case btnRIGHT: { 
        lcd.setCursor(0,0);
        if(mentor_index < NUM_MENTORS -1) mentor_index++;
        else mentor_index = 0;
        
        digitalWrite(NAME_PIN_UP,HIGH);
        lcd.print(mentors[mentor_index]);
        last_btn = btnNONE;
        break;
      }
      case btnLEFT: { 
        lcd.setCursor(0,0);
        if(mentor_index > 0) mentor_index--;
        else mentor_index = 6;
        
        digitalWrite(NAME_PIN_DOWN,HIGH);
        lcd.print(mentors[mentor_index]);
        last_btn = btnNONE;       

        break;
      }
      case btnUP: {
        lcd.setCursor(0,1);
        if(status_index < NUM_STATUSES - 1) status_index++;
        else status_index = 0;
        
        digitalWrite(STATUS_PIN_UP,HIGH);
        lcd.print(statuses[status_index]);
        last_btn = btnNONE;
        break;
      }
      case btnDOWN: {
        lcd.setCursor(0,1);
        if(status_index > 0) status_index--;
        else status_index = 2;
        
        digitalWrite(STATUS_PIN_DOWN,HIGH);
        lcd.print(statuses[status_index]);
        last_btn = btnNONE;
        break;
      }
      case btnSELECT: {
        lcd.setCursor(0,1);
        lcd.print("SELECT PRESSED"); 
        last_btn = btnNONE;
        break;
      }
      case btnNONE: {
        digitalWrite(STATUS_PIN_UP, LOW);
        digitalWrite(STATUS_PIN_DOWN, LOW);
        digitalWrite(NAME_PIN_UP, LOW);
        digitalWrite(NAME_PIN_DOWN, LOW);
        
        break;
       }
      
      
     }

     
     break;
     }
 }

}
