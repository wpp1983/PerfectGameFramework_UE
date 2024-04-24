// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

#include "Tasks/TargetingFilterTask_BasicFilterTemplate.h"
#include "Types/TargetingSystemTypes.h"

#include "TargetingFilterTask_Team.generated.h"


/**
*	@class UTargetingFilterTask_Team
*

*/
UCLASS(Blueprintable)
class UTargetingFilterTask_Team : public UTargetingFilterTask_BasicFilterTemplate
{
	GENERATED_BODY()

public:
	UTargetingFilterTask_Team(const FObjectInitializer& ObjectInitializer);

	/** Evaluation function called by derived classes to process the targeting request */
	virtual bool ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const override;

protected:
	int32 GetSourceActorTeamId(const FTargetingRequestHandle& TargetingHandle) const;

	UPROPERTY(EditAnywhere, Category = "Targeting Filter | Team", Meta = (AllowAbstract=true))
	bool bNeedEnemyTeam = true;

};

