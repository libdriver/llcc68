/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_llcc68_lora.h
 * @brief     driver llcc68 lora header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-04-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/04/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_LLCC68_LORA_H
#define DRIVER_LLCC68_LORA_H

#include "driver_llcc68_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup llcc68_example_driver llcc68 example driver function
 * @brief    llcc68 example driver modules
 * @ingroup  llcc68_driver
 * @{
 */

/**
 * @brief llcc68 lora example default definition
 */
#define LLCC68_LORA_DEFAULT_STOP_TIMER_ON_PREAMBLE      LLCC68_BOOL_FALSE                 /**< disable stop timer on preamble */
#define LLCC68_LORA_DEFAULT_REGULATOR_MODE              LLCC68_REGULATOR_MODE_DC_DC_LDO   /**< only ldo */
#define LLCC68_LORA_DEFAULT_PA_CONFIG_DUTY_CYCLE        0x02                              /**< set +17dBm power */
#define LLCC68_LORA_DEFAULT_PA_CONFIG_HP_MAX            0x03                              /**< set +17dBm power */
#define LLCC68_LORA_DEFAULT_TX_DBM                      17                                /**< +17dBm */
#define LLCC68_LORA_DEFAULT_RAMP_TIME                   LLCC68_RAMP_TIME_10US             /**< set ramp time 10 us */
#define LLCC68_LORA_DEFAULT_SF                          LLCC68_LORA_SF_9                  /**< sf9 */
#define LLCC68_LORA_DEFAULT_BANDWIDTH                   LLCC68_LORA_BANDWIDTH_125_KHZ     /**< 125khz */
#define LLCC68_LORA_DEFAULT_CR                          LLCC68_LORA_CR_4_5                /**< cr4/5 */
#define LLCC68_LORA_DEFAULT_LOW_DATA_RATE_OPTIMIZE      LLCC68_BOOL_FALSE                 /**< disable low data rate optimize */
#define LLCC68_LORA_DEFAULT_RF_FREQUENCY                480000000U                        /**< 480000000Hz */
#define LLCC68_LORA_DEFAULT_SYMB_NUM_TIMEOUT            0                                 /**< 0 */
#define LLCC68_LORA_DEFAULT_SYNC_WORD                   0x3444U                           /**< public network */
#define LLCC68_LORA_DEFAULT_RX_GAIN                     0x94                              /**< common rx gain */
#define LLCC68_LORA_DEFAULT_OCP                         0x38                              /**< 140 mA */
#define LLCC68_LORA_DEFAULT_PREAMBLE_LENGTH             12                                /**< 12 */
#define LLCC68_LORA_DEFAULT_HEADER                      LLCC68_LORA_HEADER_EXPLICIT       /**< explicit header */
#define LLCC68_LORA_DEFAULT_BUFFER_SIZE                 255                               /**< 255 */
#define LLCC68_LORA_DEFAULT_CRC_TYPE                    LLCC68_LORA_CRC_TYPE_ON           /**< crc on */
#define LLCC68_LORA_DEFAULT_INVERT_IQ                   LLCC68_BOOL_FALSE                 /**< disable invert iq */
#define LLCC68_LORA_DEFAULT_CAD_SYMBOL_NUM              LLCC68_LORA_CAD_SYMBOL_NUM_2      /**< 2 symbol */
#define LLCC68_LORA_DEFAULT_CAD_DET_PEAK                24                                /**< 24 */
#define LLCC68_LORA_DEFAULT_CAD_DET_MIN                 10                                /**< 10 */
#define LLCC68_LORA_DEFAULT_START_MODE                  LLCC68_START_MODE_WARM            /**< warm mode */
#define LLCC68_LORA_DEFAULT_RTC_WAKE_UP                 LLCC68_BOOL_TRUE                  /**< enable rtc wake up */

/**
 * @brief  llcc68 lora irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t llcc68_lora_irq_handler(void);

/**
 * @brief     lora example init
 * @param[in] *callback pointer to a callback address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t llcc68_lora_init(void (*callback)(uint16_t type, uint8_t *buf, uint16_t len));

/**
 * @brief  lora example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t llcc68_lora_deinit(void);

/**
 * @brief  lora example enter to the continuous receive mode
 * @return status code
 *         - 0 success
 *         - 1 enter failed
 * @note   none
 */
uint8_t llcc68_lora_set_continuous_receive_mode(void);

/**
 * @brief     lora example enter to the shot receive mode
 * @param[in] us time in us
 * @return    status code
 *            - 0 success
 *            - 1 enter failed
 * @note      none
 */
uint8_t llcc68_lora_set_shot_receive_mode(double us);

/**
 * @brief  lora example enter to the send mode
 * @return status code
 *         - 0 success
 *         - 1 enter failed
 * @note   none
 */
uint8_t llcc68_lora_set_send_mode(void);

/**
 * @brief     lora example send lora data
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 send failed
 * @note      none
 */
uint8_t llcc68_lora_send(uint8_t *buf, uint16_t len);

/**
 * @brief      lora example run the cad
 * @param[out] *enable pointer to a enable buffer
 * @return     status code
 *             - 0 success
 *             - 1 run failed
 * @note       none
 */
uint8_t llcc68_lora_run_cad(llcc68_bool_t *enable);

/**
 * @brief      lora example get the status
 * @param[out] *rssi pointer to a rssi buffer
 * @param[out] *snr pointer to a snr buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 * @note       none
 */
uint8_t llcc68_lora_get_status(float *rssi, float *snr);

/**
 * @brief      lora example check packet error
 * @param[out] *enable pointer to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 check packet error failed
 * @note       none
 */
uint8_t llcc68_lora_check_packet_error(llcc68_bool_t *enable);

/**
 * @brief  lora example enter to the sleep mode
 * @return status code
 *         - 0 success
 *         - 1 sleep failed
 * @note   none
 */
uint8_t llcc68_lora_sleep(void);

/**
 * @brief  lora example wake up the chip
 * @return status code
 *         - 0 success
 *         - 1 wake up failed
 * @note   none
 */
uint8_t llcc68_lora_wake_up(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
