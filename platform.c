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

    Platform dependent part of the Analog Devices AD9361 driver.  This file
    contains functions that will be called by the Analog Devices driver during
    operation.

    The purpose of the code given here is to redirect the driver
    function calls to external code (e.g. VB or Python code).  The
    driver should be compiled into a DLL that can be loaded into a VB
    program, which should initialise the function pointers defined
    below before calling the toplevel ad9361_lvds_init or
    ad9361_cmos_init driver initialisation routines.

    WARNING: This code may look convoluted.  It is.  I tried doing it in simpler
    ways but kept failing.

**************************************************************************************************/

#include <stdint.h>
#include "util.h"

/*
** Function pointers for calling back into VB code.
*/
void (*fn_usleep)(unsigned long usleep) = NULL;
int (*fn_spi_init)(uint32_t device_id, uint8_t clk_pha, uint8_t clk_pol) = NULL;
int (*fn_spi_read)(uint8_t bytes_number) = NULL;
int (*fn_spi_write_then_read)(const unsigned char *txbuf, unsigned n_tx, unsigned n_rx) = NULL;
void (*fn_gpio_init)(uint32_t device_id) = NULL;
void (*fn_gpio_direction)(uint8_t pin, uint8_t direction) = NULL;
bool (*fn_gpio_is_valid)(int number) = NULL;
void (*fn_gpio_data)(uint8_t pin, uint8_t data) = NULL;
void (*fn_gpio_set_value)(unsigned gpio, int value) = NULL;
void (*fn_udelay)(unsigned long usecs) = NULL;
void (*fn_mdelay)(unsigned long msecs) = NULL;
unsigned long (*fn_msleep_interruptable)(unsigned int msecs) = NULL;
void (*fn_axiadc_init)(struct ad9361_rf_phy *phy) = NULL;
unsigned int (*fn_axiadc_read)(struct axiadc_state *st, unsigned long reg) = NULL;
void (*fn_axiadc_write)(struct axiadc_state *st, unsigned reg, unsigned val) = NULL;


/*
** Functions to setup the above function pointers.
** Called from VB code.
*/
void ad9361_set_fn_usleep(void (*fn)(unsigned long usleep))
{
  fn_usleep = fn;
}

void ad9361_set_fn_spi_init(int32_t (*fn)(uint32_t device_id, uint8_t  clk_pha, uint8_t  clk_pol))
{
  fn_spi_init = fn;
}

void ad9361_set_fn_spi_read(int32_t (*fn)(uint8_t bytes_number))
{
  fn_spi_read = fn;
}

void ad9361_set_fn_spi_write_then_read(int (*fn)(const unsigned char *txbuf, unsigned n_tx, unsigned n_rx))
{
  fn_spi_write_then_read = fn;
}

void ad9361_set_fn_gpio_init(void (*fn)(uint32_t device_id))
{
  fn_gpio_init = fn;
}

void ad9361_set_fn_gpio_direction(void (*fn)(uint8_t pin, uint8_t direction))
{
  fn_gpio_direction = fn;
}

void ad9361_set_fn_gpio_is_valid(bool (*fn)(int number))
{
  fn_gpio_is_valid = fn;
}

void ad9361_set_fn_gpio_data(void (*fn)(uint8_t pin, uint8_t data))
{
  fn_gpio_data = fn;
}

void ad9361_set_fn_gpio_set_value(void (*fn)(unsigned gpio, int value))
{
  fn_gpio_set_value = fn;
}

void ad9361_set_fn_udelay(void (*fn)(unsigned long usecs))
{
  fn_udelay = fn;
}

void ad9361_set_fn_mdelay(void (*fn)(unsigned long msecs))
{
  fn_mdelay = fn;
}

void ad9361_set_fn_msleep_interruptable(unsigned long (*fn)(unsigned int msecs))
{
  fn_msleep_interruptable = fn;
}

void ad9361_set_fn_axiadc_init(void (*fn)(struct ad9361_rf_phy *phy))
{
  fn_axiadc_init = fn;
}

void ad9361_set_fn_axiadc_read(unsigned int (*fn)(struct axiadc_state *st, unsigned long reg))
{
  fn_axiadc_read = fn;
}

