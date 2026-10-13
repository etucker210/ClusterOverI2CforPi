#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define SLAVE_ADDR 0x44

int main(void) {
    if (gpioInitialise() < 0) {
        printf("pigpio initialization failed\n");
        return 1;
    }

    // Configure BSC peripheral as an I2C Slave at address 0x44
    bsc_xfer_t xfer;
    xfer.control = (SLAVE_ADDR << 16) | 0x305; // Set address & enable I2C slave mode

    printf("I2C Slave initialized at address 0x%02X. Listening...\n", SLAVE_ADDR);

    while (1) {
        // Query the status/buffer of the BSC peripheral
        if (bscXfer(&xfer) > 0) {
            if (xfer.rxCnt > 0) {
                printf("Received %d bytes: ", xfer.rxCnt);
                for (int i = 0; i < xfer.rxCnt; i++) {
                    printf("%c", xfer.rxBuf[i]);
                }
                printf("\n");
            }
        }
        gpioDelay(10000); // Sleep 10ms to prevent high CPU usage
    }

    // Disable BSC before exiting
    xfer.control = 0;
    bscXfer(&xfer);
    gpioTerminate();
    return 0;
}
