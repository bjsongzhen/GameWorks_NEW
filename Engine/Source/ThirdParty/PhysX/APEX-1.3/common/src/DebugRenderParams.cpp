/*
 * Copyright (c) 2008-2015, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */


// This file was generated by NxParameterized/scripts/GenParameterized.pl
// Created: 2015.06.02 04:11:40

#include "DebugRenderParams.h"
#include <string.h>
#include <stdlib.h>

using namespace NxParameterized;

namespace physx
{
namespace apex
{

using namespace DebugRenderParamsNS;

const char* const DebugRenderParamsFactory::vptr =
    NxParameterized::getVptr<DebugRenderParams, DebugRenderParams::ClassAlignment>();

const physx::PxU32 NumParamDefs = 13;
static NxParameterized::DefinitionImpl* ParamDefTable; // now allocated in buildTree [NumParamDefs];


static const size_t ParamLookupChildrenTable[] =
{
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
};

#define TENUM(type) physx::##type
#define CHILDREN(index) &ParamLookupChildrenTable[index]
static const NxParameterized::ParamLookupNode ParamLookupTable[NumParamDefs] =
{
	{ TYPE_STRUCT, false, 0, CHILDREN(0), 11 },
	{ TYPE_BOOL, false, (size_t)(&((ParametersStruct*)0)->Enable), NULL, 0 }, // Enable
	{ TYPE_F32, false, (size_t)(&((ParametersStruct*)0)->Scale), NULL, 0 }, // Scale
	{ TYPE_F32, false, (size_t)(&((ParametersStruct*)0)->LodBenefits), NULL, 0 }, // LodBenefits
	{ TYPE_F32, false, (size_t)(&((ParametersStruct*)0)->RelativeLodBenefitsScreenPos), NULL, 0 }, // RelativeLodBenefitsScreenPos
	{ TYPE_F32, false, (size_t)(&((ParametersStruct*)0)->RelativeLodBenefitsThickness), NULL, 0 }, // RelativeLodBenefitsThickness
	{ TYPE_F32, false, (size_t)(&((ParametersStruct*)0)->LodDistanceScale), NULL, 0 }, // LodDistanceScale
	{ TYPE_F32, false, (size_t)(&((ParametersStruct*)0)->RenderNormals), NULL, 0 }, // RenderNormals
	{ TYPE_F32, false, (size_t)(&((ParametersStruct*)0)->RenderTangents), NULL, 0 }, // RenderTangents
	{ TYPE_F32, false, (size_t)(&((ParametersStruct*)0)->RenderBitangents), NULL, 0 }, // RenderBitangents
	{ TYPE_BOOL, false, (size_t)(&((ParametersStruct*)0)->Bounds), NULL, 0 }, // Bounds
	{ TYPE_ARRAY, true, (size_t)(&((ParametersStruct*)0)->moduleName), CHILDREN(11), 1 }, // moduleName
	{ TYPE_REF, false, 1 * sizeof(NxParameterized::Interface*), NULL, 0 }, // moduleName[]
};


bool DebugRenderParams::mBuiltFlag = false;
NxParameterized::MutexType DebugRenderParams::mBuiltFlagMutex;

DebugRenderParams::DebugRenderParams(NxParameterized::Traits* traits, void* buf, PxI32* refCount) :
	NxParameters(traits, buf, refCount)
{
	//mParameterizedTraits->registerFactory(className(), &DebugRenderParamsFactoryInst);

	if (!buf) //Do not init data if it is inplace-deserialized
	{
		initDynamicArrays();
		initStrings();
		initReferences();
		initDefaults();
	}
}

DebugRenderParams::~DebugRenderParams()
{
	freeStrings();
	freeReferences();
	freeDynamicArrays();
}

void DebugRenderParams::destroy()
{
	// We cache these fields here to avoid overwrite in destructor
	bool doDeallocateSelf = mDoDeallocateSelf;
	NxParameterized::Traits* traits = mParameterizedTraits;
	physx::PxI32* refCount = mRefCount;
	void* buf = mBuffer;

	this->~DebugRenderParams();

	NxParameters::destroy(this, traits, doDeallocateSelf, refCount, buf);
}

const NxParameterized::DefinitionImpl* DebugRenderParams::getParameterDefinitionTree(void)
{
	if (!mBuiltFlag) // Double-checked lock
	{
		NxParameterized::MutexType::ScopedLock lock(mBuiltFlagMutex);
		if (!mBuiltFlag)
		{
			buildTree();
		}
	}

	return(&ParamDefTable[0]);
}

const NxParameterized::DefinitionImpl* DebugRenderParams::getParameterDefinitionTree(void) const
{
	DebugRenderParams* tmpParam = const_cast<DebugRenderParams*>(this);

	if (!mBuiltFlag) // Double-checked lock
	{
		NxParameterized::MutexType::ScopedLock lock(mBuiltFlagMutex);
		if (!mBuiltFlag)
		{
			tmpParam->buildTree();
		}
	}

	return(&ParamDefTable[0]);
}

NxParameterized::ErrorType DebugRenderParams::getParameterHandle(const char* long_name, Handle& handle) const
{
	ErrorType Ret = NxParameters::getParameterHandle(long_name, handle);
	if (Ret != ERROR_NONE)
	{
		return(Ret);
	}

	size_t offset;
	void* ptr;

	getVarPtr(handle, ptr, offset);

	if (ptr == NULL)
	{
		return(ERROR_INDEX_OUT_OF_RANGE);
	}

	return(ERROR_NONE);
}

NxParameterized::ErrorType DebugRenderParams::getParameterHandle(const char* long_name, Handle& handle)
{
	ErrorType Ret = NxParameters::getParameterHandle(long_name, handle);
	if (Ret != ERROR_NONE)
	{
		return(Ret);
	}

	size_t offset;
	void* ptr;

	getVarPtr(handle, ptr, offset);

	if (ptr == NULL)
	{
		return(ERROR_INDEX_OUT_OF_RANGE);
	}

	return(ERROR_NONE);
}

void DebugRenderParams::getVarPtr(const Handle& handle, void*& ptr, size_t& offset) const
{
	ptr = getVarPtrHelper(&ParamLookupTable[0], const_cast<DebugRenderParams::ParametersStruct*>(&parameters()), handle, offset);
}


/* Dynamic Handle Indices */
/* [0] - moduleName (not an array of structs) */

void DebugRenderParams::freeParameterDefinitionTable(NxParameterized::Traits* traits)
{
	if (!traits)
	{
		return;
	}

	if (!mBuiltFlag) // Double-checked lock
	{
		return;
	}

	NxParameterized::MutexType::ScopedLock lock(mBuiltFlagMutex);

	if (!mBuiltFlag)
	{
		return;
	}

	for (physx::PxU32 i = 0; i < NumParamDefs; ++i)
	{
		ParamDefTable[i].~DefinitionImpl();
	}

	traits->free(ParamDefTable);

	mBuiltFlag = false;
}

#define PDEF_PTR(index) (&ParamDefTable[index])

void DebugRenderParams::buildTree(void)
{

	physx::PxU32 allocSize = sizeof(NxParameterized::DefinitionImpl) * NumParamDefs;
	ParamDefTable = (NxParameterized::DefinitionImpl*)(mParameterizedTraits->alloc(allocSize));
	memset(static_cast<void*>(ParamDefTable), 0, allocSize);

	for (physx::PxU32 i = 0; i < NumParamDefs; ++i)
	{
		NX_PARAM_PLACEMENT_NEW(ParamDefTable + i, NxParameterized::DefinitionImpl)(*mParameterizedTraits);
	}

	// Initialize DefinitionImpl node: nodeIndex=0, longName=""
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[0];
		ParamDef->init("", TYPE_STRUCT, "STRUCT", true);






	}

