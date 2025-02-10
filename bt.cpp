#include "main.h"
#include "atask.h"
#include "rfm_send.h"

#define BT_MENU_ITEMS  10

typedef void (*task_cb)(void);

typedef struct 
{
    char tag;
    char label[16];
    char msg[80];
} bt_menu_st;

typedef struct 
{
    uint8_t sub;
    char tag;
    char label[16];
    uint8_t next_sub;
    uint8_t msg_indx;
    task_cb cb;
} bt_menu2_st;



typedef struct
{
    String rx_str; 
    bool    rx_avail;
} bt_st;

void dummy_cb(){}

bt_menu2_st menu[] 
{   //         0123456789012345
    { 0, 'T', "Time           ", 1, 0, dummy_cb },
    { 0, 'P', "Print Today    ", 0, 0, dummy_cb },
    { 0, 'D', "Directory      ", 1, 0, dummy_cb },
    { 0, 'S', "Send Sensor    ", 2, 0, dummy_cb },
    { 0, 'R', "Send Relay     ", 0, 0, dummy_cb },
    { 1, 'P', "Print Time     ", 1, 0, dummy_cb },
    { 1, 'S', "Set Time       ", 1, 0, dummy_cb },
    { 1, 'Q', "Quit           ", 0, 0, dummy_cb },
    { 2, 'W', "Water Temp     ", 1, 1, dummy_cb },
    { 2, 'O', "Outdoor Temp   ", 1, 2, dummy_cb },
    { 2, 'Q', "Quit           ", 0, 2, dummy_cb },

}


bt_menu_st bt_menu[BT_MENU_ITEMS] =
{
    //     012345678901
    {'A', "Water",      "{\"Z\":\"Dock\",\"S\":\"Water\",\"V\":1.10,\"R\":\"\"}"},
    {'B', "OD",         "{\"Z\":\"VA_OD\",\"S\":\"Temp\",\"V\":1.00,\"R\":\"\"}"},
    {'C', "Tupa",       "{\"Z\":\"Tupa\",\"S\":\"Temp\",\"V\":20.20,\"R\":\"\"}"},
    {'D', "Parvi",      "{\"Z\":\"Parvi\",\"S\":\"Temp\",\"V\":26.10,\"R\":\"\"}"},
    {'E', "Parvi",      "{\"Z\":\"Parvi\",\"S\":\"Temp\",\"V\":26.10,\"R\":\"\"}"},
    {'F', "Parvi",      "{\"Z\":\"Parvi\",\"S\":\"Temp\",\"V\":26.10,\"R\":\"\"}"},
    {'1', "Parvi On",   "{\"Z\":\"TK1\",\"S\":\"RPARV\",\"V\":\"0\",\"R\":\"\"}"},
    {'2', "Tupa Toggle","{\"Z\":\"TK1\",\"S\":\"RTUP1\",\"V\":\"T\",\"R\":\"\"}"},
    {'3', "Sauna On",   "{\"Z\":\"TK1\",\"S\":\"RSAUN\",\"V\":\"1\",\"R\":\"\"}"},
    {'4', "Sauna Off",  "{\"Z\":\"TK1\",\"S\":\"RSAUN\",\"V\":\"0\",\"R\":\"\"}"},
    /*
    {'1', "R-2-1 On",   "{\"Z\":\"TK1\",\"S\":\"R-2-1\",\"V\":\"1\",\"R\":\"\"}"},
    {'2', "R-2-1 Off",  "{\"Z\":\"TK1\",\"S\":\"R-2-1\",\"V\":\"0\",\"R\":\"\"}"},
    {'3', "R-2-2 On",   "{\"Z\":\"TK1\",\"S\":\"R-2-2\",\"V\":\"1\",\"R\":\"\"}"},
    {'4', "R-2-2 Off",  "{\"Z\":\"TK1\",\"S\":\"R-2-2\",\"V\":\"0\",\"R\":\"\"}"},
    {'5', "R-2-4 On",   "{\"Z\":\"TK1\",\"S\":\"R-2-4\",\"V\":\"1\",\"R\":\"\"}"},
    {'6', "R-2-4 Off",  "{\"Z\":\"TK1\",\"S\":\"R-2-4\",\"V\":\"0\",\"R\":\"\"}"},
    {'7', "R-2-6 On",   "{\"Z\":\"TK1\",\"S\":\"R-2-6\",\"V\":\"1\",\"R\":\"\"}"},
    {'8', "R-2-6 Off",  "{\"Z\":\"TK1\",\"S\":\"R-2-6\",\"V\":\"0\",\"R\":\"\"}"},
    {'9', "R-2-7 On",   "{\"Z\":\"TK1\",\"S\":\"R-2-7\",\"V\":\"1\",\"R\":\"\"}"},
    {'0', "R-2-7 Off",  "{\"Z\":\"TK1\",\"S\":\"R-2-7\",\"V\":\"0\",\"R\":\"\"}"},
    */

};

void bt_task(void);

atask_st bt_handle                 = {"Bluetooth      ", 10,0, 0, 255, 0, 1, bt_task};

bt_st bt;

void bt_initialize(void)
{
    atask_add_new(&bt_handle);
}

void bt_print_menu(void)
{
    for( uint8_t i = 0; i < BT_MENU_ITEMS; i++)
    {
        SerialX.print(bt_menu[i].tag);
        SerialX.print("> ");
        SerialX.println(bt_menu[i].label);
    }
}

void bt_rd_uart(void)
{
    if (SerialX.available())
    {
        bt.rx_str = SerialX.readStringUntil(0x0D);
        if (bt.rx_str.length()> 0)
        {
            bt.rx_avail = true;
            //Serial.println("BT rx is available");
            Serial.print(bt.rx_str);
            bt_print_menu();
        }
    }
}


void bt_run_menu(char c)
{
    bool menu_ok = false;
    char cup = c;
    if ((c >='a') && (c<='z')) cup += 'A'-'a';
    for( uint8_t i = 0; i < BT_MENU_ITEMS; i++)
    {
        if (cup == bt_menu[i].tag)
        {
          SerialX.println(bt_menu[i].msg);
          rfm_send_radiate_msg(bt_menu[i].msg);
          menu_ok = true;
          break;
        }
    }
    if (!menu_ok && (c != 0x0a) && (c != 0x0d)) 
    {
        Serial.println(c,HEX); 
        bt_print_menu();

    }
   
}
 // if ((uart.rx.str.charAt(0) != '<') || 

 void bt_task(void)
 {
    switch(bt_handle.state)
    {
      case 0:
        bt_handle.state = 10;
        break;
      case 10:
          //bt_rd_uart();
          if(SerialX.available())
          {
            char c = SerialX.read();
            bt_run_menu(c);

            // if (c == 'H')
            // {
            //     Serial.println(c,HEX);
            //     bt_print_menu();
            // }

          }
          break;
    }
 }