#ifndef _ALTERA_ADDRESS_MAP_H_
#define _ALTERA_ADDRESS_MAP_H_

/*
 * This file was automatically generated by the swinfo2header utility.
 * 
 * Created from SOPC Builder system 'Computer_System' in
 * file '../rtl/Computer_System.sopcinfo'.
 */

/*
 * This file contains macros for module 'ARM_A9_HPS' and devices
 * connected to the following masters:
 *   h2f_axi_master
 *   h2f_lw_axi_master
 * 
 * Do not include this header file and another header file created for a
 * different module or master group at the same time.
 * Doing so may result in duplicate macro names.
 * Instead, use the system header file which has macros with unique names.
 */

/*
 * Macros for device 'SDRAM', class 'altera_avalon_new_sdram_controller'
 * The macros are prefixed with 'SDRAM_'.
 * The prefix is the slave descriptor.
 */
#define SDRAM_COMPONENT_TYPE altera_avalon_new_sdram_controller
#define SDRAM_COMPONENT_NAME SDRAM
#define SDRAM_BASE 0x0
#define SDRAM_SPAN 67108864
#define SDRAM_END 0x3ffffff
#define SDRAM_CAS_LATENCY 3
#define SDRAM_CONTENTS_INFO 
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 2
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_POWERUP_DELAY 100.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 25
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 10
#define SDRAM_SDRAM_DATA_WIDTH 16
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 13
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 1
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_T_AC 5.5
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 20.0
#define SDRAM_T_RFC 70.0
#define SDRAM_T_RP 20.0
#define SDRAM_T_WR 14.0
#define SDRAM_MEMORY_INFO_DAT_SYM_INSTALL_DIR SIM_DIR
#define SDRAM_MEMORY_INFO_GENERATE_DAT_SYM 1
#define SDRAM_MEMORY_INFO_MEM_INIT_DATA_WIDTH 16

/*
 * Macros for device 'LEDs', class 'altera_avalon_pio'
 * The macros are prefixed with 'LEDS_'.
 * The prefix is the slave descriptor.
 */
#define LEDS_COMPONENT_TYPE altera_avalon_pio
#define LEDS_COMPONENT_NAME LEDs
#define LEDS_BASE 0x0
#define LEDS_SPAN 16
#define LEDS_END 0xf
#define LEDS_BIT_CLEARING_EDGE_REGISTER 0
#define LEDS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LEDS_CAPTURE 0
#define LEDS_DATA_WIDTH 10
#define LEDS_DO_TEST_BENCH_WIRING 0
#define LEDS_DRIVEN_SIM_VALUE 0
#define LEDS_EDGE_TYPE NONE
#define LEDS_FREQ 100000000
#define LEDS_HAS_IN 0
#define LEDS_HAS_OUT 1
#define LEDS_HAS_TRI 0
#define LEDS_IRQ_TYPE NONE
#define LEDS_RESET_VALUE 0

/*
 * Macros for device 'HEX3_HEX0', class 'altera_avalon_pio'
 * The macros are prefixed with 'HEX3_HEX0_'.
 * The prefix is the slave descriptor.
 */
#define HEX3_HEX0_COMPONENT_TYPE altera_avalon_pio
#define HEX3_HEX0_COMPONENT_NAME HEX3_HEX0
#define HEX3_HEX0_BASE 0x20
#define HEX3_HEX0_SPAN 16
#define HEX3_HEX0_END 0x2f
#define HEX3_HEX0_BIT_CLEARING_EDGE_REGISTER 0
#define HEX3_HEX0_BIT_MODIFYING_OUTPUT_REGISTER 0
#define HEX3_HEX0_CAPTURE 0
#define HEX3_HEX0_DATA_WIDTH 32
#define HEX3_HEX0_DO_TEST_BENCH_WIRING 0
#define HEX3_HEX0_DRIVEN_SIM_VALUE 0
#define HEX3_HEX0_EDGE_TYPE NONE
#define HEX3_HEX0_FREQ 100000000
#define HEX3_HEX0_HAS_IN 0
#define HEX3_HEX0_HAS_OUT 1
#define HEX3_HEX0_HAS_TRI 0
#define HEX3_HEX0_IRQ_TYPE NONE
#define HEX3_HEX0_RESET_VALUE 0

