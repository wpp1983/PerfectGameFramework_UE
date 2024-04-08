// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Logging/LogMacros.h"

class UObject;

PERFECTGAMEFRAMEWORK_API DECLARE_LOG_CATEGORY_EXTERN(LogLyra, Log, All);
PERFECTGAMEFRAMEWORK_API DECLARE_LOG_CATEGORY_EXTERN(LogLyraExperience, Log, All);
PERFECTGAMEFRAMEWORK_API DECLARE_LOG_CATEGORY_EXTERN(LogLyraAbilitySystem, Log, All);
PERFECTGAMEFRAMEWORK_API DECLARE_LOG_CATEGORY_EXTERN(LogLyraTeams, Log, All);

PERFECTGAMEFRAMEWORK_API FString GetClientServerContextString(UObject* ContextObject = nullptr);
