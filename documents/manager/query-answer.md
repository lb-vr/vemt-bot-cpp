# +query answer list ID OPTIONS
回答のフェッチ

## Syntax

```
+query answer list ID --with-user
+query answer list ID --equals VALUE
+query answer list ID --not-equals VALUE
+query answer list ID --greater VALUE
+query answer list ID --less VALUE
```

## Authentication
- Managerのみ
- queryチャンネル内のみ

## Process
- DBに問い合わせ
    - `QuestionItemsTable::getById()`で該当IDのQuestionを取得
    - `AnswersTable::getByQuestionId()`でAnswerとUserの一覧を取得
- リストアップ

## Format
```
Qx. QUESTION_TITLE (TYPE_STRING)
該当件数 : XX件
--------------------------------------------------------
// +query answer IDのみの場合
// この場合は件数が多い順に
(件数 3ケタ右寄せ) ANSWER1
(件数 3ケタ右寄せ) ANSWER2
...

// +query answer ID --with-userの場合
// ANSWERで同一の件数が多い順がベストだが
// 難しければユーザーのID昇順でok
[ID 4ケタ右寄せ] [username]#[number] > ANSWER
[ID 4ケタ右寄せ] [username]#[number] > ANSWER
...
```

# +query answer last-updated (ID)
最後に回答が更新された時間を取得する。IDの指定があれば、そのQuestionIDのみについて問い合わせる。

## Syntax
```
+query answer last-updated
+query answer last-updated ID
```

## Authentication
- Managerのみ
- queryチャンネル内のみ

## Process
- DBに問い合わせ
    - `AnswersTable::getLatestUpdate(ID?)`で最後の更新日時を取得する
    - `EntriesTable::getById()`でユーザーを引っ張るぐらいはいいかな
- メッセージを投げる
    - 「最終更新はYYYY年MM月DD日 HH時MM分SS秒にUSERNAME#NUMBERさんが更新しました。」

# +query answer csv
現時点での回答をまとめたcsvを吐き出す

## Syntax
```
+query answer csv
```

## Authentication
- Managerのみ
- queryチャンネル内のみ

## Process
- DBに問い合わせ
    - `EntriesTable::getAll`
    - `QuestionItemsTable::getAll`
    - `AnswersTable::getAll`
    - と、いうより、すべてをがっちゃんこしたすべての結果をもらうのが早い？
- csv形式になるようにファイルにダンプ
    - この時のファイル名に「最終更新時のファイル名」を付ける。
- ファイルをDiscordにアップロード

