// Fill out your copyright notice in the Description page of Project Settings.


#include "LyraPlayerController.h"

#include "LyraPlayerState.h"
#include "PerfectGameFramework/Camera/LyraPlayerCameraManager.h"

ALyraPlayerController::ALyraPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ALyraPlayerCameraManager::StaticClass();

// #if USING_CHEAT_MANAGER
// 	CheatClass = ULyraCheatManager::StaticClass();
// #endif // #if USING_CHEAT_MANAGER
}

void ALyraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(false);
}

ALyraPlayerState* ALyraPlayerController::GetLyraPlayerState() const
{
	return CastChecked<ALyraPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}