/*
 * Macros for device 'HEX5_HEX4', class 'altera_avalon_pio'
 * The macros are prefixed with 'HEX5_HEX4_'.
 * The prefix is the slave descriptor.
 */
#define HEX5_HEX4_COMPONENT_TYPE altera_avalon_pio
#define HEX5_HEX4_COMPONENT_NAME HEX5_HEX4
#define HEX5_HEX4_BASE 0x30
#define HEX5_HEX4_SPAN 16
#define HEX5_HEX4_END 0x3f
#define HEX5_HEX4_BIT_CLEARING_EDGE_REGISTER 0
#define HEX5_HEX4_BIT_MODIFYING_OUTPUT_REGISTER 0
#define HEX5_HEX4_CAPTURE 0
#define HEX5_HEX4_DATA_WIDTH 16
#define HEX5_HEX4_DO_TEST_BENCH_WIRING 0
#define HEX5_HEX4_DRIVEN_SIM_VALUE 0
#define HEX5_HEX4_EDGE_TYPE NONE
#define HEX5_HEX4_FREQ 100000000
#define HEX5_HEX4_HAS_IN 0
#define HEX5_HEX4_HAS_OUT 1
#define HEX5_HEX4_HAS_TRI 0
#define HEX5_HEX4_IRQ_TYPE NONE
#define HEX5_HEX4_RESET_VALUE 0

/*
 * Macros for device 'Slider_Switches', class 'altera_avalon_pio'
 * The macros are prefixed with 'SLIDER_SWITCHES_'.
 * The prefix is the slave descriptor.
 */
#define SLIDER_SWITCHES_COMPONENT_TYPE altera_avalon_pio
#define SLIDER_SWITCHES_COMPONENT_NAME Slider_Switches
#define SLIDER_SWITCHES_BASE 0x40
#define SLIDER_SWITCHES_SPAN 16
#define SLIDER_SWITCHES_END 0x4f
#define SLIDER_SWITCHES_BIT_CLEARING_EDGE_REGISTER 0
#define SLIDER_SWITCHES_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SLIDER_SWITCHES_CAPTURE 0
#define SLIDER_SWITCHES_DATA_WIDTH 10
#define SLIDER_SWITCHES_DO_TEST_BENCH_WIRING 1
#define SLIDER_SWITCHES_DRIVEN_SIM_VALUE 0
#define SLIDER_SWITCHES_EDGE_TYPE NONE
#define SLIDER_SWITCHES_FREQ 100000000
#define SLIDER_SWITCHES_HAS_IN 1
#define SLIDER_SWITCHES_HAS_OUT 0
#define SLIDER_SWITCHES_HAS_TRI 0
#define SLIDER_SWITCHES_IRQ_TYPE NONE
#define SLIDER_SWITCHES_RESET_VALUE 0

/*
 * Macros for device 'Pushbuttons', class 'altera_avalon_pio'
 * The macros are prefixed with 'PUSHBUTTONS_'.
 * The prefix is the slave descriptor.
 */
#define PUSHBUTTONS_COMPONENT_TYPE altera_avalon_pio
#define PUSHBUTTONS_COMPONENT_NAME Pushbuttons
#define PUSHBUTTONS_BASE 0x50
#define PUSHBUTTONS_SPAN 16
#define PUSHBUTTONS_END 0x5f
#define PUSHBUTTONS_IRQ 1
#define PUSHBUTTONS_BIT_CLEARING_EDGE_REGISTER 1
#define PUSHBUTTONS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PUSHBUTTONS_CAPTURE 1
#define PUSHBUTTONS_DATA_WIDTH 4
#define PUSHBUTTONS_DO_TEST_BENCH_WIRING 1
#define PUSHBUTTONS_DRIVEN_SIM_VALUE 0
#define PUSHBUTTONS_EDGE_TYPE FALLING
#define PUSHBUTTONS_FREQ 100000000
#define PUSHBUTTONS_HAS_IN 1
#define PUSHBUTTONS_HAS_OUT 0
#define PUSHBUTTONS_HAS_TRI 0
#define PUSHBUTTONS_IRQ_TYPE EDGE
#define PUSHBUTTONS_RESET_VALUE 0

