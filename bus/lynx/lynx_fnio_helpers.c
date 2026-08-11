/**
 *  for Atari Lynx
 */

#include <lynx.h>
#include <serial.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "lynxfnio.h"
#include "fujinet-commands.h"



/**
 * @brief calculates the checksum on the packet and stores in global var
 * @param *b pointer to buffer
 * @param len length of buffer
 */
uint8_t _checksum(char *b, uint16_t len)
{
  uint16_t i;
  uint8_t _ck;

  // initialize checksum to zero
  _ck = 0;

  for (i = 0; i < len; ++i)
    _ck ^= b[i];

  return _ck;
}


/**
 * @brief loop to get data that is used many times
 * @param b The buffer to receive into
 */
bool _serial_get_loop(char *b)
{
  clock_t start;
  start = clock();

  while (1) {
    if (ser_get(b) == SER_ERR_OK)
      return true;

    if ((clock() - start) > (LYNX_TIMEOUT * CLOCKS_PER_SEC)) {
      _fn_error = FNIO_ERR_TIMEOUT;
      return false;
    }
  }
}


bool _serial_recv_bytes(char *buf, unsigned int len)
{
    uint16_t i;
    uint8_t _r;
    clock_t start = clock();


    for (i = 0; i < len; i++) {
        while (ser_get(&_r) == SER_ERR_NO_DATA) {
            if ((clock() - start) > (LYNX_TIMEOUT * CLOCKS_PER_SEC)) {
                _fn_error = FNIO_ERR_TIMEOUT;
                return false;
            }
        }

        buf[i] = _r;
    }

    return true;
}


