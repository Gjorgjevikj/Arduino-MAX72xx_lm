/*
 * LEDMatrix8x8.h - A library that extends the LEDMatrixMax72xx  
 * 
 * This class maintains the state of the leds in memory array 
 * in the class and enables basic image operations on the display
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

#ifndef __LEDMatrix8x8_H__
#define __LEDMatrix8x8_H__

#include "LEDMatrixMax72xx.h"
#define LEDMatrix8x8_VERSION "0.2"  

#include <avr/pgmspace.h>
//#include <avr/io.h>

//typedef const uint8_t IMG_OP;
enum IMG_OP { IMG_SET, IMG_NSET, IMG_AND, IMG_CAND, IMG_NAND, IMG_OR, IMG_COR, IMG_NOR, IMG_XOR, IMG_XNOR };

template < uint8_t pinCLK, uint8_t pinCS, uint8_t pinDIN >
class LEDMatrix8x8 : public LEDMatrixMax72xx<pinCLK, pinCS, pinDIN> // covers higher graphical functions
{
  protected:
    uint8_t mem[8]; // memory copy of the LEDs state

  public:
//    LEDMatrix8x8(uint8_t clk, uint8_t cs, uint8_t din) : LEDMatrixMax72xx(clk, cs, din) { }
    LEDMatrix8x8() { }

// Functions of individual LED manipulation
////////////////////////////////////////////

    // Turn ON individual LED - LED[0][0] @ top-left
    void setLED(uint8_t x, uint8_t y, bool state = HIGH) 
    {
      const uint8_t mask = 0b00000001 << x; 
      writeCol(y, state ? (mem[y] | mask) : (mem[y] & ~mask) );
    }

    // Turn OFF individual LED - LED[0][0] @ top-left
    void resetLED(uint8_t x, uint8_t y)
    {
      setLED(x, y, LOW);
    }

    void flipLED(uint8_t x, uint8_t y)
    {
      uint8_t mask = 0b00000001 << x; 
      writeCol(y, mem[y] ^ mask);
    }

// Internal function that manipulate and updtae the current state of the display 
// without taking external data to be showed other than simple parameters for operations
////////////////////////////////////////////////////////////////////////////////////////

    // Inverts the cuurent image on the display
    void invert(void)
    {
      for(uint8_t i=0; i<8; i++)
        writeCol(i, mem[i] ^ 0xff);
    }

    // Shift down the image by n positions (n<8)
    void shd(uint8_t n = 1) 
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      for(uint8_t i=0; i<8; i++)
        writeCol(i, mem[i] << n);
    }

    // Roll down the image by n positions (n<8)
    void rld(uint8_t n = 1) 
    {
      // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      const uint8_t p = 8 - (n &= 0b00000111);
      const uint8_t m = ~((uint8_t)0) << p;
      for(uint8_t i=0; i<8; i++)
        writeCol(i, (mem[i] << n) | ((mem[i] & m) >> p) );
    }
    
    // Shift up the image by n positions (n<8)
    void shu(uint8_t n = 1) 
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      for(uint8_t i=0; i<8; i++)
        writeCol(i, mem[i] >> n);
    }

    // Roll up the image by n positions (n<8)
    void rlu(uint8_t n = 1) 
    {
      // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      const uint8_t p = 8 - (n &= 0b00000111);
      const uint8_t m = ~((uint8_t)0) >> p;
      for(uint8_t i=0; i<8; i++)
        writeCol(i, (mem[i] >> n) | ((mem[i] & m) << p) );
    }

    // Shift left the image by n positions (n<8)
    void shl(uint8_t n = 1) 
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      uint8_t i=0;
      for( ; i<8-n; i++)
        writeCol(i, mem[i+n]);
      for( ; i<8; i++)
        writeCol(i, 0);
    }

    // Roll left the image by n positions (n<8)
    void rll(uint8_t n)     
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      for(uint8_t i=0 ; i<n; i++)
        rll();
    }

    // Roll left by one position only
    void rll(void)          
    {
      uint8_t c=mem[0];
      for(uint8_t i=0 ; i<7; i++)
        writeCol(i, mem[i+1]);
      writeCol(7, c);
    }

// Shift right the image by n positions (n<8)
    void shr(uint8_t n = 1) 
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      for(uint8_t i=7 ; i>=n; i--)
        writeCol(i, mem[i-n]);
      for(uint8_t i=0 ; i<n; i++)
        writeCol(i, 0);
    }

// Roll right the image by n positions (n<8)    
    void rlr(uint8_t n)     
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      for(uint8_t i=0 ; i<n; i++)
        rlr();
    }

// roll right by one position only
    void rlr(void)          
    {
      uint8_t c=mem[7];
      for(uint8_t i=7 ; i>0; i--)
        writeCol(i, mem[i-1]);
      writeCol(0, c);
    }
    
//    void rl(void);
// Rotate right
    void rr(void) 
    {
      bool r[8][8];
      uint8_t t;
      for(uint8_t i=0 ; i<4; i++)
      {
        for(uint8_t j=0 ; j<4; j++)
        {
          bool t = mem[i] & 0x01<<j;
          r[i][j] = mem[j] & 0x01<<(8-1-i);
          r[j][8-1-i] = mem[8-1-i] & 0x01<<(8-1-j);
          r[8-1-i][8-1-j] = mem[8-1-j] & 0x01<<i;
          r[8-1-j][i] = t;
        }    
      }
      for(uint8_t i=0 ; i<8; i++)
      {
        t=0;
        for(uint8_t j=0 ; j<8; j++)
          t |= r[i][j] ? 0x01<<j : 0;
        writeCol(i, t);
      }
    }

// Flips the image over the horizontal axis    
    void flipH(void)        
    {
      uint8_t maskLSB, maskMSB, flip;
      for(uint8_t i=0; i<8; i++)
      {
        maskLSB = 0b00000001;
        maskMSB = 0b10000000; // ~(~((uint8_t)0)>>1) // word size indpendent ...
        flip = 0x00;
        for(uint8_t j=0; j<8/2; j++)
        {
            if(mem[i] & maskLSB)
              flip |= maskMSB;
            if(mem[i] & maskMSB)
              flip |= maskLSB;
            maskMSB >>= 1;
            maskLSB <<= 1;
        }
        writeCol(i, flip);
      }
    }

// Flips the image on the vertical axis    
    void flipV(void)        
    {
      uint8_t flip;
      for(uint8_t i=0, j=7; i<j; i++, j--)
      {
        flip = mem[i];
        writeCol(i, mem[j]);
        writeCol(j, flip);
      }
    }

// Functions that take external data to show on the display (may be in PROGMEM od SRAM)
///////////////////////////////////////////////////////////////////////////////////////

  // override
    void writeCol(uint8_t address, uint8_t dat) 
      { LEDMatrixMax72xx<pinCLK, pinCS, pinDIN>::writeCol(address, mem[address] = dat); }

    void clear(void) { for(uint8_t i=0; i<8; writeCol(i++,0)) ; } // Clears the display by switching off all LEDs AND clearing mem

    // set the const image img stored in SRAM on the display offseted by x,y
    void set(const uint8_t * img, int8_t x, int8_t y)
    {
      static uint8_t copy[8];
      memcpy(copy, img, 8);
      set(copy, x, y);
    }

    // set the const image img stored in FLASH on the display offseted by x,y
    void set_P(const unsigned char * img, int8_t x, int8_t y)
    {
      static uint8_t copy[8];
#ifdef __AVR_ARCH__ 
      memcpy_P(copy, img, 8);
#else
      memcpy(copy, img, 8);
#endif
      set(copy, x, y);
    }

    // set the image img on the display offseted by x,y
    void set(uint8_t * copy, int8_t x, int8_t y)
    { // not final ...
      uint8_t msk[8]={0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
//      for(int i=0; i<8; i++)  copy[i]=img[i];
//      memcpy(copy, img, 8);
      if(x>0)
      {
        shr(copy, x);
        shr(msk, x);
      }
      else if(x<0)
      {
        shl(copy, -x);
        shl(msk, -x);
      }
      if(y>0)
      {
        shd(copy, y);
        shd(msk, y);
      }
      else if(y<0)
      {
        shu(copy, -y);
        shu(msk, -y);
      }
    //  if(op ==  IMG_SET)
      {
        set(msk, IMG_CAND);
        set(copy, IMG_OR);
      }
    }

// Sets the display performing logical operation op with the current image 
// and the one stored in the 8 bytes pointed by img stored in SRAM
    void set(const uint8_t * img, IMG_OP op = IMG_SET)
    {
      for(uint8_t i=0; i<8; i++)
      {
        switch(op)
        {
          case IMG_SET  : if(img[i] != mem[i]) writeCol(i, img[i]); break;
          case IMG_NSET : writeCol(i, ~img[i]); break;
          case IMG_AND  : if(img[i] != 0xff) writeCol(i, mem[i] & img[i]); break;
          case IMG_CAND : writeCol(i, mem[i] & ~img[i]); break;
          case IMG_NAND : writeCol(i, ~(mem[i] & img[i])); break;
          case IMG_OR   : if(img[i] != 0x00) writeCol(i, mem[i] | img[i]); break;
          case IMG_COR  : writeCol(i, mem[i] | ~img[i]); break;
          case IMG_NOR  : writeCol(i, ~(mem[i] | img[i])); break;
          case IMG_XOR  : writeCol(i, mem[i] ^ img[i]); break;
          case IMG_XNOR : writeCol(i, ~(mem[i] ^ img[i])); break;
        }
      }
    }

// Sets the display performing logical operation op with the current image 
// and the one stored in the 8 bytes pointed by img stored in FLASH
    void set_P(const unsigned char * img, IMG_OP op = IMG_SET)
    {
      unsigned char SRAMcopy[8];
#ifdef __AVR_ARCH__ 
      memcpy_P(SRAMcopy, img, 8);
#else
      memcpy(SRAMcopy, img, 8);
#endif
      set(SRAMcopy, op);
//      invert(); // just a signal that comes from PROGMEM
    }

    void scroll1(const unsigned char chrImg[8], unsigned int t) // to add par direction
    {
        for(int r=7; r>0; r--)
        {
          shl();
          set(chrImg,r,0);
          delay(t);
        }
    }

    void scroll1_P(const unsigned char chrImg[8], unsigned int t) // to add par direction
    {
        for(int r=7; r>0; r--)
        {
          shl();
          set_P(chrImg,r,0);
          delay(t);
        }
    }

// Scrolls left-to-right a message stored in SRAM on the display, 
// spending t miliseconds before shifting one column
// using font definitipon stored in SRAM in which character are offseted (begin at) character offset
    void scrollMessage(const char * message, unsigned int t, const unsigned char font[][8], uint8_t offset = ' ', bool chain = false) // to add par direction
    {
      const char *p;
      scroll1(font[*message-offset],t);
      for(p = message; *(p+1); p++)
      {
        set(font[*p-offset]);   
        delay(t);
        scroll1(font[*(p+1)-offset],t);
      }
      set(font[*p-offset],0,0);
      delay(t);
      if(!chain)
        for(int i=0; i<8; i++)
        {
          shl();
          delay(t);
        }
    }

// Scrolls left-to-right a message stored in SRAM on the display, 
// spending t miliseconds before shifting one column
// using font definitipon stored in FLASH in which character are offseted (begin at) character offset
    void scrollMessage_F(const char * message, unsigned int t, const unsigned char font[][8], uint8_t offset = ' ', bool chain = false) // to add par direction
    {
      const char *p;
      scroll1_P(font[*message-offset],t);
      for(p = message; *(p+1); p++)
      {
        set_P(font[*p-offset]);   
        delay(t);
        scroll1_P(font[*(p+1)-offset],t);
      }
      set_P(font[*p-offset],0,0);
      delay(t);
      if(!chain)
        for(int i=0; i<8; i++)
        {
          shl();
          delay(t);
        }
    }

// Scrolls left-to-right a message stored in FLASH on the display, 
// spending t miliseconds before shifting one column
// using font definitipon stored in SRAM in which character are offseted (begin at) character offset
    void scrollMessage_P(const char * message, unsigned int t, const unsigned char font[][8], uint8_t offset = ' ', bool chain = false) // to add par direction
    {
#ifdef __AVR_ARCH__ 
      char c, nc;
      int i=0; 
      scroll1(font[(c = pgm_read_byte(&(message[i])))-offset],t);
      while(nc = pgm_read_byte(&(message[i])+1))
      {
        set(font[c-offset]);   
        delay(t);
        scroll1(font[nc-offset],t);
        i++;
        c=nc;
      }
      set(font[c-offset],0,0);
      delay(t);
      if(!chain)
        for(int k=0; k<8; k++)
        {
          shl();
          delay(t);
        }
#else
      scrollMessage(message, t, font, offset);
#endif
    }

// Scrolls left-to-right a message stored in FLASH on the display, 
// spending t miliseconds before shifting one column
// using font definitipon stored in FLASH in which character are offseted (begin at) character offset
    void scrollMessage_P_F(const char * message, unsigned int t, const unsigned char font[][8], uint8_t offset = ' ', bool chain = false) // to add par direction
    {
#ifdef __AVR_ARCH__ 
      char c, nc;
      int i=0; 
      scroll1_P(font[(c = pgm_read_byte(&(message[i])))-offset],t);
      while(nc = pgm_read_byte(&(message[i])+1))
      {
        set_P(font[c-offset]);   
        delay(t);
        scroll1_P(font[nc-offset],t);
        i++;
        c=nc;
      }
      set_P(font[c-offset],0,0);
      delay(t);
      if(!chain)
        for(int k=0; k<8; k++)
        {
          shl();
          delay(t);
        }
#else
      scrollMessage(message, t, font, offset);
#endif
    }

    void scrollMessage(const __FlashStringHelper* ifsh, unsigned int t, const unsigned char font[][8], uint8_t offset = ' ', bool chain = false) 
    {
#ifdef __AVR_ARCH__ 
      //PGM_P message = reinterpret_cast<PGM_P>(ifsh); 
      scrollMessage_P(reinterpret_cast<PGM_P>(ifsh), t, font, offset, chain);
#else
      scrollMessage(ifsh, t, font, offset);
#endif
    }

    void scrollMessage_F(const __FlashStringHelper* ifsh, unsigned int t, const unsigned char font[][8], uint8_t offset = ' ', bool chain = false) 
    {
#ifdef __AVR_ARCH__ 
      //PGM_P message = reinterpret_cast<PGM_P>(ifsh); 
      scrollMessage_P_F(reinterpret_cast<PGM_P>(ifsh), t, font, offset, chain);
#else
      scrollMessage(ifsh, t, font, offset);
#endif

    }

  
  protected:
    static void shd(uint8_t *img, uint8_t n) // shift every column of img down 
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      for(uint8_t i=0; i<8; i++)
        img[i] <<= n;
    }
    
    static void shu(uint8_t *img, uint8_t n) // shift every column of img up 
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      for(uint8_t i=0; i<8; i++)
        img[i] >>= n;
    }

    static void shr(uint8_t *img, uint8_t n) // shift columns of img right 
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      for(uint8_t i=7 ; i>=n; i--)
        img[i] = img[i-n];
      for(uint8_t i=0 ; i<n; i++)
        img[i] = 0;
    }
    
    static void shl(uint8_t *img, uint8_t n) // shift columns of img left 
    {
      n &= 0b00000111; // mask out the higher bits in n kepping only the 3 lowest so the value in n is < 8
      uint8_t i=0;
      for( ; i<8-n; i++)
        img[i] = img[i+n];
      for( ; i<8; i++)
        img[i] = 0;
    }

// Other utulity functions
  public:
    void printMat(HardwareSerial &s, char * comment = "")
    {
      s.print("{ // "); 
      s.println(comment);
      uint8_t rmask = 0x01;
      for(int i=0; i<8; i++)
      {
        s.print("\t0b");
        //s.print(mem[i], BIN);
        for(uint8_t m = 0b10000000; m ; m>>=1)
          s.print((mem[i] & m) ? '1' : '0');
        if(i<7) 
          s.print(","); 
    
        s.print("\t// "); 
        for(int j=0; j<8; j++)
          s.print((mem[j] & rmask) ? '#' : '.');
    
        s.println();
        rmask <<= 1;
      }
      s.println("},");
    }


};

#endif


