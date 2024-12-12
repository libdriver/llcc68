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
 * @file      driver_llcc68.h
 * @brief     driver llcc68 header file
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

#ifndef DRIVER_LLCC68_H
#define DRIVER_LLCC68_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup llcc68_driver llcc68 driver function
 * @brief    llcc68 driver modules
 * @{
 */

/**
 * @addtogroup llcc68_command_driver
 * @{
 */

/**
 * @brief llcc68 bool enumeration definition
 */
typedef enum
{
    LLCC68_BOOL_FALSE = 0x00,        /**< disable function */
    LLCC68_BOOL_TRUE  = 0x01,        /**< enable function */
} llcc68_bool_t;

/**
 * @brief llcc68 start mode enumeration definition
 */
typedef enum
{
    LLCC68_START_MODE_COLD = 0x00,        /**< cold start mode */
    LLCC68_START_MODE_WARM = 0x01,        /**< warm start mode */
} llcc68_start_mode_t;

/**
 * @brief llcc68 clock source enumeration definition
 */
typedef enum
{
    LLCC68_CLOCK_SOURCE_RC_13M     = 0x00,        /**< rc 13MHz */
    LLCC68_CLOCK_SOURCE_XTAL_32MHZ = 0x01,        /**< xtal 32MHz */
} llcc68_clock_source_t;

/**
 * @brief llcc68 regulator mode enumeration definition
 */
typedef enum
{
    LLCC68_REGULATOR_MODE_ONLY_LDO  = 0x00,        /**< only ldo used for all modes */
    LLCC68_REGULATOR_MODE_DC_DC_LDO = 0x01,        /**< dc_dc + ldo used for standby_xosc, fs, rx and rx modes */
} llcc68_regulator_mode_t;

/**
 * @brief llcc68 calibration enumeration definition
 */
typedef enum
{
    LLCC68_CALIBRATION_RC64K      = (1 << 0),        /**< rc64k calibration */
    LLCC68_CALIBRATION_RC13M      = (1 << 1),        /**< rc13M calibration */
    LLCC68_CALIBRATION_PLL        = (1 << 2),        /**< pll calibration */
    LLCC68_CALIBRATION_ADC_PULSE  = (1 << 3),        /**< adc pulse calibration */
    LLCC68_CALIBRATION_ADC_BULK_N = (1 << 4),        /**< adc bulk n calibration */
    LLCC68_CALIBRATION_ADC_BULK_P = (1 << 5),        /**< adc bulk p calibration */
    LLCC68_CALIBRATION_IMAGE      = (1 << 6),        /**< image calibration */
} llcc68_calibration_t;

/**
 * @brief llcc68 irq enumeration definition
 */
typedef enum
{
    LLCC68_IRQ_TX_DONE           = (1 << 0),        /**< packet transmission completed */
    LLCC68_IRQ_RX_DONE           = (1 << 1),        /**< packet received */
    LLCC68_IRQ_PREAMBLE_DETECTED = (1 << 2),        /**< preamble detected */
    LLCC68_IRQ_SYNC_WORD_VALID   = (1 << 3),        /**< valid sync word detected */
    LLCC68_IRQ_HEADER_VALID      = (1 << 4),        /**< valid header */
    LLCC68_IRQ_HEADER_ERR        = (1 << 5),        /**< header error */
    LLCC68_IRQ_CRC_ERR           = (1 << 6),        /**< crc error */
    LLCC68_IRQ_CAD_DONE          = (1 << 7),        /**< cad done */
    LLCC68_IRQ_CAD_DETECTED      = (1 << 8),        /**< cad detected */
    LLCC68_IRQ_TIMEOUT           = (1 << 9),        /**< timeout */
} llcc68_irq_t;

/**
 * @brief llcc68 rx tx fallback mode enumeration definition
 */
typedef enum
{
    LLCC68_RX_TX_FALLBACK_MODE_FS         = 0x40,        /**< the radio goes into fs mode after tx or rx */
    LLCC68_RX_TX_FALLBACK_MODE_STDBY_XOSC = 0x30,        /**< the radio goes into standby_xosc mode after tx or rx */
    LLCC68_RX_TX_FALLBACK_MODE_STDBY_RC   = 0x20,        /**< the radio goes into standby_rc mode after tx or rx */
} llcc68_rx_tx_fallback_mode_t;

/**
 * @brief llcc68 tcxo voltage enumeration definition
 */
typedef enum
{
    LLCC68_TCXO_VOLTAGE_1P6V = 0x00,        /**< dio3 outputs 1.6 V to supply the tcxo */
    LLCC68_TCXO_VOLTAGE_1P7V = 0x01,        /**< dio3 outputs 1.7 V to supply the tcxo */
    LLCC68_TCXO_VOLTAGE_1P8V = 0x02,        /**< dio3 outputs 1.8 V to supply the tcxo */
    LLCC68_TCXO_VOLTAGE_2P2V = 0x03,        /**< dio3 outputs 2.2 V to supply the tcxo */
    LLCC68_TCXO_VOLTAGE_2P4V = 0x04,        /**< dio3 outputs 2.4 V to supply the tcxo */
    LLCC68_TCXO_VOLTAGE_2P7V = 0x05,        /**< dio3 outputs 2.7 V to supply the tcxo */
    LLCC68_TCXO_VOLTAGE_3P0V = 0x06,        /**< dio3 outputs 3.0 V to supply the tcxo */
    LLCC68_TCXO_VOLTAGE_3P3V = 0x07,        /**< dio3 outputs 3.3 V to supply the tcxo */
} llcc68_tcxo_voltage_t;

/**
 * @brief llcc68 packet type enumeration definition
 */
typedef enum
{
    LLCC68_PACKET_TYPE_GFSK = 0x00,        /**< gfsk mode */
    LLCC68_PACKET_TYPE_LORA = 0x01,        /**< lora mode */
} llcc68_packet_type_t;

/**
 * @brief llcc68 ramp time enumeration definition
 */
typedef enum
{
    LLCC68_RAMP_TIME_10US   = 0x00,        /**< 10us */
    LLCC68_RAMP_TIME_20US   = 0x01,        /**< 20us */
    LLCC68_RAMP_TIME_40US   = 0x02,        /**< 40us */
    LLCC68_RAMP_TIME_80US   = 0x03,        /**< 80us */
    LLCC68_RAMP_TIME_200US  = 0x04,        /**< 200us */
    LLCC68_RAMP_TIME_800US  = 0x05,        /**< 800us */
    LLCC68_RAMP_TIME_1700US = 0x06,        /**< 1700us */
    LLCC68_RAMP_TIME_3400US = 0x07,        /**< 3400us */
} llcc68_ramp_time_t;

/**
 * @brief llcc68 gfsk pulse shape enumeration definition
 */
typedef enum
{
    LLCC68_GFSK_PULSE_SHAPE_NO_FILTER       = 0x00,        /**< no filter applied */
    LLCC68_GFSK_PULSE_SHAPE_GAUSSIAN_BT_0P3 = 0x08,        /**< gaussian bt 0.3 */
    LLCC68_GFSK_PULSE_SHAPE_GAUSSIAN_BT_0P5 = 0x09,        /**< gaussian bt 0.5 */
    LLCC68_GFSK_PULSE_SHAPE_GAUSSIAN_BT_0P7 = 0x0A,        /**< gaussian bt 0.7 */
    LLCC68_GFSK_PULSE_SHAPE_GAUSSIAN_BT_1   = 0x0B,        /**< gaussian bt 1 */
} llcc68_gfsk_pulse_shape_t;

/**
 * @brief llcc68 gfsk bandwidth enumeration definition
 */
typedef enum
{
    LLCC68_GFSK_BANDWIDTH_4P8_KHZ   = 0x1F,        /**< 4.8 kHz */
    LLCC68_GFSK_BANDWIDTH_5P8_KHZ   = 0x17,        /**< 5.8 kHz */
    LLCC68_GFSK_BANDWIDTH_7P3_KHZ   = 0x0F,        /**< 7.3 kHz */
    LLCC68_GFSK_BANDWIDTH_9P7_KHZ   = 0x1E,        /**< 9.7 kHz */
    LLCC68_GFSK_BANDWIDTH_11P7_KHZ  = 0x16,        /**< 11.7 kHz */
    LLCC68_GFSK_BANDWIDTH_14P6_KHZ  = 0x0E,        /**< 14.6 kHz */
    LLCC68_GFSK_BANDWIDTH_19P5_KHZ  = 0x1D,        /**< 19.5 kHz */
    LLCC68_GFSK_BANDWIDTH_23P4_KHZ  = 0x15,        /**< 23.4 kHz */
    LLCC68_GFSK_BANDWIDTH_29P3_KHZ  = 0x0D,        /**< 29.3 kHz */
    LLCC68_GFSK_BANDWIDTH_39_KHZ    = 0x1C,        /**< 39 kHz */
    LLCC68_GFSK_BANDWIDTH_46P9_KHZ  = 0x14,        /**< 46.9 kHz */
    LLCC68_GFSK_BANDWIDTH_58P6_KHZ  = 0x0C,        /**< 58.6 kHz */
    LLCC68_GFSK_BANDWIDTH_78P2_KHZ  = 0x1B,        /**< 78.2 kHz */
    LLCC68_GFSK_BANDWIDTH_93P8_KHZ  = 0x13,        /**< 93.8 kHz */
    LLCC68_GFSK_BANDWIDTH_117P3_KHZ = 0x0B,        /**< 117.3 kHz */
    LLCC68_GFSK_BANDWIDTH_156P2_KHZ = 0x1A,        /**< 156.2 kHz */
    LLCC68_GFSK_BANDWIDTH_187P2_KHZ = 0x12,        /**< 187.2 kHz */
    LLCC68_GFSK_BANDWIDTH_232P3_KHZ = 0x0A,        /**< 232.3 kHz */
    LLCC68_GFSK_BANDWIDTH_312_KHZ   = 0x19,        /**< 312 kHz */
    LLCC68_GFSK_BANDWIDTH_373P6_KHZ = 0x11,        /**< 373.6 kHz */
    LLCC68_GFSK_BANDWIDTH_467_KHZ   = 0x09,        /**< 467 kHz */
} llcc68_gfsk_bandwidth_t;

/**
 * @brief llcc68 lora spreading factor enumeration definition
 */
typedef enum
{
    LLCC68_LORA_SF_5  = 0x05,        /**< spreading factor 5 */
    LLCC68_LORA_SF_6  = 0x06,        /**< spreading factor 6 */
    LLCC68_LORA_SF_7  = 0x07,        /**< spreading factor 7 */
    LLCC68_LORA_SF_8  = 0x08,        /**< spreading factor 8 */
    LLCC68_LORA_SF_9  = 0x09,        /**< spreading factor 9 */
    LLCC68_LORA_SF_10 = 0x0A,        /**< spreading factor 10 */
    LLCC68_LORA_SF_11 = 0x0B,        /**< spreading factor 11 */
} llcc68_lora_sf_t;

/**
 * @brief llcc68 lora bandwidth enumeration definition
 */
typedef enum
{
    LLCC68_LORA_BANDWIDTH_125_KHZ   = 0x04,        /**< 125 kHz */
    LLCC68_LORA_BANDWIDTH_250_KHZ   = 0x05,        /**< 250 kHz */
    LLCC68_LORA_BANDWIDTH_500_KHZ   = 0x06,        /**< 500 kHz */
} llcc68_lora_bandwidth_t;

/**
 * @brief llcc68 lora coding rate enumeration definition
 */
typedef enum
{
    LLCC68_LORA_CR_4_5 = 0x01,        /**< cr 4/5 */
    LLCC68_LORA_CR_4_6 = 0x02,        /**< cr 4/6 */
    LLCC68_LORA_CR_4_7 = 0x03,        /**< cr 4/7 */
    LLCC68_LORA_CR_4_8 = 0x04,        /**< cr 4/8 */
} llcc68_lora_cr_t;

/**
 * @brief llcc68 gfsk preamble detector length enumeration definition
 */
typedef enum
{
    LLCC68_GFSK_PREAMBLE_DETECTOR_LENGTH_OFF     = 0x00,        /**< preamble detector length off */
    LLCC68_GFSK_PREAMBLE_DETECTOR_LENGTH_8_BITS  = 0x04,        /**< preamble detector length 8 bits */
    LLCC68_GFSK_PREAMBLE_DETECTOR_LENGTH_16_BITS = 0x05,        /**< preamble detector length 16 bits */
    LLCC68_GFSK_PREAMBLE_DETECTOR_LENGTH_24_BITS = 0x06,        /**< preamble detector length 24 bits */
    LLCC68_GFSK_PREAMBLE_DETECTOR_LENGTH_32_BITS = 0x07,        /**< preamble detector length 32 bits */
} llcc68_gfsk_preamble_detector_length_t;

/**
 * @brief llcc68 gfsk addr filter enumeration definition
 */
typedef enum
{
    LLCC68_GFSK_ADDR_FILTER_DISABLE                       = 0x00,        /**< address filtering disable */
    LLCC68_GFSK_ADDR_FILTER_ACTIVATED_NODE_ADDR           = 0x01,        /**< address filtering activated on node address */
    LLCC68_GFSK_ADDR_FILTER_ACTIVATED_NODE_BROADCAST_ADDR = 0x02,        /**< address filtering activated on node and broadcast addresses */
} llcc68_gfsk_addr_filter_t;

/**
 * @brief llcc68 gfsk packet type enumeration definition
 */
typedef enum
{
    LLCC68_GFSK_PACKET_TYPE_KNOWN_LENGTH  = 0x00,        /**< the packet length is known on both sides, 
                                                              the size of the payload is not added to the packet */
    LLCC68_GFSK_PACKET_TYPE_VARIABLE_SIZE = 0x01,        /**< the packet is on variable size, the first byte of the payload will be the
                                                              size of the packet*/
} llcc68_gfsk_packet_type_t;

/**
 * @brief llcc68 gfsk crc type enumeration definition
 */
typedef enum
{
    LLCC68_GFSK_CRC_TYPE_OFF        = 0x01,        /**< no crc */
    LLCC68_GFSK_CRC_TYPE_1_BYTE     = 0x00,        /**< crc computed on 1 byte */
    LLCC68_GFSK_CRC_TYPE_2_BYTE     = 0x02,        /**< crc computed on 2 byte */
    LLCC68_GFSK_CRC_TYPE_1_BYTE_INV = 0x04,        /**< crc computed on 1 byte and inverted */
    LLCC68_GFSK_CRC_TYPE_2_BYTE_INV = 0x06,        /**< crc computed on 2 byte and inverted */
} llcc68_gfsk_crc_type_t;

/**
 * @brief llcc68 lora header enumeration definition
 */
typedef enum
{
    LLCC68_LORA_HEADER_EXPLICIT = 0x00,        /**< explicit header */
    LLCC68_LORA_HEADER_IMPLICIT = 0x01,        /**< implicit header */
} llcc68_lora_header_t;

/**
 * @brief llcc68 lora crc type enumeration definition
 */
typedef enum
{
    LLCC68_LORA_CRC_TYPE_OFF = 0x00,        /**< crc off */
    LLCC68_LORA_CRC_TYPE_ON  = 0x01,        /**< crc on */
} llcc68_lora_crc_type_t;

/**
 * @brief llcc68 lora cad symbol num enumeration definition
 */
typedef enum
{
    LLCC68_LORA_CAD_SYMBOL_NUM_1  = 0x00,        /**< 1 symbol */
    LLCC68_LORA_CAD_SYMBOL_NUM_2  = 0x01,        /**< 2 symbol */
    LLCC68_LORA_CAD_SYMBOL_NUM_4  = 0x02,        /**< 4 symbol */
    LLCC68_LORA_CAD_SYMBOL_NUM_8  = 0x03,        /**< 8 symbol */
    LLCC68_LORA_CAD_SYMBOL_NUM_16 = 0x04,        /**< 16 symbol */
} llcc68_lora_cad_symbol_num_t;

/**
 * @brief llcc68 lora cad exit mode enumeration definition
 */
typedef enum
{
    LLCC68_LORA_CAD_EXIT_MODE_ONLY = 0x00,        /**< once done and whatever the activity on the channel,
                                                       the chip goes back to STBY_RC mode */
    LLCC68_LORA_CAD_EXIT_MODE_RX   = 0x01,        /**< the chip performs a cad operation and if an activity is detected, it stays in rx until
                                                       a packet is detected or the timer reaches the timeout defined by
                                                       cad timeout * 15.625 us */
} llcc68_lora_cad_exit_mode_t;

/**
 * @brief llcc68 fsk rx status enumeration definition
 */
typedef enum
{
    LLCC68_FSK_RX_STATUS_PREAMBLE_ERR = (1 << 7),        /**< preamble err */
    LLCC68_FSK_RX_STATUS_SYNC_ERR     = (1 << 6),        /**< sync err */
    LLCC68_FSK_RX_STATUS_ADDR_ERR     = (1 << 5),        /**< addr err */
    LLCC68_FSK_RX_STATUS_CRC_ERR      = (1 << 4),        /**< crc err*/
    LLCC68_FSK_RX_STATUS_LENGTH_ERR   = (1 << 3),        /**< length err */
    LLCC68_FSK_RX_STATUS_ABORT_ERR    = (1 << 2),        /**< abort err */
    LLCC68_FSK_RX_STATUS_PKT_RECEIVED = (1 << 1),        /**< pkt received */
    LLCC68_FSK_RX_STATUS_PKT_SEND     = (1 << 0),        /**< pkt send */
} llcc68_fsk_rx_status_t;

/**
 * @brief llcc68 op error enumeration definition
 */
typedef enum
{
    LLCC68_OP_ERROR_PA_RAMP_ERR     = (1 << 8),        /**< pa ramping failed */
    LLCC68_OP_ERROR_PLL_LOCK_ERR    = (1 << 6),        /**< pll failed to lock */
    LLCC68_OP_ERROR_XOSC_START_ERR  = (1 << 5),        /**< xosc failed to start */
    LLCC68_OP_ERROR_IMG_CALIB_ERR   = (1 << 4),        /**< img calibration failed */
    LLCC68_OP_ERROR_ADC_CALIB_ERR   = (1 << 3),        /**< adc calibration failed */
    LLCC68_OP_ERROR_PLL_CALIB_ERR   = (1 << 2),        /**< pll calibration failed */
    LLCC68_OP_ERROR_RC13M_CALIB_ERR = (1 << 1),        /**< rc13M calibration failed */
    LLCC68_OP_ERROR_RC64K_CALIB_ERR = (1 << 0),        /**< rc64k calibration failed */
} llcc68_op_error_t;

/**
 * @brief llcc68 handle structure definition
 */
typedef struct llcc68_handle_s
{
    uint8_t (*reset_gpio_init)(void);                                     /**< point to a reset_gpio_init function address */
    uint8_t (*reset_gpio_deinit)(void);                                   /**< point to a reset_gpio_deinit function address */
    uint8_t (*reset_gpio_write)(uint8_t value);                           /**< point to a reset_gpio_write function address */
    uint8_t (*busy_gpio_init)(void);                                      /**< point to a busy_gpio_init function address */
    uint8_t (*busy_gpio_deinit)(void);                                    /**< point to a busy_gpio_deinit function address */
    uint8_t (*busy_gpio_read)(uint8_t *value);                            /**< point to a busy_gpio_read function address */
    uint8_t (*spi_init)(void);                                            /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                          /**< point to a spi_deinit function address */
    uint8_t (*spi_write_read)(uint8_t *in_buf, uint32_t in_len,
                              uint8_t *out_buf, uint32_t out_len);        /**< point to a spi_write_read function address */
    void (*delay_ms)(uint32_t ms);                                        /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                      /**< point to a debug_print function address */
    void (*receive_callback)(uint16_t type,
                             uint8_t *buf, uint16_t len);                 /**< point to a receive_callback function address */
    uint8_t inited;                                                       /**< inited flag */
    uint8_t tx_done;                                                      /**< tx done flag */
    uint8_t cad_done;                                                     /**< cad done flag */
    uint8_t cad_detected;                                                 /**< cad detected flag */
    uint8_t crc_error;                                                    /**< crc error flag */
    uint8_t timeout;                                                      /**< timeout flag */
    uint8_t buf[384];                                                     /**< inner buffer */
    uint8_t receive_buf[256];                                             /**< receive buffer */
} llcc68_handle_t;

/**
 * @brief llcc68 information structure definition
 */
typedef struct llcc68_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} llcc68_info_t;

