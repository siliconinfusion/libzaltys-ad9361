/**************************************************************************************************

    Author                  : Paul Onions
    Creation date           : 16 December 2015

    Copyright 2015 - 2016 Silicon Infusion Limited

    Silicon Infusion Limited                 
    CP House
    Otterspool Way
    Watford WD25 8HP
    Hertfordshire, UK
    Tel: +44 (0)1923 650404
    Fax: +44 (0)1923 650374
    Web: www.siliconinfusion.com

    Licence: MIT, see LICENCE file for details.

***************************************************************************************************

**************************************************************************************************/

#ifndef LIBZALTYS_AD9361_H
#define LIBZALTYS_AD9361_H

#include "ad/ad9361_api.h"

/* Prototypes for the toplevel initialization functions */
struct ad9361_rf_phy *ad9361_lvds_init();
struct ad9361_rf_phy *ad9361_cmos_init();

/* Prototypes for functions to setup the callback function pointers */
void ad9361_set_fn_usleep(void (*fn)(unsigned long usleep));
void ad9361_set_fn_spi_init(int32_t (*fn)(uint32_t device_id, uint8_t  clk_pha, uint8_t  clk_pol));
void ad9361_set_fn_spi_read(int32_t (*fn)(uint8_t bytes_number));
void ad9361_set_fn_spi_write_then_read(int (*fn)(const unsigned char *txbuf, unsigned n_tx, unsigned n_rx));
void ad9361_set_fn_gpio_init(void (*fn)(uint32_t device_id));
void ad9361_set_fn_gpio_direction(void (*fn)(uint8_t pin, uint8_t direction));
void ad9361_set_fn_gpio_is_valid(bool (*fn)(int number));
void ad9361_set_fn_gpio_data(void (*fn)(uint8_t pin, uint8_t data));
void ad9361_set_fn_gpio_set_value(void (*fn)(unsigned gpio, int value));
void ad9361_set_fn_udelay(void (*fn)(unsigned long usecs));
void ad9361_set_fn_mdelay(void (*fn)(unsigned long msecs));
void ad9361_set_fn_msleep_interruptable(unsigned long (*fn)(unsigned int msecs));
void ad9361_set_fn_axiadc_init(void (*fn)(struct ad9361_rf_phy *phy));
void ad9361_set_fn_axiadc_read(unsigned int (*fn)(struct axiadc_state *st, unsigned long reg));
void ad9361_set_fn_axiadc_write(void (*fn)(struct axiadc_state *st, unsigned reg, unsigned val));

/* Prototypes for functions to access the SPI rx buffer */
void ad9361_write_rxbuf(unsigned char dat, unsigned pos);
unsigned char ad9361_read_rxbuf(unsigned pos);

#endif /* LIBZALTYS_AD9361_H */
