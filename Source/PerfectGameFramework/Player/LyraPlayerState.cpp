// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPlayerState.h"

#include "LyraLogChannels.h"
#include "PerfectGameFramework/GameModes/LyraExperienceManagerComponent.h"
#include "PerfectGameFramework/GameModes/LyraGameMode.h"
#include "PerfectGameFramework/Character/LyraPawnData.h"

ALyraPlayerState::ALyraPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void ALyraPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ALyraPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// check(AbilitySystemComponent);
	// AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

	UWorld* World = GetWorld();
	if (World && World->IsGameWorld() && World->GetNetMode() != NM_Client)
	{
		AGameStateBase* GameState = GetWorld()->GetGameState();
		check(GameState);
		ULyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<ULyraExperienceManagerComponent>();
		check(ExperienceComponent);
		ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnLyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	}
}

void ALyraPlayerState::SetPawnData(const ULyraPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		UE_LOG(LogLyra, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}

	// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PawnData, this);
	PawnData = InPawnData;

	// for (const ULyraAbilitySet* AbilitySet : PawnData->AbilitySets)
	// {
	// 	if (AbilitySet)
	// 	{
	// 		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
	// 	}
	// }
	//
	// UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_LyraAbilityReady);
	
	ForceNetUpdate();
}

void ALyraPlayerState::OnExperienceLoaded(const ULyraExperienceDefinition* CurrentExperience)
{
	if (ALyraGameMode* LyraGameMode = GetWorld()->GetAuthGameMode<ALyraGameMode>())
	{
		if (const ULyraPawnData* NewPawnData = LyraGameMode->GetPawnDataForController(GetOwningController()))
		{
			SetPawnData(NewPawnData);
		}
		else
		{
			UE_LOG(LogLyra, Error, TEXT("ALyraPlayerState::OnExperienceLoaded(): Unable to find PawnData to initialize player state [%s]!"), *GetNameSafe(this));
		}
	}
}

void ALyraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// FDoRepLifetimeParams SharedParams;
	// SharedParams.bIsPushBased = true;
	//
	// DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PawnData, SharedParams);
	// DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyPlayerConnectionType, SharedParams)
	// DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MyTeamID, SharedParams);
	// DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MySquadID, SharedParams);
	//
	// SharedParams.Condition = ELifetimeCondition::COND_SkipOwner;
	// DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, ReplicatedViewRotation, SharedParams);
	//
	// DOREPLIFETIME(ThisClass, StatTags);	
}


void ALyraPlayerState::OnRep_PawnData()
{
}
