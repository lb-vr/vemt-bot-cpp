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
phaseはパイプで区切ればor指定ができる。

```
+query list-user phase (preentry|entry|submit|accepted)
+query list-user phase (preentry|entry|submit|accepted) --count
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
List User | phase on PHASE
該当件数 XX件
--------------------------------------------------------
[ID 4ケタ右寄せ] Pre-Entry [username]#[number]
[ID 4ケタ右寄せ] Entry     [username]#[number]
[ID 4ケタ右寄せ] Submit    [username]#[number]
[ID 4ケタ右寄せ] Accepted  [username]#[number]
```

