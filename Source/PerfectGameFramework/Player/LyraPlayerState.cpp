// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPlayerState.h"

#include "LyraLogChannels.h"
#include "LyraPlayerController.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/PlayerStateComponent.h"
#include "PerfectGameFramework/AbilitySystem/LyraAbilitySystemComponent.h"
#include "PerfectGameFramework/GameModes/LyraExperienceManagerComponent.h"
#include "PerfectGameFramework/GameModes/LyraGameMode.h"
#include "PerfectGameFramework/Character/LyraPawnData.h"
#include "PerfectGameFramework/Character/LyraPawnExtensionComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(LyraPlayerState)

const FName ALyraPlayerState::NAME_LyraAbilityReady("LyraAbilitiesReady");

ALyraPlayerState::ALyraPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);	
}

void ALyraPlayerState::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ALyraPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());

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

void ALyraPlayerState::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	
	Super::BeginPlay();
}

void ALyraPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	
	Super::EndPlay(EndPlayReason);
}

void ALyraPlayerState::Reset()
{
	Super::Reset();
	
	TArray<UPlayerStateComponent*> ModularComponents;
	GetComponents(ModularComponents);
	for (UPlayerStateComponent* Component : ModularComponents)
	{
		Component->Reset();
	}
}

void ALyraPlayerState::ClientInitialize(AController* C)
{
	Super::ClientInitialize(C);
	
	if (ULyraPawnExtensionComponent* PawnExtComp = ULyraPawnExtensionComponent::FindPawnExtensionComponent(GetPawn()))
	{
		PawnExtComp->CheckDefaultInitialization();
	}
}

ALyraPlayerController* ALyraPlayerState::GetLyraPlayerController() const
{
	return Cast<ALyraPlayerController>(GetOwner());
}

UAbilitySystemComponent* ALyraPlayerState::GetAbilitySystemComponent() const
{
	return GetLyraAbilitySystemComponent();
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
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_LyraAbilityReady);
	
	ForceNetUpdate();
}

void ALyraPlayerState::CopyProperties(APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	TInlineComponentArray<UPlayerStateComponent*> PlayerStateComponents;
	GetComponents(PlayerStateComponents);
	for (UPlayerStateComponent* SourcePSComp : PlayerStateComponents)
	{
		if (UPlayerStateComponent* TargetComp = Cast<UPlayerStateComponent>(static_cast<UObject*>(FindObjectWithOuter(PlayerState, SourcePSComp->GetClass(), SourcePSComp->GetFName()))))
		{
			SourcePSComp->CopyProperties(TargetComp);
		}
	}
}

void ALyraPlayerState::OnDeactivated()
{

	bool bDestroyDeactivatedPlayerState = true;

	// switch (GetPlayerConnectionType())
	// {
	// case ELyraPlayerConnectionType::Player:
	// case ELyraPlayerConnectionType::InactivePlayer:
	// 	//@TODO: Ask the experience if we should destroy disconnecting players immediately or leave them around
	// 	// (e.g., for long running servers where they might build up if lots of players cycle through)
	// 	bDestroyDeactivatedPlayerState = true;
	// 	break;
	// default:
	// 	bDestroyDeactivatedPlayerState = true;
	// 	break;
	// }
	//
	// SetPlayerConnectionType(ELyraPlayerConnectionType::InactivePlayer);

	if (bDestroyDeactivatedPlayerState)
	{
		Destroy();
	}
}

void ALyraPlayerState::OnReactivated()
{
	// if (GetPlayerConnectionType() == ELyraPlayerConnectionType::InactivePlayer)
	// {
	// 	SetPlayerConnectionType(ELyraPlayerConnectionType::Player);
	//
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
