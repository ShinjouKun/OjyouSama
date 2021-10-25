#include "ReportArea.h"

ReportArea::ReportArea(const Vector3 & pos, ObjectManager * objectManager, shared_ptr<ModelRenderer> modelRenderer, int num)
	:modelRender(modelRenderer)
{
	position = pos;
	objManager = objectManager;
	number = num;

	Init();
}

ReportArea::~ReportArea()
{
}

void ReportArea::SetActice(bool value)
{
	isActive = value;
}

void ReportArea::SetDeath(bool value)
{
	death = value;
}

void ReportArea::Init()
{
	death = false;
	objType = ObjectType::ITEM;

	offsetScale = Vector3().one;
	scale = offsetScale;
	radius = offsetScale.x * 1.0f;

	reportRange = 5.0f;

	isActive = true;

	spehereCollider = new SphereCollider(position, radius);
	spehereCollider->SetRadiuse(radius);
	/*SetCollidder(spehereCollider);*/

	name = "AttackArea";
	key = to_string(number);
	keyname = name + key;

	modelRender->AddModel(keyname, "Resouse/bill.obj", "Resouse/bill.png");
	//modelRender->SetAncPoint(keyname, Vector3(-1.0f, -1.0f, -1.0f));
	modelRender->SetAncPoint(keyname, -scale *2);
}

void ReportArea::Update()
{
	//if (!isActive) return;

	if (isActive)
	{
		if (radius < reportRange)
		{
			//™X‚É‘å‚«‚­‚È‚Á‚Ä‚¢‚­
			scale += Vector3(0.1f, 0.1f, 0.1f);
			radius = scale.x;
			spehereCollider->SetRadiuse(radius);
			SetCollidder(spehereCollider);
		}
		else
		{

			scale = offsetScale;
			radius = offsetScale.x * 1.0f;

			spehereCollider->SetRadiuse(radius);
			isActive = false;
		}
	}
	else
	{

		death = true;
	}

	//ImGui::Checkbox("//////////////////////////////////", &isActive);

	ImGuiDebug();
}

void ReportArea::Rend()
{
	//if (!isActive) return;

	DirectXManager::GetInstance()->SetData3D();

	if (isActive)
	{
		//modelRender->Draw(keyname, position, Vector3(angle.x, -angle.y, angle.z), scale);
	}
}

void ReportArea::OnCollison(BaseCollider * info)
{
	//is(!isActive) return;
}

void ReportArea::ImGuiDebug()
{
#ifndef DEBUG

	//ImGui::Text("Detemasuyo-");
	//ImGui::SliderFloat("HP", &radius, 0, 1000.0f);

#endif // DEBUG
}
