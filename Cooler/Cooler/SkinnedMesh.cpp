#include "SkinnedMesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Texture.h"
#include "GraphicsCommon.h"
#include "GraphicsInterface.h"
#include "GraphicsMicro.h"

using namespace Cooler;

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

CSkinnedMesh::CSkinnedMesh(void)
{
	m_VAO = 0;
	memset(m_Buffers, 0, sizeof(m_Buffers));
	m_BonesNum = 0;
	m_pScene = NULL;
}


CSkinnedMesh::~CSkinnedMesh(void)
{
	__clear();
}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::SVertexBoneData::addBoneData(unsigned int vBoneID, float vWeight)
{
	unsigned int IDSize = sizeof(m_IDs) / sizeof(m_IDs[0]);
	for (unsigned int i=0; i<IDSize; i++)
	{
		if (m_Weight[i] == 0.0)
		{
			m_IDs[i] = vBoneID;
			m_Weight[i] = vWeight;
			return;
		}
	}

	_ASSERT(0);
}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::__clear()
{
	for (unsigned int i = 0 ; i < m_Textures.size() ; i++) {
		_SAFE_DELETE(m_Textures[i]);
	}

	if (m_Buffers[0] != 0)
	{
		unsigned int BufferSize = sizeof(m_Buffers) / sizeof(m_Buffers[0]);
		glDeleteBuffers(BufferSize, m_Buffers);
	}

	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

//******************************************************************
//FUNCTION:
bool CSkinnedMesh::loadMesh(const std::string& vFileName)
{
	__clear();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	unsigned int BufferSize = sizeof(m_Buffers) / sizeof(m_Buffers[0]);
	glGenBuffers(BufferSize, m_Buffers);

	int Ret = false;

	m_pScene = m_Importer.ReadFile(vFileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (m_pScene)
	{
		m_GlobalInverseTransform = mat4FromaiMatrix4x4(m_pScene->mRootNode->mTransformation);
		m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);
		Ret = __initFromScene(m_pScene, vFileName);
	}
	else {
		printf("Error parsing '%s': '%s'\n", vFileName.c_str(), m_Importer.GetErrorString());
	}

	glBindVertexArray(0);	
	return Ret;
}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::render()
{
	glBindVertexArray(m_VAO);

	for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
		const unsigned int MaterialIndex = m_Entries[i].m_MaterialIndex;

		assert(MaterialIndex < m_Textures.size());

		if (m_Textures[MaterialIndex]) {
		//	m_Textures[MaterialIndex]->bind(GL_TEXTURE0);
			glActiveTexture(GL_TEXTURE0);
			m_Textures[MaterialIndex]->useTexture();
		}

		glDrawElementsBaseVertex(GL_TRIANGLES, 
			m_Entries[i].m_NumIndices, 
			GL_UNSIGNED_INT, 
			(void*)(sizeof(unsigned int) * m_Entries[i].m_BaseIndex), 
			m_Entries[i].m_BaseVertex);
	}

	glBindVertexArray(0);
}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::boneTransform(float vTimeInSeconds, std::vector<glm::mat4>& voTransform)
{
	glm::mat4 IdentityMat = glm::mat4(1.0);

	float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = vTimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

	__readNodeHeirarchy(AnimationTime, m_pScene->mRootNode, IdentityMat);

	voTransform.resize(m_BonesNum);

	for (unsigned int i=0; i<m_BonesNum; i++)
	{
		voTransform[i] = m_BoneInfo[i].m_FinalTransformation;
	}
}

