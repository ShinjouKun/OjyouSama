#include "ModelLoader.h"
//#include"Window.h"
#include "../Device/Window.h"
#include<map>
#include"TexLoader.h"
//#include"DirectXManager.h"
#include "../Device/DirectXManager.h"

namespace {
	const int dataSize = 24;
}
OBJData & ModelLoader::GetOBJData(const string& keyWord)
{
	return Datas[keyWord];
}
ModelLoader::ModelLoader(PipeLine* pipeline) :pipeLine(pipeline)
{

}

ModelLoader::~ModelLoader()
{
	Datas.clear();
}

void ModelLoader::Load(const string& filename)
{
	OBJData data;
	wstring wf = wstring(filename.begin(), filename.end());
	const wchar_t *t = wf.c_str();
	char filePath[256];
	std::vector<std::string>outMatList;
	data.mesh.index.clear();//さっぱりさせる
	int pathTailPoint = 0;
	char loacal[dataSize + 1] = { 0x00 };
	int ret = WideCharToMultiByte(
		CP_ACP,
		0,
		t,
		-1,
		loacal,
		dataSize + 1,
		NULL,
		NULL
	);
	for (int i = ret - 1; i >= 0; i--)
	{
		if (filename[i] == '/')
		{
			pathTailPoint = i;
			break;
		}
	}
	strncpy_s(filePath, loacal, pathTailPoint + 1);
	CreateMesh(filename, data);
	CreateMatrial(data, filePath);

	Datas.emplace(filename, data);//追加完了
}


ModelLoader * ModelLoader::GetInstance(PipeLine* pipeline)
{
	static ModelLoader instance(pipeline);
	return &instance;
}

bool ModelLoader::CreateMesh(const string & filename, OBJData &outMatList)
{
	wstring wf = wstring(filename.begin(), filename.end());
	const wchar_t *t = wf.c_str();
	FILE*fp = nullptr;
	std::vector<Vector3>vertices;
	std::vector<Vector3>normals;
	std::vector<Vector3>texes;
	std::map<std::string, int>indexList;
	std::string currentMatName = "";
	char loacal[dataSize + 1] = { 0x00 };

	int ret = WideCharToMultiByte(
		CP_ACP,
		0,
		t,
		-1,
		loacal,
		dataSize + 1,
		NULL,
		NULL
	);


	fopen_s(&fp, loacal, "r");
	if (fp == nullptr)
	{
		return false;
	}

	const int LineBufferLength = 1024;
	char buffer[LineBufferLength];

	while (fgets(buffer, LineBufferLength, fp) != nullptr)
	{
		//コメントは無視
		if (buffer[0] == '#')
		{
			continue;
		}
		char* parsePoint = strchr(buffer, ' ');
		if (parsePoint == nullptr)
		{
			continue;
		}
		Replace('\n', '\0', buffer);
		//頂点関連
		if (buffer[0] == 'v')
		{
			//頂点座標
			if (buffer[1] == ' ')
			{
				ParseVKeyWordTag(vertices, &parsePoint[1]);
				//z軸の反転
				vertices[vertices.size() - 1].z *= -1.0f;
				//vertices[vertices.size() - 1].x *= -1.0f;
			}
			else if (buffer[1] == 't')
			{
				ParseVKeyWordTag(texes, &parsePoint[1]);
				texes[texes.size() - 1].y = (1.0f - texes[texes.size() - 1].y);
			}
			else if (buffer[1] == 'n')//法線
			{
				ParseVKeyWordTag(normals, &parsePoint[1]);
				//z軸回転
				normals[normals.size() - 1].z *= -1.0f;
				//normals[normals.size() - 1].x *= -1.0f;
			}

		}
		//面
		else if (buffer[0] == 'f')
		{
			ParseFKeyWordTag(outMatList, indexList, currentMatName, vertices, texes, normals, &parsePoint[1]);
		}
		else if (strstr(buffer, "mtllib") == buffer)
		{
			Replace('\n', '\0', buffer);
			//マテリアル名を保存
			outMatList.matNameList.emplace_back(&buffer[strlen("mtllib") + 1]);
		}
		else if (strstr(buffer, "usemtl") == buffer)
		{
			Replace('\n', '\0', buffer);
			currentMatName = &buffer[strlen("usemtl") + 1];
		}
	}
	fclose(fp);

	return true;
}

