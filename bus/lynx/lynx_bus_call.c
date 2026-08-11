#include "fujinet-fuji.h"
#include "fujinet-int.h"
#include "lynxfnio.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>


char _lynx_packet[1024];
uint16_t _lynx_len;
uint16_t _comlynx_init = 0;



/*bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields,
		   uint8_t aux1, uint8_t aux2, uint8_t aux3, uint8_t aux4,
		   const void *data, size_t data_length,
		   void *reply, size_t reply_length)
*/

bool fuji_bus_call(uint8_t device, uint8_t fuji_cmd, uint8_t fields, ...)
{
  uint8_t r;
  uint8_t numbytes;
  va_list ap;


  // Is Comlynx initialized?
  if (!_comlynx_init) {
    if (!fnio_init())
      return(false);
    _comlynx_init = 1;
  }

  // Reset our data length counter
  _lynx_len = 0;

  // Command is first
  _lynx_packet[_lynx_len] = fuji_cmd;
  _lynx_len++;

  // Build the packet from aux values
  /*numbytes = fuji_field_numbytes(fields);
  if (numbytes) {
    _lynx_packet[_lynx_len++] = aux1;
    numbytes--;
  }
  if (numbytes) {
    _lynx_packet[_lynx_len++] = aux2;
    numbytes--;
  }
  if (numbytes) {
    _lynx_packet[_lynx_len++] = aux3;
    numbytes--;
  }
  if (numbytes) {
    _lynx_packet[_lynx_len++] = aux4;
    numbytes--;
  }*/

  numbytes = fuji_field_numbytes(fields);
  if (numbytes > 0)
    _lynx_packet[_lynx_len++] = va_arg(ap, uint8_t);
  if (numbytes > 1)
    _lynx_packet[_lynx_len++] = va_arg(ap, uint8_t);
  if (numbytes > 2)
    _lynx_packet[_lynx_len++] = va_arg(ap, uint8_t);
  if (numbytes > 3)
    _lynx_packet[_lynx_len++] = va_arg(ap, uint8_t);
  if (fields & FUJI_FIELD_DATA) {
    const uint8_t *data = va_arg(ap, uint8_t *);
    const uint16_t data_length = va_arg(ap, uint16_t);
    
    memcpy(&_lynx_packet[_lynx_len], data, data_length);
    _lynx_len += data_length;
  }

  // Add data if it exists
  /*if (data) {
    memcpy(&_lynx_packet[_lynx_len], data, data_length);
    _lynx_len += data_length;
  }*/

  // Send the command (and data)
  r = fnio_send_buf(device, &_lynx_packet[0], _lynx_len);
  if (!r)
    return(false);

  if ((fields & FUJI_FIELD_REPLY)) {
    uint8_t *reply = va_arg(ap, uint8_t *);
    uint16_t reply_length = va_arg(ap, uint16_t);

    r = fnio_recv_buf(reply, &_lynx_len, reply_length);
    if ((!r) || (_lynx_len == 0))
      return(false);
  }
  // Get ACK from Fujinet that command succeeded
  else {
    r = fnio_recv_ack();
    if (!r)
      return(false);
  }

  // Get reply if one expected
  /*if (reply) {
    r = fnio_recv_buf(reply, &_lynx_len, reply_length);
    if ((!r) || (_lynx_len == 0))
      return(false);
  }
  // Get ACK from Fujinet that command succeeded
  else {
    r = fnio_recv_ack();
    if (!r)
      return(false);
  }*/

  va_end(ap);
  return(true);
}
