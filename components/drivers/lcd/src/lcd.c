/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <string.h>
#include <unistd.h>
#include "stdlib.h"
#include "lcd.h"
#include "font.h"
#include "sleep.h"
#include "global_config.h"

//cyj add 2019-10-22
#include "HZK16.c"
#include "unic16hzk.c"
#include "printf.h"  //cyj add 2019-10-23






static lcd_ctl_t lcd_ctl;

static uint16_t* g_lcd_display_buff = NULL;
static uint16_t g_lcd_w = 0;
static uint16_t g_lcd_h = 0;
static bool g_lcd_init = false;

// static uint16_t gray2rgb565[64]={
// 0x0000, 0x2000, 0x4108, 0x6108, 0x8210, 0xa210, 0xc318, 0xe318, 
// 0x0421, 0x2421, 0x4529, 0x6529, 0x8631, 0xa631, 0xc739, 0xe739, 
// 0x0842, 0x2842, 0x494a, 0x694a, 0x8a52, 0xaa52, 0xcb5a, 0xeb5a, 
// 0x0c63, 0x2c63, 0x4d6b, 0x6d6b, 0x8e73, 0xae73, 0xcf7b, 0xef7b, 
// 0x1084, 0x3084, 0x518c, 0x718c, 0x9294, 0xb294, 0xd39c, 0xf39c, 
// 0x14a5, 0x34a5, 0x55ad, 0x75ad, 0x96b5, 0xb6b5, 0xd7bd, 0xf7bd, 
// 0x18c6, 0x38c6, 0x59ce, 0x79ce, 0x9ad6, 0xbad6, 0xdbde, 0xfbde, 
// 0x1ce7, 0x3ce7, 0x5def, 0x7def, 0x9ef7, 0xbef7, 0xdfff, 0xffff,
// };

static uint16_t gray2rgb565_swap[64]={
0x0000, 0x0020, 0x0841, 0x0861, 0x1082, 0x10a2, 0x18c3, 0x18e3, 
0x2104, 0x2124, 0x2945, 0x2965, 0x3186, 0x31a6, 0x39c7, 0x39e7, 
0x4208, 0x4228, 0x4a49, 0x4a69, 0x528a, 0x52aa, 0x5acb, 0x5aeb, 
0x630c, 0x632c, 0x6b4d, 0x6b6d, 0x738e, 0x73ae, 0x7bcf, 0x7bef, 
0x8410, 0x8430, 0x8c51, 0x8c71, 0x9492, 0x94b2, 0x9cd3, 0x9cf3, 
0xa514, 0xa534, 0xad55, 0xad75, 0xb596, 0xb5b6, 0xbdd7, 0xbdf7, 
0xc618, 0xc638, 0xce59, 0xce79, 0xd69a, 0xd6ba, 0xdedb, 0xdefb, 
0xe71c, 0xe73c, 0xef5d, 0xef7d, 0xf79e, 0xf7be, 0xffdf, 0xffff,
};


void lcd_polling_enable(void)
{
    lcd_ctl.mode = 0;
}

void lcd_interrupt_enable(void)
{
    lcd_ctl.mode = 1;
}

