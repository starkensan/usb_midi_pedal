# USB MIDI Pedal

Pimoroni Tiny 2040を使用した、拡張可能なUSB/DIN MIDIフットペダルのファームウェアプロジェクトです。

現在はビルド環境と最小FreeRTOSファームウェアまで実装済みです。

## 想定ハードウェア

- Pimoroni Tiny 2040（PIM558、RP2040、8 MB flash）
- フットスイッチ × 6
- エクスプレッションペダル × 1
- 128 × 64 I2C OLED
- Tiny 2040内蔵RGB LED
- USB MIDI OUT
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

## ドキュメント

設計仕様と設計判断は[ドキュメント一覧](docs/README.md)にまとめています。

- [製品要件・仕様](docs/designs/product-requirements.md)：製品の目的、対象範囲、機能要件
- [アーキテクチャ](docs/architecture.md)：ディレクトリ構成、各層の責務、依存関係
- [ハードウェア仕様](docs/hardware.md)：電源、入出力回路、使用部品
- [ピン割り当て](docs/pin_assignment.md)：Tiny 2040のGPIO、I2C、UARTの割り当て
- [I2C ADC選定記録](docs/designs/adc-selection.md)：ADC候補の比較とADS1015採用理由
- 今後追加予定：RTOS、MIDI、プリセット、画面設計

## 現在の状態

- [x] 基本ディレクトリ構成
- [x] Pico SDK submodule
- [x] FreeRTOS Kernel submodule
- [x] CMakeビルド設定
- [x] Pimoroni Tiny 2040公式ボード定義の選択
- [x] 最小FreeRTOS起動とRGB LED点滅タスク
- [ ] プロジェクト固有のピン定義
- [ ] USB MIDI実装
