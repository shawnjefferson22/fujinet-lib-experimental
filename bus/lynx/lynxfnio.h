/**
 *  for Atari Lynx
 */

#ifndef LYNXFNIO_H
#define LYNXFNIO_H

#include "fujinet-int.h"

// Max message size
#define FNIO_TX_LEN_MAX       1025             // max disk block size + compression field
//#define SERIAL_PACKET_SIZE    256

#define LYNX_TIMEOUT          10

enum FNIO_ERROR_T {
        FNIO_ERR_NONE,       // success
        FNIO_ERR_TIMEOUT,    // timed out waiting for data
        FNIO_ERR_SEND_CHK,   // checksum on our sent data was bad
        FNIO_ERR_RECV_CHK,   // checksum on received data was bad
        FNIO_ERR_NO_DATA,    // no data received
        FNIO_ERR_CMD_FAILED, // command failed on Fujinet side
        FNIO_ERR_GENERAL     // undefined error
};

// Some globals
extern unsigned char _fn_error;


// helper functions
uint8_t _checksum(char *b, uint16_t len);
bool _serial_get_loop(char *b);
bool _serial_recv_bytes(char *buf, uint16_t len);

// main functions
uint8_t fnio_error();
uint8_t fnio_init(void);
uint8_t fnio_done(void);
bool fnio_send_buf(uint8_t dev, char *buf, uint16_t len);
bool fnio_recv_buf(char *buf, uint16_t *len, uint16_t maxlen);
uint8_t fnio_recv_ack(void);
void fnio_flush_recv(void);

#endif /* FUJINET_H */
