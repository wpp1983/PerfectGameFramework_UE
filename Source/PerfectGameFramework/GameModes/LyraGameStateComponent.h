// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"
#include "LyraGameStateComponent.generated.h"


class ULyraExperienceDefinition;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PERFECTGAMEFRAMEWORK_API ULyraGameStateComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	ULyraGameStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	//~End of UActorComponent interface

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "LyraGameStateComponent", meta = (DisplayName = "On Experience Loaded"))
	void OnExperienceLoaded(const ULyraExperienceDefinition* Experience);

	virtual void OnExperienceLoaded_Implementation(const ULyraExperienceDefinition* Experience);
};
