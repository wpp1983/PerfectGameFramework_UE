// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraGameState.h"

#include "LyraExperienceManagerComponent.h"
#include "GameFramework/PlayerState.h"

ALyraGameState::ALyraGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	ExperienceManagerComponent = CreateDefaultSubobject<ULyraExperienceManagerComponent>(TEXT("ExperienceManagerComponent"));

}

void ALyraGameState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ALyraGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ALyraGameState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ALyraGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ALyraGameState::AddPlayerState(APlayerState* PlayerState)
{
	Super::AddPlayerState(PlayerState);
}

void ALyraGameState::RemovePlayerState(APlayerState* PlayerState)
{
	Super::RemovePlayerState(PlayerState);
}

void ALyraGameState::SeamlessTravelTransitionCheckpoint(bool bToTransitionMap)
{
	Super::SeamlessTravelTransitionCheckpoint(bToTransitionMap);

	// Remove inactive and bots
	for (int32 i = PlayerArray.Num() - 1; i >= 0; i--)
	{
		APlayerState* PlayerState = PlayerArray[i];
		if (PlayerState && (PlayerState->IsABot() || PlayerState->IsInactive()))
		{
			RemovePlayerState(PlayerState);
		}
	}
}
