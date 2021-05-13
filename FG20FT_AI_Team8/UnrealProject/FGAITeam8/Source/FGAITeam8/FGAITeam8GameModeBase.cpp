// Copyright Epic Games, Inc. All Rights Reserved.


#include "FGAITeam8GameModeBase.h"

#include <activation.h>


#include "Kismet/GameplayStatics.h"

AMasterAIManger* AFGAITeam8GameModeBase::GetMasterAIManager()
{
	return AIManager;
}

void AFGAITeam8GameModeBase::AddEnemy()
{
	EnemiesLeft++;
}

void AFGAITeam8GameModeBase::RemoveEnemy()
{
	EnemiesLeft--;


	if(EnemiesLeft <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemies left: %f"), EnemiesLeft);
		ResetCurrentLevel();
	}
}