/* cyj add 2019-8-15 

// Initialization sequence for st7789
// ====================================
static const uint8_t ST7789V_init[] = {
  15,                   					        // 15 commands in list
  TFT_CMD_FRMCTR2, 5, 0x0c, 0x0c, 0x00, 0x33, 0x33,
  TFT_ENTRYM, 1, 0x45,
  ST_CMD_VCOMS, 1, 0x2B,
  TFT_CMD_PWCTR1, 1, 0x2C,
  TFT_CMD_PWCTR3, 2, 0x01, 0xff,
  TFT_CMD_PWCTR4, 1, 0x11,
  TFT_CMD_PWCTR5, 1, 0x20,
  ST_CMD_FRCTRL2, 1, 0x0f,
  ST_CMD_PWCTR1, 2, 0xA4, 0xA1,
  TFT_CMD_GMCTRP1, 14, 0xD0, 0x00, 0x05, 0x0E, 0x15, 0x0D, 0x37, 0x43, 0x47, 0x09, 0x15, 0x12, 0x16, 0x19,
  TFT_CMD_GMCTRN1, 14, 0xD0, 0x00, 0x05, 0x0D, 0x0C, 0x06, 0x2D, 0x44, 0x40, 0x0E, 0x1C, 0x18, 0x16, 0x19,
  TFT_MADCTL, 1, (MADCTL_MX | 8),					// Memory Access Control (orientation)

  // have use  cyj add 2019-8-15
  TFT_CMD_PIXFMT, 1, DISP_COLOR_BITS_16,            // *** INTERFACE PIXEL FORMAT: 0x66 -> 18 bit; 0x55 -> 16 bit
  TFT_CMD_SLPOUT, TFT_CMD_DELAY, 120,				//  Sleep out,	//  120 ms delay
  TFT_DISPON, TFT_CMD_DELAY, 120,
};


//ST7789**
#define ST7789_240x240_XSTART 0
#define ST7789_240x240_YSTART 80

#define ST7789_TFTWIDTH   240       ///< ST7789 max TFT width
#define ST7789_TFTHEIGHT  240       ///< ST7789 max TFT height


#define ST_CMD_DELAY 0x80 // special signifier for command lists

#define MADCTL_MY  0x80     ///< Right to left
#define MADCTL_MX  0x40     ///< Bottom to top
#define MADCTL_MV  0x20     ///< Reverse Mode
#define MADCTL_ML  0x10     ///< LCD refresh Bottom to top
#define MADCTL_RGB 0x00     ///< Red-Green-Blue pixel order
#define MADCTL_BGR 0x08     ///< Blue-Green-Red pixel order
#define MADCTL_MH  0x04     ///< LCD refresh right to left


#define TFT_NOP   	   	  0x00      ///< No-op register
#define TFT_SWRESET    	  0x01      ///< Software reset register
#define TFT_RDDID      	  0x04      ///< Read display identification information
#define TFT_RDDST      	  0x09      ///< Read Display Status 
#define TFT_SLPIN      	  0x10      ///< Enter Sleep Mode
#define TFT_SLPOUT     	  0x11      ///< Sleep Out
#define TFT_PTLON      	  0x12      ///< Partial Mode ON
#define TFT_NORON      	  0x13      ///< Normal Display Mode ON
#define TFT_INVOFF     	  0x20      ///< Display Inversion OFF
#define TFT_INVON      	  0x21      ///< Display Inversion ON 
#define TFT_GAMMASET   	  0x26      ///< Gamma Set 
#define TFT_DISPOFF    	  0x28      ///< Display OFF 
#define TFT_DISPON     	  0x29      ///< Display ON
#define TFT_CASET      	  0x2A      ///< Column Address Set 
#define TFT_RASET      	  0x2B      ///< Page Address Set
#define TFT_RAMWR      	  0x2C      ///< Memory Write
#define TFT_RAMRD      	  0x2E      ///< Memory Read
#define TFT_PTLAR      	  0x30      ///< Partial Area
#define TFT_MADCTL     	  0x36      ///< Memory Access Control
#define TFT_VSCRSADD  	  0x37      ///< Vertical Scrolling Start Address
#define TFT_COLMOD     	  0x3A      ///< COLMOD: Pixel Format Set
#define TFT_RDID1      	  0xDA      ///< Read ID 1
#define TFT_RDID2      	  0xDB      ///< Read ID 2
#define TFT_RDID3      	  0xDC      ///< Read ID 3
#define TFT_GMCTRP1    	  0xE0      ///< Positive Gamma Correction
#define TFT_GMCTRN1    	  0xE1      ///< Negative Gamma Correction

//ST7789
#define ST7789_RGBCTL     0xB1      ///< RGB Interface Control
#define ST7789_PORCTL     0xB2      ///< Porch Setting
#define ST7789_FRMCTR1    0xB3      ///< Frame Rate Control (In Partial Mode/Idle Mode)
#define ST7789_FRMCTR2    0xC6      ///< Frame Rate Control (In Normal Mode)
#define ST7789_GCCTL      0xB7      ///< Gate Control
#define ST7789_PWCTR1     0xD0      ///< Power Control 1
#define ST7789_PWCTR2     0xE8      ///< Power Control 2
#define ST7789_LCMCTRL    0xC0      ///< LCM Control
#define ST7789_VDVVRHEN   0xC2      ///< VDV and VRH Command Enable
#define ST7789_VRHSET     0xC3      ///< VRH set
#define ST7789_VDVSET     0xC4      ///< VDV set
#define ST7789_VCMOFSET   0xC5      ///< VCOM Offset
#define ST7789_VCMOSET    0xBB      ///< VCOM Setting
#define ST7789_RDMODE     0x0A      ///< Read Display Power Mode 
#define ST7789_RDMADCTL   0x0B      ///< Read Display MADCTL
#define ST7789_RDPIXFMT   0x0C      ///< Read Display Pixel Format
#define ST7789_RDIMGFMT   0x0D      ///< Read Display Image Format 
#define ST7789_RDSELFDIAG 0x0F      ///< Read Display Self-Diagnostic Result

// Color definitions
#define ST7789_BLACK       TFT_BLACK            ///<   0,   0,   0
#define ST7789_NAVY        TFT_NAVY             ///<   0,   0, 128
#define ST7789_DARKGREEN   TFT_DARKGREEN        ///<   0, 128,   0
#define ST7789_DARKCYAN    TFT_DARKCYAN         ///<   0, 128, 128
#define ST7789_MAROON      TFT_MAROON           ///< 128,   0,   0
#define ST7789_PURPLE      TFT_PURPLE           ///< 128,   0, 128
#define ST7789_OLIVE       TFT_OLIVE            ///< 128, 128,   0
#define ST7789_LIGHTGREY   TFT_LIGHTGREY        ///< 192, 192, 192
#define ST7789_DARKGREY    TFT_DARKGREY         ///< 128, 128, 128
#define ST7789_BLUE        TFT_BLUE             ///<   0,   0, 255
#define ST7789_GREEN       TFT_GREEN            ///<   0, 255,   0
#define ST7789_CYAN        TFT_CYAN             ///<   0, 255, 255
#define ST7789_RED         TFT_RED              ///< 255,   0,   0
#define ST7789_MAGENTA     TFT_MAGENTA          ///< 255,   0, 255
#define ST7789_YELLOW      TFT_YELLOW           ///< 255, 255,   0
#define ST7789_WHITE       TFT_WHITE            ///< 255, 255, 255
#define ST7789_ORANGE      TFT_ORANGE           ///< 255, 165,   0
#define ST7789_GREENYELLOW TFT_GREENYELLOW      ///< 173, 255,  47
#define ST7789_PINK        TFT_PINK             ///< 255, 128, 192

tft_write_byte(uint8_t *data_buf, uint32_t length)

*/

