### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

SPI Pin: SCK/MISO/MOSI/CS  PA5/PA6/PA7/PA4.

RESET Pin: RST PB1.

DIO1 Pin: DIO1 PB0.

BUSY Pin: BUSY PA8.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. LLCC68

#### 3.1 Command Instruction

1. Show llcc68 chip and driver information.

   ```shell
   llcc68 (-i | --information)
   ```

2. Show llcc68 help.

   ```shell
   llcc68 (-h | --help)
   ```

3. Show llcc68 pin connections of the current board.

   ```shell
   llcc68 (-p | --port)
   ```

4. Run llcc68 register test.

   ```shell
   llcc68 (-t reg | --test=reg)
   ```

5. Run llcc68 send test.

   ```shell
   llcc68 (-t lora-send | --test=lora-send)
   ```

6. Run llcc68 receive test, time is the received timeout in second. 

   ```shell
   llcc68 (-t lora-receive | --test=lora-receive) [--timeout=<time>]
   ```

7. Run llcc68 cad test.

   ```shell
   llcc68 (-t lora-cad | --test=lora-cad)
   ```

8. Run llcc68  send function, str is the send data.

   ```shell
   llcc68 (-e lora-send | --example=lora-send) [--data=<str>]
   ```

9. Run llcc68 receive function, time is the received timeout in second. 

   ```shell
   llcc68 (-e lora-receive | --example=lora-receive) [--timeout=<time>]
   ```

10. Run llcc68 cad function.

    ```shell
    llcc68 (-e lora-cad | --example=lora-cad)
    ```

11. Run llcc68 sleep function.

    ```shell
    llcc68 (-e lora-sleep | --example=lora-sleep)
    ```

12. Run llcc68 wake up function.

    ```shell
    llcc68 (-e lora-wake-up | --example=lora-wake-up)
    ```

#### 3.2 Command Example

```shell
llcc68 -i

llcc68: chip is Semtech LLCC68.
llcc68: manufacturer is Semtech.
llcc68: interface is SPI.
llcc68: driver version is 1.0.
llcc68: min supply voltage is 1.8V.
llcc68: max supply voltage is 3.7V.
llcc68: max current is 118.00mA.
llcc68: max temperature is 85.0C.
llcc68: min temperature is -40.0C.
```

```shell
llcc68 -p

llcc68: SCK connected to GPIOA PIN5.
llcc68: MISO connected to GPIOA PIN6.
llcc68: MOSI connected to GPIOA PIN7.
llcc68: CS connected to GPIOA PIN4.
llcc68: RESET connected to GPIOB PIN1.
llcc68: DIO1 connected to GPIOB PIN0.
llcc68: BUSY connected to GPIOA PIN8.
```

