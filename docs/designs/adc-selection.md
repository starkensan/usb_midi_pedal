# I2C ADC選定記録

## 概要

- 状態: 確定
- 対応Issue: #15
- 決定: EXPペダル入力用ADCにはADS1015を採用し、`ALERT/RDY`をData Ready割り込みに使用する。

## 背景

EXPペダルは、RP2040内蔵ADCではなく外部I2C ADCで読み取る。外部ADCを用いることで、EXP入力のアナログ回路をMCUから分離し、将来の拡張余地を確保する。

製品要件はEXP値を1 ms以下の周期でサンプリングするため、ADCは少なくとも1 kSPSを超える実効サンプリング速度を持つ必要がある。

## 比較対象

| ADC | 分解能・最大速度 | 電源・入力 | パッケージ | 判断 |
| --- | --- | --- | --- | --- |
| MCP3425A0T-E/CH | 16 bit時15 SPS、最速12 bit時240 SPS | 2.7〜5.5 V、差動、基準2.048 V | SOT-23-6 | 1 ms要件を満たさないため不採用 |
| LTC2301IMS | 12 bit、14 kSPS | 5 V専用、差動 | MSOP-12 | 性能は満たすが、5 V系が必要で、製品がLast Time Buyかつ秋月在庫が僅少のため不採用 |
| MCP3221A5T-I/OT | 12 bit、22.3 kSPS | 2.7〜5.5 V、単一エンド、電源基準 | SOT-23-5 | 速度・実装性は適合するが、Data Ready割り込み出力を持たないため不採用 |
| ADS1015 | 12 bit、3.3 kSPS、4 ch | 2.0〜5.5 V、単一エンドまたは差動、PGA | X2QFN-10、VSSOP-10など | 採用 |

## 決定内容

ADS1015を、3.3 V電源および400 kHz I2C Fast-modeで使用する。

- EXPペダルの両端を3.3 VとGNDへ接続する。
- ワイパーをADS1015の単一エンド入力AIN0へ接続する。
- PGAは0〜3.3 Vの入力範囲を扱える設定にする。
- 連続変換モードで1.6 kSPSを選択する。変換完了ごとに`ALERT/RDY`が出す約8 µsのパルスをData Ready割り込みとして使用する。
- `ALERT/RDY`はオープンドレイン出力であるため、3.3 Vへのプルアップ抵抗を設ける。
- Data Readyモードは、上限しきい値を`0x8000`、下限しきい値を`0x0000`、コンパレータキューを有効値に設定して有効化する。極性はactive-lowとする。
- `ALERT/RDY`はTiny 2040のGP0へ接続する。フットスイッチはMCP23017へ移すため、GP0はData Ready割り込み専用に使える。

## 選択理由

- 3.3 Vで動作するため、Tiny 2040のロジック電圧と直接接続できる。
- 1.6 kSPSで動作させても1 ms以下のサンプリング要件を満たす。
- `ALERT/RDY`を用いて変換完了時だけI2C読取りを行えるため、周期ポーリングを避けられる。
- 4入力とPGAを持つため、将来のアナログ入力追加にも対応できる。

## 影響とトレードオフ

- 分解能は12 bitであり、16 bit ADCより低い。ただしMIDI送信値は0〜127であり、EXP用途には十分である。
- Data Ready割り込み用にGP0を使用する。フットスイッチ入力にはMCP23017が追加で必要になる。
- I2Cアドレスは`0x48`とする。SH1106 OLEDは`0x3C`または`0x3D`に設定し、アドレスを重複させない。

## 参照資料

- [LTC2301製品情報（Analog Devices）](https://www.analog.com/jp/products/ltc2301.html)
- [ADS1015製品情報（Texas Instruments）](https://www.ti.com/product/ja-jp/ADS1015)
- [ADS1015データシート（Texas Instruments）](https://www.ti.com/lit/ds/symlink/ads1015.pdf)
- [MCP3425データシート](https://akizukidenshi.com/goodsaffix/MCP3425.pdf)
