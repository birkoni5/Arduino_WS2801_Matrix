#include <fix_fft.h>
#include "SPI.h"
#include "Adafruit_WS2801.h"
#include <IRremote.h>
#include <avr/pgmspace.h>
#define AudioLeft A0
#define AudioRight A1
uint8_t dataPin  = 2; 
uint8_t clockPin = 3;
Adafruit_WS2801 strip = Adafruit_WS2801(88, dataPin, clockPin);
int IR_PIN = 11;
IRrecv irrecv(IR_PIN);
decode_results results;
char im[128], data[128];
char data_avgs[11]; //number of bars
int i=0,val;
int barsNumber;
int barsHeight;
int state=0;
int on_off;
int r_color=0;
int g_color=0;
int b_color=255;
int r_background=120;
int g_background=80;
int b_background=0;
int sensivity=7;
int eq_speed=1000;
const int numReadings = 5;     // Number of samples to keep track of (for smoothing)     VU METER
int sensivityVU=1000;
int audioValue;                 // Analog value read from audio channel
int maxAudioValue = 0;          // Maximum analog value read from audio channel
int ledLevel;                   // Value to map to the output LEDs                 
int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total                                    ___________
int on[15] = {24,26,28,29,30,35,37,39,41,46,47,48,50,51,52};
int off[22] = {24,27,29,30,31,34,36,38,41,45,46,48,49,51,52,53,60,63,67,68,70,71};
int tsd[28] = {12,15,16,17,19,20,22,24,26,31,34,37,38,39,41,43,44,46,50,53,55,56,57,59,60,61,63,64};
int  pixel[8][11]={
                {0,1,2,3,4,5,6,7,8,9,10},
                {21,20,19,18,17,16,15,14,13,12,11},
                {22,23,24,25,26,27,28,29,30,31,32},
                {43,42,41,40,39,38,37,36,35,34,33},
                {44,45,46,47,48,49,50,51,52,53,54},
                {65,64,63,62,61,60,59,58,57,56,55},
                {66,67,68,69,70,71,72,73,74,75,76},
                {87,86,85,84,83,82,81,80,79,78,77}
                 };