/*
 * Macros for device 'Expansion_JP1', class 'altera_avalon_pio'
 * The macros are prefixed with 'EXPANSION_JP1_'.
 * The prefix is the slave descriptor.
 */
#define EXPANSION_JP1_COMPONENT_TYPE altera_avalon_pio
#define EXPANSION_JP1_COMPONENT_NAME Expansion_JP1
#define EXPANSION_JP1_BASE 0x60
#define EXPANSION_JP1_SPAN 16
#define EXPANSION_JP1_END 0x6f
#define EXPANSION_JP1_IRQ 11
#define EXPANSION_JP1_BIT_CLEARING_EDGE_REGISTER 1
#define EXPANSION_JP1_BIT_MODIFYING_OUTPUT_REGISTER 0
#define EXPANSION_JP1_CAPTURE 1
#define EXPANSION_JP1_DATA_WIDTH 32
#define EXPANSION_JP1_DO_TEST_BENCH_WIRING 1
#define EXPANSION_JP1_DRIVEN_SIM_VALUE 0
#define EXPANSION_JP1_EDGE_TYPE FALLING
#define EXPANSION_JP1_FREQ 100000000
#define EXPANSION_JP1_HAS_IN 0
#define EXPANSION_JP1_HAS_OUT 0
#define EXPANSION_JP1_HAS_TRI 1
#define EXPANSION_JP1_IRQ_TYPE EDGE
#define EXPANSION_JP1_RESET_VALUE 0

/*
 * Macros for device 'Bluetooth_UART', class 'altera_avalon_uart'
 * The macros are prefixed with 'BLUETOOTH_UART_'.
 * The prefix is the slave descriptor.
 */
#define BLUETOOTH_UART_COMPONENT_TYPE altera_avalon_uart
#define BLUETOOTH_UART_COMPONENT_NAME Bluetooth_UART
#define BLUETOOTH_UART_BASE 0x80
#define BLUETOOTH_UART_SPAN 32
#define BLUETOOTH_UART_END 0x9f
#define BLUETOOTH_UART_IRQ 3
#define BLUETOOTH_UART_BAUD 115200
#define BLUETOOTH_UART_DATA_BITS 8
#define BLUETOOTH_UART_FIXED_BAUD 1
#define BLUETOOTH_UART_FREQ 100000000
#define BLUETOOTH_UART_PARITY 'N'
#define BLUETOOTH_UART_SIM_CHAR_STREAM ""
#define BLUETOOTH_UART_SIM_TRUE_BAUD 0
#define BLUETOOTH_UART_STOP_BITS 1
#define BLUETOOTH_UART_SYNC_REG_DEPTH 2
#define BLUETOOTH_UART_USE_CTS_RTS 0
#define BLUETOOTH_UART_USE_EOP_REGISTER 0

/*
 * Macros for device 'Interval_Timer', class 'altera_avalon_timer'
 * The macros are prefixed with 'INTERVAL_TIMER_'.
 * The prefix is the slave descriptor.
 */
