# I2C ADC選定記録

## 概要

- 状態: 確定
- 対応Issue: #15
- 決定: EXPペダル入力用ADCにはMCP3221A5T-I/OTを採用する。

## 背景

EXPペダルは、RP2040内蔵ADCではなく外部I2C ADCで読み取る。外部ADCを用いることで、EXP入力のアナログ回路をMCUから分離し、将来の拡張余地を確保する。

製品要件はEXP値を1 ms以下の周期でサンプリングするため、ADCは少なくとも1 kSPSを超える実効サンプリング速度を持つ必要がある。

## 比較対象

| ADC | 分解能・最大速度 | 電源・入力 | パッケージ | 判断 |
| --- | --- | --- | --- | --- |
| MCP3425A0T-E/CH | 16 bit時15 SPS、最速12 bit時240 SPS | 2.7〜5.5 V、差動、基準2.048 V | SOT-23-6 | 1 ms要件を満たさないため不採用 |
| LTC2301IMS | 12 bit、14 kSPS | 5 V専用、差動 | MSOP-12 | 性能は満たすが、5 V系が必要で、製品がLast Time Buyかつ秋月在庫が僅少のため不採用 |
| ADS1015 | 12 bit、3.3 kSPS、4 ch | 2.0〜5.5 V、単一エンドまたは差動 | X2QFN-10、VSSOP-10など | 性能は満たすが、1入力には過剰で、採用候補の小型単体部品より実装・調達面で不利 |
| MCP3221A5T-I/OT | 12 bit、22.3 kSPS | 2.7〜5.5 V、単一エンド、電源基準 | SOT-23-5 | 採用 |

## 決定内容

MCP3221A5T-I/OTを、3.3 V電源および400 kHz I2C Fast-modeで使用する。

- EXPペダルの両端を3.3 VとGNDへ接続する。
- ワイパーをMCP3221のアナログ入力へ接続する。
- MCP3221は電源基準ADCであるため、EXPの励起電圧とADC電源を同じ3.3 Vにすることで、電源電圧の変動を測定値へ与えにくくする。
- 1 ms周期の読取りに対して、22.3 kSPSは十分な余裕を持つ。

## 選択理由

- 1入力だけを必要とする構成に合致する。
- 3.3 Vで動作するため、Tiny 2040のロジック電圧と直接接続できる。
- SOT-23-5で小型であり、LTC2301のMSOP-12より実装面積と部品数を抑えられる。
- マルツでカットテープとして調達可能である。

## 影響とトレードオフ

- 分解能は12 bitであり、16 bit ADCより低い。ただしMIDI送信値は0〜127であり、EXP用途には十分である。
- 入力は単一エンド1 chのため、将来EXP入力を増設する場合はADCまたはI2Cバスへ別のADCを追加する必要がある。
- MCP3221A5T-I/OTの7 bit I2Cアドレスは`0x4D`である。SH1106 OLEDは`0x3C`または`0x3D`に設定し、アドレスを重複させない。

## 参照資料

- [MCP3221データシート](https://ww1.microchip.com/downloads/aemDocuments/documents/APID/ProductDocuments/DataSheets/MCP3221-Data-Sheet-DS20001732.pdf)
- [MCP3221A5T-I/OT（マルツ）](https://www.marutsu.co.jp/pc/i/13274348/)
- [LTC2301製品情報（Analog Devices）](https://www.analog.com/jp/products/ltc2301.html)
- [ADS1015製品情報（Texas Instruments）](https://www.ti.com/product/ja-jp/ADS1015)
- [MCP3425データシート](https://akizukidenshi.com/goodsaffix/MCP3425.pdf)
