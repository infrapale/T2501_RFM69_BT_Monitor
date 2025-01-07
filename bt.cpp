#include "main.h"
#include "atask.h"


typedef struct
{
    String rx_str; 
    bool    rx_avail;
} bt_st;

atask_st bt_handle                 = {"Bluetooth      ", 100,0, 0, 255, 0, 1, bt_task};


void bt_initialize(void)
{
    atask_add_new(&bt_handle);
}

void bt_rd_uart(void)
{

    if (SerialX.available())
    {
     bt.rx_str = SerialX.readStringUntil('\n');
    if (bt.rx_str.length()> 0)
    {
        bt.rx_avail = true;
        Serial.println("BT rx is available");
    }
}

 // if ((uart.rx.str.charAt(0) != '<') || 