//******************************************************************
//FUNCTION:
bool CSkinnedMesh::__initFromScene(const aiScene* vScene, const std::string& vFileName)
{
	m_Entries.resize(vScene->mNumMeshes);
	m_Textures.resize(vScene->mNumMaterials);

	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCoords;
	std::vector<SVertexBoneData> Bones;
	std::vector<unsigned int> Indices;

	unsigned int VerticesNum = 0;
	unsigned int IndicesNum  = 0;

	for (unsigned int i=0; i<m_Entries.size(); i++)
	{
		m_Entries[i].m_MaterialIndex = m_pScene->mMeshes[i]->mMaterialIndex;
		m_Entries[i].m_NumIndices = m_pScene->mMeshes[i]->mNumFaces*3;
		m_Entries[i].m_BaseVertex = VerticesNum;
		m_Entries[i].m_BaseIndex  = IndicesNum;

		VerticesNum += m_pScene->mMeshes[i]->mNumVertices;
		IndicesNum += m_Entries[i].m_NumIndices;
	}

	Positions.reserve(VerticesNum);
	Normals.reserve(VerticesNum);
	TexCoords.reserve(VerticesNum);
	Bones.resize(VerticesNum);
	Indices.reserve(IndicesNum);

	for (unsigned int i=0; i<m_Entries.size(); i++)
	{
		const aiMesh* pAiMesh = m_pScene->mMeshes[i];
		__initMesh(i, pAiMesh, Positions, Normals, TexCoords, Bones, Indices);
	}

	if (!__initMaterials(m_pScene, vFileName))
	{
		return false;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(SVertexBoneData), (const GLvoid*)0);
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);    
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(SVertexBoneData), (const GLvoid*)16);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return (glGetError() == GL_NO_ERROR);
}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::__initMesh(unsigned int vMeshIndex, 
							  const aiMesh* vAiMesh, std::vector<glm::vec3>& voPositions, 
							  std::vector<glm::vec3>& voNormals, 
							  std::vector<glm::vec2>& voTexCoords, 
							  std::vector<SVertexBoneData>& voBones, 
							  std::vector<unsigned int>& voIndices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i=0; i<vAiMesh->mNumVertices; i++)
	{
		const aiVector3D* pPos		= &(vAiMesh->mVertices[i]);
		const aiVector3D* pNormal	= &(vAiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = vAiMesh->HasTextureCoords(0) ? &(vAiMesh->mTextureCoords[0][i]) : &Zero3D;

		voPositions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		voNormals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		voTexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	__loadBones(vMeshIndex, vAiMesh, voBones);

	for (unsigned int i=0; i<vAiMesh->mNumFaces; i++)
	{
		const aiFace& Face = vAiMesh->mFaces[i];
		_ASSERT(Face.mNumIndices == 3);
		voIndices.push_back(Face.mIndices[0]);
		voIndices.push_back(Face.mIndices[1]);
		voIndices.push_back(Face.mIndices[2]);
	}

	_ASSERTE(glGetError() == GL_NO_ERROR);
}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::__loadBones(unsigned int vMeshIndex, const aiMesh* vAiMesh, std::vector<SVertexBoneData>& voBones)
{
	for (unsigned int i=0; i<vAiMesh->mNumBones; i++)
	{
		unsigned int BoneIndex = 0;
		std::string BoneName(vAiMesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end())
		{
			BoneIndex = m_BonesNum;
			m_BonesNum++;
			SBoneInfo BoneInfo;
			m_BoneInfo.push_back(BoneInfo);
			m_BoneInfo[BoneIndex].m_BoneOffset = mat4FromaiMatrix4x4(vAiMesh->mBones[i]->mOffsetMatrix);
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else
		{
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (unsigned int k=0; k<vAiMesh->mBones[i]->mNumWeights; k++)
		{
			unsigned int VertexID = m_Entries[vMeshIndex].m_BaseVertex + vAiMesh->mBones[i]->mWeights[k].mVertexId;
			float Weight = vAiMesh->mBones[i]->mWeights[k].mWeight;
			voBones[VertexID].addBoneData(BoneIndex,Weight);
		}
	}
}

//******************************************************************
//FUNCTION:
bool CSkinnedMesh::__initMaterials(const aiScene* vScnen, const std::string& vFileName)
{
	std::string::size_type SlashIndex = vFileName.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos)
	{
		Dir = ".";
	}
	else if (SlashIndex == 0)
	{
		Dir = "/";
	}
	else
	{
		Dir = vFileName.substr(0, SlashIndex);
	}
	bool Ret = true;

	GLenum Error = glGetError();
	if (Error != GL_NO_ERROR) {                                               
		printf("OpenGL error in %s:%d: 0x%x\n", __FILE__, __LINE__, Error);  
		exit(0);                                                                
	} 
	for (unsigned int i=0; i<vScnen->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = vScnen->mMaterials[i];

		m_Textures[i] = NULL;
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE)>0)
		{
			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string FullPath = Dir+"/"+Path.data;
			//	m_Textures[i]  = new CTexture(FullPath ,GL_TEXTURE_2D);
				m_Textures[i] = new CTexture;
				/*if (!m_Textures[i]->load(FullPath.c_str()))*/
				if (!m_Textures[i]->loadImage(FullPath.c_str(), GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT))
				{
					printf("Error loading texture '%s'\n", FullPath.c_str());
					delete m_Textures[i];
					m_Textures[i] = NULL;
					Ret = false;
				}
				else
				{
					printf("Loaded texture '%s'\n", FullPath.c_str());
				}
			}
			if (!m_Textures[i]) {
			//	m_Textures[i] = new CTexture("white.png" ,GL_TEXTURE_2D);
			//	Ret = m_Textures[i]->load("white.png");
				m_Textures[i] = new CTexture;
				Ret = m_Textures[i]->loadImage("white.png", GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
			}
		}
	}
	_ASSERTE(glGetError() == GL_NO_ERROR);
	return Ret;
}

