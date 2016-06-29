/**************************************************************************************************

    Author                  : Paul Onions
    Creation date           : 9 May 2014

    COMMERCIAL IN CONFIDENCE
    (C) 2014 - 2016 Silicon Infusion Limited

    Silicon Infusion Limited                 
    CP House
    Otterspool Way
    Watford WD25 8HP
    Hertfordshire, UK
    Tel: +44 (0)1923 650404
    Fax: +44 (0)1923 650374
    Web: www.siliconinfusion.com

    This is an unpublished work the copyright of which vests in Silicon Infusion
    Limited. All rights reserved. The information contained herein is the
    property of Silicon Infusion Limited and is supplied without liability for
    errors or omissions. No part may be reproduced or used except as authorised
    by contract or other written permission. The copyright and the foregoing
    restriction on reproduction and use extend to all media in which the
    information may be embodied.

***************************************************************************************************

    Platform dependent part of the Analog Devices AD9361 driver.

**************************************************************************************************/

#include <sys/types.h>
#include <stdint.h>
#include "util.h"

/*
** Macros and constants
*/
#define ADI_REG_CNTRL			0x0044
#define ADI_R1_MODE			(1 << 2)
#define ADI_DDR_EDGESEL			(1 << 1)
#define ADI_PIN_MODE			(1 << 0)

#define ADI_REG_STATUS			0x005C
#define ADI_MUX_PN_ERR			(1 << 3)
#define ADI_MUX_PN_OOS			(1 << 2)
#define ADI_MUX_OVER_RANGE		(1 << 1)
#define ADI_STATUS			(1 << 0)

#define ADI_REG_CHAN_CNTRL(c)	        (0x0400 + (c) * 0x40)
#define ADI_PN_SEL			(1 << 10)
#define ADI_IQCOR_ENB			(1 << 9)
#define ADI_DCFILT_ENB			(1 << 8)
#define ADI_FORMAT_SIGNEXT		(1 << 6)
#define ADI_FORMAT_TYPE			(1 << 5)
#define ADI_FORMAT_ENABLE		(1 << 4)
#define ADI_PN23_TYPE			(1 << 1)
#define ADI_ENABLE			(1 << 0)

#define ADI_REG_CHAN_STATUS(c)	        (0x0404 + (c) * 0x40)
#define ADI_PN_ERR			(1 << 2)
#define ADI_PN_OOS			(1 << 1)
#define ADI_OVER_RANGE			(1 << 0)

#define ADI_REG_CHAN_CNTRL_1(c)		(0x0410 + (c) * 0x40)
#define ADI_DCFILT_OFFSET(x)		(((x) & 0xFFFF) << 16)
#define ADI_TO_DCFILT_OFFSET(x)		(((x) >> 16) & 0xFFFF)
#define ADI_DCFILT_COEFF(x)		(((x) & 0xFFFF) << 0)
#define ADI_TO_DCFILT_COEFF(x)		(((x) >> 0) & 0xFFFF)

#define ADI_REG_CHAN_CNTRL_2(c)		(0x0414 + (c) * 0x40)
#define ADI_IQCOR_COEFF_1(x)		(((x) & 0xFFFF) << 16)
#define ADI_TO_IQCOR_COEFF_1(x)		(((x) >> 16) & 0xFFFF)
#define ADI_IQCOR_COEFF_2(x)		(((x) & 0xFFFF) << 0)
#define ADI_TO_IQCOR_COEFF_2(x)		(((x) >> 0) & 0xFFFF)

/*
** Function declarations
*/
int32_t spi_init(uint32_t device_id,
		 uint8_t  clk_pha,
		 uint8_t  clk_pol);
int32_t spi_read(uint8_t *data,
		 uint8_t bytes_number);
int spi_write_then_read(struct spi_device *spi,
		        const unsigned char *txbuf, unsigned n_tx,
		        unsigned char *rxbuf, unsigned n_rx);

void gpio_init(uint32_t device_id);
void gpio_direction(uint8_t pin, uint8_t direction);
bool gpio_is_valid(int number);
void gpio_set_value(unsigned gpio, int value);

void udelay(unsigned long usecs);
void mdelay(unsigned long msecs);
unsigned long msleep_interruptible(unsigned int msecs);

void axiadc_init(struct ad9361_rf_phy *phy);
unsigned int axiadc_read(struct axiadc_state *st, unsigned long reg);
void axiadc_write(struct axiadc_state *st, unsigned reg, unsigned val);
