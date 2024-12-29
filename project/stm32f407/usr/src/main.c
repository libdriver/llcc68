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
 * @file      main.c
 * @brief     main source file
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
#include "driver_llcc68_send_receive_test.h"
#include "driver_llcc68_cad_test.h"
#include "driver_llcc68_lora.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */
uint8_t (*g_gpio_irq)(void) = NULL;        /**< gpio irq */
static volatile uint8_t gs_rx_done;        /**< rx done */

/**
 * @brief exti 0 irq
 * @note  none
 */
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

/**
 * @brief     gpio exti callback
 * @param[in] pin gpio pin
 * @note      none
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_0)
    {
        if (g_gpio_irq != NULL)
        {
            g_gpio_irq();
        }
    }
}

/**
 * @brief     interface receive callback
 * @param[in] type receive callback type
 * @param[in] *buf pointer to a buffer address
 * @param[in] len buffer length
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
            float rssi;
            float snr;
            
            llcc68_interface_debug_print("llcc68: irq rx done.\n");
            
            /* get the status */
            if (llcc68_lora_get_status((float *)&rssi, (float *)&snr) != 0)
            {
                return;
            }
            llcc68_interface_debug_print("llcc68: rssi is %0.1f.\n", rssi);
            llcc68_interface_debug_print("llcc68: snr is %0.2f.\n", snr);
            
            /* check the error */
            if (llcc68_lora_check_packet_error(&enable) != 0)
            {
                return;
            }
            if ((enable == LLCC68_BOOL_FALSE) && len)
            {
                for (i = 0; i < len; i++)
                {
                    llcc68_interface_debug_print("%c", buf[i]);
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
            llcc68_interface_debug_print("llcc68: unknown code.\n");
            
            break;
        }
    }
}

