#ifndef VEMT_PHASE_HPP
#define VEMT_PHASE_HPP

namespace vemt{

///
/// @brief �t�F�[�Y��\���N���X
///
class Phase{
public:

	static const Phase kPreEntry;	///< ���G���g���[
    static const Phase kEntry;		///< �G���g���[
	static const Phase kSubmit;		///< ���e

	Phase(void) = delete;

	/// @brief �t�F�[�Y�i���o�[����C���X�^���X�𐶐�����
	/// @param phase_int �t�F�[�Y�i���o�[
	Phase(const int phase_int) noexcept;

	/// @brief �R�s�[�R���X�g���N�^
	/// @param phase �R�s�[��
    Phase(const Phase & phase) noexcept;

	/// @brief �f�X�g���N�^
	~Phase() noexcept;

    const bool operator==(const Phase & phase) const noexcept;
    const bool operator!=(const Phase & phase) const noexcept;
    const bool operator>(const Phase & phase) const noexcept;
    const bool operator<(const Phase & phase) const noexcept;
	const bool operator==(const int phase) const noexcept;
	const bool operator!=(const int phase) const noexcept;
	const bool operator>(const int phase) const noexcept;
	const bool operator<(const int phase) const noexcept;

	/// @brief int�^�t�F�[�Y�i���o�[�ւ̕ϊ�
	/// @return �t�F�[�Y�i���o�[
	/// @sa operator()(void)
	int to_int(void) const noexcept;

	/// @brief int�^�t�F�[�Y�i���o�[�ւ̃L���X�g
	/// @return �t�F�[�Y�i���o�[
	/// @sa to_int()
	operator int(void) const noexcept;
private:
	const int phase_int_;
};

}

#endif