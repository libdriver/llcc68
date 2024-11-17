[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver LLCC68

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/llcc68/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE) 

Der Sub-GHz-Funktransceiver LLCC68 ist ideal für drahtlose Anwendungen mit großer Reichweite. Es ist für eine lange Batterielebensdauer mit nur 4,2 mA aktivem Empfangsstromverbrauch ausgelegt. Der LLCC68 kann mit hocheffizienten integrierten Leistungsverstärkern bis zu +22 dBm übertragen. Der LLCC68 unterstützt LoRa®-Modulation für LPWAN-Anwendungsfälle und (G)FSK-Modulation für Legacy-Anwendungsfälle. Es ist hochgradig konfigurierbar, um unterschiedliche Anwendungsanforderungen für Industrie und Verbraucher zu erfüllen. Das Gerät bietet LoRa®-Modulation, die mit Semtech-Transceivern kompatibel ist, die von der LoRaWAN™-Spezifikation verwendet werden, die von der LoRa Alliance™ veröffentlicht wurde. Das Funkgerät eignet sich für Systeme, die auf die Einhaltung von Funkvorschriften abzielen, einschließlich aber nicht beschränkt auf ETSI EN 300 220, FCC CFR 47 Teil 15, chinesische regulatorische Anforderungen und das japanische ARIB T-108. Die kontinuierliche Frequenzabdeckung von 150 MHz bis 960 MHz ermöglicht die Unterstützung aller wichtigen Sub-GHz-ISM-Bänder auf der ganzen Welt. Der LLCC68 kann in intelligenten Zählern, Lieferketten und Logistik, Gebäudeautomation usw. verwendet werden.

LibDriver LLCC68 ist der Treiber mit vollem Funktionsumfang von LLCC68, der von LibDriver gestartet wurde. Er bietet Funktionen für drahtloses Senden, drahtloses Empfangen, CAD usw. LibDriver ist MISRA-konform.

### Inhaltsverzeichnis

  - [Anweisung](#Anweisung)
  - [Installieren](#Installieren)
  - [Nutzung](#Nutzung)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [Dokument](#Dokument)
  - [Beitrag](#Beitrag)
  - [Lizenz](#Lizenz)
  - [Kontaktieren Sie uns](#Kontaktieren-Sie-uns)

### Anweisung

/src enthält LibDriver LLCC68-Quelldateien.

/interface enthält die plattformunabhängige Vorlage LibDriver LLCC68 SPI.

/test enthält den Testcode des LibDriver LLCC68-Treibers und dieser Code kann die erforderliche Funktion des Chips einfach testen.

/example enthält LibDriver LLCC68-Beispielcode.

/doc enthält das LibDriver LLCC68-Offlinedokument.

/Datenblatt enthält LLCC68-Datenblatt.

/project enthält den allgemeinen Beispielcode für Linux- und MCU-Entwicklungsboards. Alle Projekte verwenden das Shell-Skript, um den Treiber zu debuggen, und die detaillierten Anweisungen finden Sie in der README.md jedes Projekts.

/misra enthält die Ergebnisse des LibDriver MISRA Code Scans.

### Installieren

Verweisen Sie auf eine plattformunabhängige SPI-Schnittstellenvorlage und stellen Sie Ihren Plattform-SPI-Treiber fertig.

Fügen Sie das Verzeichnis /src, den Schnittstellentreiber für Ihre Plattform und Ihre eigenen Treiber zu Ihrem Projekt hinzu. Wenn Sie die Standardbeispieltreiber verwenden möchten, fügen Sie das Verzeichnis /example zu Ihrem Projekt hinzu.

### Nutzung

Sie können auf die Beispiele im Verzeichnis /example zurückgreifen, um Ihren eigenen Treiber zu vervollständigen. Wenn Sie die Standardprogrammierbeispiele verwenden möchten, erfahren Sie hier, wie Sie diese verwenden.

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

### Dokument

Online-Dokumente: [https://www.libdriver.com/docs/llcc68/index.html](https://www.libdriver.com/docs/llcc68/index.html).

Offline-Dokumente: /doc/html/index.html.

### Beitrag

Bitte beachten Sie CONTRIBUTING.md.

### Lizenz

Urheberrechte © (c) 2015 - Gegenwart LibDriver Alle Rechte vorbehalten



Die MIT-Lizenz (MIT)



Hiermit wird jeder Person kostenlos die Erlaubnis erteilt, eine Kopie zu erhalten

dieser Software und zugehörigen Dokumentationsdateien (die „Software“) zu behandeln

in der Software ohne Einschränkung, einschließlich, aber nicht beschränkt auf die Rechte

zu verwenden, zu kopieren, zu modifizieren, zusammenzuführen, zu veröffentlichen, zu verteilen, unterzulizenzieren und/oder zu verkaufen

Kopien der Software und Personen, denen die Software gehört, zu gestatten

dazu eingerichtet werden, unter folgenden Bedingungen:



Der obige Urheberrechtshinweis und dieser Genehmigungshinweis müssen in allen enthalten sein

Kopien oder wesentliche Teile der Software.



DIE SOFTWARE WIRD "WIE BESEHEN" BEREITGESTELLT, OHNE JEGLICHE GEWÄHRLEISTUNG, AUSDRÜCKLICH ODER

STILLSCHWEIGEND, EINSCHLIESSLICH, ABER NICHT BESCHRÄNKT AUF DIE GEWÄHRLEISTUNG DER MARKTGÄNGIGKEIT,

EIGNUNG FÜR EINEN BESTIMMTEN ZWECK UND NICHTVERLETZUNG VON RECHTEN DRITTER. IN KEINEM FALL DARF DAS

AUTOREN ODER URHEBERRECHTSINHABER HAFTEN FÜR JEGLICHE ANSPRÜCHE, SCHÄDEN ODER ANDERE

HAFTUNG, OB AUS VERTRAG, DELIKT ODER ANDERWEITIG, ENTSTEHEND AUS,

AUS ODER IM ZUSAMMENHANG MIT DER SOFTWARE ODER DER VERWENDUNG ODER ANDEREN HANDLUNGEN MIT DER

SOFTWARE.

### Kontaktieren Sie uns

Bitte senden Sie eine E-Mail an lishifenging@outlook.com.