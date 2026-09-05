# ドキュメント

このディレクトリでは、USB MIDI Pedalの要件、仕様、設計および開発ルールを管理します。

プロジェクト概要、開発環境、ビルド手順については[ルートのREADME](../README.md)を参照してください。

## ドキュメント一覧

| 状態 | ファイル | 内容 |
| --- | --- | --- |
| 要件 | [requirements/README.md](requirements/README.md) | 製品として満たす機能、用途、制約 |
| 仕様 | [specifications/README.md](specifications/README.md) | 確定したハードウェア仕様とGPIO割り当て |
| 設計 | [designs/README.md](designs/README.md) | アーキテクチャ、設計判断、機能・モジュールごとの設計 |
| 作成済み | [designs/adc-selection.md](designs/adc-selection.md) | I2C ADC候補の比較とADS1015採用判断 |
| 作成済み | [rules/README.md](rules/README.md) | 実装、検証、ドキュメント、GitHub運用の作業ルール |
| 設計予定 | `designs/rtos-design.md` | FreeRTOSタスク、優先度、周期、Queue、スタック設計 |
| 設計 | `designs/testing.md` | ホスト単体テスト、Unity/CMock、CTest構成 |
| 仕様予定 | `specifications/midi-mapping.md` | フットスイッチとEXPペダルのMIDI割り当て |
| 仕様予定 | `specifications/preset-format.md` | プリセットのデータ構造とフラッシュ保存形式 |
| 設計予定 | `designs/display-ui.md` | 128×64 OLEDの画面構成と画面遷移 |

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