/**
 * @}
 */

/**
 * @defgroup llcc68_link_driver llcc68 link driver function
 * @brief    llcc68 link driver modules
 * @ingroup  llcc68_driver
 * @{
 */

/**
 * @brief     initialize llcc68_handle_t structure
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] STRUCTURE llcc68_handle_t
 * @note      none
 */
#define DRIVER_LLCC68_LINK_INIT(HANDLE, STRUCTURE)                memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_init function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a spi_init function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_SPI_INIT(HANDLE, FUC)                  (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a spi_deinit function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_SPI_DEINIT(HANDLE, FUC)                (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_write_read function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a spi_write_read function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_SPI_WRITE_READ(HANDLE, FUC)            (HANDLE)->spi_write_read = FUC

/**
 * @brief     link reset_gpio_init function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a reset_gpio_init function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_RESET_GPIO_INIT(HANDLE, FUC)           (HANDLE)->reset_gpio_init = FUC

/**
 * @brief     link reset_gpio_deinit function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a reset_gpio_deinit function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_RESET_GPIO_DEINIT(HANDLE, FUC)         (HANDLE)->reset_gpio_deinit = FUC

/**
 * @brief     link reset_gpio_write function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a reset_gpio_write function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_RESET_GPIO_WRITE(HANDLE, FUC)          (HANDLE)->reset_gpio_write = FUC

/**
 * @brief     link busy_gpio_init function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a busy_gpio_init function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_BUSY_GPIO_INIT(HANDLE, FUC)            (HANDLE)->busy_gpio_init = FUC

/**
 * @brief     link busy_gpio_deinit function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a busy_gpio_deinit function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_BUSY_GPIO_DEINIT(HANDLE, FUC)          (HANDLE)->busy_gpio_deinit = FUC

/**
 * @brief     link busy_gpio_read function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a busy_gpio_read function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_BUSY_GPIO_READ(HANDLE, FUC)            (HANDLE)->busy_gpio_read = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_DELAY_MS(HANDLE, FUC)                  (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_DEBUG_PRINT(HANDLE, FUC)               (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE pointer to an llcc68 handle structure
 * @param[in] FUC pointer to a receive_callback function address
 * @note      none
 */
