# ピン割り当て

## 概要

- 状態: 確定（電気仕様）
- 対応Issue: #15
- 対象: Pimoroni Tiny 2040の外部GPIOと周辺回路の接続

## 割り当て

| GPIO | RP2040機能 | 接続先 | 入出力 | 備考 |
| --- | --- | --- | --- | --- |
| GP0 | GPIO | ADS1015 `ALERT/RDY` | 入力 | 立下りエッジ割り込み |
| GP1 | GPIO | 未使用 | - | 将来拡張用 |
| GP2 | I2C1 SDA | SH1106 OLED、ADS1015、MCP23017 SDA | 双方向 | 3.3 V、400 kHz |
| GP3 | I2C1 SCL | SH1106 OLED、ADS1015、MCP23017 SCL | 出力 | 3.3 V、400 kHz |
| GP4 | UART1 TX | 74HCT14入力 | 出力 | DIN MIDI OUT用 |
| GP5 | GPIO | エンコーダー押しボタン | 入力 | active-low |
| GP6 | GPIO | 未使用 | - | 将来拡張用 |
| GP7 | GPIO | 未使用 | - | 将来拡張用 |
| GP26 | GPIO | 未使用 | - | 将来拡張用 |
| GP27 | GPIO | エンコーダーA相 | 入力 | active-low |
| GP28 | UART0 TX | デバッグUART TX | 出力 | 3.3 V TTL、TX専用 |
| GP29 | GPIO | エンコーダーB相 | 入力 | active-low |

## MCP23017の割り当て

| MCP23017端子 | 接続先 | 備考 |
| --- | --- | --- |
| GPA0〜GPA5 | フットスイッチ1〜6 | active-low、外部プルアップとRC回路を使用 |
| GPA6〜GPA7、GPB0〜GPB7 | 未使用 | 将来拡張用 |
| A0〜A2 | GND | I2Cアドレスは`0x20` |

## 制約

- Tiny 2040のGPIO使用数は8本である。GP1、GP6、GP7およびGP26は将来拡張用に確保する。
- GP26〜GP29はADC機能を持つ。GP27およびGP29はデジタル入力、GP28はUART0 TXとして使用し、EXP入力は外部I2C ADCのADS1015へ接続する。
- フットスイッチはMCP23017で読み取るため、追加する場合も残る10入力を利用できる。
- UART RXは使用しない。DIN MIDI INは製品スコープ外である。
- GPIOを追加する場合は、I2C GPIOエキスパンダーの採用を検討する。
