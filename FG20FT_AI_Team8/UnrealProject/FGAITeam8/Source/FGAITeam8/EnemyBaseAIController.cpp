// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseAIController.h"

void AEnemyBaseAIController::NavigateToPosition(FVector PositionToNavigateTo)
{

	UE_LOG(LogTemp, Warning, TEXT("AIController position (%f, %f, %f)"), PositionToNavigateTo.X, PositionToNavigateTo.Y, PositionToNavigateTo.Z);

	MoveToLocation(PositionToNavigateTo);
}