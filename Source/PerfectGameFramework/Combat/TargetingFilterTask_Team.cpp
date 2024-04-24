#include "TargetingFilterTask_Team.h"
#include "PerfectGameFramework/Teams/LyraTeamStatics.h"

class ULyraTeamDisplayAsset;

UTargetingFilterTask_Team::UTargetingFilterTask_Team(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

bool UTargetingFilterTask_Team::ShouldFilterTarget(const FTargetingRequestHandle& TargetingHandle, const FTargetingDefaultResultData& TargetData) const
{
	if (AActor* TargetActor = TargetData.HitResult.GetActor())
	{
		bool bIsPartOfTeam;
		int32 TargetTeamId;
		ULyraTeamDisplayAsset* DisplayAsset;
		ULyraTeamStatics::FindTeamFromObject(TargetActor, bIsPartOfTeam, TargetTeamId, DisplayAsset, false);

		int32 SourceTeamId = GetSourceActorTeamId(TargetingHandle);
		
		return bNeedEnemyTeam ? (SourceTeamId == TargetTeamId) : (SourceTeamId != TargetTeamId);
	}

	return true;
}

int32 UTargetingFilterTask_Team::GetSourceActorTeamId(const FTargetingRequestHandle& TargetingHandle) const
{
	int32 SourceTeamId = INDEX_NONE;
	if (FTargetingSourceContext* SourceContext = FTargetingSourceContext::Find(TargetingHandle))
	{
		if (SourceContext->SourceActor)
		{
			bool bIsPartOfTeam;
			ULyraTeamDisplayAsset* DisplayAsset;
			ULyraTeamStatics::FindTeamFromObject(SourceContext->SourceActor, bIsPartOfTeam, SourceTeamId, DisplayAsset, false);
		}
	}

	return SourceTeamId;
}
