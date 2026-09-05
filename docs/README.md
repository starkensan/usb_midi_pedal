# ドキュメント

このディレクトリでは、USB MIDI Pedalの設計仕様と設計判断を管理します。

プロジェクト概要、開発環境、ビルド手順については[ルートのREADME](../README.md)を参照してください。

## ドキュメント一覧

| 状態 | ファイル | 内容 |
| --- | --- | --- |
| 作成済み | [architecture.md](architecture.md) | `app`、`lib`、`drivers`の責務と依存関係 |
| 作成済み | [designs/README.md](designs/README.md) | 機能・モジュールごとの設計ドキュメント |
| 作成済み | [designs/product-requirements.md](designs/product-requirements.md) | 製品全体の要件と仕様 |
| 作成済み | [hardware.md](hardware.md) | 電源、入出力回路、主要部品の電気仕様 |
| 作成済み | [pin_assignment.md](pin_assignment.md) | Tiny 2040のGPIO、I2C、UARTの割り当て |
| 作成済み | [designs/adc-selection.md](designs/adc-selection.md) | I2C ADC候補の比較とADS1015採用判断 |
| 作成済み | [rules/README.md](rules/README.md) | 実装、検証、ドキュメント、GitHub運用の作業ルール |
| 作成予定 | `rtos_design.md` | FreeRTOSタスク、優先度、周期、Queue、スタック設計 |
| 作成予定 | `midi_mapping.md` | フットスイッチとEXPペダルのMIDI割り当て |
| 作成予定 | `preset_format.md` | プリセットのデータ構造とフラッシュ保存形式 |
| 作成予定 | `display_ui.md` | 128×64 OLEDの画面構成と画面遷移 |

## 設計判断

重要な設計判断は`decisions/`に記録します。ファイル名には連番と内容を付けます。

```text
decisions/
├─ 0001-use-freertos.md
├─ 0002-use-single-core.md
└─ 0003-use-app-lib-drivers.md
```

各記録には、少なくとも次の項目を含めます。

- 背景
- 決定内容
- 選択理由
- 影響とトレードオフ

## 記述方針

ドキュメントの作成・更新では、[作業ルール](rules/README.md)の「ドキュメント規約」に従います。
