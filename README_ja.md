[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver LLCC68

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/llcc68/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

LLCC68 サブ GHz 無線トランシーバは、長距離ワイヤレス アプリケーションに最適です。 わずか 4.2 mA のアクティブ受信電流消費で長いバッテリ寿命を実現するように設計されています。LLCC68 は、高効率の統合パワー アンプで最大 +22 dBm の送信が可能です。LLCC68 は、LPWAN ユース ケース向けの LoRa® 変調と、 従来のユースケース。 このデバイスは、LoRa Alliance™ によってリリースされた LoRaWAN™ 仕様で使用される Semtech トランシーバーと互換性のある LoRa® 変調を提供します。この無線は、以下を含む無線規制への準拠を目標とするシステムに適しています。 ただし、ETSI EN 300 220、FCC CFR 47 Part 15、中国の規制要件、および日本の ARIB T-108 に限定されません。 150 MHz から 960 MHz までの継続的な周波数カバレッジにより、世界中のすべての主要なサブ GHz ISM 帯域をサポートできます。LLCC68 は、スマート メーター、サプライ チェーンとロジスティクス、ビルディング オートメーションなどで使用できます。

LibDriver LLCC68 は、LibDriver が起動する LLCC68 のフル機能ドライバーです。無線送信、無線受信、CAD などの機能を提供します。LibDriver は MISRA に準拠しています。

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver LLCC68のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver LLCC68用のプラットフォームに依存しないSPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver LLCC68ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver LLCC68プログラミング例が含まれています。

/ docディレクトリには、LibDriver LLCC68オフラインドキュメントが含まれています。

/ datasheetディレクトリには、LLCC68データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

/ misraはLibDriver misraコードスキャン結果を含む。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSPIバステンプレートを参照して、指定したプラットフォームのSPIバスドライバを完成させます。

/src ディレクトリ、プラットフォームのインターフェイス ドライバー、および独自のドライバーをプロジェクトに追加します。デフォルトのサンプル ドライバーを使用する場合は、/example ディレクトリをプロジェクトに追加します。

### 使用

/example ディレクトリ内のサンプルを参照して、独自のドライバーを完成させることができます。 デフォルトのプログラミング例を使用したい場合の使用方法は次のとおりです。

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

```c
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

### ドキュメント

オンラインドキュメント: [https://www.libdriver.com/docs/llcc68/index.html](https://www.libdriver.com/docs/llcc68/index.html)。

オフラインドキュメント: /doc/html/index.html。

### 貢献

CONTRIBUTING.mdを参照してください。

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com。