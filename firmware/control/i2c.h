#include <util/twi.h>

// Send I2C start condition and wait for transmission
inline void i2c_send_start(void)
{
    // Send start condition
    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN) | (1<<TWEA);

    // Wait for start condition to be transmitted
    while ( !(TWCR & (1<<TWINT)) );
}

// Send I2C stop condition
inline void i2c_send_stop(void)
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

// Send byte over I2C bus and wait for transmission
inline void i2c_transmit(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);

    // Wait for transmission
    while ( !(TWCR & (1<<TWINT)) );
}

// Read a byte from the selected I2C slave
inline uint8_t i2c_receive(void)
{
    // Mark as ready to receive
    TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);

    // Wait for transmission
    while ( !(TWCR & (1<<TWINT)) );

    // Get byte
    return TWDR;
}

// Return true if the given status is in the status register
inline bool i2c_has_status(uint8_t expected)
{
    return ( (TWSR & 0xF8) == expected );
}
