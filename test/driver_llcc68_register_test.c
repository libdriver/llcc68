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
 * @file      driver_llcc68_register_test.c
 * @brief     driver llcc68 register test source file
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

#include "driver_llcc68_register_test.h"
#include <stdlib.h>

static llcc68_handle_t gs_handle;        /**< llcc68 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t llcc68_register_test(void)
{
    uint8_t res;
    uint8_t addr, addr_check;
    uint8_t i;
    uint16_t value, value_check;
    uint8_t sync_word[8];
    uint8_t sync_word_check[8];
    uint8_t setup, setup_check;
    uint32_t rand_number;
    uint8_t modulation, modulation_check;
    uint8_t gain, gain_check;
    uint8_t config, config_check;
    uint8_t ocp, ocp_check;
    uint8_t control, control_check;
    uint8_t trim, trim_check;
    uint8_t mask, mask_check;
    uint8_t enable, enable_check;
    double us, us_check;
    uint32_t freq, freq_check;
    uint32_t br, br_check;
    uint32_t reg;
    llcc68_packet_type_t type;
    llcc68_info_t info;
    
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
    
    /* get information */
    res = llcc68_info(&info);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip info */
        llcc68_interface_debug_print("llcc68: chip is %s.\n", info.chip_name);
        llcc68_interface_debug_print("llcc68: manufacturer is %s.\n", info.manufacturer_name);
        llcc68_interface_debug_print("llcc68: interface is %s.\n", info.interface);
        llcc68_interface_debug_print("llcc68: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        llcc68_interface_debug_print("llcc68: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        llcc68_interface_debug_print("llcc68: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        llcc68_interface_debug_print("llcc68: max current is %0.2fmA.\n", info.max_current_ma);
        llcc68_interface_debug_print("llcc68: max temperature is %0.1fC.\n", info.temperature_max);
        llcc68_interface_debug_print("llcc68: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    llcc68_interface_debug_print("llcc68: start register test.\n");
    
    /* init the llcc68 */
    res = llcc68_init(&gs_handle);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: init failed.\n");
       
        return 1;
    }
    
    /* set standby mode */
    res = llcc68_set_standby(&gs_handle, LLCC68_CLOCK_SOURCE_XTAL_32MHZ);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set standby failed.\n");
        (void)llcc68_deinit(&gs_handle);
       
        return 1;
    }
    
    /* llcc68_write_register/llcc68_read_register test */
    llcc68_interface_debug_print("llcc68: llcc68_write_register/llcc68_read_register test.\n");
    
    control = rand() % 256;
    res = llcc68_write_register(&gs_handle, 0x06BC, (uint8_t *)&control, 1);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: write register failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set register 0x%02X.\n", control);
    res = llcc68_read_register(&gs_handle, 0x06BC, (uint8_t *)&control_check, 1);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: read register failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check register %s.\n", control_check == control ? "ok" : "error");
    
    /* llcc68_write_buffer/llcc68_read_buffer test */
    llcc68_interface_debug_print("llcc68: llcc68_write_buffer/llcc68_read_buffer test.\n");
    
    control = rand() % 256;
    res = llcc68_write_buffer(&gs_handle , 0, (uint8_t *)&control, 1);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: write buffer failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set buffer 0x%02X.\n", control);
    res = llcc68_read_buffer(&gs_handle, 0, (uint8_t *)&control_check, 1);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: read buffer failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check buffer %s.\n", control_check == control ? "ok" : "error");
    
    /* llcc68_timeout_convert_to_register/llcc68_timeout_convert_to_data test */
    llcc68_interface_debug_print("llcc68: llcc68_timeout_convert_to_register/llcc68_timeout_convert_to_data test.\n");
    
    us = (rand() % 10000) / 10.0;
    res = llcc68_timeout_convert_to_register(&gs_handle, us, (uint32_t *)&reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: timeout convert to register failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set timeout %f us.\n", us);
    res = llcc68_timeout_convert_to_data(&gs_handle, reg, (double *)&us_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: timeout convert to data failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check timeout %f us.\n", us_check);
    
    /* llcc68_frequency_convert_to_register/llcc68_frequency_convert_to_data test */
    llcc68_interface_debug_print("llcc68: llcc68_frequency_convert_to_register/llcc68_frequency_convert_to_data test.\n");
    
    freq = 47000000;
    res = llcc68_frequency_convert_to_register(&gs_handle, freq, (uint32_t *)&reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: frequency convert to register failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set frequency %d Hz.\n", freq);
    res = llcc68_frequency_convert_to_data(&gs_handle, reg, (uint32_t *)&freq_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: frequency convert to data failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check frequency %d Hz.\n", freq_check);
    
    /* llcc68_gfsk_bit_rate_convert_to_register/llcc68_gfsk_bit_rate_convert_to_data test */
    llcc68_interface_debug_print("llcc68: llcc68_gfsk_bit_rate_convert_to_register/llcc68_gfsk_bit_rate_convert_to_data test.\n");
    
    br = 470000;
    res = llcc68_gfsk_bit_rate_convert_to_register(&gs_handle, br, (uint32_t *)&reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: gfsk bit rate convert to register failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set bit rate %d Hz.\n", br);
    res = llcc68_gfsk_bit_rate_convert_to_data(&gs_handle, reg, (uint32_t *)&br_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: gfsk bit rate convert to data failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check bit rate %d Hz.\n", br_check);
    
    /* llcc68_gfsk_frequency_deviation_convert_to_register/llcc68_gfsk_frequency_deviation_convert_to_data test */
    llcc68_interface_debug_print("llcc68: llcc68_gfsk_frequency_deviation_convert_to_register/llcc68_gfsk_frequency_deviation_convert_to_data test.\n");
    
    freq = 10000;
    res = llcc68_gfsk_frequency_deviation_convert_to_register(&gs_handle, freq, (uint32_t *)&reg);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: gfsk frequency deviation convert to register failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: gfsk frequency deviation %d Hz.\n", freq);
    res = llcc68_gfsk_frequency_deviation_convert_to_data(&gs_handle, reg, (uint32_t *)&freq_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: gfsk frequency deviation convert to data failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check gfsk frequency deviation %d Hz.\n", freq_check);
    
    /* llcc68_set_packet_type/llcc68_get_packet_type test */
    llcc68_interface_debug_print("llcc68: llcc68_set_packet_type/llcc68_get_packet_type test.\n");
    
    /* set gfsk type */
    res = llcc68_set_packet_type(&gs_handle, LLCC68_PACKET_TYPE_GFSK);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set packet type failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set gfsk type.\n");
    res = llcc68_get_packet_type(&gs_handle, &type);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get packet type failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check packet type %s.\n", type == LLCC68_PACKET_TYPE_GFSK ? "ok" : "error");
    
    /* set lora type */
    res = llcc68_set_packet_type(&gs_handle, LLCC68_PACKET_TYPE_LORA);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set packet type failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set lora type.\n");
    res = llcc68_get_packet_type(&gs_handle, &type);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get packet type failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check packet type %s.\n", type == LLCC68_PACKET_TYPE_LORA ? "ok" : "error");
    
    /* llcc68_set_fsk_whitening_initial_value/llcc68_get_fsk_whitening_initial_value test */
    llcc68_interface_debug_print("llcc68: llcc68_set_fsk_whitening_initial_value/llcc68_get_fsk_whitening_initial_value test.\n");
    
    value = 0x0100;
    res = llcc68_set_fsk_whitening_initial_value(&gs_handle, value);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set fsk whitening initial value failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set fsk whitening initial value 0x%04X.\n", value);
    res = llcc68_get_fsk_whitening_initial_value(&gs_handle, (uint16_t *)&value_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get fsk whitening initial value failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check fsk whitening initial value %s.\n", value_check == value ? "ok" : "error");
    
    /* llcc68_set_fsk_crc_initical_value/llcc68_get_fsk_crc_initical_value test */
    llcc68_interface_debug_print("llcc68: llcc68_set_fsk_crc_initical_value/llcc68_get_fsk_crc_initical_value test.\n");
    
    value = rand() % 65536;
    res = llcc68_set_fsk_crc_initical_value(&gs_handle, value);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set fsk crc initical value failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set fsk crc initical value 0x%04X.\n", value);
    res = llcc68_get_fsk_crc_initical_value(&gs_handle, (uint16_t *)&value_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get fsk crc initical value failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check fsk crc initical value %s.\n", value_check == value ? "ok" : "error");
    
    /* llcc68_set_fsk_crc_initical_value/llcc68_get_fsk_crc_initical_value test */
    llcc68_interface_debug_print("llcc68: llcc68_set_fsk_crc_initical_value/llcc68_get_fsk_crc_initical_value test.\n");
    
    value = rand() % 65536;
    res = llcc68_set_fsk_crc_polynomial_value(&gs_handle, value);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set fsk crc polynomial value failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set fsk crc polynomial value 0x%04X.\n", value);
    res = llcc68_get_fsk_crc_polynomial_value(&gs_handle, (uint16_t *)&value_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get fsk crc polynomial value failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check fsk crc polynomial value %s.\n", value_check == value ? "ok" : "error");
    
    /* llcc68_set_fsk_sync_word/llcc68_get_fsk_sync_word test */
    llcc68_interface_debug_print("llcc68: llcc68_set_fsk_sync_word/llcc68_get_fsk_sync_word test.\n");
    
    for (i = 0; i < 8; i++)
    {
        sync_word[i] = rand() %256;
    }
    res = llcc68_set_fsk_sync_word(&gs_handle, (uint8_t *)sync_word);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set fsk sync word failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set fsk sync word 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                 sync_word[0], sync_word[1], sync_word[2], sync_word[3],
                                 sync_word[4], sync_word[5], sync_word[6], sync_word[7]);
    res = llcc68_get_fsk_sync_word(&gs_handle, (uint8_t *)sync_word_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get fsk sync word failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check fsk sync word %s.\n", memcmp((uint8_t *)sync_word, (uint8_t *)sync_word_check, 8) == 0 ? "ok" : "error");
    
    /* llcc68_set_fsk_node_address/llcc68_get_fsk_node_address test */
    llcc68_interface_debug_print("llcc68: llcc68_set_fsk_node_address/llcc68_get_fsk_node_address test.\n");
    
    addr = rand() %256;
    res = llcc68_set_fsk_node_address(&gs_handle, addr);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set fsk node address failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set fsk node address 0x%02X.\n", addr);
    res = llcc68_get_fsk_node_address(&gs_handle, (uint8_t *)&addr_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get fsk node address failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check fsk node address %s.\n", addr == addr_check ? "ok" : "error");
    
    /* llcc68_set_fsk_broadcast_address/llcc68_get_fsk_broadcast_address test */
    llcc68_interface_debug_print("llcc68: llcc68_set_fsk_broadcast_address/llcc68_get_fsk_broadcast_address test.\n");
    
    addr = rand() %256;
    res = llcc68_set_fsk_broadcast_address(&gs_handle, addr);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set fsk broadcast address failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set fsk broadcast address 0x%02X.\n", addr);
    res = llcc68_get_fsk_broadcast_address(&gs_handle, (uint8_t *)&addr_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get fsk broadcast address failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check fsk broadcast address %s.\n", addr == addr_check ? "ok" : "error");
    
    /* llcc68_set_iq_polarity/llcc68_get_iq_polarity test */
    llcc68_interface_debug_print("llcc68: llcc68_set_iq_polarity/llcc68_get_iq_polarity test.\n");
    
    setup = rand() %256;
    res = llcc68_set_iq_polarity(&gs_handle, setup);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set iq polarity failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set iq polarity 0x%02X.\n", setup);
    res = llcc68_get_iq_polarity(&gs_handle, (uint8_t *)&setup_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get iq polarity failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check iq polarity %s.\n", setup == setup_check ? "ok" : "error");
    
    /* llcc68_set_lora_sync_word/llcc68_get_lora_sync_word test */
    llcc68_interface_debug_print("llcc68: llcc68_set_lora_sync_word/llcc68_get_lora_sync_word test.\n");
    
    value = rand() % 65536;
    res = llcc68_set_lora_sync_word(&gs_handle, value);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set lora sync word failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set lora sync word 0x%04X.\n", value);
    res = llcc68_get_lora_sync_word(&gs_handle, (uint16_t *)&value_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get lora sync word failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check lora sync word %s.\n", value == value_check ? "ok" : "error");
    
    /* llcc68_get_random_number test */
    llcc68_interface_debug_print("llcc68: llcc68_get_random_number test.\n");
    
    res = llcc68_get_random_number(&gs_handle, (uint32_t *)&rand_number);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get random number failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: random number is 0x%08X.\n", rand_number);
    
    /* llcc68_set_tx_modulation/llcc68_get_tx_modulation test */
    llcc68_interface_debug_print("llcc68: llcc68_set_tx_modulation/llcc68_get_tx_modulation test.\n");
    
    res = llcc68_get_tx_modulation(&gs_handle, (uint8_t *)&modulation_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    modulation = 0;
    modulation |= 0x04;
    res = llcc68_set_tx_modulation(&gs_handle, modulation);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set tx modulation 0x%02X.\n", modulation);
    res = llcc68_get_tx_modulation(&gs_handle, (uint8_t *)&modulation_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx modulation failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check tx modulation %s.\n", modulation == modulation_check ? "ok" : "error");
    
    /* llcc68_set_rx_gain/llcc68_get_rx_gain test */
    llcc68_interface_debug_print("llcc68: llcc68_set_rx_gain/llcc68_get_rx_gain test.\n");
    
    gain = rand() % 256;
    res = llcc68_set_rx_gain(&gs_handle, gain);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rx gain failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set rx gain 0x%02X.\n", gain);
    res = llcc68_get_rx_gain(&gs_handle, (uint8_t *)&gain_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get rx gain failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check rx gain %s.\n", gain == gain_check ? "ok" : "error");
    
    /* llcc68_set_tx_clamp_config/llcc68_get_tx_clamp_config test */
    llcc68_interface_debug_print("llcc68: llcc68_set_tx_clamp_config/llcc68_get_tx_clamp_config test.\n");
    
    config = rand() % 256;
    res = llcc68_set_tx_clamp_config(&gs_handle, config);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set tx clamp config 0x%02X.\n", config);
    res = llcc68_get_tx_clamp_config(&gs_handle, (uint8_t *)&config_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get tx clamp config failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check tx clamp config %s.\n", config == config_check ? "ok" : "error");
    
    /* llcc68_set_ocp/llcc68_get_ocp test */
    llcc68_interface_debug_print("llcc68: llcc68_set_ocp/llcc68_get_ocp test.\n");
    
    ocp = rand() % 0x20;
    res = llcc68_set_ocp(&gs_handle, ocp);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set ocp failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set ocp 0x%02X.\n", ocp);
    res = llcc68_get_ocp(&gs_handle, (uint8_t *)&ocp_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get ocp failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check ocp config %s.\n", ocp == ocp_check ? "ok" : "error");
    
    /* llcc68_set_rtc_control/llcc68_get_rtc_control test */
    llcc68_interface_debug_print("llcc68: llcc68_set_rtc_control/llcc68_get_rtc_control test.\n");
    
    /* disable */
    control = 0;
    res = llcc68_set_rtc_control(&gs_handle, control);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set rtc control failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: disable rtc control.\n");
    res = llcc68_get_rtc_control(&gs_handle, (uint8_t *)&control_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get rtc control failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check rtc control %s.\n", control_check == control ? "ok" : "error");
    
    /* llcc68_set_xta_trim/llcc68_get_xta_trim test */
    llcc68_interface_debug_print("llcc68: llcc68_set_xta_trim/llcc68_get_xta_trim test.\n");
    
    trim = 0x05;
    res = llcc68_set_xta_trim(&gs_handle, trim);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set xta trim failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set xta trim 0x%02X.\n", trim);
    res = llcc68_get_xta_trim(&gs_handle, (uint8_t *)&trim_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get xta trim failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check xta trim %s.\n", trim_check == trim ? "ok" : "error");
    
    /* llcc68_set_xtb_trim/llcc68_get_xtb_trim test */
    llcc68_interface_debug_print("llcc68: llcc68_set_xtb_trim/llcc68_get_xtb_trim test.\n");
    
    trim = 0x05;
    res = llcc68_set_xtb_trim(&gs_handle, trim);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set xtb trim failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set xtb trim 0x%02X.\n", trim);
    res = llcc68_get_xtb_trim(&gs_handle, (uint8_t *)&trim_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get xtb trim failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check xtb trim %s.\n", trim_check == trim ? "ok" : "error");
    
    /* llcc68_set_dio3_output_control/llcc68_get_dio3_output_control test */
    llcc68_interface_debug_print("llcc68: llcc68_set_dio3_output_control/llcc68_get_dio3_output_control test.\n");
    
    control = 0x01;
    res = llcc68_set_dio3_output_control(&gs_handle, control);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set dio3 output control failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set dio3 output control 0x%02X.\n", control);
    res = llcc68_get_dio3_output_control(&gs_handle, (uint8_t *)&control_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get dio3 output control failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check dio3 output control %s.\n", control_check == control ? "ok" : "error");
    
    /* llcc68_set_event_mask/llcc68_get_event_mask test */
    llcc68_interface_debug_print("llcc68: llcc68_set_event_mask/llcc68_get_event_mask test.\n");
    
    mask = 0;
    res = llcc68_set_event_mask(&gs_handle, mask);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set event mask failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set event mask 0x%02X.\n", mask);
    res = llcc68_get_event_mask(&gs_handle, (uint8_t *)&mask_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get event mask failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check event mask %s.\n", mask_check == mask ? "ok" : "error");
    
    /* llcc68_set_dio_output_enable/llcc68_get_dio_output_enable test */
    llcc68_interface_debug_print("llcc68: llcc68_set_dio_output_enable/llcc68_get_dio_output_enable test.\n");
    
    enable = rand() % 0xF;
    res = llcc68_set_dio_output_enable(&gs_handle, enable);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set dio output enable failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set dio output enable 0x%02X.\n", enable);
    res = llcc68_get_dio_output_enable(&gs_handle, (uint8_t *)&enable_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get dio output enable failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check dio output enable %s.\n", enable == enable_check ? "ok" : "error");
    
    /* llcc68_set_dio_input_enable/llcc68_get_dio_input_enable test */
    llcc68_interface_debug_print("llcc68: llcc68_set_dio_input_enable/llcc68_get_dio_input_enable test.\n");
    
    res = llcc68_set_dio_output_enable(&gs_handle, 0x00);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set dio output enable failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    enable = rand() % 0xF;
    res = llcc68_set_dio_input_enable(&gs_handle, enable);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set dio input enable failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set dio input enable 0x%02X.\n", enable);
    res = llcc68_get_dio_input_enable(&gs_handle, (uint8_t *)&enable_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get dio input enable failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check dio input enable %s.\n", enable == enable_check ? "ok" : "error");
    
    /* llcc68_set_pull_up_control/llcc68_get_pull_up_control test */
    llcc68_interface_debug_print("llcc68: llcc68_set_pull_up_control/llcc68_get_pull_up_control test.\n");
    
    control = rand() % 0x0F;
    res = llcc68_set_pull_up_control(&gs_handle, control);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set pull up control failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set pull up control 0x%02X.\n", control);
    res = llcc68_get_pull_up_control(&gs_handle, (uint8_t *)&control_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get pull up control failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check pull up control %s.\n", control == control_check ? "ok" : "error");
    
    /* llcc68_set_pull_down_control/llcc68_get_pull_down_control test */
    llcc68_interface_debug_print("llcc68: llcc68_set_pull_down_control/llcc68_get_pull_down_control test.\n");
    
    res = llcc68_set_pull_up_control(&gs_handle, 0x00);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set pull up control failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    control = rand() % 0x0F;
    res = llcc68_set_pull_down_control(&gs_handle, control);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: set pull down control failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: set pull down control 0x%02X.\n", control);
    res = llcc68_get_pull_down_control(&gs_handle, (uint8_t *)&control_check);
    if (res != 0)
    {
        llcc68_interface_debug_print("llcc68: get pull down control failed.\n");
        (void)llcc68_deinit(&gs_handle);
        
        return 1;
    }
    llcc68_interface_debug_print("llcc68: check pull down control %s.\n", control == control_check ? "ok" : "error");
    
    /* finish register test */
    llcc68_interface_debug_print("llcc68: finish register test.\n");
    (void)llcc68_deinit(&gs_handle);
    
    return 0;
}
