// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ModularPlayerState.h"
#include "LyraPlayerState.generated.h"

class ULyraAbilitySystemComponent;
class UAbilitySystemComponent;
class ALyraPlayerController;
class ULyraExperienceDefinition;
class ULyraPawnData;
/**
 * 
 */
UCLASS()
class PERFECTGAMEFRAMEWORK_API ALyraPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ALyraPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerState")
	ALyraPlayerController* GetLyraPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "Lyra|PlayerState")
	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const ULyraPawnData* InPawnData);
	
	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;
	//~End of AActor interface

	//~APlayerState interface
	virtual void ClientInitialize(AController* C) override;
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void OnDeactivated() override;
	virtual void OnReactivated() override;
	//~End of APlayerState interface
	
	static const FName NAME_LyraAbilityReady;
	
private:
	void OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience);

protected:
	UFUNCTION()
	void OnRep_PawnData();
	
protected:

	
	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const ULyraPawnData> PawnData;


private:

	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Lyra|PlayerState")
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;
};