void ad9361_set_fn_axiadc_write(void (*fn)(struct axiadc_state *st, unsigned reg, unsigned val))
{
  fn_axiadc_write = fn;
}


/*
** Static buffer into which VB code will write SPI read data.
*/
#define STATIC_RXBUF_LEN 8
unsigned char static_rxbuf[STATIC_RXBUF_LEN];


/*
** Functions for VB code to call to access the above buffer.
*/
void ad9361_write_rxbuf(unsigned char dat, unsigned pos) {
  if (pos < STATIC_RXBUF_LEN)
    static_rxbuf[pos] = dat;
}

unsigned char ad9361_read_rxbuf(unsigned pos) {
  if (pos < STATIC_RXBUF_LEN) {
    return static_rxbuf[pos];
  } else {
    return 0;
  }
}


/*
** Driver platform functions
*/
static void usleep(unsigned long usleep)
{
  if (fn_usleep != NULL)
    (fn_usleep)(usleep);
}


int32_t spi_init(uint32_t device_id,
                 uint8_t  clk_pha,
                 uint8_t  clk_pol)
{
  if (fn_spi_init != NULL) {
    return (fn_spi_init)(device_id, clk_pha, clk_pol);
  } else {
    return 0;
  }
}


int32_t spi_read(uint8_t *data,
                 uint8_t bytes_number)
{
  int rtncode = 0;
  unsigned n;

  for (n=0; n<STATIC_RXBUF_LEN; n++)
    static_rxbuf[n] = 0;

  if (fn_spi_read != NULL)
    rtncode = (fn_spi_read)(bytes_number);

  for (n=0; n<bytes_number; n++)
    data[n] = static_rxbuf[n];

  return rtncode;
}


int spi_write_then_read(struct spi_device *spi,
                        const unsigned char *txbuf, unsigned n_tx,
                        unsigned char *rxbuf, unsigned n_rx)
{
  int rtncode = 0;
  unsigned n;

  for (n=0; n<STATIC_RXBUF_LEN; n++)
    static_rxbuf[n] = 0;

  if (fn_spi_write_then_read != NULL)
    rtncode = (fn_spi_write_then_read)(txbuf, n_tx, n_rx);

  for (n=0; n<n_rx; n++)
    rxbuf[n] = static_rxbuf[n];

  return rtncode;
}


void gpio_init(uint32_t device_id)
{
  if (fn_gpio_init != NULL)
    (fn_gpio_init)(device_id);
}


void gpio_direction(uint8_t pin, uint8_t direction)
{
  if (fn_gpio_direction != NULL)
    (fn_gpio_direction)(pin, direction);
}


bool gpio_is_valid(int number)
{
  if (fn_gpio_is_valid != NULL) {
    return (fn_gpio_is_valid)(number);
  } else {
    return 0;
  }
}


void gpio_data(uint8_t pin, uint8_t data)
{
  if (fn_gpio_data != NULL)
    (fn_gpio_data)(pin, data);
}


void gpio_set_value(unsigned gpio, int value)
{
  if (fn_gpio_set_value != NULL)
    (fn_gpio_set_value)(gpio, value);
}


void udelay(unsigned long usecs)
{
  if (fn_udelay != NULL)
    (fn_udelay)(usecs);
}


void mdelay(unsigned long msecs)
{
  if (fn_mdelay != NULL)
    (fn_mdelay)(msecs);
}


unsigned long msleep_interruptible(unsigned int msecs)
{
  if (fn_msleep_interruptable != NULL) {
    return (fn_msleep_interruptable)(msecs);
  } else {
    return 0;
  }
}


void axiadc_init(struct ad9361_rf_phy *phy)
{
  if (fn_axiadc_init != NULL)
    (fn_axiadc_init)(phy);
}


unsigned int axiadc_read(struct axiadc_state *st, unsigned long reg)
{
  if (fn_axiadc_read != NULL) {
    return (fn_axiadc_read)(st, reg);
  } else {
    return 0;
  }
}


void axiadc_write(struct axiadc_state *st, unsigned reg, unsigned val)
{
  if (fn_axiadc_write != NULL)
    (fn_axiadc_write)(st, reg, val);
}

