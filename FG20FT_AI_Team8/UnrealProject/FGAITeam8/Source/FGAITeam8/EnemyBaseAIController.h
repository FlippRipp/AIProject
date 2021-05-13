// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class FGAITEAM8_API AEnemyBaseAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	void NavigateToPosition(FVector PositionToNavigateTo);
	
};