boolean ledState[11]; // Falling
int8_t x,y,dx,dy; // ----------------PONG
int8_t sh1y,sh2y,s1,s2,randommove;
int delayPong=15;
void setup() 
{
  for (int thisReading = 0; thisReading < numReadings; thisReading++)                // VU-Meter
    readings[thisReading] = 0;//                                                    ____________
  irrecv.enableIRIn(); // Start the receiver
  strip.begin();
  strip.show();
  randomSeed(analogRead(5));
  x = 3;
  y = 7;
  sh1y=3;
  sh2y=3;
  dx = 1;
  dy = 1;
  s1 = 0;
  s2 = 0;
}
void loop() 
{ 
  /***************************************************************************************************************
  ***************************************************************************************     IR Remote control
  ***************************************************************************************************************/
if(irrecv.decode(&results)) //this checks to see if a code has been received
  {
  if(results.value == 0xFD9A65) // button  on/off
    {
    int color=0;
    if(on_off==1)
      {
      on_off=0; 
      for (int x=0; x < 11; x++) 
        for (int y=0; y < 8; y++)
          strip.setPixelColor(pixel[y][x], 0,0,0);
      for(int i=255;i>color;i--)
        {
        for (int x=0; x < 22; x++)
          strip.setPixelColor(off[x], 0,0,i);
        strip.show();
        delay(5);
        }
      for (int x=0; x < 22; x++)
        strip.setPixelColor(off[x], 0,0,0);
      strip.show();
      }
    else
      {
      for(int i=color;i<256;i++)
        {
        for (int x=0; x < 15; x++)
          strip.setPixelColor(on[x], 0,0,i);
        strip.show();
        delay(5);
        }
      for (int x=0; x < 15; x++)
        strip.setPixelColor(on[x], 0,0,0);
      strip.show();
      state=1;
      on_off=1;
      }
    }
  if(results.value == 0xFD4AB5) // button  1
    {
    for (int x=0; x < 11; x++) 
      for (int y=0; y < 8; y++)
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    state=1;
    }
  if(results.value == 0xFD0AF5) // button  2
    {
    for (int x=0; x < 11; x++) 
      for (int y=0; y < 8; y++)
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    state=2;
    }
  if(results.value == 0xFD08F7) // button  3
    {
    for (int x=0; x < 11; x++) 
      for (int y=0; y < 8; y++)
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    state=3;
    }
  if(results.value == 0xFD6A95) // button  4
    {
    for (int x=0; x < 11; x++) 
      for (int y=0; y < 8; y++)
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    state=4;
    }
  if(results.value == 0xFD2AD5) // button  5
    {
    for (int x=0; x < 11; x++) 
      for (int y=0; y < 8; y++)
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    state=5;
    }
  if(results.value == 0xFD28D7) // button  6
    {
    for (int x=0; x < 11; x++) 
      for (int y=0; y < 8; y++)
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    state=6;
    }  
  if(results.value == 0xFD728D) // button  7
    {
    for (int x=0; x < 11; x++) 
      for (int y=0; y < 8; y++)
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    state=7;
    } 
  if(results.value == 0xFD48B7) // button  EPG -> White/Black edition
    {
    r_color=255;
    g_color=255;
    b_color=255;
    r_background=0;
    g_background=0;
    b_background=0;
    if(state==7)
      for (int x=0; x < 11; x++) 
        for (int y=0; y < 8; y++)
          strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);   
    }
  if(results.value == 0xFDE01F) // blue button
    {
    r_color=0;
    g_color=0;
    b_color=255;
    }
  if(results.value == 0xFD629D) // red button
    {
    r_color=255;
    g_color=0;
    b_color=0;
    }
  if(results.value == 0xFD22DD) // green button
    {
    r_color=0;
    g_color=255;
    b_color=0;
    }
  if(results.value == 0xFD20DF) // yellow button
    {
    r_color=255;
    g_color=255;
    b_color=0;
    }
  if(results.value == 0xFD52AD) // TV/Radio button
    {
    r_background=150;
    g_background=0;
    b_background=0;
    if(state==7)
      for (int x=0; x < 11; x++) 
        for (int y=0; y < 8; y++)
          strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    }
  if(results.value == 0xFD12ED) // Audio button
    {
    r_background=0;
    g_background=150;
    b_background=0;
    if(state==7)
      for (int x=0; x < 11; x++) 
        for (int y=0; y < 8; y++)
          strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    }
  if(results.value == 0xFD10EF) // Subtitle button
    {
    r_background=100;
    g_background=100;
    b_background=0;
    if(state==7)
      for (int x=0; x < 11; x++) 
        for (int y=0; y < 8; y++)
          strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    }
  if(results.value == 0xFDD02F) // Text button
    {
    r_background=0;
    g_background=0;
    b_background=150;
    if(state==7)
      for (int x=0; x < 11; x++) 
        for (int y=0; y < 8; y++)
          strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
    }
  if(results.value == 0xFD609F) // CH+ button
    {
    if(state>0 && state<3)
      {  
      sensivity++;
      if(sensivity==11)
        sensivity=1;
      }
    if(state==3 || state==4)
      {
      sensivityVU=+100;
      if(sensivityVU==1600)
        sensivityVU=200;
      }
    if(state==7)
      {
      delayPong-=5;
      if(delayPong<0)
        delayPong=60;
      }
    }
  if(results.value == 0xFD6897) // CH- button
    {
    if(state>0 && state<3)
      { 
      sensivity--;
      if(sensivity==0)
        sensivity=10;
      }
    if(state==3 || state==4)
      {
      sensivityVU=-100;
      if(sensivityVU==100)
        sensivityVU=1500;
      }
    if(state==7)
      {
      delayPong+=5;
      if(delayPong>60)
        delayPong=0;
      }
    }
  if(results.value == 0xFDD827) // VOL+ button
    {
    if(state>0 && state<3)
      {   
      eq_speed+=500;
      if(eq_speed==5500)
        eq_speed=0;
      }
    }
  if(results.value == 0xFD5AA5) // VOL- button
    {
    if(state>0 && state<3)
      { 
      eq_speed-=500;
      if(eq_speed==-500)
        eq_speed=5000;
      }
    }
  irrecv.resume(); //receive the next value
  }
  /*************************************************************************************************************
  ***************************************************************************************   Music equaliser
  *************************************************************************************************************/
if(state==1 && on_off==1) 
  {
  barsNumber=11;
  barsHeight=8;
  strip.show();
  for (i=0; i < 128; i++)
    {                                     
    val = analogRead(A0);                             
    data[i] = val;                                       
    im[i] = 0;                                                     
    }  
  fix_fft(data,im,7,0);
  for (i=0; i< 64;i++)                                     
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); // this gets the absolute value of the values in the array, so we're only dealing with positive numbers
  for (i=0; i<barsNumber; i++)
    {
    data_avgs[i] =data[i*4] + data[i*4 + 1] + data[i*4 + 2] + data[i*4 + 3];  // average together 
    if (i == 0)
      data_avgs[i] >>= 1;
    data_avgs[i] = map(data_avgs[i], 0, sensivity, 0, 7);                     // remap values for display //    <________________________________--------------  
    }
  for (int x=0; x < barsNumber; x++) 
    for (int y=0; y < barsHeight; y++)
      {
      if (y < data_avgs[x])
        {
        if((r_color>0 && r_background>0 && g_color==0 && g_background==0)||(g_color>0 && g_background>0 && r_color==0 && r_background==0)||(g_color>0 && g_background>0 && r_color>0 && r_background>0 && b_color==0)||(b_color>0 && b_background>0 && r_color==0))
          {
          strip.setPixelColor(pixel[y][x], b_color, r_color, g_color);
          for(int p=0;p<y;p++)
            strip.setPixelColor(pixel[p][x], r_background, g_background, b_background);
          }
        else
          strip.setPixelColor(pixel[y][x], r_color, g_color, b_color);
        }
      else
        {
        delayMicroseconds(eq_speed);
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background); 
        }
      } 
    
  }
  /*************************************************************************************************************
  ***************************************************************************   Music equaliser(2) 
  *************************************************************************************************************/
