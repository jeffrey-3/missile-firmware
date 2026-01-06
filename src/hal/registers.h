#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdbool.h>
#include <stdint.h>

#define BIT(x) (1UL << (x))

struct rcc_registers {
    volatile uint32_t CR, ICSCR, CFGR, PLLCFGR, RESERVED, CRRCR, CIER, CIFR,
        CICR, IOPRSTR, AHBRSTR, APBRSTR1, APBRSTR2, IOPENR, AHBRNR, APBENR1,
        APBENR2, IOPSMENR, AHBSMENR, APBSMENR1, APBSMENR2, CCIPR, CCIPR2, BDCR,
        CSR;
};
#define RCC ((struct rcc_registers *) 0x40021000)

struct systick_registers {
    volatile uint32_t CTRL, LOAD, VAL, CALIB;
};
#define SYSTICK ((struct systick_registers *) 0xe000e010)

struct gpio_registers {
    volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR,
        AFR[2], BRR;
};
#define GPIO(bank) ((struct gpio_registers *) (0x50000000 + 0x400 * (bank)))

struct uart_registers {
    volatile uint32_t CR1, CR2, CR3, BRR, GTPR, RTOR, RQR, ISR, ICR, RDR, TDR,
        PRESC;
};
#define UART1 ((struct uart_registers *) 0x40013800)

struct spi_registers {
    volatile uint32_t CR1, CR2, SR, DR, CRCPR, RXCRCR, TXCRCR, I2SCFGR, I2SPR;
};
#define SPI1 ((struct spi_registers *) 0x40013000)
#define SPI2 ((struct spi_registers *) 0x40003800)

#endif // REGISTERS_H
