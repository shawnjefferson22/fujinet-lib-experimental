#include "fujinet-fuji.h"
#include "fujinet-network.h"
#include <stdio.h>
#include <string.h>
#include <tgi.h>


AdapterConfig ac;
char buffer[256];
char s[64];
unsigned char status;

int main()
{
   // Setup TGI
  tgi_install(tgi_static_stddrv);
  tgi_init();
  tgi_setcolor(TGI_COLOR_WHITE);
  tgi_setbgcolor(TGI_COLOR_BLACK);
  tgi_clear();

  // setup joystick
  //joy_install(joy_static_stddrv);



                     //012345678901234567890
  tgi_outtextxy(1, 1, "Searching for FN...");
  if (!fuji_get_adapter_config(&ac))
    strcpy(ac.fn_version, "FAIL");

  sprintf(s, "FN: %-14s", ac.fn_version);
  tgi_outtextxy(1, 9, s);


  if (!fuji_get_wifi_status(&status))
    tgi_outtextxy(1, 20, "FAIL:get_wifi_status");
  if (status == connected)
    tgi_outtextxy(1, 20, "Wifi connected!");

  // Hello FujiNet!  
  network_open("N1:https://httpbin.org/base64/SGVsbG8gRnVqaU5ldCE=", 4, 0);
  network_read("N1", buffer, 14);
  buffer[14] = '\0';
  network_close("N1:");

  tgi_outtextxy(1, 40, "Hello Fujinet!");
  tgi_outtextxy(1, 48, buffer);

  /*
  if (!fuji_get_host_prefix(1, &buffer))
    tgi_outtextxy(1,28, "FAIL: get_host_prefix");
  sprintf(s, "Prefix: %s\n", buffer);
  tgi_outtextxy(1, 37, s);
*/

  while(1);
  return 0;
}