int lcd_init(uint32_t freq, bool oct, uint16_t offset_w, uint16_t offset_h, bool invert_color, uint16_t width, uint16_t height)
{
    uint8_t data = 0;
    lcd_ctl.start_offset_w0 = offset_w;
    lcd_ctl.start_offset_h0 = offset_h;
    if(g_lcd_w != width || g_lcd_h != height)
    {
        if(g_lcd_display_buff)
        {
            free(g_lcd_display_buff);
        }
        g_lcd_display_buff = (uint16_t*)malloc(width*height*2);
        if(!g_lcd_display_buff)
            return 12; //ENOMEM
        g_lcd_w = width;
        g_lcd_h = height;
    }
    tft_hard_init(freq, oct);
    /*soft reset*/
    tft_write_command(SOFTWARE_RESET);
    msleep(150);
    /*exit sleep*/
    tft_write_command(SLEEP_OFF);
    msleep(500);
    /*pixel format*/
    tft_write_command(PIXEL_FORMAT_SET);
    data = 0x55;
    tft_write_byte(&data, 1);
    msleep(10);
    
    g_lcd_init = true;

    lcd_set_direction(DIR_YX_RLDU);
    if(invert_color)
    {
        tft_write_command(INVERSION_DISPALY_ON);
        msleep(10);
    }
    tft_write_command(NORMAL_DISPALY_ON);
    msleep(10);
    /*display on*/
    tft_write_command(DISPALY_ON);
    msleep(100);
    lcd_polling_enable();
 
    //cyj add 2019-10-29 
    fpioa_set_function(23, FUNC_GPIOHS0 + 7);
    gpiohs_set_drive_mode(7, GPIO_DM_OUTPUT);
    gpiohs_set_pin(7, GPIO_PV_HIGH);

    return 0;
}

