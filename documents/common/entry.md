# +entry

エントリー時のコマンド

## Syntax

```
+entry
```

## Authorized
- サーバー内にいる人だれでも
- entry期間のみ
- テキストチャンネル「entry」のみ有効

## Process

- UserIDから、すでにentry済みでないか確認
    - `EntriesTable::getByDiscordUID(uid)`
    - 戻り値の配列のサイズが0であれば次へ
    - エントリー済みの場合は「エントリー済みです」と案内し、テキストチャンネルへのリンクを張ったメッセージをメンション
- QuestionをDBからフェッチ
    - `QuestionItemsTable::getAll()`
    - `<unknown>` : Question全体のTitleとHeaderを取得
    - `bot::Question`インスタンスを生成
- User名のテキストチャットを「contact」カテゴリの中に作成
    - everyone : 読み書き不可
    - vemt-bot : 読み書き可能
    - manager : 読み書き可能
    - エントリーユーザー : 読み書き可能
    - entry途中でプログラムが失敗した場合、このテキストチャットは手動で削除してほしい
- Userに対して、exhibitorの権限を付与
- UserのDMにQuestionから生成した案内テキストを生成し投げる
- EntriesTableにユーザーをinsert
    - `EntriesTable::insert()`
    - この時に、ステータスをまとめているメッセージのIDと、DMチャンネルのIDを一緒に格納してもらう。
- entryチャンネルに「仮エントリーが完了しました」とともにチャンネルリンクをメンションする。
