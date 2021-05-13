// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/GameModeBase.h"
#include "FGAITeam8GameModeBase.generated.h"

class AMasterAIManger;

/**
 * 
 */
UCLASS()
class FGAITEAM8_API AFGAITeam8GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	
public:
	AMasterAIManger* AIManager;

	AActor* PlayerCharacter;
	int EnemiesLeft = 0;

	AMasterAIManger* GetMasterAIManager();

	UFUNCTION(BlueprintImplementableEvent)
	void ResetCurrentLevel();

	void AddEnemy();
	void RemoveEnemy();
};
