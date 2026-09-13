# RTOS同期機能 設計

## 概要

- 状態: 実装済み
- 対応Issue: #66, #72
- 目的: タスク間の状態通知、通知数の管理、共有資源の排他を、静的に確保したFreeRTOSオブジェクトで提供する。
- 背景: mailboxだけでは、メッセージ本体を伴わない複数状態の通知や共有資源の排他を表現できない。

## スコープ

### 対象

- Event Flagによるビット状態の設定、消去、取得、待機
- counting semaphore（binary semaphoreを含む）による通知と待機
- 優先度継承を行うmutexによるロックとアンロック
- 静的確保したFreeRTOS Event GroupおよびSemaphoreによる実装

### 対象外

- ISRからの状態設定、通知、ロック、アンロック
- recursive mutex、reader-writer lock、条件変数
- オブジェクトの破棄と動的メモリ割り当て

## 責務と依存関係

```mermaid
flowchart LR
    app[app task] --> rtos_wrapper[lib/rtos_wrapper]
    rtos_wrapper --> eventgroup[FreeRTOS Event Group]
    rtos_wrapper --> semaphore[FreeRTOS Semaphore]
```

- `lib/rtos_wrapper`はFreeRTOSの静的オブジェクトを使う同期機能、無効な引数の検出、共通の型を提供する。
- FreeRTOSは本プロジェクトの固定依存であり、`event_flags_t`、`semaphore_t`、`mutex_t`はFreeRTOSの型を内部に保持する。

## 公開インターフェース

```c
#define EVENT_FLAGS_USER_BITS_MASK UINT32_C(0x00ffffff)
typedef uint32_t event_flags_bits_t;

error_code_t event_flags_init(event_flags_t *event_flags);
error_code_t event_flags_set(event_flags_t *event_flags, event_flags_bits_t bits);
error_code_t event_flags_clear(event_flags_t *event_flags, event_flags_bits_t bits);
error_code_t event_flags_wait(event_flags_t *event_flags, event_flags_bits_t bits,
                              bool wait_all, bool clear_on_exit, uint32_t timeout_ms,
                              event_flags_bits_t *observed_bits);
error_code_t event_flags_get(const event_flags_t *event_flags, event_flags_bits_t *bits);

error_code_t semaphore_init(semaphore_t *semaphore,
                            size_t maximum_count, size_t initial_count);
error_code_t semaphore_give(semaphore_t *semaphore);
error_code_t semaphore_take(semaphore_t *semaphore, uint32_t timeout_ms);

error_code_t freertos_mutex_init(mutex_t *mutex);
error_code_t mutex_lock(mutex_t *mutex, uint32_t timeout_ms);
error_code_t mutex_unlock(mutex_t *mutex);
```

- Event Flagの利用可能なビットは`EVENT_FLAGS_USER_BITS_MASK`で示す下位24ビットだけであり、`0`や範囲外のビットを指定した操作は失敗する。
- 全APIは成功時に`ERROR_CODE_OK`を返す。NULL引数は`ERROR_CODE_INVALID_ARGUMENT`、`0`または範囲外のEvent Flagビットと表現不能なSemaphore数は`ERROR_CODE_OUT_OF_RANGE`、初期化前またはFreeRTOS操作不能は`ERROR_CODE_NOT_READY`を返す。
- `event_flags_wait`は、`wait_all`が`true`なら全ビット、`false`ならいずれか1ビットを待機する。待機条件を満たさない場合は`ERROR_CODE_TIMEOUT`を返し、待機終了時点のビット状態を`observed_bits`へ返す。`clear_on_exit`が`true`の場合は、条件を満たしたビットをFreeRTOSが消去する。
- `semaphore_init`で`maximum_count`を`1`にするとbinary semaphoreとして利用できる。`initial_count`は`maximum_count`以下かつ、`maximum_count`は0より大きくなければならない。
- mutexは作成時に解放済みであり、所有タスクだけが`mutex_unlock`を呼び出せる。FreeRTOS実装は優先度継承を提供する。
- 初期化関数が`ERROR_CODE_OK`を返した後にだけ操作関数を使用する。`mutex`の初期化にはPico SDKの同名関数とのリンク名衝突を避けるため`freertos_mutex_init`を使用する。未初期化のCオブジェクトを検査して安全に扱うことはできない。
- `event_flags_t`、`semaphore_t`、`mutex_t`の内部フィールドは初期化後に変更しない。

## 処理フロー

```mermaid
sequenceDiagram
    participant Producer as 通知タスク
    participant API as lib/rtos_wrapper
    participant Consumer as 待機タスク

    Producer->>API: event_flags_set / semaphore_give
    API->>API: FreeRTOS APIを呼び出し
    API-->>Producer: true / false
    Consumer->>API: event_flags_wait / semaphore_take / mutex_lock
    API->>API: 指定時間までFreeRTOS APIで待機
    API-->>Consumer: true / false
```

## RTOS・ハードウェア上の考慮

- 実行コンテキスト: タスクのみ。ISRからは呼び出さない。
- FreeRTOSオブジェクトは`event_flags_t`、`semaphore_t`、`mutex_t`の内部バッファを使って静的に確保し、Heapを使用しない。
- タイムアウトはミリ秒で指定する。有限待機として表現できる最大tick以上は`portMAX_DELAY - 1` tickへ飽和させる。
- 各オブジェクトは静的記憶域期間で確保し、それを利用するすべてのタスクより長く存続させる。初期化と利用を並行させず、`*_init`が`true`を返した後にだけ操作関数を使用する。
- mutexを保持したままブロッキングI/Oや長時間処理を行わない。

## 検証方法

- Debugファームウェアをビルドし、FreeRTOSの静的 Event Group と Semaphore API を含めてコンパイルできることを確認する。
