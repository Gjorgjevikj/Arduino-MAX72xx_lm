/*
 * LEDMatrixMax72xx.h - A simple library for comunicating the popular 
 * LED drivers when used for driving an 8x8 LED matrix display 
 * 
 * This is a simple standalone class for the primitive operations on the  
 * Max72xx (MAX7219 & MAX7221) used as driver of an 8x8 LED matrix display
 * Supports a single display 
 * (support for multiple cahined displays to come - hopefully :)
 * 
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE either express or implied.
 * 
 * Released into the public domain.
 * 
 * By Dejan Gjorgjevikj <dejan.gjorgjevikj@gmail.com>
 * 20 May 2016 
 * 
 */

#ifndef __LEDMatrixMax72xx_H__
#define __LEDMatrixMax72xx_H__

#if defined(ARDUINO) && (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#define LEDMatrixMax72xx_VERSION "0.1"  

template < uint8_t pinCLK, uint8_t pinCS, uint8_t pinDIN >
class LEDMatrixMax72xx // covering only basic low level functions
{
  protected:
    enum MAX72xx_REGISTER { DECODEMODE = 0x09, INTENSITY, SCANLIMIT, SHUTDOWN, DISPLAYTEST = 0x0f };
//    uint8_t pinCLK;
//    uint8_t pinCS;
//    uint8_t pinDIN;

    static void writeB(uint8_t data) // sends data serially bit-by-bit  
    {   
      digitalWrite(pinCS, LOW);  
      for(uint8_t mask=0b10000000; mask; mask>>=1) //  for(uint8_t i=0; i<8; i++)
      {    
        digitalWrite(pinCLK, LOW);
        digitalWrite(pinDIN, data & mask); //    digitalWrite(pinDIN, data & 0b10000000);  data <<= 1;
        //delayMicroseconds(50);
        digitalWrite(pinCLK, HIGH);
      }                                 
    }

    static void write(uint8_t address, uint8_t data) // sends data to address
    {
      digitalWrite(pinCS, LOW);
      writeB(address);          
      writeB(data);               
      digitalWrite(pinCS, HIGH);
    }

  public:
//    LEDMatrixMax72xx(uint8_t clk, uint8_t cs, uint8_t din) : pinCLK(clk), pinCS(cs), pinDIN(din) { }
    LEDMatrixMax72xx() { }
    void begin(void) 
    {
      pinMode(pinCLK, OUTPUT);
      pinMode(pinCS, OUTPUT);
      pinMode(pinDIN, OUTPUT);
      init();
      clear();
    }

    void writeCol(uint8_t address, uint8_t dat) { write(address+1, dat); } // Writes a single column of LEDs
    static void intensity(uint8_t val) { write(INTENSITY, val); } // Sets LEDs intensity 0-15
    static void test(bool v = true) { write(DISPLAYTEST, v); } // Test mode 1 - test mode, 0 - normal operation
    static void shutDown(bool v = false) { write(SHUTDOWN, v); } // Test mode 1 - normal operation, 0 - Shutdown Mode
    static void init(void)
    {
      write(DECODEMODE, 0x00); // No decode for LED matrix
      write(INTENSITY, 0x03); // Set initial intensity to 3
      write(SCANLIMIT, 0x07); // Scan-Limit all
      write(SHUTDOWN, 0x01);  // Wake-up    
      write(DISPLAYTEST, 0x00); // Normal Operation
    }

    void clear(void) { for(uint8_t i=0; i<8; writeCol(i++,0)) ; } // Clears the display by switching off all LEDs
};

#endif


