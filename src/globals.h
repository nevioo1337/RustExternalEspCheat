#pragma once
#include <emmintrin.h>

struct Vector3 {
	float x, y, z;
};

struct Vector4 {
	float x, y, z, w;
};

enum BoneList : int {
	head = 47,
	neck = 46,
	l_upperarm = 24,
	r_upperarm = 55,
	l_forearm = 25,
	r_forearm = 56,
	l_hand = 26,
	r_hand = 57,
	spine4 = 22,
	pelvis = 19,
	l_hip = 1,
	r_hip = 13,
	l_knee = 2,
	r_knee = 14,
	l_foot = 3,
	r_foot = 15,
};

struct BonesStruct {
	Vector3 head;
	Vector3 neck;
	Vector3 l_upperarm;
	Vector3 r_upperarm;
	Vector3 l_forearm;
	Vector3 r_forearm;
	Vector3 l_hand;
	Vector3 r_hand;
	Vector3 spine4;
	Vector3 pelvis;
	Vector3 l_hip;
	Vector3 r_hip;
	Vector3 l_knee;
	Vector3 r_knee;
	Vector3 l_foot;
	Vector3 r_foot;
};

float Dot(const Vector3& Vec1, const Vector3& Vec2) {
	return Vec1.x * Vec2.x + Vec1.y * Vec2.y + Vec1.z * Vec2.z;
}

struct Matrix4x4 {
	float m00; // 1
	float m10; // 2
	float m20; // 3
	float m30; // 4

	float m01; // 5
	float m11; // 6
	float m21; // 7
	float m31; // 8

	float m02; // 9
	float m12; // 10
	float m22; // 11
	float m32; // 12

	float m03; // 13
	float m13; // 14
	float m23; // 15
	float m33; // 16
};

bool WorldToScreen(Matrix4x4 viewMatrix, const Vector3 entityPos, Vector3& screenPos) {
	Vector3 TransVec = { viewMatrix.m30, viewMatrix.m31, viewMatrix.m32 };
	Vector3 RightVec = { viewMatrix.m00, viewMatrix.m01, viewMatrix.m02 };
	Vector3 UpVec = { viewMatrix.m10, viewMatrix.m11, viewMatrix.m12 };
	float w = Dot(TransVec, entityPos) + viewMatrix.m33;
	if (w < 0.1f) return false;
	float y = Dot(UpVec, entityPos) + viewMatrix.m13;
	float x = Dot(RightVec, entityPos) + viewMatrix.m03;
	screenPos = { (1920 / 2) * (1.f + x / w), (1080 / 2) * (1.f - y / w) };

	return true;
}

struct TransformAccessReadOnly {
	uintptr_t	pTransformData;
	int			pIndex;
};

struct TransformData {
	uintptr_t pTransformArray;
	uintptr_t pTransformIndices;
};

struct Matrix3x4 {
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
};

void read_array(uintptr_t address, void* buffer, size_t size)
{
	SIZE_T bytesRead;
	ReadProcessMemory(ProcUtils::hProc, (BYTE*)address, buffer, size, &bytesRead);
}

static Vector3 GetTransformPosition(uintptr_t transform)
{
	uintptr_t transform_internal = MemUtils::RPM<uintptr_t>(transform + 0x10);

	if (!transform_internal)
		return Vector3();
	try
	{
		__m128 result;

		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		TransformAccessReadOnly pTransformAccessReadOnly = MemUtils::RPM<TransformAccessReadOnly>(transform_internal + 0x38);
		TransformData transformData = MemUtils::RPM<TransformData>(pTransformAccessReadOnly.pTransformData + 0x18);

		if (!transformData.pTransformArray)
			return Vector3();

		if (!transformData.pTransformIndices)
			return Vector3();

		size_t sizeMatriciesBuf = sizeof(Matrix3x4) * pTransformAccessReadOnly.pIndex + sizeof(Matrix3x4);
		size_t sizeIndicesBuf = sizeof(int) * pTransformAccessReadOnly.pIndex + sizeof(int);

		void* pMatriciesBuf = malloc(sizeMatriciesBuf);
		void* pIndicesBuf = malloc(sizeIndicesBuf);

		if (pMatriciesBuf && pIndicesBuf)
		{
			if (transformData.pTransformArray == 0 || transformData.pTransformIndices == 0)
				return Vector3();

			// Read Matricies array into the buffer
			read_array(transformData.pTransformArray, pMatriciesBuf, sizeMatriciesBuf);
			// Read Indices array into the buffer
			read_array(transformData.pTransformIndices, pIndicesBuf, sizeIndicesBuf);

			__m128 Result = *reinterpret_cast<__m128*>(reinterpret_cast<uint64_t>(pMatriciesBuf) + (0x30 * pTransformAccessReadOnly.pIndex));
			int TransformIndex = *reinterpret_cast<int*>((reinterpret_cast<uint64_t>(pIndicesBuf) + (0x4 * pTransformAccessReadOnly.pIndex)));

			while (TransformIndex >= 0 && TransformIndex < (sizeMatriciesBuf / 0x30))
			{
				if (!pMatriciesBuf || !pIndicesBuf || !&result || !&TransformIndex)
					return Vector3();

				Matrix3x4 Matrix = *reinterpret_cast<Matrix3x4*>(reinterpret_cast<uint64_t>(pMatriciesBuf) + (0x30 * TransformIndex));

				if (!&Matrix)
					return Vector3();

				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x00));	// xxxx
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x55));	// yyyy
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x8E));	// zwxy
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0xDB));	// wzyw
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0xAA));	// zzzz
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*reinterpret_cast<__m128i*>(&Matrix._21), 0x71));	// yxwy
				__m128 tmp7 = _mm_mul_ps(*reinterpret_cast<__m128*>(&Matrix._31), Result);

				Result = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
							tmp7)), *(__m128*)(&Matrix._11));

				TransformIndex = *reinterpret_cast<int*>(reinterpret_cast<uint64_t>(pIndicesBuf) + (0x4 * TransformIndex));
			}

			free(pMatriciesBuf);
			free(pIndicesBuf);

			return Vector3{ Result.m128_f32[0], Result.m128_f32[1], Result.m128_f32[2] };
		}

		return {};
	}
	catch (...) {
		return Vector3();
	}
}