//******************************************************************
//FUNCTION:
unsigned int CSkinnedMesh::__findPosition(float vAnimationTime, const aiNodeAnim* vNodeAnim)
{
	for (unsigned int i=0; i<vNodeAnim->mNumPositionKeys-1; i++)
	{
		if (vAnimationTime < (float)vNodeAnim->mPositionKeys[i+1].mTime)
		{
			return i;
		}
	}

	_ASSERT(0);

	return 0;
}

//******************************************************************
//FUNCTION:
unsigned int CSkinnedMesh::__findRotation(float vAnimationTime, const aiNodeAnim* vNodeAnim)
{
	_ASSERT(vNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i=0; i<vNodeAnim->mNumRotationKeys-1; i++)
	{
		if (vAnimationTime < (float)vNodeAnim->mRotationKeys[i+1].mTime)
		{
			return i;
		}
	}

	_ASSERT(0);
	return 0;
}

//******************************************************************
//FUNCTION:
unsigned int CSkinnedMesh::__findScaling(float vAnimationTime, const aiNodeAnim* vNodeAnim)
{
	_ASSERT(vNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i=0; i<vNodeAnim->mNumScalingKeys-1; i++)
	{
		if (vAnimationTime < (float)vNodeAnim->mScalingKeys[i+1].mTime)
		{
			return i;
		}
	}

	_ASSERT(0);
	return 0;
}

