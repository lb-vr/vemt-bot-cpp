# +config SUBCOMMAND
様々な設定を行うことができる

# +config question JSONFILE
質問をセットすることができる

## Syntax

```
+config question JSONFILE
```

## Authorized
- bot-controlチャンネルのみ
- BOT-Admin権限を持つ者のみ
- Entry期間を始めたらこのコマンドは使用不可

## Process
- jsonファイルをダウンロードする
- 文字列読み込んで、パース
    - `Question`クラスでjsonパース
    - 失敗したらそこでエラーメッセージをつけてリジェクト
- QuestionItemsTableの中身を全て削除
    - 残して「Not Used」をつけてもいいが、本当に二度と使わないから消していいんじゃない？
- `Question`クラスをDBへ流し込む
    - `QuestionItemsTable::insert()`を問題の数だけループして順次insert？
    - `<unknown>` : configかどこかにTitleとHeaderを格納
- `Question`クラスを一度すべて再取得する or 完全データでQuestionを再構築
    - `QuestionItemsTable::getAll()`でID付きQuestionItemsを取得
- `Question`からID付きで生成した質問一覧をプレビュー。

# +config question ID COLUMN

特定の質問に対して更新をかける

## Syntax

```

```
