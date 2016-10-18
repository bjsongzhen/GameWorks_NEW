#pragma once

// NvFlow begin

struct GridExportParamsNvFlow
{
	FIntVector BlockDim;
	FIntVector BlockDimBits;
	FVector    BlockDimInv;
	FIntVector LinearBlockDim;
	FIntVector LinearBlockOffset;
	FVector    DimInv;
	FVector    VDim;
	FVector    VDimInv;
	FIntVector PoolGridDim;
	FIntVector GridDim;
	bool       IsVTR;
	FMatrix    WorldToVolume;
	float      VelocityScale;

	FShaderResourceViewRHIRef DataSRV;
	FShaderResourceViewRHIRef BlockTableSRV;
};

struct GridAccessHooksNvFlow
{
	virtual uint32 NvFlowQueryGridExportParams(FRHICommandListImmediate& RHICmdList, const FBox& Bounds, uint32 MaxCount, GridExportParamsNvFlow* ResultParamsList) = 0;
};

extern ENGINE_API struct GridAccessHooksNvFlow* GGridAccessNvFlowHooks;

// NvFlow end