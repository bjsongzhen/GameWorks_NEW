// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "GameplayAbility.h"
#include "GameplayAbilityTargetActor.generated.h"

UCLASS(Blueprintable, abstract)
class GAMEPLAYABILITIES_API AGameplayAbilityTargetActor : public AActor
{
	GENERATED_UCLASS_BODY()

public:

	/** Native classes can set this to call the non-blueprintable  */
	UPROPERTY()
	bool	StaticTargetFunction;

	virtual FGameplayAbilityTargetDataHandle StaticGetTargetData(UWorld * World, const FGameplayAbilityActorInfo* ActorInfo, FGameplayAbilityActivationInfo ActivationInfo) { return FGameplayAbilityTargetDataHandle(); }

	/** Otherwise, the actor will be instantiated and call the blueprintable function GetTargetData */
	UFUNCTION(BlueprintImplementableEvent, Category = "Targeting")
	FGameplayAbilityTargetDataHandle StaticGetTargetData(UWorld * World, FGameplayAbilityActorInfo ActorInfo, FGameplayAbilityActivationInfo ActivationInfo);

	
	virtual void StartTargeting(UGameplayAbility* Ability);

	/** Outside code is saying 'stop and just give me what you have' */
	virtual void ConfirmTargeting();

	/** Outside code is saying 'stop everything and just forget about it' */
	virtual void CancelTargeting();

	// ------------------------------
	
	FAbilityTargetData	TargetDataReadyDelegate;
	FAbilityTargetData	CanceledDelegate;

	virtual bool IsNetRelevantFor(class APlayerController* RealViewer, AActor* Viewer, const FVector& SrcLocation);

	UPROPERTY(BlueprintReadOnly, Category = Ability)
	TWeakObjectPtr<APlayerController> MasterPC;
};