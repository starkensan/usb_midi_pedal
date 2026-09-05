# RGB LEDドライバ設計

## 概要

- 状態: 実装済み
- 対応Issue: #20
- 目的: Tiny 2040内蔵RGB LEDの色設定と点灯状態を、アプリケーションから単純なAPIで制御する。

## スコープ

### 対象

- RGB各チャネルを組み合わせた色の指定
- 点灯、消灯、点灯状態の反転
- 起動時の全チャネル消灯

### 対象外

- PWMによる輝度制御
- 点滅・フェード・時間制御
- 起動、設定モード、保存結果、MIDI送信などの状態通知パターン

## 責務と依存関係

```mermaid
flowchart LR
    app[app] --> led[rgb_led driver]
    led --> gpio[Pico SDK GPIO]
    gpio --> hardware[Tiny 2040 RGB LED]
```

- `drivers/rgb_led/`は、色と点灯状態をGPIO出力へ変換する。
- 呼び出し元は点滅周期や製品状態との対応付けを管理する。
- Pico SDK APIは本ドライバ内に閉じ込める。

## 公開インターフェース

```c
typedef enum {
    RGB_LED_COLOR_OFF,
    RGB_LED_COLOR_RED,
    RGB_LED_COLOR_GREEN,
    RGB_LED_COLOR_BLUE,
    RGB_LED_COLOR_YELLOW,
    RGB_LED_COLOR_MAGENTA,
    RGB_LED_COLOR_CYAN,
    RGB_LED_COLOR_WHITE,
} rgb_led_color_t;

void rgb_led_init(void);
void rgb_led_set_color(rgb_led_color_t color);
void rgb_led_on(void);
void rgb_led_off(void);
void rgb_led_toggle(void);
```

- `rgb_led_set_color()`は色を記憶し、点灯中なら直ちに表示を更新する。消灯中は点灯状態を変えない。
- `rgb_led_on()`は記憶済みの色で点灯する。`rgb_led_off()`は色を保持して消灯する。
- `rgb_led_toggle()`は記憶済みの色を保ったまま点灯状態だけを反転する。
- 初期化前の呼び出し、および同時呼び出しはサポートしない。

## RTOS・ハードウェア上の考慮

- Tiny 2040のGP18（R）、GP19（G）、GP20（B）を出力に設定する。
- 内蔵LEDはactive-lowのため、各チャネルを点灯するときはGPIOをLow、消灯するときはHighに出力する。
- 本ドライバはFreeRTOSに依存せず、タスク、ISR、排他制御を持たない。

## 検証方法

- Debugビルドが成功することを確認する。
- 実機で赤、緑、青、混色、消灯、および`toggle`による点灯状態反転を確認する。