if(state==2 && on_off==1) 
  {
  int pixel[8][11]={
                {87,86,85,84,83,82,81,80,79,78,77},
                {66,67,68,69,70,71,72,73,74,75,76},
                {65,64,63,62,61,60,59,58,57,56,55},
                {44,45,46,47,48,49,50,51,52,53,54},
                {43,42,41,40,39,38,37,36,35,34,33},
                {22,23,24,25,26,27,28,29,30,31,32},
                {21,20,19,18,17,16,15,14,13,12,11},
                {0,1,2,3,4,5,6,7,8,9,10}               
                 };
  barsNumber=11;
  barsHeight=8;
  strip.show();
  for (i=0; i < 128; i++)
    {                                     
    val = analogRead(A0);                             
    data[i] = val;                                       
    im[i] = 0;                                                     
    }  
  fix_fft(data,im,7,0);
  for (i=0; i< 64;i++)                                     
    data[i] = sqrt(data[i] * data[i] + im[i] * im[i]); // this gets the absolute value of the values in the array, so we're only dealing with positive numbers
  for (i=0; i<barsNumber; i++)
    {
    data_avgs[i] =data[i*4] + data[i*4 + 1] + data[i*4 + 2] + data[i*4 + 3];  // average together 
    if (i == 0)
      data_avgs[i] >>= 1;
    data_avgs[i] = map(data_avgs[i], 0, sensivity, 0, 7);                     // remap values for display //    <________________________________--------------  
    }
  for (int x=0; x < barsNumber; x++) 
    for (int y=0; y < barsHeight; y++)
      {
      if (y < data_avgs[x])
        {
        if((r_color>0 && r_background>0 && g_color==0 && g_background==0)||(g_color>0 && g_background>0 && r_color==0 && r_background==0)||(g_color>0 && g_background>0 && r_color>0 && r_background>0 && b_color==0)||(b_color>0 && b_background>0 && r_color==0))
          {
          strip.setPixelColor(pixel[y][x], b_color, r_color, g_color);
          for(int p=0;p<y;p++)
            strip.setPixelColor(pixel[p][x], r_background, g_background, b_background);
          }
        else
          strip.setPixelColor(pixel[y][x], r_color, g_color, b_color);
        }
      else
        {
        delayMicroseconds(eq_speed);
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background); 
        }
      } 
    
  }

  /*************************************************************************************************************
  **************************************************************************   VU-Meter
  *************************************************************************************************************/
if(state==3 && on_off==1)
  {
  strip.show();
  total =- readings[index];
  readings[index] = analogRead(AudioRight);
  total =+ readings[index];
  index++;
  if (index >= numReadings)
    index = 0; 
  audioValue = total / numReadings;  // average
  if (audioValue > maxAudioValue)
    maxAudioValue = audioValue;
  ledLevel = map(audioValue, 0, 1023, 0, sensivityVU);
  if (ledLevel > barsNumber)
    {
    ledLevel = barsNumber;
    } 
  for (int x=0; x < barsNumber; x++) 
    {
    if (x < ledLevel)
      for (int y=0; y < barsHeight; y++)
        if((r_color>0 && r_background>0 && g_color==0 && g_background==0)||(g_color>0 && g_background>0 && r_color==0 && r_background==0)||(g_color>0 && g_background>0 && r_color>0 && r_background>0 && b_color==0)||(b_color>0 && b_background>0 && r_color==0))
          {
          strip.setPixelColor(pixel[y][x], b_color, r_color, g_color);
          for(int p=0;p<y;p++)
            strip.setPixelColor(pixel[p][x], r_background, g_background, b_background);
          }
        else
          strip.setPixelColor(pixel[y][x], r_color, g_color, b_color); 
    else
      {
      delay(10);
      for (int y=0; y < barsHeight; y++)
        strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
      }
    }
  }
