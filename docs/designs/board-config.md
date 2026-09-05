# ボード設定設計

## 概要

- 種別: 実装設計
- 対応Issue: #28
- 対象: Pimoroni Tiny 2040のピンおよびペリフェラル設定

`src/board/board_config.h`は、製品基板に固有のGPIO、I2CおよびUARTの設定値を一元管理する。

## スコープ

### 対象

- I2C1のピン、通信速度および接続デバイスのI2Cアドレス
- ADS1015のData Ready入力ピン
- DIN MIDIおよびデバッグUARTのインスタンスと送信ピン
- ロータリーエンコーダーの入力ピン
- 内蔵RGB LEDの各チャネルのGPIOとactive-low設定

### 対象外

- GPIOおよび周辺機能の初期化
- I2CおよびUARTの通信処理
- 未使用GPIOの割り当て

## 責務と依存関係

```mermaid
flowchart LR
    driver[drivers] --> config[board_config.h]
    config --> pico[Pico SDK hardware headers]
```

- `board_config.h`は基板固有の定数だけを提供する。
- GPIO初期化と通信制御は`drivers`が担う。
- Pico SDKへの依存は`board`内に閉じ込める。

## 公開インターフェース

`board_config.h`は次の接頭辞を持つマクロを公開する。

- `BOARD_I2C_*`: 共有I2Cバスのインスタンス、ピン、通信速度およびデバイスアドレス
- `BOARD_EXPRESSION_ADC_READY_PIN`: ADS1015のData Ready入力ピン
- `BOARD_DIN_MIDI_UART_*`: DIN MIDI出力用UART
- `BOARD_DEBUG_UART_*`: デバッグUART
- `BOARD_ENCODER_*`: ロータリーエンコーダーの入力ピン
- `BOARD_RGB_LED_*`: 内蔵RGB LEDのR/G/B各チャネルのGPIOと極性

I2CおよびUARTのインスタンスはPico SDKの`i2c1`、`uart0`、`uart1`を用いる。

## 検証方針

- `board_config.h`を含むファームウェアをDebug構成でビルドする。
- 値が[ピン割り当て](../specifications/pin_assignment.md)と一致することをレビューする。