void lcd_destroy()
{
    if(g_lcd_display_buff)
    {
        free(g_lcd_display_buff);
        g_lcd_display_buff = NULL;
    }
    g_lcd_w = 0;
    g_lcd_h = 0;
}


uint16_t lcd_get_width()
{
    return g_lcd_w;
}

uint16_t lcd_get_height()
{
    return g_lcd_h;
}


void lcd_set_direction(lcd_dir_t dir)
{
    if(!g_lcd_init)
        return;
    //dir |= 0x08;  //excahnge RGB
    lcd_ctl.dir = dir;
    if (dir & DIR_XY_MASK)
    {
        lcd_ctl.width = g_lcd_w - 1;
        lcd_ctl.height = g_lcd_h - 1;
        lcd_ctl.start_offset_w = lcd_ctl.start_offset_h0;
        lcd_ctl.start_offset_h = lcd_ctl.start_offset_w0;
    }
    else
    {
        lcd_ctl.width = g_lcd_h - 1;
        lcd_ctl.height = g_lcd_w - 1;
        lcd_ctl.start_offset_w = lcd_ctl.start_offset_w0;
        lcd_ctl.start_offset_h = lcd_ctl.start_offset_h0;
    }
    
    tft_write_command(MEMORY_ACCESS_CTL);
    tft_write_byte((uint8_t *)&dir, 1);
}

static uint32_t lcd_freq = CONFIG_LCD_DEFAULT_FREQ;
void lcd_set_freq(uint32_t freq)
{
    tft_set_clk_freq(freq);
    lcd_freq = freq;
}

uint32_t lcd_get_freq()
{
    return lcd_freq;
}


void lcd_set_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t data[4] = {0};

    x1 += lcd_ctl.start_offset_w;
    x2 += lcd_ctl.start_offset_w;
    y1 += lcd_ctl.start_offset_h;
    y2 += lcd_ctl.start_offset_h;

    data[0] = (uint8_t)(x1 >> 8);
    data[1] = (uint8_t)(x1);
    data[2] = (uint8_t)(x2 >> 8);
    data[3] = (uint8_t)(x2);
    tft_write_command(HORIZONTAL_ADDRESS_SET);
    tft_write_byte(data, 4);

    data[0] = (uint8_t)(y1 >> 8);
    data[1] = (uint8_t)(y1);
    data[2] = (uint8_t)(y2 >> 8);
    data[3] = (uint8_t)(y2);
    tft_write_command(VERTICAL_ADDRESS_SET);
    tft_write_byte(data, 4);

    tft_write_command(MEMORY_WRITE);
}

void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_set_area(x, y, x, y);
    tft_write_byte((uint8_t*)&color, 2);
}