//******************************************************************
//FUNCTION:
const aiNodeAnim* CSkinnedMesh::__findNodeAnim(const aiAnimation* vAnimation, const std::string& vNodeName)
{
	for (unsigned int i=0; i<vAnimation->mNumChannels; i++)
	{
		const aiNodeAnim* pNodeAnim = vAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == vNodeName)
		{
			return pNodeAnim;
		}
	}

	return NULL;
}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::__calcInterpolatedPosition(aiVector3D& voOut, float vAnimationTime, const aiNodeAnim* vNodeAnim)
{
	if (vNodeAnim->mNumPositionKeys == 1)
	{
		voOut = vNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionFrame = __findPosition(vAnimationTime, vNodeAnim);
	unsigned int NextPositionFrame = (PositionFrame + 1) % vNodeAnim->mNumPositionKeys;
	_ASSERT(NextPositionFrame < vNodeAnim->mNumPositionKeys);

	const aiVectorKey& Key = vNodeAnim->mPositionKeys[PositionFrame];
	const aiVectorKey& NextKey = vNodeAnim->mPositionKeys[NextPositionFrame];
	double DeltaTime = NextKey.mTime - Key.mTime;
	if (DeltaTime<0.0)
	{
		DeltaTime += m_pScene->mAnimations[0]->mDuration;
	}
	if (DeltaTime > 0.0)
	{
		float Factory = (float)(vAnimationTime - Key.mTime) / DeltaTime;
		voOut = Key.mValue + (NextKey.mValue - Key.mValue)*Factory;
	}
	else
	{
		voOut = Key.mValue;
	}

}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::__calcInterpolatedRotation(aiQuaternion& voOut, float vAnimationTime, const aiNodeAnim* vNodeAnim)
{
	if (vNodeAnim->mNumRotationKeys == 1)
	{
		voOut = vNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationFrame = __findRotation(vAnimationTime, vNodeAnim);
	unsigned int NextRotationFrame = (RotationFrame + 1) % vNodeAnim->mNumRotationKeys;
	_ASSERT(NextRotationFrame < vNodeAnim->mNumRotationKeys);
	
	const aiQuatKey&   Key = vNodeAnim->mRotationKeys[RotationFrame];
	const aiQuatKey&  NextKey = vNodeAnim->mRotationKeys[NextRotationFrame];
	double DeltaTime = NextKey.mTime - Key.mTime;
	if (DeltaTime<0.0)
	{
		DeltaTime += m_pScene->mAnimations[0]->mDuration;
	}
	if (DeltaTime > 0.0)
	{
		float Factory = (float)(vAnimationTime - Key.mTime) / DeltaTime;
		aiQuaternion::Interpolate(voOut, Key.mValue, NextKey.mValue, Factory);
	}
	else
	{
		voOut = Key.mValue;
	}
}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::__calcInterpolatedScaling(aiVector3D& voOut, float vAnimationTime, const aiNodeAnim* vNodeAnim)
{
	if (vNodeAnim->mNumScalingKeys == 1)
	{
		voOut = vNodeAnim->mScalingKeys[0].mValue;
		return;
	}

// TODO: (thom) interpolation maybe? This time maybe even logarithmic, not linear (by author)
	unsigned int ScalingFrame = __findScaling(vAnimationTime, vNodeAnim);
//	voOut = vNodeAnim->mScalingKeys[ScalingFrame].mValue;

	unsigned int NextScalingFrame = (ScalingFrame + 1) % vNodeAnim->mNumScalingKeys;
	_ASSERT(NextScalingFrame < vNodeAnim->mNumScalingKeys);

	float DeltaTime = (float)(vNodeAnim->mScalingKeys[NextScalingFrame].mTime - vNodeAnim->mScalingKeys[ScalingFrame].mTime);
	float Factory = (vAnimationTime - (float)vNodeAnim->mScalingKeys[ScalingFrame].mTime) / DeltaTime;
	_ASSERT(Factory >= 0.0f && Factory <= 1.0f);

	const aiVector3D& Start = vNodeAnim->mScalingKeys[ScalingFrame].mValue;
	const aiVector3D& End   = vNodeAnim->mScalingKeys[NextScalingFrame].mValue;
	aiVector3D Delta = End - Start;
	voOut = Start + Factory * Delta;
}

//******************************************************************
//FUNCTION:
void CSkinnedMesh::__readNodeHeirarchy(float vAnimationTime, const aiNode* vNode, const glm::mat4& vParentTransform)
{
	std::string NodeName(vNode->mName.data);
	
	const aiAnimation* pAnimation = m_pScene->mAnimations[0];

	glm::mat4 NodeTransformation = mat4FromaiMatrix4x4(vNode->mTransformation);

	const aiNodeAnim* pNodeAnim = __findNodeAnim(pAnimation, NodeName);

	if (pNodeAnim)
	{
		aiVector3D Scaling;
		__calcInterpolatedScaling(Scaling, vAnimationTime, pNodeAnim);
		glm::mat4 ScalingMat = glm::scale(glm::mat4(1.0), glm::vec3(Scaling.x, Scaling.y, Scaling.z));

		aiQuaternion RotationQ;
		__calcInterpolatedRotation(RotationQ, vAnimationTime, pNodeAnim);
		glm::mat4 RotationMat = mat4FromaiMatrix3x3(RotationQ.GetMatrix());

		aiVector3D Translation;
		__calcInterpolatedPosition(Translation, vAnimationTime, pNodeAnim);
		glm::mat4 TranslationMat = glm::translate(glm::mat4(1.0), glm::vec3(Translation.x, Translation.y, Translation.z));

		NodeTransformation = TranslationMat * RotationMat * ScalingMat;
	}

	glm::mat4 GlobalTransformation = vParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
	{
		unsigned int BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].m_FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].m_BoneOffset;
	}

	for (unsigned int i=0; i<vNode->mNumChildren; i++)
	{
		__readNodeHeirarchy(vAnimationTime, vNode->mChildren[i], GlobalTransformation);
	}
}
