#pragma once

#include <memory>
#include <list>
#include <vector>

class Emitter;
//ここではエミッターを扱いサポートする
class ParticleSystem
{
private:
	ParticleSystem();

public:
	~ParticleSystem();

	static ParticleSystem& instance();
	//終了処理
	void finalize();
	//追加
	void add(Emitter* add);
	//更新
	void update();
	//描画
	void draw();

	//中身をすべて消す
	void reset();

private:
	//コピー禁止
	ParticleSystem(const ParticleSystem&) = delete;
	ParticleSystem& operator=(const ParticleSystem&) = delete;
	ParticleSystem(ParticleSystem&&) = delete;
	ParticleSystem& operator=(ParticleSystem&&) = delete;

private:
	static ParticleSystem* mInstance;
	std::vector<std::shared_ptr<Emitter>> mEmitters;
	std::list<std::shared_ptr<Emitter>> mPendingEmitter;
};