/***********************************************************************************************
******************************************************************** BLINK
**********************************************************************************************/
if(state==4 && on_off==1)
  {
  strip.show();
  total =- readings[index];
  readings[index] = analogRead(AudioRight);
  total =+ readings[index];
  index++;
  if (index >= numReadings)
    index = 0; 
  audioValue = total / numReadings;  // average
  if (audioValue > maxAudioValue)
    maxAudioValue = audioValue;
  ledLevel = map(audioValue, 0, 1023, 0, sensivityVU); 
    if (ledLevel > 205)
      ledLevel = 205;
    for (int x=0; x < 206; x++) 
      if (x < ledLevel)
        for (int z=0; z < 11; z++) 
          for (int y=0; y < 8; y++)
            {  
            int t=255/x;
            if(r_color==255 && g_color==0)
              strip.setPixelColor(pixel[y][z],t,0,0); 
            if(g_color==255 && r_color==0)
              strip.setPixelColor(pixel[y][z],0,t,0); 
            if(r_color==255 && g_color==255 && b_color==0)
              strip.setPixelColor(pixel[y][z],t,t,0);  
            if(b_color==255 && g_color==0)
              strip.setPixelColor(pixel[y][z],0,0,t);
            if(r_color==255 && g_color==255 && b_color==255)
              strip.setPixelColor(pixel[y][z],t,t,t);     
            }
  }
/***********************************************************************************************
******************************************************************** FALLING
**********************************************************************************************/
if(state==5 && on_off==1)
  {
  DropColumn(random(11));// Randomly choose a column
  delay(random(50));
  }
/***********************************************************************************************
******************************************************************** TSD
**********************************************************************************************/
if(state==6 && on_off==1)
  {
  for (int x=0; x < 11; x++) 
    for (int y=0; y < 8; y++)
      strip.setPixelColor(pixel[y][x], r_background, g_background, b_background);
  for (int x=0; x < 28; x++)
    strip.setPixelColor(tsd[x], r_color, g_color, b_color);
  strip.show();
  }
/***********************************************************************************************
******************************************************************** PONG
**********************************************************************************************/  
if(state==7 && on_off==1)
  {
  if(x==9 || x==1)
    {
    dx=-dx;// Collision detection
    if (x==1)// check the first ship (left side)
      if (sh1y!=y && sh1y+1!=y)
        s2++;
    else // check the second ship (right side)
      if (sh2y!=y && sh2y+1!=y) 
        s1++;
    }
  if (y==7 || y==0) 
    dy=-dy;
    x=x+dx; 
  y=y+dy;
  strip.setPixelColor(pixel[y][x], r_color, g_color, b_color);
  strip.setPixelColor(pixel[sh1y][0], r_color, g_color, b_color);
  strip.setPixelColor(pixel[sh1y+1][0], r_color, g_color, b_color);
  strip.setPixelColor(pixel[sh2y][10], r_color, g_color, b_color);
  strip.setPixelColor(pixel[sh2y+1][10], r_color, g_color, b_color);
  strip.setPixelColor(pixel[y-dy][x-dx], r_background, g_background, b_background);
  strip.setPixelColor(pixel[y-dy][x-dx], r_background, g_background, b_background);
  strip.setPixelColor(pixel[sh1y-1][0], r_background, g_background, b_background);
  strip.setPixelColor(pixel[sh1y+2][0], r_background, g_background, b_background);
  strip.setPixelColor(pixel[sh2y-1][10], r_background, g_background, b_background);
  strip.setPixelColor(pixel[sh2y+2][10], r_background, g_background, b_background);
  strip.show();
  if (dx<0)//* The ships moves when the ball go in their direction. They follow it magically ;) :This code is too smart, in fact he is perfekt :)  
    {
    if (sh1y>y)
      sh1y--;
    if (sh1y<y)
      sh1y++;
    }
  if (dx>0)
    {
    if (sh2y>y) 
      sh2y--;
    if (sh2y<y)
      sh2y++;
    }
  if (sh1y>6) sh1y=6;
  if (sh2y>6) sh2y=6;
  if (sh1y<0) sh1y=0;
  if (sh2y<0) sh2y=0;
  delay(delayPong);  
  }
}
/***********************************************************************************************
******************************************************************** FALLING
**********************************************************************************************/
void DropColumn(int col)
{
if (ledState[col] == false) // Check the LED column state If it's not lit then light it
  {  
  for (int i=7; i>=0; i--)// cycle through each LED in the column
    {
    strip.setPixelColor(pixel[i][col], r_color, g_color, b_color);
    delay(10);
    strip.show();
    }
  ledState[col]=true;
  }
else // otherwise shut it off 
  {
  for (int i=7; i>=0; i--)// cycle through each LED in the column
    {
    strip.setPixelColor(pixel[i][col],  r_background, g_background, b_background);
    delay(10);
    strip.show();
    }
  ledState[col]=false;
  }
}