	// Initialize DefinitionImpl node: nodeIndex=1, longName="Enable"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[1];
		ParamDef->init("Enable", TYPE_BOOL, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("shortDescription", "Enable/disable debug rendering", true);
		ParamDefTable[1].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=2, longName="Scale"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[2];
		ParamDef->init("Scale", TYPE_F32, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("shortDescription", "Debug rendering scale", true);
		ParamDefTable[2].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=3, longName="LodBenefits"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[3];
		ParamDef->init("LodBenefits", TYPE_F32, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("shortDescription", "LOD benefit debug visualization", true);
		ParamDefTable[3].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=4, longName="RelativeLodBenefitsScreenPos"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[4];
		ParamDef->init("RelativeLodBenefitsScreenPos", TYPE_F32, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

		static HintImpl HintTable[2];
		static Hint* HintPtrTable[2] = { &HintTable[0], &HintTable[1], };
		HintTable[0].init("max", physx::PxF64(1.0), true);
		HintTable[1].init("min", physx::PxF64(-1.0), true);
		ParamDefTable[4].setHints((const NxParameterized::Hint**)HintPtrTable, 2);

#else

		static HintImpl HintTable[3];
		static Hint* HintPtrTable[3] = { &HintTable[0], &HintTable[1], &HintTable[2], };
		HintTable[0].init("max", physx::PxF64(1.0), true);
		HintTable[1].init("min", physx::PxF64(-1.0), true);
		HintTable[2].init("shortDescription", "The y-axis value of the relative benefits bar (-1.0 - 1.0)", true);
		ParamDefTable[4].setHints((const NxParameterized::Hint**)HintPtrTable, 3);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=5, longName="RelativeLodBenefitsThickness"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[5];
		ParamDef->init("RelativeLodBenefitsThickness", TYPE_F32, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("shortDescription", "The thickness scale of the relative benefits bar", true);
		ParamDefTable[5].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=6, longName="LodDistanceScale"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[6];
		ParamDef->init("LodDistanceScale", TYPE_F32, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("shortDescription", "LOD distance debug visualization", true);
		ParamDefTable[6].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=7, longName="RenderNormals"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[7];
		ParamDef->init("RenderNormals", TYPE_F32, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("shortDescription", "Draws the normals (blue) of the rendered mesh. (scalable)", true);
		ParamDefTable[7].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=8, longName="RenderTangents"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[8];
		ParamDef->init("RenderTangents", TYPE_F32, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("shortDescription", "Draws the tangents (red) of the rendered mesh. (scalable)", true);
		ParamDefTable[8].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=9, longName="RenderBitangents"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[9];
		ParamDef->init("RenderBitangents", TYPE_F32, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("shortDescription", "Draws the bitangents (green) of the rendered mesh. (scalable)", true);
		ParamDefTable[9].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=10, longName="Bounds"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[10];
		ParamDef->init("Bounds", TYPE_BOOL, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

#else

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("shortDescription", "Draws the bounds of every apex actor", true);
		ParamDefTable[10].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */





	}

	// Initialize DefinitionImpl node: nodeIndex=11, longName="moduleName"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[11];
		ParamDef->init("moduleName", TYPE_ARRAY, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		ParamDefTable[11].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#else

		static HintImpl HintTable[2];
		static Hint* HintPtrTable[2] = { &HintTable[0], &HintTable[1], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		HintTable[1].init("shortDescription", "Module name", true);
		ParamDefTable[11].setHints((const NxParameterized::Hint**)HintPtrTable, 2);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */


		static const char* const RefVariantVals[] = { "BasicFSDebugRenderParams", "BasicIosDebugRenderParams", "ParticleIosDebugRenderParams", "ClothingDebugRenderParams", "DestructibleDebugRenderParams", "EmitterDebugRenderParams", "ExplosionDebugRenderParams", "FieldBoundaryDebugRenderParams", "ForceFieldDebugRenderParams", "IofxDebugRenderParams", "FluidIosDebugRenderParams", "TurbulenceFSDebugRenderParams", "ParticlesDebugRenderParams", "DynamicSystemDebugRenderParams" };
		ParamDefTable[11].setRefVariantVals((const char**)RefVariantVals, 14);


		ParamDef->setArraySize(-1);
		static const physx::PxU8 dynHandleIndices[1] = { 0, };
		ParamDef->setDynamicHandleIndicesMap(dynHandleIndices, 1);

	}

	// Initialize DefinitionImpl node: nodeIndex=12, longName="moduleName[]"
	{
		NxParameterized::DefinitionImpl* ParamDef = &ParamDefTable[12];
		ParamDef->init("moduleName", TYPE_REF, NULL, true);

#ifdef NX_PARAMETERIZED_HIDE_DESCRIPTIONS

		static HintImpl HintTable[1];
		static Hint* HintPtrTable[1] = { &HintTable[0], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		ParamDefTable[12].setHints((const NxParameterized::Hint**)HintPtrTable, 1);

#else

		static HintImpl HintTable[2];
		static Hint* HintPtrTable[2] = { &HintTable[0], &HintTable[1], };
		HintTable[0].init("INCLUDED", physx::PxU64(1), true);
		HintTable[1].init("shortDescription", "Module name", true);
		ParamDefTable[12].setHints((const NxParameterized::Hint**)HintPtrTable, 2);

#endif /* NX_PARAMETERIZED_HIDE_DESCRIPTIONS */


		static const char* const RefVariantVals[] = { "BasicFSDebugRenderParams", "BasicIosDebugRenderParams", "ParticleIosDebugRenderParams", "ClothingDebugRenderParams", "DestructibleDebugRenderParams", "EmitterDebugRenderParams", "ExplosionDebugRenderParams", "FieldBoundaryDebugRenderParams", "ForceFieldDebugRenderParams", "IofxDebugRenderParams", "FluidIosDebugRenderParams", "TurbulenceFSDebugRenderParams", "ParticlesDebugRenderParams", "DynamicSystemDebugRenderParams" };
		ParamDefTable[12].setRefVariantVals((const char**)RefVariantVals, 14);



	}

	// SetChildren for: nodeIndex=0, longName=""
	{
		static Definition* Children[11];
		Children[0] = PDEF_PTR(1);
		Children[1] = PDEF_PTR(2);
		Children[2] = PDEF_PTR(3);
		Children[3] = PDEF_PTR(4);
		Children[4] = PDEF_PTR(5);
		Children[5] = PDEF_PTR(6);
		Children[6] = PDEF_PTR(7);
		Children[7] = PDEF_PTR(8);
		Children[8] = PDEF_PTR(9);
		Children[9] = PDEF_PTR(10);
		Children[10] = PDEF_PTR(11);

		ParamDefTable[0].setChildren(Children, 11);
	}

	// SetChildren for: nodeIndex=11, longName="moduleName"
	{
		static Definition* Children[1];
		Children[0] = PDEF_PTR(12);

		ParamDefTable[11].setChildren(Children, 1);
	}

	mBuiltFlag = true;

}
void DebugRenderParams::initStrings(void)
{
}

void DebugRenderParams::initDynamicArrays(void)
{
	moduleName.buf = NULL;
	moduleName.isAllocated = true;
	moduleName.elementSize = sizeof(NxParameterized::Interface*);
	moduleName.arraySizes[0] = 0;
}

void DebugRenderParams::initDefaults(void)
{

	freeStrings();
	freeReferences();
	freeDynamicArrays();
	Enable = bool(false);
	Scale = physx::PxF32(0);
	LodBenefits = physx::PxF32(0);
	RelativeLodBenefitsScreenPos = physx::PxF32(-0.8);
	RelativeLodBenefitsThickness = physx::PxF32(0.6);
	LodDistanceScale = physx::PxF32(0);
	RenderNormals = physx::PxF32(0);
	RenderTangents = physx::PxF32(0);
	RenderBitangents = physx::PxF32(0);
	Bounds = bool(false);

	initDynamicArrays();
	initStrings();
	initReferences();
}

void DebugRenderParams::initReferences(void)
{
}

void DebugRenderParams::freeDynamicArrays(void)
{
	if (moduleName.isAllocated && moduleName.buf)
	{
		mParameterizedTraits->free(moduleName.buf);
	}
}

void DebugRenderParams::freeStrings(void)
{
}

void DebugRenderParams::freeReferences(void)
{

	for (int i = 0; i < moduleName.arraySizes[0]; ++i)
	{
		if (moduleName.buf[i])
		{
			moduleName.buf[i]->destroy();
		}
	}
}

} // namespace apex
} // namespace physx