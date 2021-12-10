#include "ParticleManager.h"
//#include"Camera.h"
//#include"DirectXManager.h"
#include"../Render/Camera.h"
#include"../Device/DirectXManager.h"
ParticleManager::ParticleManager(PipeLine* pipe) :pipeLine(pipe)
{
	Init();
}

ParticleManager::~ParticleManager()
{
	particleDatas.clear();
}

void ParticleManager::Init()
{
	matProjectionP = Matrix4::Identity;
	matProjectionP.m[3][0] = -1.0f;
	matProjectionP.m[3][1] = 1.0f;
	matProjectionP.m[0][0] = 2.0f / Window::Window_Width;
	matProjectionP.m[1][1] = -2.0f / Window::Window_Height;
}

void ParticleManager::Update(const string& key)
{
	auto& d = particleDatas[key];
	
	//�������s�����p�[�e�B�N���̍폜
	d.particles.remove_if([](ParticleStateus& x) {return x.frame >= x.numFrame; });
	//�S�p�[�e�B�N���X�V
	for (std::forward_list<ParticleStateus>::iterator it = d.particles.begin(); it != d.particles.end(); it++)
	{
		//�o�߃t���[����
		it->frame++;
		//���x�ɉ����x�����Z
		it->vel = it->vel + it->acc;
		//���x�ɂ��ړ�
		it->pos = it->pos + it->vel;

		//�J���[
		//it->color = it->color;
		//�X�P�[��
		float f = (float)it->numFrame / it->frame;
		//�X�P�[���̐��`�ۊ�
		it->scale = (it->eScale - it->sScale) / f;
		it->scale += it->sScale;


	}
	//���_�f�[�^���o�b�t�@��
	VertexParticle* vertMap = nullptr;
	result = d.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		//for (auto itr = particleDatas.begin(); itr != particleDatas.end(); itr++)
		//{
		//	auto& d = itr->second;
		for (std::forward_list<ParticleStateus>::iterator it = d.particles.begin(); it != d.particles.end(); it++)
		{
			vertMap->pos = it->pos;
			vertMap->color = it->color;
			vertMap->scale = it->scale;
			vertMap++;
		}
		//}
		d.vertBuff->Unmap(0, nullptr);
	}
	//�s��̓]��
	ConstBuff*constMap;//�萔�o�b�t�@�Ƀf�[�^��]��
	constMap = nullptr;
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = Camera::matView *matProjectionP;
	constMap->matBill = matBill;
	d.constBuff->Unmap(0, nullptr);

}

void ParticleManager::OllUpDate()
{
	 matBill = Matrix4::Identity;

	Vector3 zAxis = Vector3::normalize(Camera::target - Camera::eye);
	Vector3 xAxis = Vector3::normalize(Vector3::cross(Camera::up, zAxis));
	Vector3 yAxis = Vector3::normalize(Vector3::cross(zAxis, xAxis));
	float temp[4][4] =
	{
		{xAxis.x,xAxis.y,xAxis.z,0.0f},
		{yAxis.x,yAxis.y,yAxis.z,0.0f},
		{zAxis.x,zAxis.y,zAxis.z,0.0f},
		{0.0f,0.0f,0.0f,1.0f}
	};
	matBill = Matrix4(temp);
	for (auto itr = particleDatas.begin(); itr != particleDatas.end(); itr++)
	{
		Update(itr->first);
	}
}

void ParticleManager::CreateBuff(const string & key)
{
	auto& d = particleDatas[key];
	//���_�o�b�t�@�쐬
	result = DirectXManager::GetInstance()->Dev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(d.vertex[0])*d.vertex.size()),//���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&d.vertBuff));

	VertexParticle* vertMap = nullptr;
	//�o�b�t�@�փf�[�^�𑗐M
	result = d.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, d.vertex.data(), sizeof(d.vertex[0])* d.vertex.size());
	d.vertBuff->Unmap(0, nullptr);


	//���_�o�b�t�@�r���[
	d.vbView.BufferLocation = d.vertBuff->GetGPUVirtualAddress();
	d.vbView.SizeInBytes = sizeof(d.vertex[0])*d.vertex.size();
	d.vbView.StrideInBytes = sizeof(d.vertex[0]);


	//�萔�o�b�t�@
	result = DirectXManager::GetInstance()->Dev()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),//�A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBuff) + 0xff)&~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&d.constBuff));

	XMMATRIX xx = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)Window::Window_Width / Window::Window_Height,
		0.1f, 1000.0f
	);
	ConstBuff*constMap;//�萔�o�b�t�@�Ƀf�[�^��]��
	result = d.constBuff->Map(0, nullptr, (void**)&constMap);//�}�b�s���O
	float xxx[4][4] =
	{
		{xx.r[0].m128_f32[0],xx.r[0].m128_f32[1],xx.r[0].m128_f32[2],xx.r[0].m128_f32[3]},
		{xx.r[1].m128_f32[0],xx.r[1].m128_f32[1],xx.r[1].m128_f32[2],xx.r[1].m128_f32[3]},
		{xx.r[2].m128_f32[0],xx.r[2].m128_f32[1],xx.r[2].m128_f32[2],xx.r[2].m128_f32[3]},
		{xx.r[3].m128_f32[0],xx.r[3].m128_f32[1],xx.r[3].m128_f32[2],xx.r[3].m128_f32[3]}
	};

	matProjectionP = Matrix4(xxx);
	constMap->mat = matProjectionP;
	d.constBuff->Unmap(0, nullptr);
}

