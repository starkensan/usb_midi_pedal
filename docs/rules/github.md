# GitHub IssueとPull Requestの運用

- Issueコメント、Pull Request作成、push、Issueの作成・更新・クローズなど、リモートリポジトリへ影響する操作は、実行前に必ずユーザーの確認を取ってください。
- リポジトリへ変更を加える作業は、着手前にGitHub Issueを作成してください。
- Issueのタイトルと本文は日本語で記述してください。
- Issue本文には、背景または目的、作業範囲、完了条件を記載してください。
- IssueおよびPull Requestの本文をCLIから渡す場合は、文字列`\n`ではなく実際の改行を使用してください。PowerShellではヒア文字列または`--body-file`を使用します。作成または更新後は、GitHub上でMarkdownの改行・見出し・箇条書きが正しく表示されることを確認してください。
- Issueに対応する作業は、作業開始前に次のように最新の`origin/develop`を取得し、それを基点とする作業ブランチを作成してください。

```powershell
git fetch origin develop
git switch -c feature-<issue番号>-<概要> origin/develop
```

- 1 Issueにつき1つの作業ブランチを使用してください。
- 作業ブランチ名は、`feature-xxxx`形式の小文字kebab-caseを使用してください。
- 例：Issue #12のUSB MIDI実装は`feature-issue-12-usb-midi`とします。
- `develop`ブランチ上で直接作業しないでください。
- 作業中に重要な判断や作業範囲の変更があった場合は、Issueへコメントしてください。
- 作業完了後は、作業ブランチをGitHubへpushし、`develop`をマージ先とするPull Requestを作成してください。
- Pull Requestのタイトルは、`<種別>: <日本語の概要> refs #<Issue番号>`の形式で記載してください。
- 種別には`docs`、`feat`、`fix`、`build`など、作業内容に合うものを使用してください。
- 例：`feat: USB MIDI送信を実装 refs #12`
- Pull Request本文は、作業種別、概要、影響スコープ、編集ファイル、その他の順で記載してください。
- 「その他」には、対応するIssue番号と実施した確認内容を必ず記載してください。
- Pull Request本文は、次のテンプレートを使用してください。

```markdown
## 作業種別

<!-- ドキュメント作成、実装、不具合修正、設定変更など -->

## 概要

<!-- 何を、なぜ変更したか -->

## 影響スコープ

<!-- 影響する機能、ハードウェア、ビルド環境、ドキュメントなど -->

## 編集ファイル

<!-- 主な編集ファイルと変更内容 -->

## その他

- 対応Issue: #<Issue番号>
- 確認: <!-- 実行したビルド・テスト、または省略理由 -->
```

- `Closes #<Issue番号>`などのクローズキーワードは、Pull Requestのマージ先がリポジトリの既定ブランチである場合にだけ自動クローズとして機能します。本プロジェクトのPull Requestは`develop`をマージ先とするため、本文には`対応Issue: #<Issue番号>`を記載し、`develop`へのマージ後、ユーザーの確認を得てIssueを手動でクローズしてください。
- 質問への回答や読み取り専用の調査など、リポジトリを変更しない作業ではIssueを作成する必要はありません。
