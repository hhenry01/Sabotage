#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdlib.h>
#include "hwlib.h"
#include "address_map.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
#include "bluetooth_driver.h"

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

/*
 * Establish a virtual address mapping for the physical addresses starting at base, and
 * extending by span bytes.
 */
void* map_physical(int fd, unsigned int base, unsigned int span)
{
   void *virtual_base;

   // Get a mapping from physical addresses to virtual addresses
   virtual_base = mmap (NULL, span, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, base);
   if (virtual_base == MAP_FAILED)
   {
      printf ("ERROR: mmap() failed...\n");
      close (fd);
      return (NULL);
   }
   return virtual_base;
}

int init(void) {
  int fd;
  if ((fd = open("/dev/mem", (O_RDWR | O_SYNC))) == -1) {
    printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
  }

  // void *virtual_base = mmap(NULL, HW_REGS_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, HW_REGS_BASE);

	// if( virtual_base == MAP_FAILED ) {
	// 	printf( "ERROR: mmap() failed...\n" );
	// 	close( fd );
	// 	return( 1 );
	// }

  void *virtual_base;
  if ((virtual_base = map_physical(fd, 0xff200000, 0x5000)) == NULL) {
    return -1;
  }

  // void *h2p_lw_bt_addr = virtual_base + ((unsigned long)(ALT_LWFPGASLVS_OFST + BLUETOOTH_UART_BASE)
  //                        & (unsigned long)(HW_REGS_MASK));
  void *h2p_lw_bt_addr = virtual_base + BLUETOOTH_UART_BASE;
  printf("bt_addr: %x\n", (int)h2p_lw_bt_addr);
  bluetooth_inst = malloc(sizeof(bluetooth_uart_t));
  // TODO error check for malloc fail.
  bluetooth_inst->addr = h2p_lw_bt_addr;
  bluetooth_inst->fd = fd;
  bluetooth_inst->rxdata = h2p_lw_bt_addr + RXDATA_OFST;
  bluetooth_inst->txdata = h2p_lw_bt_addr + TXDATA_OFST;
  bluetooth_inst->status = h2p_lw_bt_addr + STATUS_OFST;
  bluetooth_inst->ctrl   = h2p_lw_bt_addr + CONTRL_OFST;
  bluetooth_inst->divisor= h2p_lw_bt_addr + DIVISR_OFST;
  bluetooth_inst->eop    = h2p_lw_bt_addr + EOP_OFST;
  printf("%x\n", *((int*) h2p_lw_bt_addr));
  printf("%x\n", *((int*) (h2p_lw_bt_addr + 1)));
  return 0;
}

int kill(void) {
  uint16_t *tx = bluetooth_inst->txdata;
  printf("%x\n", *tx);
  // uint16_t data = tx & DATA_MSK;
  // tx ^= data;
  // tx |= (int)CMD;
  uint8_t i = 0;
  for (i = 0; i < 3; i++) {
    printf("Wait %d\n", i);
    while(!(*tx & TXRDY_MSK)); // Wait
    *tx |= (int)CMD;
  }
  printf("Wait 3\n");
  while(!(*tx & TXRDY_MSK)); // Wait
  *tx |= (int)'K';
  return 0;
}

int main(void) {
  init();
  kill();
  return 0;
}