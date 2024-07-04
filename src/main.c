#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* change this to any other UART peripheral if desired */
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)

/* Get the UART device specified in the device tree */
static const struct device *const uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

/* Function to initialize UART with specific settings */
void uart_init(void)
{
    struct uart_config uart_cfg;

    /* Set UART configuration parameters */
    uart_cfg.baudrate = 9600;
    uart_cfg.parity = UART_CFG_PARITY_NONE;
    uart_cfg.stop_bits = UART_CFG_STOP_BITS_1;
    uart_cfg.data_bits = UART_CFG_DATA_BITS_8;
    uart_cfg.flow_ctrl = UART_CFG_FLOW_CTRL_NONE;

    /* Configure the UART device with the settings */
    int ret = uart_configure(uart_dev, &uart_cfg);
    if (ret != 0) {
        printk("Failed to configure UART: %d\n", ret);
    }
}

/* Function to send a command via UART */
void send_uart_command(const uint8_t *cmd, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        uart_poll_out(uart_dev, cmd[i]);
        k_msleep(1);  // Small delay for each byte
    }
}

/* Function to receive data from UART */
int receive_uart_data(uint8_t *buf, size_t length, k_timeout_t timeout)
{
    size_t received_len = 0;
    int ret;
    uint32_t start_time = k_uptime_get();

    while (received_len < length) {
        ret = uart_poll_in(uart_dev, &buf[received_len]);
        if (ret == 0) {
            received_len++;
        } else if (ret == -1) {
            k_msleep(1);
            if (k_uptime_get() - start_time > timeout.ticks) {
                return -ETIMEDOUT;
            }
        } else {
            return ret;
        }
    }
    return 0;
}

int main(void)
{
    if (!device_is_ready(uart_dev)) {
        printk("UART device not found!\n");
        return 0;
    }

    uart_init();  // Initialize UART with specified settings

    uint8_t query_command[3] = {0xA5, 0x09, 0xAE};  // Query command
    uint8_t rx_buf[9];  // Buffer to store received data

    while (1) {
        send_uart_command(query_command, sizeof(query_command));  // Send query command

        int ret = receive_uart_data(rx_buf, sizeof(rx_buf), K_MSEC(100));
        if (ret == 0) {
            // Check for the correct frame header
            if (rx_buf[0] == 0x5A && rx_buf[1] == 0x5A) {
                // Print received data bytes for debugging
                printk("Received Data:");
                for (int i = 0; i < sizeof(rx_buf); i++) {
                    printk(" %02X", rx_buf[i]);
                }
                printk("\n");

                // Calculate light intensity
                uint32_t lux = (rx_buf[4] << 24) | (rx_buf[5] << 16) | (rx_buf[6] << 8) | rx_buf[7];
        		uint32_t luxValue = lux / 100.0;

                printf("Light intensity: %d\n", luxValue);
            } else {
                printk("Invalid frame header received: 0x%02X 0x%02X\n", rx_buf[0], rx_buf[1]);
            }
        } else if (ret == -ETIMEDOUT) {
            printk("UART receive timeout\n");
        } else {
            printk("UART receive error: %d\n", ret);
        }

        k_msleep(1000);  // Small delay before sending the next query
    }

    return 0;
}
