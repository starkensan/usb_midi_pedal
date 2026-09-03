# USB MIDI Pedal

Pimoroni Tiny 2040を使用した、拡張可能なUSB/DIN MIDIフットペダルのファームウェアプロジェクトです。

現在はプロジェクト構成を設計している段階です。ビルド設定とファームウェア実装は今後追加します。

## 想定ハードウェア

- Pimoroni Tiny 2040（PIM558、RP2040、8 MB flash）
- フットスイッチ × 6
- エクスプレッションペダル × 1
- 128 × 64 I2C OLED
- Tiny 2040内蔵RGB LED
- USB MIDI IN/OUT
- DIN MIDI OUT

## ソフトウェア構成

- C
- Raspberry Pi Pico SDK
- FreeRTOS（単一コア構成を予定）
- TinyUSB
- Pico SDKとFreeRTOS KernelはGit submoduleとして管理予定

## ディレクトリ構成

```text
usb_midi_pedal/
├─ boards/                  Pico SDK向けボード定義
├─ cmake/                   CMakeインポートおよび補助設定
├─ config/                  FreeRTOS、TinyUSB、アプリ設定
├─ docs/                    設計資料と設計判断の記録
│  └─ decisions/
├─ external/                Git submoduleなどの外部依存
├─ src/
│  ├─ app/                  製品固有処理、FreeRTOSタスク、状態管理
│  │  └─ tasks/
│  ├─ board/                基板固有の初期化とピン設定
│  ├─ drivers/              Pico SDKおよびデバイス依存処理
│  │  ├─ din_midi/
│  │  ├─ display/
│  │  ├─ expression/
│  │  ├─ flash_storage/
│  │  ├─ footswitch/
│  │  ├─ rgb_led/
│  │  └─ usb_midi/
│  └─ lib/                  ハードウェア非依存の再利用可能な処理
│     ├─ graphics/
│     ├─ input/
│     ├─ midi/
│     └─ preset/
├─ tests/                   PC上で実行する単体テスト
│  ├─ lib/
│  └─ mocks/
└─ tools/                   開発用補助ツール
```

## 依存関係の方針

```text
app ────────→ lib
 │
 └─────────→ drivers ─────→ Pico SDK / TinyUSB

drivers ───→ libのデータ型
lib ──×────→ app / drivers / FreeRTOS / Pico SDK
```

- `app`はFreeRTOSタスクや製品全体の振る舞いを担当します。
- `lib`はハードウェアに依存せず、PC上で単体テスト可能にします。
- `drivers`はGPIO、ADC、I2C、UART、USB、flashなどを担当します。
- `external`は外部依存専用とし、プロジェクト内部の`lib`と区別します。

## 現在の状態

- [x] 基本ディレクトリ構成
- [ ] Pico SDK submodule
- [ ] FreeRTOS Kernel submodule
- [ ] CMakeビルド設定
- [ ] ボードおよびピン定義
- [ ] 最小FreeRTOS起動
- [ ] USB MIDI実装
