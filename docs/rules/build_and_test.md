# ビルドと検証

WindowsでRaspberry Pi Pico VS Code拡張のツールを使用する場合は、次を実行してください。

```powershell
.\tools\build.ps1
```

Releaseビルドには次を使用します。

```powershell
.\tools\build.ps1 -Configuration Release
```

PATH設定済みの環境ではCMake Presetも利用できます。

```powershell
cmake --preset debug
cmake --build --preset debug
```

## ホスト単体テスト

ハードウェアに依存しないロジックとドライバは、開発PC上でUnityとCMockを
使用してテストします。ホストテスト用プリセットはPico SDKとFreeRTOSを
読み込まないため、ファームウェアとは分けて構成してください。

```powershell
cmake --preset host-tests
cmake --build --preset host-tests
ctest --test-dir build/host-tests --output-on-failure
```

ホストテストにはネイティブCコンパイラと、CMockのモック生成用にRuby 3.0以降が
必要です。UnityとCMockを利用できるように、リポジトリを取得した後はすべての
サブモジュールを初期化してください。

変更後は、少なくともDebugビルドが成功することを確認してください。ビルドできない環境では、実行できなかった理由を明記してください。

## コミット前の確認

- ソースコードを変更した場合は、コミット前にDebugビルドを実行し、エラーなく完了することを確認してください。
- ソースコードを変更した場合は、コミット前にプロジェクトで用意されているテストをすべて実行し、成功することを確認してください。
- ビルドまたはテストが失敗した状態ではコミットせず、原因を修正してから再実行してください。
- テストが未整備、または環境上の理由で実行できない場合は、確認済みとは扱わず、その理由をユーザーとIssueへ明記してください。
- 実行したコマンドと結果は、対応するIssueへ記録してください。
- ドキュメントのみを変更した場合、ビルドとテストは省略できます。
