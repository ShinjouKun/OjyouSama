#pragma once

//PlayTimer�G�t�F�N�g�̃p�����[�^
struct PlayTimerParam
{
	//�ݒ肵��������(�P��: �b)
	float setTime;
	//�Đ����x(�s�b�`)
	float frequencyRatio;

	PlayTimerParam() :
		setTime(0.f),
		frequencyRatio(1.f)
	{
	}
};
