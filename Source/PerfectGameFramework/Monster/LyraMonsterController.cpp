

#include "LyraMonsterController.h"

#include "LyraLogChannels.h"
#include "Components/GameFrameworkComponentManager.h"
#include "PerfectGameFramework/AbilitySystem/LyraAbilitySet.h"
#include "PerfectGameFramework/AbilitySystem/LyraAbilitySystemComponent.h"
#include "PerfectGameFramework/AbilitySystem/Attributes/LyraCombatSet.h"
#include "PerfectGameFramework/AbilitySystem/Attributes/LyraHealthSet.h"
#include "PerfectGameFramework/Character/LyraPawnData.h"

// Sets default values
ALyraMonsterController::ALyraMonsterController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<ULyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
    HealthSet = CreateDefaultSubobject<ULyraHealthSet>(TEXT("HealthSet"));
    CombatSet = CreateDefaultSubobject<ULyraCombatSet>(TEXT("CombatSet"));
}

void ALyraMonsterController::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ALyraMonsterController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
}

UAbilitySystemComponent* ALyraMonsterController::GetAbilitySystemComponent() const
{
	return GetLyraAbilitySystemComponent();
}

void ALyraMonsterController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = MyTeamID;

		// MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MyTeamID, this);
		MyTeamID = NewTeamID;
		ConditionalBroadcastTeamChanged(this, OldTeamID, NewTeamID);
	}
	else
	{
		UE_LOG(LogLyraTeams, Error, TEXT("Cannot set team for %s on non-authority"), *GetPathName(this));
	}
}

FGenericTeamId ALyraMonsterController::GetGenericTeamId() const
{
	return MyTeamID;
}

FOnLyraTeamIndexChangedDelegate* ALyraMonsterController::GetOnTeamIndexChangedDelegate()
{
	return &OnTeamChangedDelegate;
}

void ALyraMonsterController::SetPawnData(const ULyraPawnData* InPawnData)
{
	check(InPawnData);

	if (PawnData)
	{
		UE_LOG(LogLyra, Error, TEXT("Trying to set PawnData [%s] on player state [%s] that already has valid PawnData [%s]."), *GetNameSafe(InPawnData), *GetNameSafe(this), *GetNameSafe(PawnData));
		return;
	}
	PawnData = InPawnData;

	for (const ULyraAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
}


