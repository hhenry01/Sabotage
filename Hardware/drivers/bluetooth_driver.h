#include <stdint.h>
#include "address_map.h"

#define RXDATA_OFST 0x0
#define TXDATA_OFST 0x1
#define STATUS_OFST 0x2
#define CONTRL_OFST 0x3
#define DIVISR_OFST 0x4
#define EOP_OFST    0x5

#define DATA_MSK    0x3F
#define TXRDY_MSK   0x40

#define CMD '$'

typedef struct bluetooth_uart {
  uint16_t *rxdata;
  uint16_t *txdata;
  uint16_t *status;
  uint16_t *ctrl;
  uint16_t *divisor;
  uint16_t *eop;
  int fd;
  void *addr;
} bluetooth_uart_t;

bluetooth_uart_t *bluetooth_inst;

int init(void);

int kill(void);