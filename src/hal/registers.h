#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

struct rcc_reg {
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t RESERVED;
    volatile uint32_t CRRCR;
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    volatile uint32_t IOPRSTR;
    volatile uint32_t AHBRSTR;
    volatile uint32_t APBRSTR1;
    volatile uint32_t APBRSTR2;
    volatile uint32_t IOPENR;
    volatile uint32_t AHBRNR;
    volatile uint32_t APBENR1;
    volatile uint32_t APBENR2;
    volatile uint32_t IOPSMENR;
    volatile uint32_t AHBSMENR;
    volatile uint32_t APBSMENR1;
    volatile uint32_t APBSMENR2;
    volatile uint32_t CCIPR;
    volatile uint32_t CCIPR2;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
};

struct systick_reg {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
};

struct nvic_reg {
    volatile uint32_t ISER;
    volatile uint32_t RESERVED0[31];
    volatile uint32_t ICER;
    volatile uint32_t RESERVED1[31];
    volatile uint32_t ISPR;
    volatile uint32_t RESERVED2[31];
    volatile uint32_t ICPR;
    volatile uint32_t RESERVED3[31];
    volatile uint32_t RESERVED4[64];
    volatile uint32_t IP[8];
};

struct gpio_reg {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
    volatile uint32_t BRR;
};

struct tim_reg {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR1;
    volatile uint32_t CCMR3;
    volatile uint32_t CCR5;
    volatile uint32_t CCR6;
    volatile uint32_t AF1;
    volatile uint32_t AF2;
    volatile uint32_t TISEL;
};

struct uart_reg {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
    volatile uint32_t PRESC;
};

struct spi_reg {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t CRCPR;
    volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR;
    volatile uint32_t I2SCFGR;
    volatile uint32_t I2SPR;
};

#define RCC ((struct rcc_reg *) 0x40021000)
#define SYSTICK ((struct systick_reg *) 0xE000E010)
#define NVIC ((struct nvic_reg *) 0xE000E100)
#define GPIOA ((struct gpio_reg *) 0x50000000)
#define GPIOB ((struct gpio_reg *) 0x50000400)
#define GPIOC ((struct gpio_reg *) 0x50000800)
#define TIM1 ((struct tim_reg *) 0x40012C00)
#define TIM3 ((struct tim_reg *) 0x40000400)
#define UART1 ((struct uart_reg *) 0x40013800)
#define SPI1 ((struct spi_reg *) 0x40013000)
#define SPI2 ((struct spi_reg *) 0x40003800)

#endif // REGISTERS_H