//cyj add 2019-10-21 process chinese begin
uint8_t if_debug=0;
int	TFT_X = 0;
int	TFT_Y = 0;
uint16_t hzcolor;
unsigned char key_bit[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
uint8_t hzk_flag,Cur_HZK16[32];
uint8_t Get_Curent_HZK(uint32_t hz_offset)
{   
    
    uint32_t offset;
    if(hzk_flag==0)for(offset=0;offset<32;offset++)Cur_HZK16[offset]=hzk16[hz_offset + offset];
    else for(offset=0;offset<32;offset++)Cur_HZK16[offset]=unic16hzk[hz_offset + offset];
    return 0;
}


void lcd_draw_Dot8(uint16_t x, uint16_t y, char ch, uint16_t color)
{
    uint8_t j = 0;
    uint8_t data = 0;
    data = ch;
    //data=1;
    for (j = 0; j < 8; j++)
        {
            if (data & 0x80)
                lcd_draw_point(x + j, y, color);
            data <<= 1;
        }
   
}



void hz_disp()
{
   uint8_t hz_col,hz_row,ifbit;
   for(hz_row=0;hz_row<16;hz_row++)
     for(hz_col=0;hz_col<2;hz_col++) 
           {
               ifbit=0;
               //if(if_debug)mp_printf(&mp_plat_print, "hzdot =:%x",Cur_HZK16[hz_col +hz_row*2]);
               //for(ifbit=0;ifbit<8;ifbit++)
                   //if(key_bit[ifbit] & Cur_HZK16[hz_col +hz_row*2])
                         lcd_draw_Dot8(TFT_X+hz_col*8 + ifbit, TFT_Y+hz_row, Cur_HZK16[hz_col +hz_row*2],hzcolor); 
                                   		
           }     
}




//process chinese end 

void lcd_draw_char(uint16_t x, uint16_t y, char c, uint16_t color)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t data = 0;

    for (i = 0; i < 16; i++)
    {
        data = ascii0816[c * 16 + i];
        for (j = 0; j < 8; j++)
        {
            if (data & 0x80)
                lcd_draw_point(x + j, y, color);
            data <<= 1;
        }
        y++;
    }
}

void lcd_draw_string(uint16_t x, uint16_t y, char *str, uint16_t color)
{
    // chinese middle var  cyj add 2019-10-21
    uint8_t ch;        
    uint8_t hz_h,hz_l,hz_3;
    uint16_t hz_unic;
    uint32_t hz_offset,err;
    uint8_t i = 0;

    TFT_X = x;
    TFT_Y = y;
    hzcolor = color;

    while (*str)
    {
        //proces chinest char set  cyj add 2019-10-21
        ch = *str; // get string character
        
        //chinese proc 16*16  cyj 
        if((ch & 0xf0) == 0xe0)
                { 
                  hz_h = ch;
                  str++;
                  hz_l = *str;
                  str++;
                  hz_3 = *str;
                  printk("cur is unic hzk code,h=%d,m=%d,L=%d\r\n",hz_h,hz_l,hz_3);  //cyj add 2019-10-22 
                  if((hz_l & 0xc0) == 0x80 && (hz_3 & 0xc0) == 0x80 )
                     {
                        //uint32_t utf8 = (0x00ff0000 & (uint32_t)hz_h << 16) | (0x0000ff00 & (uint32_t)hz_l << 8) | (0x000000ff & (uint32_t)hz_3);
                        hz_unic = hz_h;
                        hz_unic = (hz_unic<<12);
                        hz_l = (hz_l <<2);      
                        hz_unic +=(hz_l <<4);
                        hz_3 &=0x3f;
                        hz_unic +=hz_3;
                        //if(if_debug)mp_printf(&mp_plat_print, "hzunic =:%x",hz_unic); 
                        hzk_flag=1;
                        hz_offset = (hz_unic - 0x4e00)*32;			   
                        err = Get_Curent_HZK(hz_offset);
                        hz_disp();			
                        TFT_X+=16;   //1pix space 
                        x=TFT_X;
                        str++;
                     }
                 }
         //gb2312
         else if (ch > 0xA0) {                           
                           hz_h = ch;
                           str++;
                           hz_l = *str;
                           printk("cur is gb2312 hzk code,h=%d,L=%d \r\n",hz_h,hz_l);  //cyj add 2019-10-22 
                           //hz_h = 0xc3;
                           //hz_l = 0xc0;
                           hzk_flag=0;
                           hz_offset = (94*(hz_h-0xa0-1)+(hz_l-0xa0-1))*32;			   
                           err = Get_Curent_HZK(hz_offset);
                           //if(if_debug)mp_printf(&mp_plat_print, "hz:h=%d,l=%d offset=: %d,err=:%d",hz_h,hz_l,hz_offset,err);
                           hz_disp();			
                           TFT_X+=16;   //1pix space  
                           x=TFT_X; 
                           str++;                        
                       }
        else
            {
		//old process englisht part
                printk("cur is enligsh code %d\r\n",*str);  //cyj add 2019-10-21 
		lcd_draw_char(x, y, *str, color);
		str++;
		x += 8;
                TFT_X=x;                         
            }
    }
}

