// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "LandscapeSplineProxies.h"
#include "LandscapeSplineSegment.h"

LANDSCAPE_API void HLandscapeSplineProxy_Tangent::Serialize(FArchive& Ar)
{
	Ar << SplineSegment;
}