#define INTERVAL_TIMER_COMPONENT_TYPE altera_avalon_timer
#define INTERVAL_TIMER_COMPONENT_NAME Interval_Timer
#define INTERVAL_TIMER_BASE 0x2000
#define INTERVAL_TIMER_SPAN 32
#define INTERVAL_TIMER_END 0x201f
#define INTERVAL_TIMER_IRQ 0
#define INTERVAL_TIMER_ALWAYS_RUN 0
#define INTERVAL_TIMER_COUNTER_SIZE 32
#define INTERVAL_TIMER_FIXED_PERIOD 0
#define INTERVAL_TIMER_FREQ 100000000
#define INTERVAL_TIMER_LOAD_VALUE 12499999
#define INTERVAL_TIMER_MULT 0.001
#define INTERVAL_TIMER_PERIOD 125.0
#define INTERVAL_TIMER_PERIOD_UNITS ms
#define INTERVAL_TIMER_RESET_OUTPUT 0
#define INTERVAL_TIMER_SNAPSHOT 1
#define INTERVAL_TIMER_TICKS_PER_SEC 8
#define INTERVAL_TIMER_TIMEOUT_PULSE_OUTPUT 0

/*
 * Macros for device 'Interval_Timer_2', class 'altera_avalon_timer'
 * The macros are prefixed with 'INTERVAL_TIMER_2_'.
 * The prefix is the slave descriptor.
 */
#define INTERVAL_TIMER_2_COMPONENT_TYPE altera_avalon_timer
#define INTERVAL_TIMER_2_COMPONENT_NAME Interval_Timer_2
#define INTERVAL_TIMER_2_BASE 0x2020
#define INTERVAL_TIMER_2_SPAN 32
#define INTERVAL_TIMER_2_END 0x203f
#define INTERVAL_TIMER_2_IRQ 2
#define INTERVAL_TIMER_2_ALWAYS_RUN 0
#define INTERVAL_TIMER_2_COUNTER_SIZE 32
#define INTERVAL_TIMER_2_FIXED_PERIOD 0
#define INTERVAL_TIMER_2_FREQ 100000000
#define INTERVAL_TIMER_2_LOAD_VALUE 12499999
#define INTERVAL_TIMER_2_MULT 0.001
#define INTERVAL_TIMER_2_PERIOD 125.0
#define INTERVAL_TIMER_2_PERIOD_UNITS ms
#define INTERVAL_TIMER_2_RESET_OUTPUT 0
#define INTERVAL_TIMER_2_SNAPSHOT 1
#define INTERVAL_TIMER_2_TICKS_PER_SEC 8
#define INTERVAL_TIMER_2_TIMEOUT_PULSE_OUTPUT 0

/*
 * Macros for device 'SysID', class 'altera_avalon_sysid_qsys'
 * The macros are prefixed with 'SYSID_'.
 * The prefix is the slave descriptor.
 */
#define SYSID_COMPONENT_TYPE altera_avalon_sysid_qsys
#define SYSID_COMPONENT_NAME SysID
#define SYSID_BASE 0x2040
#define SYSID_SPAN 8
#define SYSID_END 0x2047
#define SYSID_ID 0
#define SYSID_TIMESTAMP 1646352824

/*
 * Macros for device 'VGA_Subsystem_VGA_Pixel_RGB_Resampler', class 'altera_up_avalon_video_rgb_resampler'
 * The macros are prefixed with 'VGA_SUBSYSTEM_VGA_PIXEL_RGB_RESAMPLER_'.
 * The prefix is the slave descriptor.
 */
#define VGA_SUBSYSTEM_VGA_PIXEL_RGB_RESAMPLER_COMPONENT_TYPE altera_up_avalon_video_rgb_resampler
#define VGA_SUBSYSTEM_VGA_PIXEL_RGB_RESAMPLER_COMPONENT_NAME VGA_Subsystem_VGA_Pixel_RGB_Resampler
#define VGA_SUBSYSTEM_VGA_PIXEL_RGB_RESAMPLER_BASE 0x3010
#define VGA_SUBSYSTEM_VGA_PIXEL_RGB_RESAMPLER_SPAN 4
#define VGA_SUBSYSTEM_VGA_PIXEL_RGB_RESAMPLER_END 0x3013

/*
 * Macros for device 'Pixel_DMA_Addr_Translation', class 'altera_up_avalon_video_dma_ctrl_addr_trans'
 * The macros are prefixed with 'PIXEL_DMA_ADDR_TRANSLATION_'.
 * The prefix is the slave descriptor.
 */