/*

cyj add 2019-10-21

*/

void lcd_ram_draw_string(char *str, uint32_t *ptr, uint16_t font_color, uint16_t bg_color)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t data = 0;
    uint8_t *pdata = NULL;
    uint16_t width = 0;
    uint32_t *pixel = NULL;

    font_color = (font_color<<8) | (font_color>>8&0x00ff);
    bg_color   = (bg_color<<8)   | (bg_color>>8&0x00ff);

    width = 4 * strlen(str);
    while (*str)
    {
        pdata = (uint8_t *)&ascii0816[(*str) * 16];  //pdata point to dot buffer
        for (i = 0; i < 16; i++)
        {
            data = *pdata++;
            pixel = ptr + i * width;  //pixel address set 
            for (j = 0; j < 4; j++)
            {
                switch (data >> 6)  //high 2 bit porcess 
                {	
                    case 0:
                        *pixel =  bg_color | ((uint32_t)bg_color << 16);
                        break;
                    case 2:
                        *pixel = font_color | ((uint32_t)bg_color << 16) ;
                        break;
                    case 1:
                        *pixel = bg_color | ((uint32_t)font_color << 16) ;
                        break;
                    case 3:
                        *pixel = font_color | ((uint32_t)font_color << 16) ;
                        break;
                    default:
                        *pixel = 0;
                        break;
                }
                data <<= 2;
                pixel++;
            }
        }
        str++;
        ptr += 4;
    }
}

void lcd_clear(uint16_t color)
{
    uint32_t data = ((uint32_t)color << 16) | (uint32_t)color;
    lcd_set_area(0, 0, lcd_ctl.width, lcd_ctl.height);
    tft_fill_data(&data, g_lcd_h * g_lcd_w / 2);
}

void lcd_fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	if((x1 == x2) || (y1 == y2)) return;
	uint32_t data = ((uint32_t)color << 16) | (uint32_t)color;
    lcd_set_area(x1, y1, x2-1, y2-1);
    tft_fill_data(&data, (x2 - x1) * (y2 - y1) / 2);
}

void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, uint16_t color)
{
    uint32_t data_buf[640] = {0};
    uint32_t *p = data_buf;
    uint32_t data = color;
    uint32_t index = 0;

    data = (data << 16) | data;
    for (index = 0; index < 160 * width; index++)
        *p++ = data;

    lcd_set_area(x1, y1, x2, y1 + width - 1);
    tft_write_byte((uint8_t*)data_buf, ((x2 - x1 + 1) * width + 1)*2 );
    lcd_set_area(x1, y2 - width + 1, x2, y2);
    tft_write_byte((uint8_t*)data_buf, ((x2 - x1 + 1) * width + 1)*2 );
    lcd_set_area(x1, y1, x1 + width - 1, y2);
    tft_write_byte((uint8_t*)data_buf, ((y2 - y1 + 1) * width + 1)*2 );
    lcd_set_area(x2 - width + 1, y1, x2, y2);
    tft_write_byte((uint8_t*)data_buf, ((y2 - y1 + 1) * width + 1)*2 );
}