static Vector3 GetBoneLocation(uintptr_t player, int boneIndex) {
	uintptr_t playerModel = MemUtils::RPM<uintptr_t>(player + 0xB8);
	uintptr_t boneTransforms = MemUtils::RPM<uintptr_t>(playerModel + 0x48);
	uintptr_t bone = MemUtils::RPM<uintptr_t>((boneTransforms + (0x20 + (boneIndex * 0x8))));

	return GetTransformPosition(bone);
}

namespace globals {
	namespace offsets {
		namespace GameAssembly {
			uintptr_t baseAddress;

			uintptr_t baseNetworkablePtr = 0x33AB5B8;
			namespace BaseNetworkable {
				uintptr_t staticFieldPtr = 0xb8;
				uintptr_t clientEntitiesPtr = 0x0;
				uintptr_t entityListPtr = 0x10;
				uintptr_t entityListValsPtr = 0x28;
				uintptr_t entityListValsCountPtr = 0x10;
				uintptr_t entityListValsBufferPtr = 0x18;

				std::vector<uintptr_t> GetEntList() {
					int entCount = MemUtils::RPMChain<int>(MemUtils::RPM<uintptr_t>(baseAddress + baseNetworkablePtr), { staticFieldPtr, clientEntitiesPtr, entityListPtr, entityListValsPtr, entityListValsCountPtr });
					uintptr_t dereferncedEntList = MemUtils::RPMChain<uintptr_t>(MemUtils::RPM<uintptr_t>(baseAddress + baseNetworkablePtr), { staticFieldPtr, clientEntitiesPtr, entityListPtr, entityListValsPtr, entityListValsBufferPtr });

					std::vector<uintptr_t> entList;
					for (size_t i = 0; i < entCount; i++) {
						uintptr_t ent = MemUtils::RPM<uintptr_t>(dereferncedEntList + (0x20 + (i * 0x8)));
						if (ent != NULL) {
							entList.push_back(ent);
						}
					}
					return entList;
				}
			}

			uintptr_t entTypePtr = 0x0; // still have to understand why and how
			uintptr_t entType1Ptr = 0x10; // still have to understand why and how

			namespace BaseEntity {

			}

			namespace BasePlayer {
				uintptr_t playerModelPtr = 0x680;
				namespace PlayerModel {
					uintptr_t positionPtr = 0x1B8;

					Vector3 GetPosition(uintptr_t entity) {
						uintptr_t playerModel = MemUtils::RPM<uintptr_t>(entity + playerModelPtr);
						return MemUtils::RPM<Vector3>(playerModel + positionPtr);
					}
				}
			}
		}

		namespace UnityPlayer {
			uintptr_t baseAddress;

			uintptr_t cameraManagerPtr = 0x1A77540;
			namespace CameraManager {
				uintptr_t localPlayerPtr = 0x0;
				uintptr_t firstCamPtr = 0x0;
				uintptr_t cameraObjPtr = 0x30;
				uintptr_t cameraObjClassPtr = 0x30;
				uintptr_t entityPtr = 0x18;
				uintptr_t projectionMatrixPtr = 0x2E4;

				Matrix4x4 GetViewMatrix() {
					return MemUtils::RPM<Matrix4x4>(MemUtils::RPMChain<uintptr_t>(MemUtils::RPM<uintptr_t>(baseAddress + cameraManagerPtr), { localPlayerPtr, firstCamPtr, cameraObjPtr, cameraObjClassPtr, entityPtr }) + projectionMatrixPtr);
				}
			}
		}
	}

	namespace gui {
		ImDrawList* drawList;
		bool bShowMenu = true;

		namespace cheats {
			bool bTracer = false;
			bool bBoneEsp = false;
		}
	}

	namespace cheatInfo {
		Matrix4x4 viewMatrix;

		std::vector<uintptr_t> entList;
		std::vector<std::string> entNames;

		std::vector<Vector3> playerWorldPositions;
		std::vector<Vector3> playerScreenPositions;

		std::vector<BonesStruct> bonesWorldPositions;
		std::vector<BonesStruct> bonesScreenPositions;
	}
}