#define PIXEL_DMA_ADDR_TRANSLATION_COMPONENT_TYPE altera_up_avalon_video_dma_ctrl_addr_trans
#define PIXEL_DMA_ADDR_TRANSLATION_COMPONENT_NAME Pixel_DMA_Addr_Translation
#define PIXEL_DMA_ADDR_TRANSLATION_BASE 0x3020
#define PIXEL_DMA_ADDR_TRANSLATION_SPAN 16
#define PIXEL_DMA_ADDR_TRANSLATION_END 0x302f

/*
 * Macros for device 'Char_DMA_Addr_Translation', class 'altera_up_avalon_video_dma_ctrl_addr_trans'
 * The macros are prefixed with 'CHAR_DMA_ADDR_TRANSLATION_'.
 * The prefix is the slave descriptor.
 */
#define CHAR_DMA_ADDR_TRANSLATION_COMPONENT_TYPE altera_up_avalon_video_dma_ctrl_addr_trans
#define CHAR_DMA_ADDR_TRANSLATION_COMPONENT_NAME Char_DMA_Addr_Translation
#define CHAR_DMA_ADDR_TRANSLATION_BASE 0x3030
#define CHAR_DMA_ADDR_TRANSLATION_SPAN 16
#define CHAR_DMA_ADDR_TRANSLATION_END 0x303f

/*
 * Macros for device 'Video_In_DMA_Addr_Translation', class 'altera_up_avalon_video_dma_ctrl_addr_trans'
 * The macros are prefixed with 'VIDEO_IN_DMA_ADDR_TRANSLATION_'.
 * The prefix is the slave descriptor.
 */
#define VIDEO_IN_DMA_ADDR_TRANSLATION_COMPONENT_TYPE altera_up_avalon_video_dma_ctrl_addr_trans
#define VIDEO_IN_DMA_ADDR_TRANSLATION_COMPONENT_NAME Video_In_DMA_Addr_Translation
#define VIDEO_IN_DMA_ADDR_TRANSLATION_BASE 0x3060
#define VIDEO_IN_DMA_ADDR_TRANSLATION_SPAN 16
#define VIDEO_IN_DMA_ADDR_TRANSLATION_END 0x306f

/*
 * Macros for device 'Video_In_Subsystem_Video_In_Edge_Detection_Subsystem_Edge_Detection_Router_Controller', class 'altera_avalon_pio'
 * The macros are prefixed with 'VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_'.
 * The prefix is the slave descriptor.
 */
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_COMPONENT_TYPE altera_avalon_pio
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_COMPONENT_NAME Video_In_Subsystem_Video_In_Edge_Detection_Subsystem_Edge_Detection_Router_Controller
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_BASE 0x3070
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_SPAN 16
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_END 0x307f
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_BIT_CLEARING_EDGE_REGISTER 0
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_BIT_MODIFYING_OUTPUT_REGISTER 0
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_CAPTURE 0
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_DATA_WIDTH 1
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_DO_TEST_BENCH_WIRING 0
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_DRIVEN_SIM_VALUE 0
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_EDGE_TYPE NONE
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_FREQ 100000000
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_HAS_IN 0
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_HAS_OUT 1
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_HAS_TRI 0
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_IRQ_TYPE NONE
#define VIDEO_IN_SUBSYSTEM_VIDEO_IN_EDGE_DETECTION_SUBSYSTEM_EDGE_DETECTION_ROUTER_CONTROLLER_RESET_VALUE 0

/*
 * Macros for device 'Onchip_SRAM', class 'altera_avalon_onchip_memory2'
 * The macros are prefixed with 'ONCHIP_SRAM_'.
 * The prefix is the slave descriptor.
 */
