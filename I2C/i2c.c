#include "i2c.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdint.h>

bool sfp_i2c_init(i2c_inst_t *i2c, uint sda, uint scl, uint baudrate)
{
    i2c_init(i2c, baudrate);

    gpio_set_function(sda, GPIO_FUNC_I2C);
    gpio_set_function(scl, GPIO_FUNC_I2C);

    gpio_pull_up(sda);
    gpio_pull_up(scl);

    return true;
}

bool sfp_read_block(i2c_inst_t *i2c,uint8_t dev_addr,uint8_t start_offset,uint8_t *buffer,uint8_t length)
{
    if (!buffer || length == 0)
        return false;

    /* 1. Envia offset interno */
    int ret = i2c_write_blocking(
        i2c,
        dev_addr,
        &start_offset,
        1,
        true               // repeated start
    );

    if (ret != 1)
        return false;

    /* 2. Lê dados sequenciais (EEPROM)*/
    ret = i2c_read_blocking(
        i2c,
        dev_addr,
        buffer,
        length,
        false
    );

    return (ret == length);
}

