#pragma once
#include<string>
#include<iostream>
#include<vector>
#include<unordered_map>
#include"Vector4.h"
#include"Vector3.h"
#include"Vector2.h"
#include<d3d12.h>
#include<d3dx12.h>
#include<map>
#include"PipeLine.h"


#pragma comment(lib,"d3d12.lib")

using namespace std;
using namespace Microsoft::WRL;

//�}�e���A���p
struct OBJMatM
{
	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	string TexKeyWord;
	string TextureName;
	string MatName;
	Vector4 color;
};
struct VertexModel
{
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
};

struct Mesh
{
	vector<VertexModel>vertex;
	vector<unsigned short>index;

};
struct OBJData
{
	Mesh mesh;
	OBJMatM mat;
	VertexModel vert;
	vector<string>matNameList;
};

class ModelLoader
{
public:
	OBJData& GetOBJData(const string& keyWord);
	ModelLoader(PipeLine* pipeline);
	~ModelLoader();
	void Load(const string& filename);

	static ModelLoader* GetInstance(PipeLine* pipeline);
private:

	//��͗p
	void Split(char splitChar, char* buffer, vector<string>&out);//�؂���
	void Replace(char searchChar, char replaceChar, char*buffer);//��芷���@�u������
	void ParseVKeyWordTag(vector<Vector3>&outVertices, char* buff);
	void ParseFKeyWordTag(OBJData& outObjVert, map<string, int>&indexList, string currentMat, vector<Vector3>&vertices, vector<Vector3>& textures, vector<Vector3>&normals, char*buffer);//�C���v�b�g���C�A�E�g�쐬
	void ParseShashKeyWordTag(int* list, char* buffer);
	bool CreateMesh(const string& filename, OBJData &outMatList);
	bool CreateMatrial(OBJData &objMat, string filePath);
	//obj
	map<const string, OBJData>Datas;//���f���f�[�^�̃��X�g
	PipeLine* pipeLine = nullptr;
};