#define ONCHIP_SRAM_COMPONENT_TYPE altera_avalon_onchip_memory2
#define ONCHIP_SRAM_COMPONENT_NAME Onchip_SRAM
#define ONCHIP_SRAM_BASE 0x8000000
#define ONCHIP_SRAM_SPAN 262144
#define ONCHIP_SRAM_END 0x803ffff
#define ONCHIP_SRAM_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_SRAM_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_SRAM_CONTENTS_INFO ""
#define ONCHIP_SRAM_DUAL_PORT 1
#define ONCHIP_SRAM_GUI_RAM_BLOCK_TYPE AUTO
#define ONCHIP_SRAM_INIT_CONTENTS_FILE Computer_System_Onchip_SRAM
#define ONCHIP_SRAM_INIT_MEM_CONTENT 1
#define ONCHIP_SRAM_INSTANCE_ID NONE
#define ONCHIP_SRAM_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_SRAM_RAM_BLOCK_TYPE AUTO
#define ONCHIP_SRAM_READ_DURING_WRITE_MODE DONT_CARE
#define ONCHIP_SRAM_SINGLE_CLOCK_OP 1
#define ONCHIP_SRAM_SIZE_MULTIPLE 1
#define ONCHIP_SRAM_SIZE_VALUE 262144
#define ONCHIP_SRAM_WRITABLE 1
#define ONCHIP_SRAM_MEMORY_INFO_DAT_SYM_INSTALL_DIR SIM_DIR
#define ONCHIP_SRAM_MEMORY_INFO_GENERATE_DAT_SYM 1
#define ONCHIP_SRAM_MEMORY_INFO_GENERATE_HEX 1
#define ONCHIP_SRAM_MEMORY_INFO_HAS_BYTE_LANE 0
#define ONCHIP_SRAM_MEMORY_INFO_HEX_INSTALL_DIR QPF_DIR
#define ONCHIP_SRAM_MEMORY_INFO_MEM_INIT_DATA_WIDTH 32
#define ONCHIP_SRAM_MEMORY_INFO_MEM_INIT_FILENAME Computer_System_Onchip_SRAM

/*
 * Macros for device 'VGA_Subsystem_Char_Buf_Subsystem_Onchip_SRAM', class 'altera_avalon_onchip_memory2'
 * The macros are prefixed with 'VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_'.
 * The prefix is the slave descriptor.
 */
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_COMPONENT_TYPE altera_avalon_onchip_memory2
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_COMPONENT_NAME VGA_Subsystem_Char_Buf_Subsystem_Onchip_SRAM
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_BASE 0x9000000
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_SPAN 8192
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_END 0x9001fff
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_CONTENTS_INFO ""
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_DUAL_PORT 1
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_GUI_RAM_BLOCK_TYPE AUTO
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_INIT_CONTENTS_FILE Computer_System_VGA_Subsystem_Char_Buf_Subsystem_Onchip_SRAM
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_INIT_MEM_CONTENT 1
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_INSTANCE_ID NONE
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_NON_DEFAULT_INIT_FILE_ENABLED 0
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_RAM_BLOCK_TYPE AUTO
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_READ_DURING_WRITE_MODE DONT_CARE
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_SINGLE_CLOCK_OP 1
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_SIZE_MULTIPLE 1
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_SIZE_VALUE 8192
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_WRITABLE 1
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_MEMORY_INFO_DAT_SYM_INSTALL_DIR SIM_DIR
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_MEMORY_INFO_GENERATE_DAT_SYM 1
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_MEMORY_INFO_GENERATE_HEX 1
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_MEMORY_INFO_HAS_BYTE_LANE 0
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_MEMORY_INFO_HEX_INSTALL_DIR QPF_DIR
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_MEMORY_INFO_MEM_INIT_DATA_WIDTH 32
#define VGA_SUBSYSTEM_CHAR_BUF_SUBSYSTEM_ONCHIP_SRAM_MEMORY_INFO_MEM_INIT_FILENAME Computer_System_VGA_Subsystem_Char_Buf_Subsystem_Onchip_SRAM


#endif /* _ALTERA_ADDRESS_MAP_H_ */
