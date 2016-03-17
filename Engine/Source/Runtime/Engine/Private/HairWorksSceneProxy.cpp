// @third party code - BEGIN HairWorks
#include "EnginePrivate.h"
#include <Nv/Foundation/NvMemoryReadStream.h>
#include "AllowWindowsPlatformTypes.h"
#include <Nv/Platform/Dx11/Foundation/NvDx11Handle.h>
#include "HideWindowsPlatformTypes.h"
#include "HairWorksSDK.h"
#include "Engine/HairWorksAsset.h"
#include "HairWorksSceneProxy.h"

// Debug render console variables.
#define HairVisualizationCVarDefine(Name)	\
	static TAutoConsoleVariable<int> CVarHairVisualization##Name(TEXT("r.HairWorks.Visualization.") TEXT(#Name), 0, TEXT(""), ECVF_RenderThreadSafe)

static TAutoConsoleVariable<int> CVarHairVisualizationHair(TEXT("r.HairWorks.Visualization.")TEXT("Hair"), 1, TEXT(""), ECVF_RenderThreadSafe);
HairVisualizationCVarDefine(GuideCurves);
HairVisualizationCVarDefine(SkinnedGuideCurves);
HairVisualizationCVarDefine(ControlPoints);
HairVisualizationCVarDefine(GrowthMesh);
HairVisualizationCVarDefine(Bones);
HairVisualizationCVarDefine(BoundingBox);
HairVisualizationCVarDefine(CollisionCapsules);
HairVisualizationCVarDefine(HairInteraction);
HairVisualizationCVarDefine(PinConstraints);
HairVisualizationCVarDefine(ShadingNormal);
HairVisualizationCVarDefine(ShadingNormalCenter);

#undef HairVisualizationCVarDefine

FHairWorksSceneProxy::FHairWorksSceneProxy(const UPrimitiveComponent* InComponent, UHairWorksAsset& InHair, NvHw::HairInstanceId InHairInstanceId) :
	FPrimitiveSceneProxy(InComponent),
	HairInstanceId(InHairInstanceId)
{
	HairTextures.SetNumZeroed(NvHw::EHairTextureType::COUNT_OF);
}

FHairWorksSceneProxy::~FHairWorksSceneProxy()
{
}

uint32 FHairWorksSceneProxy::GetMemoryFootprint(void) const
{
	return 0;
}

void FHairWorksSceneProxy::Draw(EDrawType DrawType)const
{
	if(HairInstanceId == NvHw::HAIR_INSTANCE_ID_NULL)
		return;

	if (DrawType == EDrawType::Visualization)
	{
		HairWorks::GetSDK()->renderVisualization(HairInstanceId);
	}
	else
	{
		// Special for shadow
		NvHw::HairInstanceDescriptor HairDesc;
		HairWorks::GetSDK()->getInstanceDescriptor(HairInstanceId, HairDesc);

		if (DrawType == EDrawType::Shadow)
		{
			HairDesc.m_useBackfaceCulling = false;

			HairWorks::GetSDK()->updateInstanceDescriptor(HairInstanceId, HairDesc);
		}

		// Handle shader cache.
		NvHw::ShaderCacheSettings ShaderCacheSetting;
		ShaderCacheSetting.setFromInstanceDescriptor(HairDesc);
		check(HairTextures.Num() == NvHw::EHairTextureType::COUNT_OF);
		for(int i = 0; i < NvHw::EHairTextureType::COUNT_OF; i++)
		{
			ShaderCacheSetting.setTextureUsed(i, HairTextures[i] != nullptr);
		}

		HairWorks::GetSDK()->addToShaderCache(ShaderCacheSetting);

		// Draw
		NvHw::ShaderSettings HairShaderSettings;
		HairShaderSettings.m_useCustomConstantBuffer = true;
		HairShaderSettings.m_shadowPass = (DrawType == EDrawType::Shadow);

		HairWorks::GetSDK()->renderHairs(HairInstanceId, &HairShaderSettings);
	}
}

FPrimitiveViewRelevance FHairWorksSceneProxy::GetViewRelevance(const FSceneView* View)const
{
	FPrimitiveViewRelevance ViewRel;
	ViewRel.bDrawRelevance = IsShown(View);
	ViewRel.bShadowRelevance = IsShadowCast(View);
	ViewRel.bDynamicRelevance = true;
	ViewRel.bRenderInMainPass = false;	// Hair is rendered in a special path.

	ViewRel.bHairWorks = View->Family->EngineShowFlags.HairWorks;

	return ViewRel;
}

void FHairWorksSceneProxy::UpdateDynamicData_RenderThread(const FDynamicRenderData & DynamicData)
{
	if (HairInstanceId == NvHw::HAIR_INSTANCE_ID_NULL)
		return;

	// Update normal center bone
	auto HairDesc = DynamicData.HairInstanceDesc;

	// Merge global visualization flags.
#define HairVisualizationCVarUpdate(CVarName, MemberVarName)	\
	HairDesc.m_visualize##MemberVarName |= CVarHairVisualization##CVarName.GetValueOnRenderThread() != 0

	HairDesc.m_drawRenderHairs &= CVarHairVisualizationHair.GetValueOnRenderThread() != 0;
	HairVisualizationCVarUpdate(GuideCurves, GuideHairs);
	HairVisualizationCVarUpdate(SkinnedGuideCurves, SkinnedGuideHairs);
	HairVisualizationCVarUpdate(ControlPoints, ControlVertices);
	HairVisualizationCVarUpdate(GrowthMesh, GrowthMesh);
	HairVisualizationCVarUpdate(Bones, Bones);
	HairVisualizationCVarUpdate(BoundingBox, BoundingBox);
	HairVisualizationCVarUpdate(CollisionCapsules, Capsules);
	HairVisualizationCVarUpdate(HairInteraction, HairInteractions);
	HairVisualizationCVarUpdate(PinConstraints, PinConstraints);
	HairVisualizationCVarUpdate(ShadingNormal, ShadingNormals);
	HairVisualizationCVarUpdate(ShadingNormalCenter, ShadingNormalBone);

#undef HairVisualizerCVarUpdate

	// Other
	HairDesc.m_modelToWorld = (gfsdk_float4x4&)GetLocalToWorld().M;
	HairDesc.m_useViewfrustrumCulling = false;

	// Set parameters to HairWorks
	HairWorks::GetSDK()->updateInstanceDescriptor(HairInstanceId, HairDesc);	// Mainly for simulation.

	// Update textures
	check(DynamicData.Textures.Num() == NvHw::EHairTextureType::COUNT_OF);
	HairTextures.SetNumZeroed(NvHw::EHairTextureType::COUNT_OF);
	for(auto Idx = 0; Idx < HairTextures.Num(); ++Idx)
	{
		auto* Texture = DynamicData.Textures[Idx];
		if(Texture == nullptr)
			continue;

		if(Texture->Resource == nullptr)
			continue;

		HairTextures[Idx] = static_cast<FTexture2DResource*>(Texture->Resource)->GetTexture2DRHI();
	}

	for (auto Idx = 0; Idx < NvHw::EHairTextureType::COUNT_OF; ++Idx)
	{
		auto TextureRef = HairTextures[Idx];
		HairWorks::GetSDK()->setTexture(HairInstanceId, (NvHw::HairTextureType::Enum)Idx, Nv::Dx11Type::getHandle(HairWorks::GetD3DHelper().GetShaderResourceView(TextureRef.GetReference())));
	}
}
// @third party code - END HairWorks
