#include "SoundStreaming.h"
#include "../Data/SoundData.h"
#include "../Player/SoundPlayer.h"
#include "../Player/SoundPlayTimer.h"
#include "../Player/Buffer/BufferSubmitter.h"
#include "../Voice/SourceVoice.h"
#include "../../Math/Math.h"
#include <utility>

SoundStreaming::SoundStreaming(SourceVoice & sourceVoice, SoundPlayer & player, std::unique_ptr<ISoundLoader>& loader, const WaveFormat & format):
	mSourceVoice(sourceVoice),
	mPlayer(player),
	mBufferSubmitter(std::make_unique<BufferSubmitter>(sourceVoice)),
	mLoader(std::move(loader)),
	mBuffer{nullptr,nullptr},
	mPrimary(0),
	mSecondary(1),
	READ_SIZE(format.avgBytesPerSec),
	mRemainBufferSize(0),
	mWrite(0),
	mEndOfFile(false)
{
	for (size_t i = 0; i < BUFFER_COUNT; ++i)
	{
		mBuffer[i] = new BYTE[READ_SIZE];
	}
	mRemainBufferSize = mLoader->size() % READ_SIZE;
}

SoundStreaming::~SoundStreaming()
{
	for (size_t i = 0; i < BUFFER_COUNT; i++) 
	{
		delete[] mBuffer[i];
		mBuffer[i] = nullptr;
	}
}

void SoundStreaming::polling()
{
	//�t�@�C���̏I�[�Ȃ炱��ȏ�ǂݍ��߂Ȃ�
	if (mEndOfFile) 
	{
		return;
	}

	XAUDIO2_VOICE_STATE state = { 0 };
	mSourceVoice.getXAudio2SourceVoice()->GetState(&state);
	//�Đ��L���[��BUFFER_QUEUE_MAX�����Ȃ�V���Ƀo�b�t�@��ǉ�����
	if (state.BuffersQueued < BUFFER_QUEUE_MAX) 
	{
		std::swap(mPrimary, mSecondary);
		addBuffer();
	}
}

void SoundStreaming::seek(float point)
{
	initialize();

	//�V�[�N�ʒu���Ȃ̍ŏ����w���Ă���Ȃ炱���ŏI���
	if (Math::nearZero(point)) {
		return;
	}

	const auto& data = mSourceVoice.getSoundData();
	auto sec = data.clamp(point);
	auto seekPoint = static_cast<unsigned>(data.averageBytesPerSec * sec);

	recomputeRemainBufferSize(seekPoint);

	mLoader->seek(static_cast<long>(seekPoint));
	mWrite = seekPoint;
}

void SoundStreaming::addBuffer()
{
	long res = 0;
	//���̓ǂݍ��݂��f�[�^�T�C�Y�𒴂���Ȃ�
	if (mWrite + READ_SIZE > mLoader->size()) 
	{
		res = read(mRemainBufferSize);
	}
	else 
	{
		//�Đ����Ԃ𑵂��邽�߂ɖ������
		if (mWrite == 0) {
			mPlayer.getPlayTimer().setPlayTime(0.f);
		}

		res = read(READ_SIZE);
	}
	//�Ȃ����ǂݍ��߂Ȃ��Ȃ�̂�
	if (res == 0)
	{
		mEndOfFile = true;
		return;//�����I�ɏI��
	}
	mWrite += res;

	//�������ݗʂ��f�[�^�T�C�Y�𒴂�����I��
	if (mWrite >= mLoader->size()) 
	{
		mEndOfFile = true;
	}

	//�o�b�t�@�쐬
	SimpleSoundBuffer buf;
	buf.buffer = mBuffer[mSecondary];
	buf.size = res;
	buf.isEndOfStream = mEndOfFile;

	mBufferSubmitter->submitSimpleBuffer(buf);
}

long SoundStreaming::read(unsigned readSize)
{
	return mLoader->read(&mBuffer[mSecondary], readSize);
}

void SoundStreaming::initialize()
{
	mLoader->seekBegin();
	mWrite = 0;
	mEndOfFile = false;
	recomputeRemainBufferSize(0);
}

void SoundStreaming::recomputeRemainBufferSize(unsigned point)
{
	auto sub = mLoader->size() - point;
	mRemainBufferSize = sub % READ_SIZE;
}
