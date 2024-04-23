#pragma once

#include "CoreMinimal.h"
#include "ModularAIController.h"
#include "PerfectGameFramework/Teams/LyraTeamAgentInterface.h"
#include "LyraMonsterController.generated.h"

class ULyraPawnData;
class ULyraAbilitySet;
class UAbilitySystemComponent;
class ULyraAbilitySystemComponent;

UCLASS()
class PERFECTGAMEFRAMEWORK_API ALyraMonsterController : public AModularAIController, public ILyraTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALyraMonsterController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~AActor interface
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	//~End of AActor interface

	
	UFUNCTION(BlueprintCallable, Category = "Lyra|MonsterController")
	ULyraAbilitySystemComponent* GetLyraAbilitySystemComponent() const { return AbilitySystemComponent; }

	//~ILyraTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual FOnLyraTeamIndexChangedDelegate* GetOnTeamIndexChangedDelegate() override;
	//~End of ILyraTeamAgentInterface interface


	void SetPawnData(const ULyraPawnData* InPawnData);
	
	// The ability system component sub-object used by player characters.
	UPROPERTY(VisibleAnywhere, Category = "Lyra|MonsterController")
	TObjectPtr<ULyraAbilitySystemComponent> AbilitySystemComponent;
	
	// Health attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class ULyraHealthSet> HealthSet;
	// Combat attribute set used by this actor.
	UPROPERTY()
	TObjectPtr<const class ULyraCombatSet> CombatSet;

	UPROPERTY()
	FOnLyraTeamIndexChangedDelegate OnTeamChangedDelegate;

	UPROPERTY()
	FGenericTeamId MyTeamID;

	UPROPERTY()
	TObjectPtr<const ULyraPawnData> PawnData;
};