/**
 * @brief     llcc68 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t llcc68(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"data", required_argument, NULL, 1},
        {"timeout", required_argument, NULL, 2},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    char data[33] = "LibDriver";
    uint32_t timeout = 1000;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* data */
            case 1 :
            {
                /* set the data */
                memset(data, 0, sizeof(char) * 33);
                strncpy(data, optarg, 32);

                break;
            }
            
            /* timeout */
            case 2 :
            {
                /* set the timeout */
                timeout = atol(optarg);
                
                break;
            } 
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        uint8_t res;
        
        /* run reg test */
        res = llcc68_register_test();
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_lora-send", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = llcc68_interrupt_test_irq_handler;
        
        /* run send test */
        res = llcc68_send_test();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("t_lora-receive", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = llcc68_interrupt_test_irq_handler;

        /* run receive test */
        res = llcc68_receive_test(timeout);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("t_lora-cad", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = llcc68_cad_test_irq_handler;

        /* run cad test */
        res = llcc68_cad_test();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_lora-send", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = llcc68_lora_irq_handler;
        
        /* lora init */
        res = llcc68_lora_init(a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* set send mode */
        res = llcc68_lora_set_send_mode();
        if (res != 0)
        {
            (void)llcc68_lora_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* output */
        llcc68_interface_debug_print("llcc68: send %s.\n", data);
        
        /* send data */
        res = llcc68_lora_send((uint8_t *)data, (uint16_t)strlen(data));
        if (res != 0)
        {
            (void)llcc68_lora_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* lora deinit */
        res = llcc68_lora_deinit();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_lora-receive", type) == 0)
    {
        uint8_t res;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = llcc68_lora_irq_handler;
        
        /* lora init */
        res = llcc68_lora_init(a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* start receiving */
        llcc68_interface_debug_print("llcc68: start receiving...\n");
        gs_rx_done = 0;

        /* start receive */
        res = llcc68_lora_set_continuous_receive_mode();
        if (res != 0)
        {
            (void)llcc68_lora_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        while ((timeout != 0) && (gs_rx_done == 0))
        {
            timeout--;
            llcc68_interface_delay_ms(1000);
        }
        if (gs_rx_done == 0)
        {
            /* receive timeout */
            llcc68_interface_debug_print("llcc68: receive timeout.\n");
            (void)llcc68_lora_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* lora deinit */
        res = llcc68_lora_deinit();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_lora-cad", type) == 0)
    {
        uint8_t res;
        llcc68_bool_t enable;
        
        /* gpio init */
        res = gpio_interrupt_init();
        if (res != 0)
        {
            return 1;
        }

        /* set gpio irq */
        g_gpio_irq = llcc68_lora_irq_handler;
        
        /* lora init */
        res = llcc68_lora_init(a_callback);
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* run cad */
        res = llcc68_lora_run_cad(&enable);
        if (res != 0)
        {
            (void)llcc68_lora_deinit();
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }
        
        /* output the result */
        if (enable == LLCC68_BOOL_TRUE)
        {
            llcc68_interface_debug_print("llcc68: cad detected.\n");
        }
        else
        {
            llcc68_interface_debug_print("llcc68: cad not detected.\n");
        }
        
        /* lora deinit */
        res = llcc68_lora_deinit();
        if (res != 0)
        {
            (void)gpio_interrupt_deinit();
            g_gpio_irq = NULL;
            
            return 1;
        }

        /* gpio deinit */
        (void)gpio_interrupt_deinit();
        g_gpio_irq = NULL;
        
        return 0;
    }
    else if (strcmp("e_lora-sleep", type) == 0)
    {
        uint8_t res;
        
        /* lora init */
        res = llcc68_lora_init(a_callback);
        if (res != 0)
        {
            return 1;
        }
        
        /* sleep */
        if (llcc68_lora_sleep() != 0)
        {
            return 1;
        }
        
        /* output */
        llcc68_interface_debug_print("llcc68: sleep.\n");
        
        return 0;
    }
    else if (strcmp("e_lora-wake-up", type) == 0)
    {
        uint8_t res;
        
        /* lora init */
        res = llcc68_lora_init(a_callback);
        if (res != 0)
        {
            return 1;
        }
        
        /* wake up */
        if (llcc68_lora_wake_up() != 0)
        {
            return 1;
        }
        
        /* output */
        llcc68_interface_debug_print("llcc68: wake up.\n");
        
        /* deinit */
        res = llcc68_lora_deinit();
        if (res != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        llcc68_interface_debug_print("Usage:\n");
        llcc68_interface_debug_print("  llcc68 (-i | --information)\n");
        llcc68_interface_debug_print("  llcc68 (-h | --help)\n");
        llcc68_interface_debug_print("  llcc68 (-p | --port)\n");
        llcc68_interface_debug_print("  llcc68 (-t reg | --test=reg)\n");
        llcc68_interface_debug_print("  llcc68 (-t lora-send | --test=lora-send)\n");
        llcc68_interface_debug_print("  llcc68 (-t lora-receive | --test=lora-receive) [--timeout=<time>]\n");
        llcc68_interface_debug_print("  llcc68 (-t lora-cad | --test=lora-cad)\n");
        llcc68_interface_debug_print("  llcc68 (-e lora-send | --example=lora-send) [--data=<str>]\n");
        llcc68_interface_debug_print("  llcc68 (-e lora-receive | --example=lora-receive) [--timeout=<time>]\n");
        llcc68_interface_debug_print("  llcc68 (-e lora-cad | --example=lora-cad)\n");
        llcc68_interface_debug_print("  llcc68 (-e lora-sleep | --example=lora-sleep)\n");
        llcc68_interface_debug_print("  llcc68 (-e lora-wake-up | --example=lora-wake-up)\n");
        llcc68_interface_debug_print("\n");
        llcc68_interface_debug_print("Options:\n");
        llcc68_interface_debug_print("      --data=<str>            Set the send data.([default: LibDriver])\n");
        llcc68_interface_debug_print("  -e <lora-send | lora-receive | lora-cad | lora-sleep | lora-wake-up>, --example=<lora-send\n");
        llcc68_interface_debug_print("     | lora-receive | lora-cad | lora-sleep | lora-wake-up>\n");
        llcc68_interface_debug_print("                              Run the driver example.\n");
        llcc68_interface_debug_print("  -h, --help                  Show the help.\n");
        llcc68_interface_debug_print("  -i, --information           Show the chip information.\n");
        llcc68_interface_debug_print("  -p, --port                  Display the pin connections of the current board.\n");
        llcc68_interface_debug_print("  -t <reg | lora-send | lora-receive | lora-cad>, --test=<reg | lora-send | lora-receive | lora-cad>\n");
        llcc68_interface_debug_print("                              Run the driver test.\n");
        llcc68_interface_debug_print("      --timeout=<time>        Set the timeout in ms.([default: 1000])\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        llcc68_info_t info;
        
        /* print llcc68 info */
        llcc68_info(&info);
        llcc68_interface_debug_print("llcc68: chip is %s.\n", info.chip_name);
        llcc68_interface_debug_print("llcc68: manufacturer is %s.\n", info.manufacturer_name);
        llcc68_interface_debug_print("llcc68: interface is %s.\n", info.interface);
        llcc68_interface_debug_print("llcc68: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        llcc68_interface_debug_print("llcc68: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        llcc68_interface_debug_print("llcc68: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        llcc68_interface_debug_print("llcc68: max current is %0.2fmA.\n", info.max_current_ma);
        llcc68_interface_debug_print("llcc68: max temperature is %0.1fC.\n", info.temperature_max);
        llcc68_interface_debug_print("llcc68: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        llcc68_interface_debug_print("llcc68: SCK connected to GPIOA PIN5.\n");
        llcc68_interface_debug_print("llcc68: MISO connected to GPIOA PIN6.\n");
        llcc68_interface_debug_print("llcc68: MOSI connected to GPIOA PIN7.\n");
        llcc68_interface_debug_print("llcc68: CS connected to GPIOA PIN4.\n");
        llcc68_interface_debug_print("llcc68: RESET connected to GPIOB PIN1.\n");
        llcc68_interface_debug_print("llcc68: DIO1 connected to GPIOB PIN0.\n");
        llcc68_interface_debug_print("llcc68: BUSY connected to GPIOA PIN8.\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;
    
    /* stm32f407 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart init */
    uart_init(115200);
    
    /* shell init && register llcc68 function */
    shell_init();
    shell_register("llcc68", llcc68);
    uart_print("llcc68: welcome to libdriver llcc68.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("llcc68: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("llcc68: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("llcc68: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("llcc68: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("llcc68: param is invalid.\n");
            }
            else
            {
                uart_print("llcc68: unknown status code.\n");
            }
            uart_flush();
        }
        delay_ms(100);
    }
}