#define SWAP_16(x) ((x>>8&0xff) | (x<<8))

typedef int (*dual_func_t)(int);
extern volatile dual_func_t dual_func;
static uint16_t* g_pixs_draw_pic = NULL;
static uint32_t g_pixs_draw_pic_size = 0;

static int swap_pixs_half(int core)
{
    uint32_t i;
    uint16_t* p = g_pixs_draw_pic;
    for(i=g_pixs_draw_pic_size/2; i<g_pixs_draw_pic_size ; i+=2)
    {
        g_lcd_display_buff[i] = SWAP_16(*(p+1));
        g_lcd_display_buff[i+1] = SWAP_16(*(p));
        p+=2;
    }
    return 0;
}

void lcd_draw_picture(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint32_t *ptr)
{
    uint32_t i;
    uint16_t* p = (uint16_t*)ptr;
    g_pixs_draw_pic_size = width*height;
    lcd_set_area(x1, y1, x1 + width - 1, y1 + height - 1);
    g_pixs_draw_pic = p+g_pixs_draw_pic_size/2;
    dual_func = swap_pixs_half;
    for(i=0; i< g_pixs_draw_pic_size/2; i+=2)
    {
        g_lcd_display_buff[i] = SWAP_16(*(p+1));
        g_lcd_display_buff[i+1] = SWAP_16(*(p));
        p+=2;
    }
    while(dual_func){}
    tft_write_word((uint32_t*)g_lcd_display_buff, width * height / 2);
}

//draw pic's roi on (x,y)
//x,y of LCD, w,h is pic; rx,ry,rw,rh is roi
void lcd_draw_pic_roi(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t rx, uint16_t ry, uint16_t rw, uint16_t rh, uint32_t *ptr)
{	
	int y_oft;
	uint8_t* p;
	for(y_oft = 0; y_oft < rh; y_oft++)
	{	//draw line by line
		p = (uint8_t *)(ptr) + w*2*(y_oft+ry) + 2*rx;
		lcd_set_area(x, y+y_oft, x + rw - 1, y+y_oft);
		tft_write_byte((uint8_t*)p, rw*2);//, lcd_ctl.mode ? 2 : 0);
	}
}


void lcd_draw_pic_gray(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, uint8_t *ptr)
{	
    uint32_t i;
    lcd_set_area(x1, y1, x1 + width - 1, y1 + height - 1);
    uint32_t size = width*height;
    for(i=0; i< size; i+=2)
    {
        g_lcd_display_buff[i] = gray2rgb565_swap[ptr[i+1]>>2];
        g_lcd_display_buff[i+1] = gray2rgb565_swap[ptr[i]>>2];
    }
    tft_write_word((uint32_t*)g_lcd_display_buff, width * height / 2);
}

void lcd_draw_pic_grayroi(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t rx, uint16_t ry, uint16_t rw, uint16_t rh, uint8_t *ptr)
{
	int y_oft;
	uint8_t* p;
	for(y_oft = 0; y_oft < rh; y_oft++)
	{	//draw line by line
		p = (uint8_t *)(ptr) + w*(y_oft+ry) + rx;
		lcd_draw_pic_gray(x, y+y_oft, rw, 1, p);
	}
	return;
}

void lcd_ram_cpyimg(char* lcd, int lcdw, char* img, int imgw, int imgh, int x, int y)
{
	int i;
	for(i=0;i<imgh;i++)
	{
		memcpy(lcd+lcdw*2*(y+i)+x*2,img+imgw*2*i,imgw*2);
	}
	return;
}




