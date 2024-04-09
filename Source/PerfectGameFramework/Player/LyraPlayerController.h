// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LyraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PERFECTGAMEFRAMEWORK_API ALyraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ALyraPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerController")
	ALyraPlayerState* GetLyraPlayerState() const;

	//~AActor interface
	virtual void BeginPlay() override;
	//~End of AActor interface


};
