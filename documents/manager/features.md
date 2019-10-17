- 回答が最後に更新された時間
    - `+query answer last-updated`みたいな感じで

// TEMP
# +config question preview OPTION
Questionをプレビュー表示できる
## Authenticate
- bot-controlの
- `Question`インスタンスを完全生成
    - `QuestionItemsTable::getAll()`でID付きQuestionItemsを取得
    - `RegistryTable::getString(KEY)`でそのほかのデータを取得
- `Question`からID付きで生成した質問一覧をプレビュー。
