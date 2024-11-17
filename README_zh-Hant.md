[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver LLCC68

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/llcc68/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

LLCC68 sub-GHz無線電收發器是遠程無線應用的理想選擇。 LLCC68可通過高效集成功率放大器傳輸高達+22 dBm的功率。 LLCC68支持LPWAN使用情况下的LoRa ® 調製，支持傳統使用情况下（G）FSK調製。 它可高度配寘，以滿足工業和消費者使用的不同應用要求。 該設備提供與LoRaWAN使用的Semtech收發器相容的LoRa ® 調製 ™  LoRa聯盟發佈的規範 ™. 該無線電適用於目標符合無線電法規的系統，包括但不限於ETSI EN 300 220、FCC CFR 47第15部分、中國法規要求和日本ARIB T-108。 從150 MHz到960 MHz的連續頻率覆蓋允許支持全球所有主要的Sub GHz ISM頻段。 LCC68可用於智慧電錶、供應鏈和物流、樓宇自動化等。

LibDriver LLCC68是LibDriver推出的LLCC68全功能驅動，提供無線發送、無線接收、cad等功能並且它符合MISRA標準。

### 目錄

  - [說明](#說明)
  - [安裝](#安裝)
  - [使用](#使用)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [文檔](#文檔)
  - [貢獻](#貢獻)
  - [版權](#版權)
  - [聯繫我們](#聯繫我們)

### 說明

/src目錄包含了LibDriver LLCC68的源文件。

/interface目錄包含了LibDriver LLCC68與平台無關的SPI總線模板。

/test目錄包含了LibDriver LLCC68驅動測試程序，該程序可以簡單的測試芯片必要功能。

/example目錄包含了LibDriver LLCC68編程範例。

/doc目錄包含了LibDriver LLCC68離線文檔。

/datasheet目錄包含了LLCC68數據手冊。

/project目錄包含了常用Linux與單片機開發板的工程樣例。所有工程均採用shell腳本作為調試方法，詳細內容可參考每個工程裡面的README.md。

/misra目錄包含了LibDriver MISRA程式碼掃描結果。

### 安裝

參考/interface目錄下與平台無關的SPI總線模板，完成指定平台的SPI總線驅動。

將/src目錄，您使用平臺的介面驅動和您開發的驅動加入工程，如果您想要使用默認的範例驅動，可以將/example目錄加入您的工程。

### 使用

您可以參考/example目錄下的程式設計範例完成適合您的驅動，如果您想要使用默認的程式設計範例，以下是它們的使用方法。

#### example basic send

```C
#include "driver_llcc68_lora.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
static uint8_t gs_rx_done;

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
            break;
        }
    }

}

/* gpio init */
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
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

llcc68_interface_debug_print("llcc68: send %s.\n", "123");

/* send data */
res = llcc68_lora_send((uint8_t *)"123", strlen("123"));
if (res != 0)
{
    (void)llcc68_lora_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;

}

/* deinit */
res = llcc68_lora_deinit();
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;

}
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

#### example basic receive

```C
#include "driver_llcc68_lora.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint32_t timeout;
static uint8_t gs_rx_done;

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
                return 1;
            }
            llcc68_interface_debug_print("llcc68: rssi is %0.1f.\n", rssi);
            llcc68_interface_debug_print("llcc68: snr is %0.2f.\n", snr);
            
            /* check the error */
            if (llcc68_lora_check_packet_error(&enable) != 0)
            {
                return 1;
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
            break;
        }
    }

}

/* gpio init */
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
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
timeout = 3000;

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

/* deinit */
res = llcc68_lora_deinit();
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;

}
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

return 0;
```

### 文檔

在線文檔: [https://www.libdriver.com/docs/llcc68/index.html](https://www.libdriver.com/docs/llcc68/index.html)。

離線文檔: /doc/html/index.html。

### 貢獻

請參攷CONTRIBUTING.md。

### 版權

版權 (c) 2015 - 現在 LibDriver 版權所有

MIT 許可證（MIT）

特此免費授予任何獲得本軟件副本和相關文檔文件（下稱“軟件”）的人不受限制地處置該軟件的權利，包括不受限制地使用、複製、修改、合併、發布、分發、轉授許可和/或出售該軟件副本，以及再授權被配發了本軟件的人如上的權利，須在下列條件下：

上述版權聲明和本許可聲明應包含在該軟件的所有副本或實質成分中。

本軟件是“如此”提供的，沒有任何形式的明示或暗示的保證，包括但不限於對適銷性、特定用途的適用性和不侵權的保證。在任何情況下，作者或版權持有人都不對任何索賠、損害或其他責任負責，無論這些追責來自合同、侵權或其它行為中，還是產生於、源於或有關於本軟件以及本軟件的使用或其它處置。

### 聯繫我們

請聯繫lishifenging@outlook.com。