void ParticleManager::CreateParticleData(const string & key, const string& filename)
{

	particleDatas.emplace(key, ParticleData());//�ǉ�
	auto& d = particleDatas[key];
	d.vertex.clear();
	d.vertex.resize(vertCount);
	std::vector<DirectVertexP>v;
	v.clear();
	v.resize(d.vertex.size());
	for (size_t i = 0; i < v.size(); i++)
	{
		v[i].pos.x = d.vertex[i].pos.x;
		v[i].pos.y = d.vertex[i].pos.y;
		v[i].pos.z = d.vertex[i].pos.z;
	}
	v.clear();
	CreateBuff(key);
	//TexName���L�[�Ƀe�N�X�`���f�[�^���Ăяo����data�ɓo�^����
	d.texData = &TexLoader::GetInstance(pipeLine)->GetTexList(filename);
}

void ParticleManager::DrawParticleBill(const string & key)
{
	auto& d = particleDatas[key];

	DirectXManager::GetInstance()->CmdList()->IASetVertexBuffers(0, 1, &d.vbView);
	DirectXManager::GetInstance()->CmdList()->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	//�V�F�[�_���\�[�X�r���[�Z�b�g
	DirectXManager::GetInstance()->CmdList()->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(pipeLine->GetDescripterHeap()->GetGPUDescriptorHandleForHeapStart(),
			d.texData->TexNum,
			DirectXManager::GetInstance()->Dev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	int num = std::distance(d.particles.begin(), d.particles.end());
	DirectXManager::GetInstance()->CmdList()->DrawInstanced((UINT)std::distance(d.particles.begin(), d.particles.end()), 1, 0, 0);
	//DirectXManager::GetInstance()->CmdList()->DrawInstanced(1, 1, 0, 0);
}

void ParticleManager::DrawParticleBillY(const string & key)
{
	//Matrix4 matBill = Matrix4::Identity;
	//auto& d = particleDatas[key];

	//Vector3 zAxis = Vector3::normalize(Camera::target - Camera::eye);
	//Vector3 xAxis = Vector3::normalize(Vector3::cross(Camera::up, zAxis));
	//Vector3 yAxis = Vector3::normalize(Camera::up);
	//zAxis = Vector3::cross(xAxis, yAxis);
	//float temp[4][4] =
	//{
	//	{xAxis.x,xAxis.y,xAxis.z,0.0f},
	//	{yAxis.x,yAxis.y,yAxis.z,0.0f},
	//	{zAxis.x,zAxis.y,zAxis.z,0.0f},
	//	{0.0f,0.0f,0.0f,1.0f}
	//};
	//matBill = Matrix4(temp);

	////�s��̓]��
	//ConstBuff*constMap;//�萔�o�b�t�@�Ƀf�[�^��]��
	//result = d.constBuff->Map(0, nullptr, (void**)&constMap);
	//constMap->matBill = Camera::matView *matProjectionP;
	//constMap->matBill = matBill;

	//d.constBuff->Unmap(0, nullptr);

	//DirectXManager::GetInstance()->CmdList()->IASetVertexBuffers(0, 1, &d.vbView);
	//DirectXManager::GetInstance()->CmdList()->SetGraphicsRootConstantBufferView(0, d.constBuff->GetGPUVirtualAddress());
	////�V�F�[�_���\�[�X�r���[�Z�b�g
	//DirectXManager::GetInstance()->CmdList()->SetGraphicsRootDescriptorTable(
	//	1,
	//	CD3DX12_GPU_DESCRIPTOR_HANDLE(pipeLine->GetDescripterHeap()->GetGPUDescriptorHandleForHeapStart(),
	//		d.texNum,
	//		DirectXManager::GetInstance()->Dev()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	//DirectXManager::GetInstance()->CmdList()->DrawInstanced(d.vertex.size(),1,0,0);
}

void ParticleManager::OllDraw()
{
	
	DirectXManager::GetInstance()->SetDataParticle();
	for (auto itr = particleDatas.begin(); itr != particleDatas.end(); itr++)
	{
		DrawParticleBill(itr->first);
	}
}

void ParticleManager::Add(const string& key, int life, const Vector3 & pos, const Vector3 & vel, const Vector3 & acc, float sScale, float eScale, const Vector4 & color, const Vector3& angle)
{
	auto& d = particleDatas[key];
	if (std::distance(d.particles.begin(), d.particles.end()) >= vertCount)
	{
		return;
	}
	d.particles.emplace_front();
	ParticleStateus& p = d.particles.front();
	//�l�̃Z�b�g
	p.pos = pos;
	p.vel = vel;
	p.acc = acc;
	p.sScale = sScale;
	p.eScale = eScale;
	p.color = color;
	p.numFrame = life;
	p.angle = angle;

	int num = std::distance(d.particles.begin(), d.particles.end());
}

ParticleEmitterBox::ParticleEmitterBox(shared_ptr<ParticleManager> partM) :particle(partM)
{

}

ParticleEmitterBox::~ParticleEmitterBox()
{

}

void ParticleEmitterBox::LoadAndSet(const string& key, const string& filename)
{
	TexLoader::GetInstance(pipeLine)->Load(filename);
	particle->CreateParticleData(key, filename);
}

void ParticleEmitterBox::EmitterUpdate(const string& key, const Vector3& pos, const Vector3& angle)
{
	//�e�����������Ƃ���
	this->pos = pos;
	this->angle = angle;
	for (int i = 0; i < 10; i++)
	{
		const float rndVel = 0.5f;
		Vector3 pVel;
		pVel.x = (float)rand() / RAND_MAX * rndVel - rndVel / 2.0f;
		pVel.y = (float)rand() / RAND_MAX * rndVel - rndVel / 2.0f;
		pVel.z = (float)rand() / RAND_MAX * rndVel - rndVel / 2.0f;
		pVel *= Matrix4::RotateZ(angle.z);
		pVel *= Matrix4::RotateX(angle.x);
		pVel *= Matrix4::RotateY(-angle.y);

		Vector3 acc;
		const float rndAcc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rndAcc;

		//���X�g�ɒǉ�
		particle->Add(key, 30, pos, pVel, acc, 5.0f, 0.0f, Vector4(1, 1, 1, 1), angle);
	}
}

void ParticleEmitterBox::EmitterUpdateBIG(const string & key, const Vector3 & pos, const Vector3 & angle)
{
	//�G���S
	this->pos = pos;
	this->angle = angle;
	for (int i = 0; i < 15; i++)
	{
		const float rndVel = 0.5f;
		Vector3 pVel;
		pVel.x = (float)rand() / RAND_MAX * rndVel - rndVel / 2.0f;
		pVel.y = (float)rand() / RAND_MAX * rndVel - rndVel / 2.0f;
		pVel.z = (float)rand() / RAND_MAX * rndVel - rndVel / 2.0f;
		pVel *= Matrix4::RotateZ(angle.z);
		pVel *= Matrix4::RotateX(angle.x);
		pVel *= Matrix4::RotateY(-angle.y);

		Vector3 acc;
		const float rndAcc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rndAcc;

		//���X�g�ɒǉ�
		particle->Add(key, 60, pos, pVel, acc, 15.0f, 0.0f, Vector4(1, 1, 1, 1), angle);
	}
}

void ParticleEmitterBox::EmitterUpdateUpGas(const string & key, const Vector3 & pos, const Vector3& angle)
{
	//�L���^�s���̉�
	this->pos = pos;
	this->angle = angle;
	for (int i = 0; i < 10; i++)
	{
		const float rndVel = 0.5f;
		Vector3 pVel;
		pVel.z = (float)rand() / RAND_MAX * rndVel + rndVel / 2.0f;
		pVel *= Matrix4::RotateZ(angle.z);
		pVel *= Matrix4::RotateX(angle.x);
		pVel *= Matrix4::RotateY(-angle.y);
		Vector3 acc;
		const float rndAcc = 0.005f;
		//���X�g�ɒǉ�
		particle->Add(key, 80, pos, pVel, acc, 0.3f, 0.3f, Vector4(1, 1, 1, 1), angle);
	}
}

void ParticleEmitterBox::EmitterUpdateFireGas(const string & key, const Vector3 & pos, const Vector3 & angle)
{
	//�e�B���m�̉Ή�(�����)
	this->pos = pos;
	this->angle = angle;
	for (int i = 0; i < 10; i++)
	{
		const float rndVel = 7.5f;
		Vector3 pVel;
		pVel.z = (float)rand() / RAND_MAX * rndVel + rndVel / 2.0f;
		pVel *= Matrix4::RotateZ(angle.z);
		pVel *= Matrix4::RotateX(angle.x);
		pVel *= Matrix4::RotateY(angle.y);
		Vector3 acc;
		const float rndAcc = 0.001f;
		//���X�g�ɒǉ�
		particle->Add(key, 30, pos, pVel, acc, 0.0f, 55.0f, Vector4(1, 1, 1, 1), angle);
	}
}

void ParticleEmitterBox::SetPos(Vector3 pos)
{
	this->pos = pos;
}

void ParticleEmitterBox::SetAngle(Vector3 angle)
{
	this->angle = angle;
}
