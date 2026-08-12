/**
 *  for Atari Lynx
*/

#include <lynx.h>
#include <serial.h>
#include <stdlib.h>
#include <stdio.h>
#include "lynxfnio.h"
#include "fujinet-commands.h"



bool fnio_recv_buf(char *buf, unsigned int *len, unsigned int maxlen)
{
  //uint16_t i;
  char t;
  char _r;
  uint8_t _ck;


  // reset error status
  _fn_error = FNIO_ERR_NONE;

  // Get first length byte
  if(!_serial_get_loop(&t))
    return false;
  *len = t << 8;

  // Get second length byte
  if(!_serial_get_loop(&t))
    return false;
  *len |= t & 0xFF;

  if ((*len > maxlen) || (*len > FNIO_TX_LEN_MAX)) {
    fnio_flush_recv();
    return false;
  }

  if (!_serial_recv_bytes(buf, *len))
    return(false);

  // Get the checksum
  if (!_serial_get_loop(&t))
    return false;

  // checksum matches?
  _ck = _checksum(buf, *len);
  if (t == _ck) {
    ser_put(FUJICMD_ACK); // ACK
    ser_get(&_r);         // get reflected data
    return true;          // succes, checksum matches
  }

  _fn_error = FNIO_ERR_RECV_CHK;
  ser_put(FUJICMD_NAK);   // NACK, checksum bad
  ser_get(&_r);           // get reflected data
  *len = 0;               // return zero length
  return false;           // checksum bad
}
