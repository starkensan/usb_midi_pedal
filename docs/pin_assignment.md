# ピン割り当て

## 概要

- 状態: 確定（電気仕様）
- 対応Issue: #15
- 対象: Pimoroni Tiny 2040の外部GPIOと周辺回路の接続

## 割り当て

| GPIO | RP2040機能 | 接続先 | 入出力 | 備考 |
| --- | --- | --- | --- | --- |
| GP0 | GPIO | フットスイッチ1 | 入力 | active-low |
| GP1 | GPIO | フットスイッチ2 | 入力 | active-low |
| GP2 | I2C1 SDA | SH1106 OLED、MCP3221 SDA | 双方向 | 3.3 V、400 kHz |
| GP3 | I2C1 SCL | SH1106 OLED、MCP3221 SCL | 出力 | 3.3 V、400 kHz |
| GP4 | UART1 TX | 74HCT14入力 | 出力 | DIN MIDI OUT用 |
| GP5 | GPIO | フットスイッチ3 | 入力 | active-low |
| GP6 | GPIO | フットスイッチ4 | 入力 | active-low |
| GP7 | GPIO | フットスイッチ5 | 入力 | active-low |
| GP26 | GPIO | フットスイッチ6 | 入力 | active-low |
| GP27 | GPIO | エンコーダーA相 | 入力 | active-low |
| GP28 | GPIO | エンコーダーB相 | 入力 | active-low |
| GP29 | GPIO | エンコーダー押しボタン | 入力 | active-low |

## 制約

- Tiny 2040の外部GPIO 12本をすべて使用する。
- GP26〜GP29はADC機能を持つが、本製品ではデジタル入力として使用する。EXP入力は外部I2C ADCのMCP3221へ接続する。
- UART RXは使用しない。DIN MIDI INは製品スコープ外である。
- GPIOを追加する場合は、I2C GPIOエキスパンダーの採用を検討する。