bool ModelLoader::CreateMatrial(OBJData &objMat, std::string filePath)
{
	char buffer[1024];

	for (auto matFileName : objMat.matNameList)
	{
		FILE* fp = nullptr;
		std::string name = filePath;//ファイルパス
		name += matFileName;//ここでｍｔｌ＋ファイルパスに

		fopen_s(&fp, name.c_str(), "r");

		if (fp == nullptr)
		{
			return false;
		}
		while (fgets(buffer, 1024, fp) != nullptr)
		{
			//マテリアルグループ名
			if (strstr(buffer, "newmtl") == buffer)
			{
				Replace('\n', '\0', buffer);
				objMat.mat.MatName = &buffer[strlen("newmtl") + 1];
			}
			else if (strstr(buffer, "map_Kd") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::string texName = &buffer[strlen("map_Kd") + 1];
				// ファイルパス込みで保存
				objMat.mat.TextureName = filePath + texName;

				std::vector<std::string>split;
				Split('.', (char*)texName.c_str(), split);

				if (split.size() > 1)
				{
					objMat.mat.TexKeyWord = split[0];
				}
				//ここで画像の読み込み
				TexLoader::GetInstance(pipeLine)->Load(objMat.mat.TextureName.c_str());
			}
			//Ambientカラー
			else if (strstr(buffer, "Ka") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string>split;
				Split(' ', &buffer[strlen("Ka") + 1], split);
				for (int i = 0; i < split.size(); i++)
				{
					objMat.mat.Ambient[i] = atof(split[i].c_str());
				}
			}
			//デフユーズからー
			else if (strstr(buffer, "Kd") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string>split;
				Split(' ', &buffer[strlen("Kd") + 1], split);

				for (int i = 0; i < split.size(); i++)
				{
					objMat.mat.Diffuse[i] = atof(split[i].c_str());
				}
			}
			else if (strstr(buffer, "Ks") == buffer)
			{
				Replace('\n', '\0', buffer);
				std::vector<std::string>split;
				Split(' ', &buffer[strlen("Ks") + 1], split);
				for (int i = 0; i < split.size(); i++)
				{
					objMat.mat.Specular[i] = (float)atof(split[i].c_str());
				}
			}
		}
		fclose(fp);
	}
	return true;
}

void ModelLoader::Split(char splitChar, char * buffer, std::vector<std::string>& out)
{
	int count = 0;
	if (buffer == nullptr)
	{
		return;
	}
	int startPoint = 0;

	while (buffer[count] != '\0')
	{
		if (buffer[count] == splitChar)
		{
			if (startPoint != count)
			{
				char splitStr[256] = { 0 };
				strncpy_s(splitStr, 256, &buffer[startPoint], count - startPoint);
				out.emplace_back(splitStr);
			}
			else
			{
				out.emplace_back("");
			}

			startPoint = count + 1;
		}
		count++;
	}
	if (startPoint != count)
	{
		char splitStr[256] = { 0 };
		strncpy_s(splitStr, 256, &buffer[startPoint], count - startPoint);
		out.emplace_back(splitStr);
	}
}

void ModelLoader::Replace(char searchChar, char replaceChar, char * buffer)
{
	int len = strlen(buffer);
	for (int i = 0; i < len; i++)
	{
		if (buffer[i] == searchChar)
		{
			buffer[i] = replaceChar;
		}
	}
}

void ModelLoader::ParseVKeyWordTag(std::vector<Vector3>& outVertices, char * buff)
{
	std::vector<std::string>splitStrings;
	Split(' ', buff, splitStrings);

	int count = 0;
	float values[3] = { 0.0f };
	for (std::string str : splitStrings)
	{
		values[count] = atof(str.c_str());
		count++;
	}
	outVertices.push_back(Vector3(values[0], values[1], values[2]));
}

void ModelLoader::ParseFKeyWordTag(OBJData& outObjVert, std::map<std::string, int>& indexList, std::string currentMat, std::vector<Vector3>& vertices, std::vector<Vector3>& textures, std::vector<Vector3>& normals, char * buffer)
{
	int count = 0;
	int vertexInfo[3] =
	{
		-1,-1,-1,
	};
	std::vector<std::string>spaceSplit;
	Split(' ', buffer, spaceSplit);
	for (int i = 0; i < spaceSplit.size(); i++)
	{
		VertexModel objVert;
		ParseShashKeyWordTag(vertexInfo, (char*)spaceSplit[i].c_str());
		for (int i = 0; i < 3; i++)
		{
			if (vertexInfo[i] == -1)
			{
				continue;
			}
			int id = vertexInfo[i];

			switch (i)
			{
			case 0:
				objVert.pos = vertices[id];
				break;
			case 1:
				objVert.uv = Vector2(textures[id].x, textures[id].y);
				break;
			case 2:
				objVert.normal = normals[id];
				break;
			default:
				break;
			}

		}
#define OPTIMIZATION (0)
#if OPTIMIZATION
		std::string key = "";
		for (int i = 0; i < 3; i++)
		{
			std::ostringstream sout;
			sout << std::setfill('0') << std::setw(5) << vertexInfo[i];
			key += sout.str();
		}
		if (indexList.count(key) > 0)
		{
			outObjVert.push_back(indexList[key]);
		}
		else
		{
			outObjVert.push_back(vertex);
			outObjVert.push_back(outObjVert.size() - 1);
			indexList[key] = outObjVert.size();
		}
#else
		//頂点バッファリストへ追加
		outObjVert.mesh.vertex.push_back(objVert);
		outObjVert.mat.MatName.push_back(outObjVert.mat.MatName.size());
		//インデックスバッファに追加
		outObjVert.mesh.index.push_back(outObjVert.mesh.index.size());
#endif

	}
	//ポリゴンの作成の頂点順番を反転する
	int size = outObjVert.mesh.index.size();
	int temp = outObjVert.mesh.index[size - 1];
	outObjVert.mesh.index[size - 1] = outObjVert.mesh.index[size - 3];
	outObjVert.mesh.index[size - 3] = temp;
}

void ModelLoader::ParseShashKeyWordTag(int * list, char * buffer)
{
	int counter = 0;
	std::vector<std::string>slashSplit;
	Split('/', buffer, slashSplit);
	for (std::string str : slashSplit)
	{
		if (str.size() > 0)
		{
			list[counter] = atoi(str.c_str()) - 1;
		}
		counter++;
	}
}
