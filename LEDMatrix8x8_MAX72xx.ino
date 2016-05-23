/*
 * Example code using 
 * LEDMatrixMax72xx and LEDMatrix8x8 libraries
 * 
 * Supports a single display 
 * (support for multiple cahined displays to come - hopefully :)
 * 
 * Released into the public domain.
 * 
 * By Dejan Gjorgjevikj <dejan.gjorgjevikj@gmail.com>
 * 20 May 2016 
 * 
 */


#define ADVANCED_F

#define FONT_VARIANT_GFX_HEART_8x8
#ifdef ADVANCED_F
#define FONT_VARIANT_BASIC_8x8
#define FONT_VARIANT_BOLD_8x8
#define FONT_VARIANT_VGA1
#endif

#include "fonts_8x8.h"
#include <avr/pgmspace.h>
//#include <avr/io.h>

#ifndef ADVANCED_F
#include "LEDMatrixMax72xx.h"
#else
#include "LEDMatrix8x8.h"
#endif

#ifdef __STM32F1__ 
#pragma message("__STM32F1__ architecture: using pins DIN=PB3, CS=PB4, CLK=PB5.") 
#define Max7219_pinCLK PB5
#define Max7219_pinCS PB4
#define Max7219_pinDIN PB3
#define F(X) (X)
#define PSTR(X) (X)
#endif

#ifdef __AVR_ARCH__ 
#pragma message("__AVR_ARCH__ architecture: using pins DIN=12, CS=10, CLK=11.") 
#define Max7219_pinCLK 11
#define Max7219_pinCS 10
#define Max7219_pinDIN 12
#endif


///////////////////////////////////////////////////////////////////

//LEDMatrixMax72xx lm(Max7219_pinCLK, Max7219_pinCS, Max7219_pinDIN); 
#ifndef ADVANCED_F
LEDMatrixMax72xx <Max7219_pinCLK, Max7219_pinCS, Max7219_pinDIN> lm;
#else
LEDMatrix8x8 <Max7219_pinCLK, Max7219_pinCS, Max7219_pinDIN> lm; 
#endif

///////////////////////////////////////////////////////////////////

int freeRam () 
{
  #ifdef __AVR_ARCH__ 
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
  #else
  return (-1); 
  #endif
}

void ftakespm_P(const char * s)
{
  char displaychr;
  for (int k = 0; k < 15; k++)
  {
    displaychr = pgm_read_byte(&(s[k]));
    Serial.print(displaychr);
  }
  Serial.println();

}

void setup()
{
  Serial.begin(115200);

  lm.begin();
  lm.test();
  delay(500);
  lm.test(false);

  Serial.print (F("Free memory = "));
  Serial.println (freeRam());
  Serial.println ();


  static const char signMessage[][80] PROGMEM = {{"I AM PREDATOR,  UNSEEN COMBATANT. CREATED BY THE UNITED STATES DEPART"} , {"123"}};
  static const char Message[] PROGMEM = {"UNSEEN COMBATANT. CREATED BY THE UNITED STATES DEPART"};

  Serial.println ("...");
  ftakespm_P(signMessage[0]);
  ftakespm_P(Message);
  Serial.println ("...");
  
/*
  char buffer[30];
  Serial.println ("digits[0][...] PROGMEM, HEX");
  for(int i=0; i<8; i++)
  {
    sprintf(buffer, "&digits[0][%d] = %p\n", i, &(digits[0][i]));
    Serial.print(buffer);
  }
  Serial.println ("digits[1][...] PROGMEM, HEX");
  for(int i=0; i<8; i++)
  {
    sprintf(buffer, "&digits[1][%d] = %p\n", i, &(digits[1][i]));
    Serial.print(buffer);
  }

  Serial.println ();
  Serial.println ("digits_b[0][...], HEX");
  for(int i=0; i<8; i++)
  {
    sprintf(buffer, "&digits_b[0][%d] = %p\n", i, &(digits_b[0][i]));
    Serial.print(buffer);
  }
  Serial.println ("digits_b[1][...] PROGMEM, HEX");
  for(int i=0; i<8; i++)
  {
    sprintf(buffer, "&digits_b[1][%d] = %p\n", i, &(digits_b[1][i]));
    Serial.print(buffer);
  }
*/ 
}

