// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LyraGameInstance.generated.h"

class ALyraPlayerController;
/**
 * 
 */
UCLASS()
class PERFECTGAMEFRAMEWORK_API ULyraGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	ULyraGameInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	ALyraPlayerController* GetPrimaryPlayerController() const;
	

protected:

	virtual void Init() override;
	virtual void Shutdown() override;

};
