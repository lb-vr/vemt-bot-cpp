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
    - `RegistryTable::updateString("title", TITLE))` こんな感じでconfigかどこかにTitleとHeaderを格納
- `Question`クラスを一度すべて再取得する or 完全データでQuestionを再構築
    - `QuestionItemsTable::getAll()`でID付きQuestionItemsを取得
- `Question`からID付きで生成した質問一覧をプレビュー。

# +config question add JSON

質問を追加する。これもentry期間開始後のみに使われる。

## Syntax

```
+config question add {
    "text": "TEXT",
    "details": ["DETAIL_LINE", "DETAIL_LINE", ...],
    "type": "string",
    "regex": ".+",
}
```

## Authorized
- bot-controlチャンネルのみ
- BOT-Admin権限を持つ者のみ
- entry期間開始後のみ

## Constraints
- 必須項目は作れない
- 回答時間制限も作れない

## Process
- json文字列をパースする
    - 失敗したら不正なjsonとしてエラーで弾く
- jsonから設定値を読み出す
- QuestionItemを新しく作る
- DBへinsertする
    - `QuestionItemsTable::insert()`的なもので追加
- 追加されたものだけをID付きでメッセージにプレビューする

# +config question update ID COLUMN

特定の質問に対して更新をかける。ここのコマンドは基本的にentryが開始された後に使われる。

## Syntax

```
+config question update ID detail MULTILINE_MESSAGE
+config question update ID is_required false
+config question update ID required_when_phase PHASE_STRING
+config question update ID required_when_datetime DATETIME_STRING
```

## Authorized
- bot-controlチャンネルのみ
- BOT-Admin権限を持つ者のみ
- entry開始後のみ

## Constraints
- PHASE_STRINGは、現在の値よりも後段である必要がある
- DATETIME_STRINGは、現時点の時刻よりも1日（24時間）以上後である必要がある（要検討…）

## Process
- IDをもとにQuestionItemをフェッチ
    - `QuestionItemsTable::getById(ID)`
    - 該当するIDがない場合は「該当する質問が存在しません」でエラー
- それぞれの引数が正しいかバリデート
    - 特に上の「Constraints」の項目のチェックは直に書かないといけないと思う
    - ダメだったら適宜エラーメッセージを出して終了
- フェッチしたQuestionItemにアップデート
- QuestionItemとしてのバリデート
    - ダメだったら適宜エラーメッセージを出して終了
- QuestionItemsTableを更新
    - `QuestionItemsTable::update()`
- 該当のQuestionItemをプレビュー表示

# +config question preview
- `Question`インスタンスを完全生成
    - `QuestionItemsTable::getAll()`でID付きQuestionItemsを取得
    - `RegistryTable::getString(KEY)`でそのほかのデータを取得
- `Question`からID付きで生成した質問一覧をプレビュー。

