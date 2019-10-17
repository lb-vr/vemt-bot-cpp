# +query user KEY VALUE 
ユーザー個人に関する情報を取得する

## Syntax

```
+query user id USER_ID
+query user discordUserId DISCORD_USER_ID
+query user mention @MENTION#9999
```

## Authenticate
- Managerのみ
- queryチャンネル内のみ

## Process
- DBに問い合わせる
    - `EntriesTable::getBy*()`で適宜データを呼び出す
- 得られたデータを整形して表示する
    - 回答までは表示しない

# +query list-user KEY VARGS...
該当ユーザーをリストアップする

## Syntax

```
+query list-user phase (preentry/entry/submit/accepted)
+query list-user answer // 全ての答え、ここに入れるべき…？
+query list-user answered ID 
+query list-user answered ID --equals VALUE
+query list-user answered ID --greater VALUE
+query list-user answered ID --less VALUE
+query list-user answered ID --regex REGEX_RULE // あまり実装したくない
+query list-user unanswered ID
```

## Authenticate
- Managerのみ
- queryチャンネル内のみ

## Process
- DBに問い合わせ
    - `EntriesTable::getBy**()`で取得
- リストアップ

## Format
```
Qx. QUESTION_TITLE (TYPE_STRING)
該当件数 : XX件
--------------------------------------------------------
[ID 4ケタ右寄せ] [username]#[number]
    > ANSWER...
```

