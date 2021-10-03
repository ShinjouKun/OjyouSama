#pragma once
#include"BaseScene.h"
//#include"Input.h"
#include<memory>
#include "../Device/Input.h"
#include "../Render/Camera.h"
#include "../Render/ModelRenderer.h"
#include "../Render/TexRenderer.h"
#include "../Collision/CollisionPrimitive.h"
#include "../Actor/Player.h"
#include "../Actor/ObjectManager.h"
class Garage :public BaseScene
{
public:
	Garage();
	~Garage();

private:

	// BaseScene ����Čp������܂���
	virtual void StartScene() override;

	virtual void UpdateScene() override;

	virtual void DrawScene() override;

	Camera* camera;
	ObjectManager* objM;
	Player* player;//�v���C���[�̎��̐���

};
