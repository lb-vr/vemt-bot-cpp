#ifndef VEMT_PHASE_HPP
#define VEMT_PHASE_HPP

namespace vemt{

///
/// @brief フェーズを表すクラス
///
class Phase{
public:

	static const Phase kPreEntry;	///< 仮エントリー
    static const Phase kEntry;		///< エントリー
	static const Phase kSubmit;		///< 入稿
	static const Phase kPublish;	///< 開催

	Phase(void) = delete;

	/// @brief フェーズナンバーからインスタンスを生成する
	/// @param phase_int フェーズナンバー
	Phase(const int phase_int) noexcept;

	/// @brief コピーコンストラクタ
	/// @param phase コピー元
    Phase(const Phase & phase) noexcept;

	Phase & operator=(const Phase & phase) noexcept;
	Phase & operator=(const int phase_int) noexcept;

	/// @brief デストラクタ
	~Phase() noexcept;

    const bool operator==(const Phase & phase) const noexcept;
    const bool operator!=(const Phase & phase) const noexcept;
    const bool operator>(const Phase & phase) const noexcept;
    const bool operator<(const Phase & phase) const noexcept;
	const bool operator==(const int phase) const noexcept;
	const bool operator!=(const int phase) const noexcept;
	const bool operator>(const int phase) const noexcept;
	const bool operator<(const int phase) const noexcept;

	/// @brief int型フェーズナンバーへの変換
	/// @return フェーズナンバー
	/// @sa operator()(void)
	int to_int(void) const noexcept;

	/// @brief int型フェーズナンバーへのキャスト
	/// @return フェーズナンバー
	/// @sa to_int()
	operator int(void) const noexcept;
private:
	int phase_int_;
};

}

#endif