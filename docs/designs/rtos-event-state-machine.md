# RTOSイベント駆動ステートマシン 設計

## 概要

- 状態: 実装済み
- 対応Issue: #68
- 目的: mailboxのイベントを現在状態のコールバックへ配送し、状態遷移をEvent Flagで通知する。

## 責務と依存関係

```mermaid
flowchart LR
    producer[送信タスク] --> mailbox[mailbox]
    mailbox --> machine[state_machine]
    machine --> callback[状態コールバック]
    machine --> flags[Event Flag]
```

## 公開インターフェース

```c
typedef struct { uint32_t id; uintptr_t parameter; } state_machine_event_t;
bool state_machine_process_next(state_machine_t *machine, uint32_t timeout_ms);
```

送信側は`state_machine_event_t`をmailboxへ送る。`state_machine_process_next`は1件受信して現在状態のコールバックを実行し、状態遷移時だけ指定Event Flagをセットする。状態ハンドラ配列は呼び出し側が静的に所有する。

## 検証方法

- ホスト単体テストでイベントIDとparameterの配送、状態遷移、Event Flag通知、空mailboxを確認する。
