﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "PerfectGameFramework/GameFeatures/GameFeatureAction_AddInputContextMapping.h" 

#include "LyraHeroComponent.generated.h"


struct FInputActionValue;
class ULyraInputConfig;
class ULyraCameraMode;
/**
 * Component that sets up input and camera handling for player controlled pawns (or bots that simulate players).
 * This depends on a PawnExtensionComponent to coordinate initialization.
 */
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class PERFECTGAMEFRAMEWORK_API ULyraHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:

	ULyraHeroComponent(const FObjectInitializer& ObjectInitializer);

	/** Returns the hero component if one exists on the specified actor. */
	UFUNCTION(BlueprintPure, Category = "Lyra|Hero")
	static ULyraHeroComponent* FindHeroComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<ULyraHeroComponent>() : nullptr); }

	/** Overrides the camera from an active gameplay ability */
	void SetAbilityCameraMode(TSubclassOf<ULyraCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** Clears the camera override if it is set */
	void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);

	/** Adds mode-specific input config */
	void AddAdditionalInputConfig(const ULyraInputConfig* InputConfig);

	/** Removes a mode-specific input config if it has been added */
	void RemoveAdditionalInputConfig(const ULyraInputConfig* InputConfig);

	/** True if this is controlled by a real player and has progressed far enough in initialization where additional input bindings can be added */
	bool IsReadyToBindInputs() const;
	
	/** The name of the extension event sent via UGameFrameworkComponentManager when ability inputs are ready to bind */
	static const FName NAME_BindInputsNow;

	/** The name of this component-implemented feature */
	static const FName NAME_ActorFeatureName;

	//~ Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; }
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//~ End IGameFrameworkInitStateInterface interface

	
protected:
    
    virtual void OnRegister() override;
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    virtual void InitializePlayerInput(UInputComponent* PlayerInputComponent);
    
    void Input_AbilityInputTagPressed(FGameplayTag InputTag);
    void Input_AbilityInputTagReleased(FGameplayTag InputTag);
    
    void Input_Move(const FInputActionValue& InputActionValue);
    void Input_LookMouse(const FInputActionValue& InputActionValue);
    void Input_LookStick(const FInputActionValue& InputActionValue);
    void Input_Crouch(const FInputActionValue& InputActionValue);
    void Input_AutoRun(const FInputActionValue& InputActionValue);
    
    TSubclassOf<ULyraCameraMode> DetermineCameraMode() const;
    	
    // void OnInputConfigActivated(const FLoadedMappableConfigPair& ConfigPair);
    // void OnInputConfigDeactivated(const FLoadedMappableConfigPair& ConfigPair);

protected:
	
    UPROPERTY(EditAnywhere)
    TArray<FInputMappingContextAndPriority> DefaultInputMappings;
    	
    /** Camera mode set by an ability. */
    UPROPERTY()
    TSubclassOf<ULyraCameraMode> AbilityCameraMode;
    
    /** Spec handle for the last ability to set a camera mode. */
    FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;
    
    /** True when player input bindings have been applied, will never be true for non - players */
    bool bReadyToBindInputs;
};
