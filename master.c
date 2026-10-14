// master.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define I2C_BUS "/dev/i2c-1"  // Default I2C bus on Raspberry Pi
#define SLAVE_ADDR 0x44        // Target slave address

int main(void) {
    int file;
    char *message = "Hello Pi";

    // Open the I2C bus
    if ((file = open(I2C_BUS, O_RDWR)) < 0) {
        perror("Failed to open the I2C bus");
        return 1;
    }

    // Configure the target device address
    if (ioctl(file, I2C_SLAVE, SLAVE_ADDR) < 0) {
        perror("Failed to acquire bus access / talk to slave");
        close(file);
        return 1;
    }

    printf("Sending data to slave at address 0x%02X...\n", SLAVE_ADDR);

    while (1) {
        int len = strlen(message);
        if (write(file, message, len) != len) {
            perror("Failed to write to the i2c bus");
        } else {
            printf("Sent: %s\n", message);
        }
        sleep(1);
    }

    close(file);
    return 0;
}