void loop()
{
  // test the basic low lewel functions
  lm.clear();
  lm.intensity(10);
  delay(500);

{
  lm.setLED(0,0);
  delay(500);
  lm.setLED(1,1);
  delay(500);
  lm.setLED(3,3);
  delay(500);
}
 /*
  for(int i=0; i<16; i++)
  {
    for(int j=0; j<8; j++)
      lm.writeCol(j, heart_anim[i][j]);
    delay(100);
  }
  delay(500);
*/
///////////////////////////////////////////////
#ifdef ADVANCED_F

  // test the advanced functions
  //lm.clear();
  lm.intensity(5);

  for(int j=0; j<10; j++)
  {
    lm.set(digits_b[j]);
//    lm.printMat(Serial, "");
/*    for(int i=0; i<8; i++)
      for(int k=0; k<8; k++)
      if(digits_b[j][i] & 0x1<<k)
        lm.setLED(i,k);
      else
        lm.setLED(i,k,false);
*/        
        
    delay(100);
  }   
//  unsigned char oneItem[8];

  for(int j=0; j<10; j++)
  {
//    for(int k=0; k<8; k++)
    {
//      memcpy_P (&oneItem, &digits[j], 8);
//      lm.set(oneItem);
      lm.set_P(digits[j]);
//      lm.set(digits[j]);

      lm.printMat(Serial, "");
//      lm.writeCol(k, clmn);
    }
      
    delay(100);
  }   
  
  delay(500);

/*
  lm.clear();
  lm.set_P(heart_anim[15]);
  lm.shd();
  delay(500);
  for(int i=0; i<10; i++)
  {
    lm.shl();
    delay(100);
  }
  delay(500);


  lm.clear();
  lm.set_P(heart_anim[15]);
  lm.shd();
  delay(500);
  for(int i=0; i<10; i++)
  {
    lm.shr();
    delay(100);
  }

  lm.clear();
  lm.set_P(heart_anim[15]);
  lm.shd();
  delay(500);
  
  for(int i=0; i<16; i++)
  {
    lm.rlr();
    delay(100);
  }

  for(int i=0; i<16; i++)
  {
    lm.rll();
    delay(100);
  }
  delay(500);

  for(int i=0; i<16; i++)
  {
    lm.rlu();
    delay(100);
  }
  delay(500);
  
  for(int i=0; i<16; i++)
  {
    lm.rld();
    delay(100);
  }

    lm.rlu();
    lm.rlu();
    lm.rlu();
  for(int i=0; i<3; i++)
  {
    for(int j=0; j<4; j++)
    {
      lm.rld();
      lm.rlr();
      delay(100);
    }
    for(int j=0; j<4; j++)
    {
      lm.rld();
      lm.rll();
      delay(100);
    }
    for(int j=0; j<4; j++)
    {
      lm.rlu();
      lm.rll();
      delay(100);
    }
    for(int j=0; j<4; j++)
    {
      lm.rlu();
      lm.rlr();
      delay(100);
    }
  }
  lm.rld();
  lm.rld();
  lm.rld();

  for(int i=0; i<16; i++)
  {
    lm.intensity(i);
    delay(100);
  }
  for(int i=15; i>=0; i--)
  {
    lm.intensity(i);
    delay(100);
  }
  lm.intensity(5);

  lm.set_P(digits[7]);
  delay(500);
  lm.flipH();
  delay(200);
  lm.flipH();
  delay(200);
  lm.flipV();
  delay(200);
  lm.flipV();

  delay(1000);

  lm.set_P(digits[4]);
  delay(500);
  lm.flipH();
  delay(150);
  lm.flipH();
  delay(150);
  lm.flipV();
  delay(150);
  lm.flipV();

  delay(150);

  for(int i=0; i<8; i++)
    for(int j=0; j<8; j++)
    {
      lm.flipLED(i,j);
      delay(20);
    }
 

  for(int i=0; i<8; i++)
    for(int j=0; j<8; j++)
    {
      lm.setLED(j,i);
      delay(20);
    }
  
  for(int i=0; i<8; i++)
    for(int j=0; j<8; j++)
    {
      lm.resetLED(i,j);
      delay(20);
    }


const uint8_t sls[] = 
{
  0b01100110, // .##..##.
  0b11111111, // ########
  0b11111111, // ########
  0b01100110, // .##..##.
  0b01100110, // .##..##.
  0b11111111, // ########
  0b11111111, // ########
  0b01100110  // .##..##.
};
  
  lm.set_P(heart_anim[15]);   // IMG_SET
  delay(150);

  lm.set(sls);
  delay(200);
  lm.set_P(heart_anim[15], IMG_NSET);   
  delay(200);

  lm.set(sls);
  delay(200);
  lm.set_P(heart_anim[15], IMG_AND);   
  delay(200);

  lm.set(sls);
  delay(200);
  lm.set_P(heart_anim[15], IMG_NAND);   
  delay(200);

  lm.set(sls);
  delay(200);
  lm.set_P(heart_anim[15], IMG_OR);   
  delay(200);

  lm.set(sls);
  delay(200);
  lm.set_P(heart_anim[15], IMG_NOR);   
  delay(200);

  lm.set(sls);
  delay(200);
  lm.set_P(heart_anim[15], IMG_XOR);   
  delay(200);

  lm.set(sls);
  delay(200);
  lm.set_P(heart_anim[15], IMG_XNOR);   
  delay(200);

/////////////////

  lm.set_P(heart_anim[15]);   
  delay(500);
  lm.set(sls, 3, 3);
  delay(500);
  
  lm.set_P(heart_anim[15]);   
  delay(500);
  lm.set(sls, -2, -4);
  delay(500);

  lm.set_P(heart_anim[15]);   
  delay(500);
  for(int i=7; i>=0; i--)
  {
    lm.set(sls, i, i);
    delay(200);
  }
*/

////////////////
/*  
  char s[]="0123456789";

  for(int i=0; i<9; i++)
  {
    lm.set(digits[s[i]-'0']);   
    delay(50);
    for(int r=7; r>0; r--)
    {
      lm.shl();
      lm.set(digits[s[i+1]-'0'],r,0);
      delay(50);
    }
  }

*/

/*
  char message[]="Test message to scroll around...";
  int msize=sizeof(message);

  for(char *p = message; *p; p++)
  {
    lm.set(font_VGA[*p-' ']);   
    delay(50);
    for(int r=7; r>0; r--)
    {
      lm.shl();
      lm.set(font_VGA[*(p+1)-' '],r,0);
      delay(50);
    }
  }
*/

  lm.clear();
  delay(10);
  for(int i = 0; i<8; i++)
    for(int j = 0; j<8; j++)
    {
      lm.setLED(j,i);
      delay(10);
      lm.resetLED(j,i);
      delay(10);
    }
   
  delay(1000);
/*
  const char m0[] = "9876543210";
  lm.set(digits_b[m0[8]-'0']);
  delay(500);
  lm.scrollMessage(m0, 50, digits_b, '0', true);
  delay(1000);

  static const char M1[] PROGMEM = {"9753102468"};
  lm.scrollMessage_P(M1, 50, digits_b, '0');

  lm.scrollMessage(F("321654987"), 50, digits_b, '0');
  delay(500);
*/
  lm.scrollMessage("1100", 50, digits_b, '0');
  delay(500);
 
  lm.scrollMessage_F("1357986420", 100, digits, '0');
  delay(500);
  lm.scrollMessage_F(F("Dejan ANA EVA Zorica ..."), 50, font_VGA, ' ');
//  lm.scrollMessage(m0, 100, digits_b, '0');
  static const char m1[] PROGMEM = "Zorica EVA ANA Dejan ...";
  lm.scrollMessage_P_F(m1, 50, font_VGA, ' ');
//  lm.scrollMessage(F("0123456789"), 50, digits_b, '0');

//  lm.scrollMessage(F("Zorica EVA ANA Dejan ..."), 100, font_VGA, ' ');
//  lm.scrollMessage(F("0123456789"), 50, digits_b, '0');


  char md[][3]={"94","85","32","5;","+3","-2"};
  for(unsigned char c=0; c<6; c++)
  {
    lm.set_P(digits_s[md[c][0]-'*']);   
    lm.set_P(digits_s[md[c][1]-'*'],4,0);   
    delay(1000);
//    for(int r=7; r>0; r--)
//    {
//      lm.shl();
//      lm.set(digits[s[i+1]-'0'],r,0);
//      delay(50);
//    }
  }





#endif
}