#define DRIVER_LLCC68_LINK_RECEIVE_CALLBACK(HANDLE, FUC)          (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup llcc68_command_driver llcc68 command driver function
 * @brief    llcc68 command driver modules
 * @ingroup  llcc68_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an llcc68 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t llcc68_info(llcc68_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle pointer to an llcc68 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t llcc68_irq_handler(llcc68_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an llcc68 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset gpio initialization failed
 *            - 5 busy gpio initialization failed
 *            - 6 reset chip failed
 * @note      none
 */
uint8_t llcc68_init(llcc68_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an llcc68 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 *            - 5 power down failed
 *            - 6 busy gpio deinit failed
 *            - 7 reset gpio deinit failed
 * @note      none
 */
uint8_t llcc68_deinit(llcc68_handle_t *handle);

/**
 * @brief     send the lora data
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] standby_src clock source
 * @param[in] preamble_length preamble length
 * @param[in] header_type header type
 * @param[in] crc_type crc type
 * @param[in] invert_iq_enable bool value
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @param[in] us timeout
 * @return    status code
 *            - 0 success
 *            - 1 send failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 *            - 5 send timeout
 *            - 6 irq timeout
 *            - 7 unknown result
 * @note      none
 */
uint8_t llcc68_lora_transmit(llcc68_handle_t *handle, llcc68_clock_source_t standby_src,
                             uint16_t preamble_length, llcc68_lora_header_t header_type,
                             llcc68_lora_crc_type_t crc_type, llcc68_bool_t invert_iq_enable,
                             uint8_t *buf, uint16_t len, uint32_t us);

/**
 * @brief      run the cad
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *enable pointer to an enable buffer
 * @return     status code
 *             - 0 success
 *             - 1 lora cad failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 *             - 5 cad timeout
 * @note       none
 */
uint8_t llcc68_lora_cad(llcc68_handle_t *handle, llcc68_bool_t *enable);

/**
 * @brief     enter to the single receive mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] us rx timeout
 * @return    status code
 *            - 0 success
 *            - 1 lora single receive failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_single_receive(llcc68_handle_t *handle, double us);

/**
 * @brief     enter to the continuous receive mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 lora continuous receive failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_continuous_receive(llcc68_handle_t *handle);

/**
 * @brief     write the register
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] reg register address
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write register failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_write_register(llcc68_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      read the register
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  reg register address
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read register failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_read_register(llcc68_handle_t *handle, uint16_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief     write the buffer
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] offset buffer offset
 * @param[in] *buf pointer to a data buffer
 * @param[in] len data length
 * @return    status code
 *            - 0 success
 *            - 1 write buffer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_write_buffer(llcc68_handle_t *handle, uint8_t offset, uint8_t *buf, uint16_t len);

/**
 * @brief      read the buffer
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  offset buffer offset
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len data length
 * @return     status code
 *             - 0 success
 *             - 1 read buffer failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_read_buffer(llcc68_handle_t *handle, uint8_t offset, uint8_t *buf, uint16_t len);

/**
 * @brief      check the packet error
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *enable pointer to a bool buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_check_packet_error(llcc68_handle_t *handle, llcc68_bool_t *enable);

/**
 * @brief     enter to the sleep mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] mode start mode
 * @param[in] rtc_wake_up_enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set sleep failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_sleep(llcc68_handle_t *handle, llcc68_start_mode_t mode, llcc68_bool_t rtc_wake_up_enable);

/**
 * @brief     enter to the standby mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] src clock source
 * @return    status code
 *            - 0 success
 *            - 1 set standby failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_standby(llcc68_handle_t *handle, llcc68_clock_source_t src);

/**
 * @brief     enter to the frequency synthesis mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set frequency synthesis failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_frequency_synthesis(llcc68_handle_t *handle);

/**
 * @brief     enter to the tx mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] timeout tx timeout
 * @return    status code
 *            - 0 success
 *            - 1 set tx failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      0x000000 means timeout disable, tx single mode
 */
uint8_t llcc68_set_tx(llcc68_handle_t *handle, uint32_t timeout);

/**
 * @brief     enter to the rx mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] timeout rx timeout
 * @return    status code
 *            - 0 success
 *            - 1 set rx failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      0x000000 means timeout disable, rx single mode
 *            0xFFFFFF means rx continuous mode
 */
uint8_t llcc68_set_rx(llcc68_handle_t *handle, uint32_t timeout);

/**
 * @brief      convert the timeout to the register raw data
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  us timeout
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_timeout_convert_to_register(llcc68_handle_t *handle, double us, uint32_t *reg);

/**
 * @brief      convert the register raw data to the timeout
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  reg register raw data
 * @param[out] *us pointer to a us buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_timeout_convert_to_data(llcc68_handle_t *handle, uint32_t reg, double *us);

/**
 * @brief     stop timer on preamble
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set stop timer on preamble failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_stop_timer_on_preamble(llcc68_handle_t *handle, llcc68_bool_t enable);

/**
 * @brief     set the rx duty cycle
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] rx_period rx period
 * @param[in] sleep_period sleep period
 * @return    status code
 *            - 0 success
 *            - 1 set rx duty cycle failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      t_preamble + t_header <= 2 * rx_period + sleep_period
 */
uint8_t llcc68_set_rx_duty_cycle(llcc68_handle_t *handle, uint32_t rx_period, uint32_t sleep_period);

/**
 * @brief     run the cad
 * @param[in] *handle pointer to an llcc68 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set cad failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_cad(llcc68_handle_t *handle);

/**
 * @brief     enter to the tx continuous wave mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set tx continuous wave failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_tx_continuous_wave(llcc68_handle_t *handle);

/**
 * @brief     enter to the tx infinite preamble mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 set tx infinite preamble failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_tx_infinite_preamble(llcc68_handle_t *handle);

/**
 * @brief     set the regulator_mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] mode regulator mode
 * @return    status code
 *            - 0 success
 *            - 1 set regulator mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_regulator_mode(llcc68_handle_t *handle, llcc68_regulator_mode_t mode);

/**
 * @brief     set the calibration settings
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] settings calibration param
 * @return    status code
 *            - 0 success
 *            - 1 set calibration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_calibration(llcc68_handle_t *handle, uint8_t settings);

/**
 * @brief     set the calibration image frequency
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] freq1 image frequency 1
 * @param[in] freq2 image frequency 2
 * @return    status code
 *            - 0 success
 *            - 1 set calibration image failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_calibration_image(llcc68_handle_t *handle, uint8_t freq1, uint8_t freq2);

/**
 * @brief     set the pa config
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] pa_duty_cycle pa duty cycle
 * @param[in] hp_max max power
 * @return    status code
 *            - 0 success
 *            - 1 set the pa config failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_pa_config(llcc68_handle_t *handle, uint8_t pa_duty_cycle, uint8_t hp_max);

/**
 * @brief     set the rx tx fallback mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] mode rx tx fallback mode
 * @return    status code
 *            - 0 success
 *            - 1 set rx tx fallback mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_rx_tx_fallback_mode(llcc68_handle_t *handle, llcc68_rx_tx_fallback_mode_t mode);

/**
 * @brief     set the dio irq params
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] irq_mask irq mask
 * @param[in] dio1_mask dio1 mask
 * @param[in] dio2_mask dio2 mask
 * @param[in] dio3_mask dio3 mask
 * @return    status code
 *            - 0 success
 *            - 1 set dio irq params failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_dio_irq_params(llcc68_handle_t *handle, uint16_t irq_mask, uint16_t dio1_mask,
                                  uint16_t dio2_mask, uint16_t dio3_mask);

/**
 * @brief      get the irq status
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get irq status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_irq_status(llcc68_handle_t *handle, uint16_t *status);

/**
 * @brief     clear the irq status
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] clear_irq_param irq mask
 * @return    status code
 *            - 0 success
 *            - 1 clear irq status failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_clear_irq_status(llcc68_handle_t *handle, uint16_t clear_irq_param);

/**
 * @brief     set dio2 as rf switch ctrl
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set dio2 as rf switch ctrl failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_dio2_as_rf_switch_ctrl(llcc68_handle_t *handle, llcc68_bool_t enable);

/**
 * @brief     set dio3 as tcxo ctrl
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] voltage tcxo voltage
 * @param[in] delay tcxo ctrl delay
 * @return    status code
 *            - 0 success
 *            - 1 set dio3 as tcxo ctrl failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_dio3_as_tcxo_ctrl(llcc68_handle_t *handle, llcc68_tcxo_voltage_t voltage, uint32_t delay);

/**
 * @brief      convert the frequency to the register raw data
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  freq frequency
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_frequency_convert_to_register(llcc68_handle_t *handle, uint32_t freq, uint32_t *reg);

/**
 * @brief      convert the register raw data to the frequency
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  reg register raw data
 * @param[out] *freq pointer to a frequency buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_frequency_convert_to_data(llcc68_handle_t *handle, uint32_t reg, uint32_t *freq);

/**
 * @brief     set the rf frequency
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] reg rf frequency register data
 * @return    status code
 *            - 0 success
 *            - 1 set rf frequency failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_rf_frequency(llcc68_handle_t *handle, uint32_t reg);

/**
 * @brief     set the packet type
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] type packet type
 * @return    status code
 *            - 0 success
 *            - 1 set packet type failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_packet_type(llcc68_handle_t *handle, llcc68_packet_type_t type);

/**
 * @brief      get the packet type
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *type pointer to a packet type buffer
 * @return     status code
 *             - 0 success
 *             - 1 get packet type failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_packet_type(llcc68_handle_t *handle, llcc68_packet_type_t *type);

/**
 * @brief     set the tx params
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] dbm rf power 
 * @param[in] t ramp time
 * @return    status code
 *            - 0 success
 *            - 1 set tx params failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_tx_params(llcc68_handle_t *handle, int8_t dbm, llcc68_ramp_time_t t);

/**
 * @brief     set the modulation params in GFSK mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] br bit rate
 * @param[in] shape pulse shape
 * @param[in] bw bandwidth
 * @param[in] fdev frequency deviation
 * @return    status code
 *            - 0 success
 *            - 1 set gfsk modulation params failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_gfsk_modulation_params(llcc68_handle_t *handle, uint32_t br, llcc68_gfsk_pulse_shape_t shape, 
                                          llcc68_gfsk_bandwidth_t bw, uint32_t fdev);

/**
 * @brief      convert the bit rate to the register raw data
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  br bit rate
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_gfsk_bit_rate_convert_to_register(llcc68_handle_t *handle, uint32_t br, uint32_t *reg);

/**
 * @brief      convert the register raw data to the bit rate
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  reg register raw data
 * @param[out] *br pointer to a bit rate buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_gfsk_bit_rate_convert_to_data(llcc68_handle_t *handle, uint32_t reg, uint32_t *br);

/**
 * @brief      convert the frequency deviation to the register raw data
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  freq frequency deviation
 * @param[out] *reg pointer to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_gfsk_frequency_deviation_convert_to_register(llcc68_handle_t *handle, uint32_t freq, uint32_t *reg);

/**
 * @brief      convert the register raw data to the frequency deviation
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  reg register raw data
 * @param[out] *freq pointer to a frequency deviation buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_gfsk_frequency_deviation_convert_to_data(llcc68_handle_t *handle, uint32_t reg, uint32_t *freq);

/**
 * @brief     set the modulation params in LoRa mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] sf spreading factor
 * @param[in] bw bandwidth
 * @param[in] cr coding rate
 * @param[in] low_data_rate_optimize_enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set lora modulation params failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_lora_modulation_params(llcc68_handle_t *handle, llcc68_lora_sf_t sf, llcc68_lora_bandwidth_t bw, 
                                          llcc68_lora_cr_t cr, llcc68_bool_t low_data_rate_optimize_enable);

/**
 * @brief     set the packet params in GFSK mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] preamble_length preamble length
 * @param[in] detector_length preamble detector length
 * @param[in] sync_word_length sync word length
 * @param[in] filter address filter
 * @param[in] packet_type packet type
 * @param[in] payload_length length of the payload
 * @param[in] crc_type crc type
 * @param[in] whitening_enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set gfsk packet params failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 *            - 5 sync word length is over 0x40
 * @note      none
 */
uint8_t llcc68_set_gfsk_packet_params(llcc68_handle_t *handle, uint16_t preamble_length,
                                      llcc68_gfsk_preamble_detector_length_t detector_length,
                                      uint8_t sync_word_length, llcc68_gfsk_addr_filter_t filter,
                                      llcc68_gfsk_packet_type_t packet_type, uint8_t payload_length,
                                      llcc68_gfsk_crc_type_t crc_type, llcc68_bool_t whitening_enable);

/**
 * @brief     set the packet params in LoRa mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] preamble_length preamble length
 * @param[in] header_type header type
 * @param[in] payload_length length of payload
 * @param[in] crc_type crc type
 * @param[in] invert_iq_enable bool value
 * @return    status code
 *            - 0 success
 *            - 1 set lora packet params failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_lora_packet_params(llcc68_handle_t *handle, uint16_t preamble_length,
                                      llcc68_lora_header_t header_type, uint8_t payload_length,
                                      llcc68_lora_crc_type_t crc_type, llcc68_bool_t invert_iq_enable);

/**
 * @brief     set the cad params
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] num cad symbol number
 * @param[in] cad_det_peak cad detection peak
 * @param[in] cad_det_min min cad detection peak 
 * @param[in] mode cad exit mode
 * @param[in] timeout cad timeout
 * @return    status code
 *            - 0 success
 *            - 1 set cad params failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_cad_params(llcc68_handle_t *handle, llcc68_lora_cad_symbol_num_t num,
                              uint8_t cad_det_peak, uint8_t cad_det_min, llcc68_lora_cad_exit_mode_t mode,
                              uint32_t timeout);

/**
 * @brief     set the buffer base address
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] tx_base_addr tx base address
 * @param[in] rx_base_addr rx base address
 * @return    status code
 *            - 0 success
 *            - 1 set buffer base address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_buffer_base_address(llcc68_handle_t *handle, uint8_t tx_base_addr, uint8_t rx_base_addr);

/**
 * @brief     set the lora symbol number timeout
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] symb_num symbol number
 * @return    status code
 *            - 0 success
 *            - 1 set lora symb num timeout failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_lora_symb_num_timeout(llcc68_handle_t *handle, uint8_t symb_num);

/**
 * @brief      get the status
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_status(llcc68_handle_t *handle, uint8_t *status);

/**
 * @brief      get the rx buffer status
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *payload_length_rx pointer to an rx payload length buffer
 * @param[out] *rx_start_buffer_pointer pointer to an rx start pointer buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx buffer status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_rx_buffer_status(llcc68_handle_t *handle, uint8_t *payload_length_rx, uint8_t *rx_start_buffer_pointer);

/**
 * @brief      get the packet status in GFSK mode
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *rx_status pointer to an rx status buffer
 * @param[out] *rssi_sync_raw pointer to an rssi sync raw buffer
 * @param[out] *rssi_avg_raw pointer to an rssi avg raw buffer
 * @param[out] *rssi_sync pointer to an rssi sync buffer
 * @param[out] *rssi_avg pointer to an rssi avg buffer
 * @return     status code
 *             - 0 success
 *             - 1 get gfsk packet status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_gfsk_packet_status(llcc68_handle_t *handle, uint8_t *rx_status, uint8_t *rssi_sync_raw,
                                      uint8_t *rssi_avg_raw, float *rssi_sync, float *rssi_avg);

/**
 * @brief      get the packet status in LoRa mode
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *rssi_pkt_raw pointer to an rssi packet raw buffer
 * @param[out] *snr_pkt_raw pointer to an snr packet raw buffer
 * @param[out] *signal_rssi_pkt_raw pointer to a signal rssi packet raw buffer
 * @param[out] *rssi_pkt pointer to an rssi packet buffer
 * @param[out] *snr_pkt pointer to an snr packet buffer
 * @param[out] *signal_rssi_pkt pointer to a signal rssi packet buffer
 * @return     status code
 *             - 0 success
 *             - 1 get lora packet status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_lora_packet_status(llcc68_handle_t *handle, uint8_t *rssi_pkt_raw, uint8_t *snr_pkt_raw,
                                      uint8_t *signal_rssi_pkt_raw, float *rssi_pkt, float *snr_pkt, float *signal_rssi_pkt);

/**
 * @brief      get the instantaneous rssi
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *rssi_inst_raw pointer to an rssi instantaneous raw buffer
 * @param[out] *rssi_inst pointer to an rssi instantaneous buffer
 * @return     status code
 *             - 0 success
 *             - 1 get instantaneous rssi failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_instantaneous_rssi(llcc68_handle_t *handle, uint8_t *rssi_inst_raw, float *rssi_inst);

/**
 * @brief      get the stats
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *pkt_received pointer to a packet received counter buffer
 * @param[out] *pkt_crc_error pointer to a packet crc error buffer
 * @param[out] *pkt_length_header_error pointer to a packet length header error buffer
 * @return     status code
 *             - 0 success
 *             - 1 get stats failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_stats(llcc68_handle_t *handle, uint16_t *pkt_received, uint16_t *pkt_crc_error, uint16_t *pkt_length_header_error);

/**
 * @brief     reset the stats
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] pkt_received received packet counter mask
 * @param[in] pkt_crc_error packet crc error mask
 * @param[in] pkt_length_header_error packet length header error mask
 * @return    status code
 *            - 0 success
 *            - 1 reset stats failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_reset_stats(llcc68_handle_t *handle, uint16_t pkt_received, uint16_t pkt_crc_error, uint16_t pkt_length_header_error);

/**
 * @brief      get the device errors
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *op_error pointer to an op error buffer
 * @return     status code
 *             - 0 success
 *             - 1 get device errors failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_device_errors(llcc68_handle_t *handle, uint16_t *op_error);

/**
 * @brief     clear the device errors
 * @param[in] *handle pointer to an llcc68 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 clear device errors failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_clear_device_errors(llcc68_handle_t *handle);

/**
 * @}
 */

/**
 * @defgroup llcc68_register_driver llcc68 register driver function
 * @brief    llcc68 register driver modules
 * @ingroup  llcc68_driver
 * @{
 */

/**
 * @brief     set the whitening initial value in FSK mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] value set value
 * @return    status code
 *            - 0 success
 *            - 1 set fsk whitening initial value failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_fsk_whitening_initial_value(llcc68_handle_t *handle, uint16_t value);

/**
 * @brief      get the whitening initial value in FSK mode
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *value pointer to a value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fsk whitening initial value failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_fsk_whitening_initial_value(llcc68_handle_t *handle, uint16_t *value);

/**
 * @brief     set the crc initial value in FSK mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] value set value
 * @return    status code
 *            - 0 success
 *            - 1 set fsk crc initial value failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_fsk_crc_initical_value(llcc68_handle_t *handle, uint16_t value);

/**
 * @brief      get the crc initical value in FSK mode
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *value pointer to a value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fsk crc initical value failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_fsk_crc_initical_value(llcc68_handle_t *handle, uint16_t *value);

/**
 * @brief     set the crc polynomial value in FSK mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] value set value
 * @return    status code
 *            - 0 success
 *            - 1 set fsk crc polynomial value failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_fsk_crc_polynomial_value(llcc68_handle_t *handle, uint16_t value);

/**
 * @brief      get the crc polynomial value in FSK mode
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *value pointer to a value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fsk crc polynomial value failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_fsk_crc_polynomial_value(llcc68_handle_t *handle, uint16_t *value);

/**
 * @brief     set the sync word in FSK mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] *sync_word pointer to sync word buffer
 * @return    status code
 *            - 0 success
 *            - 1 set fsk sync word failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_fsk_sync_word(llcc68_handle_t *handle, uint8_t sync_word[8]);

/**
 * @brief      get the sync word in FSK mode
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *sync_word pointer to sync word buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fsk sync word failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_fsk_sync_word(llcc68_handle_t *handle, uint8_t sync_word[8]);

/**
 * @brief     set the node address in FSK mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] addr node address
 * @return    status code
 *            - 0 success
 *            - 1 set fsk node address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_fsk_node_address(llcc68_handle_t *handle, uint8_t addr);

/**
 * @brief      get the node address in FSK mode
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *addr pointer to a node address buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fsk node address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_fsk_node_address(llcc68_handle_t *handle, uint8_t *addr);

/**
 * @brief     set the broadcast address in FSK mode
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] addr broadcast address
 * @return    status code
 *            - 0 success
 *            - 1 set fsk broadcast address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_fsk_broadcast_address(llcc68_handle_t *handle, uint8_t addr);

/**
 * @brief      get the broadcast address in FSK mode
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *addr pointer to a broadcast address buffer
 * @return     status code
 *             - 0 success
 *             - 1 get fsk broadcast address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_fsk_broadcast_address(llcc68_handle_t *handle, uint8_t *addr);

/**
 * @brief     set the iq polarity
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] setup settings
 * @return    status code
 *            - 0 success
 *            - 1 set iq polarity failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_iq_polarity(llcc68_handle_t *handle, uint8_t setup);

/**
 * @brief      get the iq polarity
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *setup pointer to a settings buffer
 * @return     status code
 *             - 0 success
 *             - 1 get iq polarity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_iq_polarity(llcc68_handle_t *handle, uint8_t *setup);

/**
 * @brief     set the lora sync word
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] sync_word sync word
 * @return    status code
 *            - 0 success
 *            - 1 set lora sync word failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_lora_sync_word(llcc68_handle_t *handle, uint16_t sync_word);

/**
 * @brief      get the lora sync word
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *sync_word pointer to an sync word buffer
 * @return     status code
 *             - 0 success
 *             - 1 get lora sync word failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_lora_sync_word(llcc68_handle_t *handle, uint16_t *sync_word);

/**
 * @brief      get the random number
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *r pointer to a random number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get random number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_random_number(llcc68_handle_t *handle, uint32_t *r);

/**
 * @brief     set the tx modulation
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] modulation tx modulation
 * @return    status code
 *            - 0 success
 *            - 1 set tx modulation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_tx_modulation(llcc68_handle_t *handle, uint8_t modulation);

/**
 * @brief      get the tx modulation
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *modulation pointer to a tx modulation buffer
 * @return     status code
 *             - 0 success
 *             - 1 get tx modulation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_tx_modulation(llcc68_handle_t *handle, uint8_t *modulation);

/**
 * @brief     set the rx gain
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] gain rx gain
 * @return    status code
 *            - 0 success
 *            - 1 set rx gain failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_rx_gain(llcc68_handle_t *handle, uint8_t gain);

/**
 * @brief      get the rx gain
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *gain pointer to an rx gain buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx gain failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_rx_gain(llcc68_handle_t *handle, uint8_t *gain);

/**
 * @brief     set the tx clamp config
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] config tx clamp config
 * @return    status code
 *            - 0 success
 *            - 1 set tx clamp config failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_tx_clamp_config(llcc68_handle_t *handle, uint8_t config);

/**
 * @brief      get the tx clamp config
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *config pointer to a tx clamp config buffer
 * @return     status code
 *             - 0 success
 *             - 1 get tx clamp config failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_tx_clamp_config(llcc68_handle_t *handle, uint8_t *config);

/**
 * @brief     set the ocp
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] ocp over current protection level
 * @return    status code
 *            - 0 success
 *            - 1 set ocp failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_ocp(llcc68_handle_t *handle, uint8_t ocp);

/**
 * @brief      get the ocp
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *ocp pointer to an over current protection level buffer
 * @return     status code
 *             - 0 success
 *             - 1 get ocp failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_ocp(llcc68_handle_t *handle, uint8_t *ocp);

/**
 * @brief     set the rtc control
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] control rtc control
 * @return    status code
 *            - 0 success
 *            - 1 set rtc control failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_rtc_control(llcc68_handle_t *handle, uint8_t control);

/**
 * @brief      get the rtc control
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *control pointer to an rtc control buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rtc control failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_rtc_control(llcc68_handle_t *handle, uint8_t *control);

/**
 * @brief     set the xta trim
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] trim xta trim
 * @return    status code
 *            - 0 success
 *            - 1 set xta trim failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_xta_trim(llcc68_handle_t *handle, uint8_t trim);

/**
 * @brief      get the xta trim
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *trim pointer to an xta trim buffer
 * @return     status code
 *             - 0 success
 *             - 1 get xta trim failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_xta_trim(llcc68_handle_t *handle, uint8_t *trim);

/**
 * @brief     set the xtb trim
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] trim xtb trim
 * @return    status code
 *            - 0 success
 *            - 1 set xtb trim failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_xtb_trim(llcc68_handle_t *handle, uint8_t trim);

/**
 * @brief      get the xtb trim
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *trim pointer to an xtb trim buffer
 * @return     status code
 *             - 0 success
 *             - 1 get xtb trim failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_xtb_trim(llcc68_handle_t *handle, uint8_t *trim);

/**
 * @brief     set the dio3 output
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] control dio3 output control
 * @return    status code
 *            - 0 success
 *            - 1 set dio3 output control failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_dio3_output_control(llcc68_handle_t *handle, uint8_t control);

/**
 * @brief      get the dio3 output
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *control pointer to a dio3 output control buffer
 * @return     status code
 *             - 0 success
 *             - 1 get dio3 output control failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_dio3_output_control(llcc68_handle_t *handle, uint8_t *control);

/**
 * @brief     set the event mask
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] mask event mask
 * @return    status code
 *            - 0 success
 *            - 1 set event mask failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_event_mask(llcc68_handle_t *handle, uint8_t mask);

/**
 * @brief      get the event mask
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *mask pointer to an event mask buffer
 * @return     status code
 *             - 0 success
 *             - 1 get event mask failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_event_mask(llcc68_handle_t *handle, uint8_t *mask);

/**
 * @brief     set the dio output enable
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] enable enable settings
 * @return    status code
 *            - 0 success
 *            - 1 set dio output enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_dio_output_enable(llcc68_handle_t *handle, uint8_t enable);

/**
 * @brief      get the dio output enable
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *enable pointer to an enable settings buffer
 * @return     status code
 *             - 0 success
 *             - 1 get dio output enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_dio_output_enable(llcc68_handle_t *handle, uint8_t *enable);

/**
 * @brief     set the dio input enable
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] enable enable settings
 * @return    status code
 *            - 0 success
 *            - 1 set dio input enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_dio_input_enable(llcc68_handle_t *handle, uint8_t enable);

/**
 * @brief      get the dio input enable
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *enable pointer to an enable settings buffer
 * @return     status code
 *             - 0 success
 *             - 1 get dio input enable failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_dio_input_enable(llcc68_handle_t *handle, uint8_t *enable);

/**
 * @brief     set the pull up control
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] control pull up control
 * @return    status code
 *            - 0 success
 *            - 1 set pull up control failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_pull_up_control(llcc68_handle_t *handle, uint8_t control);

/**
 * @brief      get the pull up control
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *control pointer to a pull up control buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pull up control failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_pull_up_control(llcc68_handle_t *handle, uint8_t *control);

/**
 * @brief     set the pull down control
 * @param[in] *handle pointer to an llcc68 handle structure
 * @param[in] control pull down control
 * @return    status code
 *            - 0 success
 *            - 1 set pull down control failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 chip is busy
 * @note      none
 */
uint8_t llcc68_set_pull_down_control(llcc68_handle_t *handle, uint8_t control);

/**
 * @brief      get the pull down control
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[out] *control pointer to a pull down control buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pull down control failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 chip is busy
 * @note       none
 */
uint8_t llcc68_get_pull_down_control(llcc68_handle_t *handle, uint8_t *control);

/**
 * @}
 */

/**
 * @defgroup llcc68_extern_driver llcc68 extern driver function
 * @brief    llcc68 extern driver modules
 * @ingroup  llcc68_driver
 * @{
 */

/**
 * @brief      write and read register
 * @param[in]  *handle pointer to an llcc68 handle structure
 * @param[in]  *in_buf pointer to an input buffer
 * @param[in]  in_len input length
 * @param[out] *out_buf pointer to an output buffer
 * @param[in]  out_len output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t llcc68_write_read_reg(llcc68_handle_t *handle, uint8_t *in_buf, uint32_t in_len,
                              uint8_t *out_buf, uint32_t out_len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
