#include "main.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "atask.h"
#include "alphaled.h"

#define  ALPHA_LED_BUFF_LEN  80
Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

typedef struct 
{
  char    src_buff[ALPHA_LED_BUFF_LEN];
  char    disp_buff[4+1];
  uint8_t cursor;
} alphaled_st;

alphaled_st alphaled;
atask_st alpha_handle                 = {"Alpha LED     ", 300,0, 0, 255, 0, 1, alphaled_task};

void alphaled_fill_disp(void)
{
  uint8_t buff_pos = alphaled.cursor;
  for (uint8_t led_pos = 0; led_pos < 4; led_pos++)
  {
    alphaled.disp_buff[led_pos] = alphaled.src_buff[buff_pos];
    buff_pos++;
    if (buff_pos >= ALPHA_LED_BUFF_LEN ) buff_pos = 0;
    if (alphaled.src_buff[buff_pos] == 0x00) buff_pos = 0;
  }
  alphaled.cursor++;
  if (alphaled.cursor >= ALPHA_LED_BUFF_LEN ) alphaled.cursor = 0;
  if (alphaled.src_buff[alphaled.cursor] == 0x00) alphaled.cursor = 0;
}
void alphaled_initialize(void)
{
  atask_add_new(&alpha_handle);  
}

void alphaled_task(void)
{
  switch(alpha_handle.state)
  {
    case 0:
        alpha4.begin(0x72);
        alphaled.cursor = 0;
        strncpy(alphaled.src_buff, APP_NAME, ALPHA_LED_BUFF_LEN);
        alphaled_fill_disp();
        alpha_handle.state = 10;
        break;
    case 10:
        for (uint8_t led_pos = 0; led_pos < 4; led_pos++)
        {
            alpha4.writeDigitAscii(led_pos, alphaled.disp_buff[led_pos]);
        }
        alpha4.writeDisplay();
        //alpha_handle.state = 20;
        alphaled_fill_disp();
        break;
    case 20:
        alpha_handle.state = 30;
        break;
    case 30:
        alpha_handle.state = 10;
        break;

  }
}

void alphaled_print(char *new_txt)
{
    strncpy(alphaled.src_buff, new_txt, ALPHA_LED_BUFF_LEN);
    alphaled.cursor = 0;
}