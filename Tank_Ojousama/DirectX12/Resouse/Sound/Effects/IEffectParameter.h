#pragma once

//�G�t�F�N�g�p�����[�^�C���^�[�t�F�[�X
class IEffectParameter
{
public:
	virtual ~IEffectParameter() = default;
	virtual void setEffectParameters(int effectID, const void* parameters, unsigned parametersByteSize) = 0;
	virtual void getEffectParameters(int effectID, void* parameters, unsigned parametersByteSize) const = 0;
};