```shell
llcc68 -t reg

llcc68: chip is Semtech LLCC68.
llcc68: manufacturer is Semtech.
llcc68: interface is SPI.
llcc68: driver version is 1.0.
llcc68: min supply voltage is 1.8V.
llcc68: max supply voltage is 3.7V.
llcc68: max current is 118.00mA.
llcc68: max temperature is 85.0C.
llcc68: min temperature is -40.0C.
llcc68: start register test.
llcc68: llcc68_write_register/llcc68_read_register test.
llcc68: set register 0xB0.
llcc68: check register ok.
llcc68: llcc68_write_buffer/llcc68_read_buffer test.
llcc68: set buffer 0xCB.
llcc68: check buffer ok.
llcc68: llcc68_timeout_convert_to_register/llcc68_timeout_convert_to_data test.
llcc68: set timeout 640.900000 us.
llcc68: check timeout 640.625000 us.
llcc68: llcc68_frequency_convert_to_register/llcc68_frequency_convert_to_data test.
llcc68: set frequency 47000000 Hz.
llcc68: check frequency 47000000 Hz.
llcc68: llcc68_gfsk_bit_rate_convert_to_register/llcc68_gfsk_bit_rate_convert_to_data test.
llcc68: set bit rate 470000 Hz.
llcc68: check bit rate 470156 Hz.
llcc68: llcc68_gfsk_frequency_deviation_convert_to_register/llcc68_gfsk_frequency_deviation_convert_to_data test.
llcc68: gfsk frequency deviation 10000 Hz.
llcc68: check gfsk frequency deviation 9999 Hz.
llcc68: llcc68_set_packet_type/llcc68_get_packet_type test.
llcc68: set gfsk type.
llcc68: check packet type ok.
llcc68: set lora type.
llcc68: check packet type ok.
llcc68: llcc68_set_fsk_whitening_initial_value/llcc68_get_fsk_whitening_initial_value test.
llcc68: set fsk whitening initial value 0x0100.
llcc68: check fsk whitening initial value ok.
llcc68: llcc68_set_fsk_crc_initical_value/llcc68_get_fsk_crc_initical_value test.
llcc68: set fsk crc initical value 0xD868.
llcc68: check fsk crc initical value ok.
llcc68: llcc68_set_fsk_crc_initical_value/llcc68_get_fsk_crc_initical_value test.
llcc68: set fsk crc polynomial value 0xAD07.
llcc68: check fsk crc polynomial value ok.
llcc68: llcc68_set_fsk_sync_word/llcc68_get_fsk_sync_word test.
llcc68: set fsk sync word 0x45 0xC8 0x7F 0x13 0xC4 0x6C 0x2B 0xBA.
llcc68: check fsk sync word ok.
llcc68: llcc68_set_fsk_node_address/llcc68_get_fsk_node_address test.
llcc68: set fsk node address 0x61.
llcc68: check fsk node address ok.
llcc68: llcc68_set_fsk_broadcast_address/llcc68_get_fsk_broadcast_address test.
llcc68: set fsk broadcast address 0x65.
llcc68: check fsk broadcast address ok.
llcc68: llcc68_set_iq_polarity/llcc68_get_iq_polarity test.
llcc68: set iq polarity 0xA1.
llcc68: check iq polarity ok.
llcc68: llcc68_set_lora_sync_word/llcc68_get_lora_sync_word test.
llcc68: set lora sync word 0xAC72.
llcc68: check lora sync word ok.
llcc68: llcc68_get_random_number test.
llcc68: random number is 0x6EBB76DD.
llcc68: llcc68_set_tx_modulation/llcc68_get_tx_modulation test.
llcc68: set tx modulation 0x04.
llcc68: check tx modulation ok.
llcc68: llcc68_set_rx_gain/llcc68_get_rx_gain test.
llcc68: set rx gain 0x64.
llcc68: check rx gain ok.
llcc68: llcc68_set_tx_clamp_config/llcc68_get_tx_clamp_config test.
llcc68: set tx clamp config 0x39.
llcc68: check tx clamp config ok.
llcc68: llcc68_set_ocp/llcc68_get_ocp test.
llcc68: set ocp 0x1D.
llcc68: check ocp config ok.
llcc68: llcc68_set_rtc_control/llcc68_get_rtc_control test.
llcc68: disable rtc control.
llcc68: check rtc control ok.
llcc68: llcc68_set_xta_trim/llcc68_get_xta_trim test.
llcc68: set xta trim 0x05.
llcc68: check xta trim ok.
llcc68: llcc68_set_xtb_trim/llcc68_get_xtb_trim test.
llcc68: set xtb trim 0x05.
llcc68: check xtb trim ok.
llcc68: llcc68_set_dio3_output_control/llcc68_get_dio3_output_control test.
llcc68: set dio3 output control 0x01.
llcc68: check dio3 output control ok.
llcc68: llcc68_set_event_mask/llcc68_get_event_mask test.
llcc68: set event mask 0x00.
llcc68: check event mask ok.
llcc68: llcc68_set_dio_output_enable/llcc68_get_dio_output_enable test.
llcc68: set dio output enable 0x00.
llcc68: check dio output enable ok.
llcc68: llcc68_set_dio_input_enable/llcc68_get_dio_input_enable test.
llcc68: set dio input enable 0x07.
llcc68: check dio input enable ok.
llcc68: llcc68_set_pull_up_control/llcc68_get_pull_up_control test.
llcc68: set pull up control 0x0A.
llcc68: check pull up control ok.
llcc68: llcc68_set_pull_down_control/llcc68_get_pull_down_control test.
llcc68: set pull down control 0x05.
llcc68: check pull down control ok.
llcc68: finish register test.
```

