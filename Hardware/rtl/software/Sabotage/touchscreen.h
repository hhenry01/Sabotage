#include <stdbool.h>

#define MAX_INTERRUPT_OUT_TRANSFER_SIZE 64
#define MAX_INTERRUPT_IN_TRANSFER_SIZE 64
#define MODE_GENERIC 1
#define MODE_MOUSE 2
#define MODE_DIGITIZER 3
    
// Fixed value
#define SYNC                    0x55

// Commands
#define CMD_TOUCH_ENABLE        0x12
#define CMD_TOUCH_DISABLE       0x13
#define CMD_CALIBRATE           0x14
#define CMD_DEBUG				0x15
#define CMD_REG_READ            0x20
#define CMD_REG_WRITE           0x21
#define CMD_EE_READ             0x28
#define CMD_EE_WRITE            0x29
#define CMD_EE_READ_PARAMS      0x2B
#define CMD_EE_WRITE_PARAMS     0x23

// Status
#define STATUS_OK               0x00
#define STATUS_UNRECOGNIZED     0x01
#define STATUS_TIMEOUT          0x04
#define STATUS_EEPARAMS_ERR     0x05
#define STATUS_CAL_CANCEL       0xFC

//no response packet for these commands
#define CMD_HID_GENERIC         0x70
#define CMD_USB_MOUSE           0x71
#define CMD_DIGITIZE            0x72
    
#define VENDOR_ID               0x04D8
#define PID_GENERIC             0x0C01 //generic HID
#define PID_MOUSE               0x0C02 //mouse
#define PID_DIGITIZER           0x0C03 //digitizer
    
#define CAL_TYPE_4POINT         0x01
#define CAL_TYPE_9POINT         0x02
#define CAL_TYPE_25POINT        0x03
#define CAL_TYPE_AR1000         0x04

// session will be timeout in 250ms
#define TIMEOUT 250

/* a data type to hold a point/coord */
typedef struct { int x, y; } Point ;

// structure for command and response packets
typedef struct {
    unsigned char sync;
    unsigned char size;
    unsigned char cmd;
    unsigned char data[MAX_INTERRUPT_OUT_TRANSFER_SIZE-3];
} Command;

typedef struct {
    unsigned char sync;
    unsigned char size;
    unsigned char status;
    unsigned char cmd;
    unsigned char data[MAX_INTERRUPT_IN_TRANSFER_SIZE-4];
} Response;



void sendCommand(int cmd, FILE *fp);

int getResponse(FILE *fp);

int TouchLookup(Point p);

// initialize touch screen controller
int Init_Touch();

int ScreenTouched(FILE *fp);

int checkReleased(FILE *fp);

void WaitForTouch(FILE *fp);

//void WaitForRelease();

//void WaitForRead();

Point GetPress(FILE *fp);

Point GetRelease(FILE *fp);

//void testTouch(void);
