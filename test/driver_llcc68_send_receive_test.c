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
 * @file      driver_llcc68_send_receive_test.c
 * @brief     driver llcc68 send receive test source file
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

#include "driver_llcc68_send_receive_test.h"

static llcc68_handle_t gs_handle;        /**< llcc68 handle */
static uint8_t gs_rx_done;               /**< rx done */
static uint8_t gs_send_buffer[256];      /**< inner send buffer*/

/**
 * @brief     interface receive callback
 * @param[in] type is the receive callback type
 * @param[in] *buf points to a buffer address
 * @param[in] len is the buffer length
 * @note      none
 */
static void a_callback(uint16_t type, uint8_t *buf, uint16_t len)
{
    switch (type)
    {
        case LLCC68_IRQ_TX_DONE :
        {
            llcc68_interface_debug_print("llcc68: irq tx done.\n");
            
            break;
        }
        case LLCC68_IRQ_RX_DONE :
        {
            uint16_t i;
            llcc68_bool_t enable;
            uint8_t rssi_pkt_raw;
            uint8_t snr_pkt_raw;
            uint8_t signal_rssi_pkt_raw;
            float rssi_pkt;
            float snr_pkt;
            float signal_rssi_pkt;
            
            llcc68_interface_debug_print("llcc68: irq rx done.\n");
            
            /* get the status */
            if (llcc68_get_lora_packet_status(&gs_handle, (uint8_t *)&rssi_pkt_raw, (uint8_t *)&snr_pkt_raw,
                                             (uint8_t *)&signal_rssi_pkt_raw, (float *)&rssi_pkt,
                                             (float *)&snr_pkt, (float *)&signal_rssi_pkt) != 0)
            {
                return;
            }
            llcc68_interface_debug_print("llcc68: rssi is %0.1f.\n", rssi_pkt);
            llcc68_interface_debug_print("llcc68: snr is %0.2f.\n", snr_pkt);
            llcc68_interface_debug_print("llcc68: signal rssi is %0.1f.\n", signal_rssi_pkt);
            
            /* check the error */
            if (llcc68_check_packet_error(&gs_handle, &enable) != 0)
            {
                return;
            }
            if ((enable == LLCC68_BOOL_FALSE) && len)
            {
                for (i = 0; i < len; i++)
                {
                    llcc68_interface_debug_print("0x%02X ", buf[i]);
                }
                llcc68_interface_debug_print("\n");
                gs_rx_done = 1;
            }
            
            break;
        }
        case LLCC68_IRQ_PREAMBLE_DETECTED :
        {
            llcc68_interface_debug_print("llcc68: irq preamble detected.\n");
            
            break;
        }
        case LLCC68_IRQ_SYNC_WORD_VALID :
        {
            llcc68_interface_debug_print("llcc68: irq valid sync word detected.\n");
            
            break;
        }
        case LLCC68_IRQ_HEADER_VALID :
        {
            llcc68_interface_debug_print("llcc68: irq valid header.\n");
            
            break;
        }
        case LLCC68_IRQ_HEADER_ERR :
        {
            llcc68_interface_debug_print("llcc68: irq header error.\n");
            
            break;
        }
        case LLCC68_IRQ_CRC_ERR :
        {
            llcc68_interface_debug_print("llcc68: irq crc error.\n");
            
            break;
        }
        case LLCC68_IRQ_CAD_DONE :
        {
            llcc68_interface_debug_print("llcc68: irq cad done.\n");
            
            break;
        }
        case LLCC68_IRQ_CAD_DETECTED :
        {
            llcc68_interface_debug_print("llcc68: irq cad detected.\n");
            
            break;
        }
        case LLCC68_IRQ_TIMEOUT :
        {
            llcc68_interface_debug_print("llcc68: irq timeout.\n");
            
            break;
        }
        default :
        {
            break;
        }
    }
}

