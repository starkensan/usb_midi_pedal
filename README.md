# USB MIDI Pedal

Pimoroni Tiny 2040を使用した、拡張可能なUSB/DIN MIDIフットペダルのファームウェアプロジェクトです。

現在はビルド環境と最小FreeRTOSファームウェアまで実装済みです。

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
- Raspberry Pi Pico SDK 2.3.0
- FreeRTOS Kernel V11.3.0（単一コア構成）
- TinyUSB
- Pico SDKとFreeRTOS KernelはGit submoduleとして管理

## セットアップ

必要なツールは以下のとおりです。

- Git
- CMake 3.20以降
- Ninja
- Arm GNU Toolchain (`arm-none-eabi-gcc`)
- picotool

Windowsでは、Raspberry Pi Pico VS Code拡張が管理するツールを利用できます。

リポジトリを取得した後、submoduleを初期化します。

```powershell
git submodule update --init --recursive
```

## ビルド

Raspberry Pi Pico VS Code拡張のツールが`%USERPROFILE%\.pico-sdk`にあるWindows環境では、補助スクリプトがツールを自動検出します。

```powershell
.\tools\build.ps1
```

Releaseビルドは次のように実行します。

```powershell
.\tools\build.ps1 -Configuration Release
```

NinjaとArm GNU ToolchainがPATHに設定済みの環境では、CMake Presetを直接利用できます。

```powershell
cmake --preset debug
cmake --build --preset debug
```

生成物は`build/debug/src/`または`build/release/src/`に出力されます。Tiny 2040へ書き込むファイルは`usb_midi_pedal.uf2`です。

## ディレクトリ構成

```text
usb_midi_pedal/
├─ boards/                  必要になった場合の独自ボード定義
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
- [x] Pico SDK submodule
- [x] FreeRTOS Kernel submodule
- [x] CMakeビルド設定
- [x] Pimoroni Tiny 2040公式ボード定義の選択
- [x] 最小FreeRTOS起動とRGB LED点滅タスク
- [ ] プロジェクト固有のピン定義
- [ ] USB MIDI実装
