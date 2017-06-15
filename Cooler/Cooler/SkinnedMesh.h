#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>

namespace Cooler
{
	class CTexture;

	class CSkinnedMesh
	{
	protected:
#define INVALID_MATERIAL 0xFFFFFFFF
#define INVALID_VALUE	 0xffffffff
#define NUM_BONES_PER_VERTEX 4

		struct SBoneInfo
		{
			glm::mat4 m_BoneOffset;
			glm::mat4 m_FinalTransformation;

			SBoneInfo()
			{
				m_BoneOffset = glm::mat4(0.0);
				m_FinalTransformation = glm::mat4(0.0);
			}

		};

		struct SVertexBoneData
		{
			unsigned int m_IDs[NUM_BONES_PER_VERTEX];
			float m_Weight[NUM_BONES_PER_VERTEX];

			SVertexBoneData()
			{
				reset();
			}

			void reset()
			{
				memset(m_IDs, 0, sizeof(m_IDs));
				memset(m_Weight, 0, sizeof(m_Weight));
			}

			void addBoneData(unsigned int vBoneID, float vWeight);
		};

		enum EVBType
		{
			INDEX_BUFFER,
			POS_VB,
			NORMAL_VB,
			TEXCOORD_VB,
			BONE_VB,
			NUM_VBs
		};

		struct SMeshEntry
		{
			unsigned int m_NumIndices;
			unsigned int m_BaseVertex;
			unsigned int m_BaseIndex;
			unsigned int m_MaterialIndex;

			SMeshEntry()
			{
				m_NumIndices = 0;
				m_BaseVertex = 0;
				m_BaseIndex = 0;
				m_MaterialIndex = INVALID_MATERIAL;
			}
		};

	public:
		CSkinnedMesh(void);
		~CSkinnedMesh(void);

		bool loadMesh(const std::string& vFileName);
		void render();

		unsigned int getBonesNumber() const {};

		void boneTransform(float vTimeInSeconds, std::vector<glm::mat4>& voTransform);

	private:
		GLuint m_VAO;
		GLuint m_Buffers[NUM_VBs];

		std::vector<SMeshEntry> m_Entries;
		std::vector<CTexture*> m_Textures;

		std::map<std::string, unsigned int> m_BoneMapping;
		unsigned int m_BonesNum;
		std::vector<SBoneInfo> m_BoneInfo;
		glm::mat4 m_GlobalInverseTransform;

		const aiScene* m_pScene;
		Assimp::Importer m_Importer;

		void __calcInterpolatedScaling(aiVector3D& voOut, float vAnimationTime, const aiNodeAnim* vNodeAnim);
		void __calcInterpolatedRotation(aiQuaternion& voOut, float vAnimationTime, const aiNodeAnim* vNodeAnim);
		void __calcInterpolatedPosition(aiVector3D& voOut, float vAnimationTime, const aiNodeAnim* vNodeAnim);
		unsigned int __findScaling(float vAnimationTime, const aiNodeAnim* vNodeAnim);
		unsigned int __findRotation(float vAnimationTime, const aiNodeAnim* vNodeAnim);
		unsigned int __findPosition(float vAnimationTime, const aiNodeAnim* vNodeAnim);
		const aiNodeAnim* __findNodeAnim(const aiAnimation* vAnimation, const std::string& vNodeName);
		void __readNodeHeirarchy(float vAnimationTime, const aiNode* vNode, const glm::mat4& vParentTransform);
		bool __initFromScene(const aiScene* vScene, const std::string& vFileName);
		void __initMesh(unsigned int vMeshIndex,
			const aiMesh* vAiMesh,
			std::vector<glm::vec3>& voPositions,
			std::vector<glm::vec3>& voNormals,
			std::vector<glm::vec2>& voTexCoords,
			std::vector<SVertexBoneData>& voBones,
			std::vector<unsigned int>& voIndices);
		void __loadBones(unsigned int vMeshIndex, const aiMesh* vAiMesh, std::vector<SVertexBoneData>& voBones);
		bool __initMaterials(const aiScene* vScnen, const std::string& vFileName);
		void __clear();
	};

}
