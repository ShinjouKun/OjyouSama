#include "Collision.h"
#include<cmath>

Collision::Collision()
{
}

Collision::~Collision()
{
}

bool Collision::StoSColl(const Sphere & s1, const Sphere & s2,Vector3* inter)
{
	//���a�̒��S�_�����f���̌��_�Ɉˑ����Ă���̂Œ��ӂ���
	float length = Vector3((s1.center-s2.center)).Length();
	float radSam = s1.radius + s2.radius;
	radSam *= radSam;
	if (length <= radSam)
	{
		if (inter)
		{
			float t = s2.radius / (s1.radius + s2.radius);
			*inter = Vector3::Lerp(s1.center, s2.center, t);
		}
		return true;
	}
	return false;

	////���a�̒��S�_�����f���̌��_�Ɉˑ����Ă���̂Œ��ӂ���
	//float length = Vector3((s1.center - s2.center)).Length();
	//float radSam = s1.radius + s2.radius;
	//if (length <= radSam)
	//{
	//	return true;
	//}
	//return false;
}

bool Collision::AABBColl(const AABB & box1, const AABB & box2)
{
	if (box1.min.x > box2.max.x)return false;
	if (box1.max.x < box2.min.x)return false;

	if (box1.min.y > box2.max.y)return false;
	if (box1.max.y < box2.min.y)return false;

	if (box1.min.z > box2.max.z)return false;
	if (box1.max.z < box2.min.z)return false;

	return true;//�Փˊm�F
}

bool Collision::StoPColl(const Sphere& sphere, const Plane& plane, Vector3 * inter)
{
	//���W�n�̌��_���狅�̒��S���W�ւ̋���
	float distV = Vector3::dot(sphere.center,plane.normal);
	//���ʂ̌��_���������Y���邱�Ƃɂ��A���ʂƋ��̒��S�Ƃ̋������o��
	float dist = distV - plane.distance;
	//�����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius)return false;

	//�^����_���v�Z
	if (inter)
	{
		*inter = -dist * plane.normal + sphere.center;
	}
	return true;
}

void Collision::PointToTriangle(const Vector3 & point, const Triangle & triangle, Vector3 * closest)
{
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	Vector3 p0_p2 = triangle.p2 - triangle.p0;
	Vector3 p0_pt = point - triangle.p0;

	float d1 = Vector3::dot(p0_p1,p0_pt);
	float d2 = Vector3::dot(p0_p2, p0_pt);

	if (d1 <= 0.0f&&d2 <= 0.0f)
	{
		//p0����ԋ߂�
		*closest = triangle.p0;
		return;
	}
	//point��p1�̊O���̒��_�̈�̒��ɂ��邩�`�F�b�N
	Vector3 p1_pt = point - triangle.p1;
	float d3 = Vector3::dot(p0_p1, p1_pt);
	float d4 = Vector3::dot(p0_p2, p1_pt);
	if (d3 >= 0.0f&&d4 <= d3)
	{
		*closest = triangle.p1;
		return;
	}
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f&&d1 >= 0.0f&&d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}
	Vector3 p2_pt = point - triangle.p2;
	float d5 = Vector3::dot(p0_p1, p2_pt);
	float d6 = Vector3::dot(p0_p2, p2_pt);
	if (d6 >= 0.0f&&d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f&&d2 >= 0.0f&&d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f&&d6)
	{
		float w = (d4 - d3 / ((d4 - d3) + (d5 - d6)) >= 0.0f);
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::StoTColl(const Sphere & sphere, const Triangle & triangle, Vector3 * inter)
{
	/*Vector3 p;
	PointToTriangle(sphere.center, triangle, &p);
	Vector3 v = p - sphere.center;
	v = Vector3::dot2(v, v);
	if (v > sphere.radius*sphere.radius)return false;

	if (inter)
	{
		*inter = p;
	}*/
	return true;
}