```shell
llcc68 -t lora-send

llcc68: start send test.
llcc68: irq tx done.
llcc68: finish send test.
```

```shell
llcc68 -t lora-receive --timeout=1000

llcc68: start receive test.
llcc68: start receiving...
llcc68: irq preamble detected.
llcc68: irq valid header.
llcc68: irq rx done.
llcc68: rssi is -69.0.
llcc68: snr is 11.00.
llcc68: signal rssi is -69.0.
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F 0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F 0x20 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x29 0x2A 0x2B 0x2C 0x2D 0x2E 0x2F 0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x3A 0x3B 0x3C 0x3D 0x3E 0x3F 0x40 0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 0x49 0x4A 0x4B 0x4C 0x4D 0x4E 0x4F 0x50 0x51 0x52 0x53 0x54 0x55 0x56 0x57 0x58 0x59 0x5A 0x5B 0x5C 0x5D 0x5E 0x5F 0x60 0x61 0x62 0x63 0x64 0x65 0x66 0x67 0x68 0x69 0x6A 0x6B 0x6C 0x6D 0x6E 0x6F 0x70 0x71 0x72 0x73 0x74 0x75 0x76 0x77 0x78 0x79 0x7A 0x7B 0x7C 0x7D 0x7E 0x7F 0x80 0x81 0x82 0x83 0x84 0x85 0x86 0x87 0x88 0x89 0x8A 0x8B 0x8C 0x8D 0x8E 0x8F 0x90 0x91 0x92 0x93 0x94 0x95 0x96 0x97 0x98 0x99 0x9A 0x9B 0x9C 0x9D 0x9E 0x9F 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 0xA6 0xA7 0xA8 0xA9 0xAA 0xAB 0xAC 0xAD 0xAE 0xAF 0xB0 0xB1 0xB2 0xB3 0xB4 0xB5 0xB6 0xB7 0xB8 0xB9 0xBA 0xBB 0xBC 0xBD 0xBE 0xBF 
llcc68: finish receive test.
```

```shell
llcc68 -t lora-cad

llcc68: start cad test.
llcc68: irq cad done.
llcc68: cad not detected.
llcc68: finish cad test.
```

```shell
llcc68 -e lora-send --data=LibDriver

llcc68: send LibDriver.
llcc68: irq tx done.
```

```shell
llcc68 -e lora-receive --timeout=100

llcc68: start receiving...
llcc68: irq rx done.
llcc68: rssi is -71.0.
llcc68: snr is 10.00.
LibDriver
```

```shell
llcc68 -e lora-cad

llcc68: irq cad done.
llcc68: cad not detected.
```

```shell
llcc68 -e lora-sleep

llcc68: sleep.
```

```shell
llcc68 -e lora-wake-up

llcc68: wake up.
```

```shell
llcc68 -h

Usage:
  llcc68 (-i | --information)
  llcc68 (-h | --help)
  llcc68 (-p | --port)
  llcc68 (-t reg | --test=reg)
  llcc68 (-t lora-send | --test=lora-send)
  llcc68 (-t lora-receive | --test=lora-receive) [--timeout=<time>]
  llcc68 (-t lora-cad | --test=lora-cad)
  llcc68 (-e lora-send | --example=lora-send) [--data=<str>]
  llcc68 (-e lora-receive | --example=lora-receive) [--timeout=<time>]
  llcc68 (-e lora-cad | --example=lora-cad)
  llcc68 (-e lora-sleep | --example=lora-sleep)
  llcc68 (-e lora-wake-up | --example=lora-wake-up)

Options:
      --data=<str>            Set the send data.([default: LibDriver])
  -e <lora-send | lora-receive | lora-cad | lora-sleep | lora-wake-up>, --example=<lora-send
     | lora-receive | lora-cad | lora-sleep | lora-wake-up>
                              Run the driver example.
  -h, --help                  Show the help.
  -i, --information           Show the chip information.
  -p, --port                  Display the pin connections of the current board.
  -t <reg | lora-send | lora-receive | lora-cad>, --test=<reg | lora-send | lora-receive | lora-cad>
                              Run the driver test.
      --timeout=<time>        Set the timeout in ms.([default: 1000])
```

