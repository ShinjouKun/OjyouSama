#pragma once

#include "IVoice.h"
#include <xaudio2.h>
#include <memory>
#include <vector>

//ボイス出力管理
class OutputVoices
{
public:
	OutputVoices(IVoice& voice);
	~OutputVoices();

	//ボイスの出力先を追加
	void addOutputVoice(const IVoice& voice, bool useFilter = false, bool isApply = true);
	//添字に対応するディスクリプタを返す
	const XAUDIO2_SEND_DESCRIPTOR& getDesc(unsigned index) const;
	//ディスクリプタ配列のサイズを返す
	size_t size() const;

private:
	OutputVoices(const OutputVoices&) = delete;
	OutputVoices& operator=(const OutputVoices&) = delete;

	//設定を適用する
	void apply();

private:
	IVoice& mVoice;
	std::vector<XAUDIO2_SEND_DESCRIPTOR> mDescs;

};