/**
 * @brief  llcc68 interrupt test irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t llcc68_interrupt_test_irq_handler(void)
{
    if (llcc68_irq_handler(&gs_handle) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief  send test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t llcc68_send_test(void)
{
    uint8_t res;
    uint32_t reg;
    uint8_t modulation;
    uint8_t config;
    uint8_t i;
    
    /* link interface function */
    DRIVER_LLCC68_LINK_INIT(&gs_handle, llcc68_handle_t);
    DRIVER_LLCC68_LINK_SPI_INIT(&gs_handle, llcc68_interface_spi_init);
    DRIVER_LLCC68_LINK_SPI_DEINIT(&gs_handle, llcc68_interface_spi_deinit);
    DRIVER_LLCC68_LINK_SPI_WRITE_READ(&gs_handle, llcc68_interface_spi_write_read);
    DRIVER_LLCC68_LINK_RESET_GPIO_INIT(&gs_handle, llcc68_interface_reset_gpio_init);
    DRIVER_LLCC68_LINK_RESET_GPIO_DEINIT(&gs_handle, llcc68_interface_reset_gpio_deinit);
    DRIVER_LLCC68_LINK_RESET_GPIO_WRITE(&gs_handle, llcc68_interface_reset_gpio_write);
    DRIVER_LLCC68_LINK_BUSY_GPIO_INIT(&gs_handle, llcc68_interface_busy_gpio_init);
    DRIVER_LLCC68_LINK_BUSY_GPIO_DEINIT(&gs_handle, llcc68_interface_busy_gpio_deinit);
    DRIVER_LLCC68_LINK_BUSY_GPIO_READ(&gs_handle, llcc68_interface_busy_gpio_read);
    DRIVER_LLCC68_LINK_DELAY_MS(&gs_handle, llcc68_interface_delay_ms);
    DRIVER_LLCC68_LINK_DEBUG_PRINT(&gs_handle, llcc68_interface_debug_print);
    DRIVER_LLCC68_LINK_RECEIVE_CALLBACK(&gs_handle, llcc68_interface_receive_callback);
    
    /* start send test */
    llcc68_interface_debug_print("llcc68: start send test.\n");
    
    /* init the llcc68 */
    res = llcc68_init(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: init failed.\n");
       
        return 1;
    }
    
    /* enter standby */
    res = llcc68_set_standby(&gs_handle, LLCC68_CLOCK_SOURCE_XTAL_32MHZ);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set standby failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable stop timer on preamble */
    res = llcc68_set_stop_timer_on_preamble(&gs_handle, LLCC68_BOOL_FALSE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: stop timer on preamble failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set dc dc ldo */
    res = llcc68_set_regulator_mode(&gs_handle, LLCC68_REGULATOR_MODE_DC_DC_LDO);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set regulator mode failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set +17dBm power */
    res = llcc68_set_pa_config(&gs_handle, 0x02, 0x03);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set pa config failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enter to stdby rc mode */
    res = llcc68_set_rx_tx_fallback_mode(&gs_handle, LLCC68_RX_TX_FALLBACK_MODE_STDBY_XOSC);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rx tx fallback mode failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set dio irq */
    res = llcc68_set_dio_irq_params(&gs_handle, 0x03FFU, 0x03FFU, 0x0000, 0x0000);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set dio irq params failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear irq status */
    res = llcc68_clear_irq_status(&gs_handle, 0x03FFU);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: clear irq status failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set lora mode */
    res = llcc68_set_packet_type(&gs_handle, LLCC68_PACKET_TYPE_LORA);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set packet type failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* +17dBm */
    res = llcc68_set_tx_params(&gs_handle, 17, LLCC68_RAMP_TIME_10US);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx params failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* sf9, 125khz, cr4/5, disable low data rate optimize */
    res = llcc68_set_lora_modulation_params(&gs_handle, LLCC68_LORA_SF_9, LLCC68_LORA_BANDWIDTH_125_KHZ, 
                                            LLCC68_LORA_CR_4_5, LLCC68_BOOL_FALSE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora modulation params failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert the frequency */
    res = llcc68_frequency_convert_to_register(&gs_handle, 480100000U, (uint32_t *)&reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: convert to register failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the frequency */
    res = llcc68_set_rf_frequency(&gs_handle, reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rf frequency failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set base address */
    res = llcc68_set_buffer_base_address(&gs_handle, 0x00, 0x00);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set buffer base address failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* 1 lora symb num */
    res = llcc68_set_lora_symb_num_timeout(&gs_handle, 0);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora symb num timeout failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* reset stats */
    res = llcc68_reset_stats(&gs_handle, 0x0000, 0x0000, 0x0000);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: reset stats failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear device errors */
    res = llcc68_clear_device_errors(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: clear device errors failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the lora sync word */
    res = llcc68_set_lora_sync_word(&gs_handle, 0x1424U);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora sync word failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get tx modulation */
    res = llcc68_get_tx_modulation(&gs_handle, (uint8_t *)&modulation);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    modulation |= 0x04;
    
    /* set the tx modulation */
    res = llcc68_set_tx_modulation(&gs_handle, modulation);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the rx gain */
    res = llcc68_set_rx_gain(&gs_handle, 0x94);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rx gain failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the ocp */
    res = llcc68_set_ocp(&gs_handle, 0x38);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set ocp failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get the tx clamp config */
    res = llcc68_get_tx_clamp_config(&gs_handle, (uint8_t *)&config);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    config |= 0x1E;
    
    /* set the tx clamp config */
    res = llcc68_set_tx_clamp_config(&gs_handle, config);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* generate the buffer */
    for (i = 0; i < 192; i++)
    {
        gs_send_buffer[i] = i;
    }
    
    /* send the data */
    res = llcc68_lora_transmit(&gs_handle, LLCC68_CLOCK_SOURCE_XTAL_32MHZ,
                               50, LLCC68_LORA_HEADER_EXPLICIT,
                               LLCC68_LORA_CRC_TYPE_ON, LLCC68_BOOL_FALSE,
                              (uint8_t *)gs_send_buffer, 192, 0);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: lora send failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* finish send test */
    llcc68_interface_debug_print("llcc68: finish send test.\n");
    
    /* deinit */
    (void)llcc68_deinit(&gs_handle);
    
    return 0;
}

/**
 * @brief     receive test
 * @param[in] s is the timeout time
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t llcc68_receive_test(uint32_t s)
{
    uint8_t res;
    uint32_t reg;
    uint8_t modulation;
    uint8_t config;
    uint8_t setup;
    
    /* link interface function */
    DRIVER_LLCC68_LINK_INIT(&gs_handle, llcc68_handle_t);
    DRIVER_LLCC68_LINK_SPI_INIT(&gs_handle, llcc68_interface_spi_init);
    DRIVER_LLCC68_LINK_SPI_DEINIT(&gs_handle, llcc68_interface_spi_deinit);
    DRIVER_LLCC68_LINK_SPI_WRITE_READ(&gs_handle, llcc68_interface_spi_write_read);
    DRIVER_LLCC68_LINK_RESET_GPIO_INIT(&gs_handle, llcc68_interface_reset_gpio_init);
    DRIVER_LLCC68_LINK_RESET_GPIO_DEINIT(&gs_handle, llcc68_interface_reset_gpio_deinit);
    DRIVER_LLCC68_LINK_RESET_GPIO_WRITE(&gs_handle, llcc68_interface_reset_gpio_write);
    DRIVER_LLCC68_LINK_BUSY_GPIO_INIT(&gs_handle, llcc68_interface_busy_gpio_init);
    DRIVER_LLCC68_LINK_BUSY_GPIO_DEINIT(&gs_handle, llcc68_interface_busy_gpio_deinit);
    DRIVER_LLCC68_LINK_BUSY_GPIO_READ(&gs_handle, llcc68_interface_busy_gpio_read);
    DRIVER_LLCC68_LINK_DELAY_MS(&gs_handle, llcc68_interface_delay_ms);
    DRIVER_LLCC68_LINK_DEBUG_PRINT(&gs_handle, llcc68_interface_debug_print);
    DRIVER_LLCC68_LINK_RECEIVE_CALLBACK(&gs_handle, a_callback);
    
    /* start receive test */
    llcc68_interface_debug_print("llcc68: start receive test.\n");
    
    /* init the llcc68 */
    res = llcc68_init(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: init failed.\n");
       
        return 1;
    }
    
    /* enter standby */
    res = llcc68_set_standby(&gs_handle, LLCC68_CLOCK_SOURCE_XTAL_32MHZ);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set standby failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable stop timer on preamble */
    res = llcc68_set_stop_timer_on_preamble(&gs_handle, LLCC68_BOOL_FALSE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: stop timer on preamble failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set dc dc ldo */
    res = llcc68_set_regulator_mode(&gs_handle, LLCC68_REGULATOR_MODE_DC_DC_LDO);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set regulator mode failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set +17dBm power */
    res = llcc68_set_pa_config(&gs_handle, 0x02, 0x03);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set pa config failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* enter to stdby rc mode */
    res = llcc68_set_rx_tx_fallback_mode(&gs_handle, LLCC68_RX_TX_FALLBACK_MODE_STDBY_XOSC);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rx tx fallback mode failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set dio irq */
    res = llcc68_set_dio_irq_params(&gs_handle, 0x03FFU, 0x03FFU, 0x0000, 0x0000);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set dio irq params failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear irq status */
    res = llcc68_clear_irq_status(&gs_handle, 0x03FFU);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: clear irq status failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set lora mode */
    res = llcc68_set_packet_type(&gs_handle, LLCC68_PACKET_TYPE_LORA);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set packet type failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* +17dBm */
    res = llcc68_set_tx_params(&gs_handle, 17, LLCC68_RAMP_TIME_10US);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx params failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* sf9, 125khz, cr4/5, disable low data rate optimize */
    res = llcc68_set_lora_modulation_params(&gs_handle, LLCC68_LORA_SF_9, LLCC68_LORA_BANDWIDTH_125_KHZ, 
                                            LLCC68_LORA_CR_4_5, LLCC68_BOOL_FALSE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora modulation params failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* convert the frequency */
    res = llcc68_frequency_convert_to_register(&gs_handle, 480100000U, (uint32_t *)&reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: convert to register failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the frequency */
    res = llcc68_set_rf_frequency(&gs_handle, reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rf frequency failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set base address */
    res = llcc68_set_buffer_base_address(&gs_handle, 0x00, 0x00);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set buffer base address failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set lora symb num */
    res = llcc68_set_lora_symb_num_timeout(&gs_handle, 0);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora symb num timeout failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* reset stats */
    res = llcc68_reset_stats(&gs_handle, 0x0000, 0x0000, 0x0000);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: reset stats failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* clear device errors */
    res = llcc68_clear_device_errors(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: clear device errors failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the lora sync word */
    res = llcc68_set_lora_sync_word(&gs_handle, 0x1424U);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora sync word failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get tx modulation */
    res = llcc68_get_tx_modulation(&gs_handle, (uint8_t *)&modulation);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    modulation |= 0x04;
    
    /* set the tx modulation */
    res = llcc68_set_tx_modulation(&gs_handle, modulation);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the rx gain */
    res = llcc68_set_rx_gain(&gs_handle, 0x94);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rx gain failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set the ocp */
    res = llcc68_set_ocp(&gs_handle, 0x38);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set ocp failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get the tx clamp config */
    res = llcc68_get_tx_clamp_config(&gs_handle, (uint8_t *)&config);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    config |= 0x1E;
    
    /* set the tx clamp config */
    res = llcc68_set_tx_clamp_config(&gs_handle, config);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set lora packet params */
    res = llcc68_set_lora_packet_params(&gs_handle, 50,
                                        LLCC68_LORA_HEADER_EXPLICIT, 255,
                                        LLCC68_LORA_CRC_TYPE_ON, LLCC68_BOOL_FALSE);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora packet params failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* get iq polarity */
    res = llcc68_get_iq_polarity(&gs_handle, (uint8_t *)&setup);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get iq polarity failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    setup |= 1 << 2;
    
    /* set the iq polarity */
    res = llcc68_set_iq_polarity(&gs_handle, setup);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set iq polarity failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start receive */
    res = llcc68_continuous_receive(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: lora continuous receive failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    
    /* start receiving */
    llcc68_interface_debug_print("llcc68: start receiving...\n");
    gs_rx_done = 0;
    
    while ((s != 0) && (gs_rx_done == 0))
    {
        s--;
        llcc68_interface_delay_ms(1000);
    }
    if (gs_rx_done == 0)
    {
        /* receive timeout */
        llcc68_interface_debug_print("llcc68: receive timeout.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    else
    {
        /* finish receive test */
        llcc68_interface_debug_print("llcc68: finish receive test.\n");
        (void)llcc68_deinit(&gs_handle);
    }
    
    return 0;
}
