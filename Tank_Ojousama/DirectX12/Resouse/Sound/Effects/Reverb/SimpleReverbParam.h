#pragma once

//�ȈՔŃ��o�[�u�p�����[�^
struct SimpleReverbParam
{
	//�ǔ��˂̍d��(0.f �` 1.f)
	float diffusion;
	//�����̃T�C�Y(0.f �` 1.f)
	float roomSize;

	SimpleReverbParam() :
		diffusion(0.9f),
		roomSize(0.6f)
	{
	}
};