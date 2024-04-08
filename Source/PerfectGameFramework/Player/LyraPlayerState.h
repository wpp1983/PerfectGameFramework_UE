// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LyraPlayerState.generated.h"

class ULyraExperienceDefinition;
class ULyraPawnData;
/**
 * 
 */
UCLASS()
class PERFECTGAMEFRAMEWORK_API ALyraPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALyraPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface
	
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const ULyraPawnData* InPawnData);

private:
	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);

protected:
	UFUNCTION()
	void OnRep_PawnData();
	
protected:

	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const ULyraPawnData> PawnData;
};
