#pragma once
#include "BaseObject.h"
#include"ObjectManager.h"
#include<memory>
//#include"ModelRenderer.h"
//#include"ParticleManager.h"
#include "../Render/ModelRenderer.h"
#include "../Render/ParticleManager.h"
class TaihouEnemy:public BaseObject
{
public:
	TaihouEnemy(Vector3 pos, Vector3 ang, ObjectManager * obj, shared_ptr<ModelRenderer> m, shared_ptr<ParticleManager>p,int num);
	~TaihouEnemy();
	void Shot();
	void Move();
private:


	// BaseObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void Init() override;

	virtual void Update() override;

	virtual void Rend() override;

	ObjectManager* objM;
	shared_ptr<ModelRenderer>taihouModel;
	shared_ptr<ParticleManager>taihouParticle;
	shared_ptr<ParticleEmitterBox>taihouParticleBox;
	int HP;
	bool ShotFlag;
	int ShotCount;
	bool HitFlag;//–³“GŠÔ‚Ì‚½‚ß
	int HitCount;
	int bulletStock;//’e‚ğ‚¢‚Á‚Ï‚¢‚¾‚·‚½‚ß

	//•¡”‚¾‚·—p
	int number = 0;
	string name;//ƒL[ƒ[ƒh“o˜^
	string name2;
	string num;//•¡”—pƒiƒ“ƒo[
	string numName;//ã“ñ‚Â‚Ì•¡‡‘Ì
	string numName2;//ã“ñ‚Â‚Ì•¡‡‘Ì
	int Count;

	// BaseObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void OnCollison(const CollisonInfo & info) override;

	// BaseObject ‚ğ‰î‚µ‚ÄŒp³‚³‚ê‚Ü‚µ‚½
	virtual void ImGuiDebug() override;
};
