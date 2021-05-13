// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <Actor.h>


#include "GameFramework/Actor.h"
#include "MasterAIManger.generated.h"

class AEnemybase;

UCLASS()
class FGAITEAM8_API AMasterAIManger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMasterAIManger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float MeleeAttackTimer = 0;
	TArray<AEnemybase*> MeleeAttackers;

	void UpdateAttackQueues(float DeltaTime);

	TArray<AActor*> UsedFlankPoints;

public:
	
	AActor* PlayerCharacter;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> FlankPoints;
	
	UPROPERTY(EditAnywhere)
	TArray<AActor*> SpawnPoints;
	
	UPROPERTY(EditAnywhere)
	TArray<AActor*> RangedSpots;

	UPROPERTY(EditAnywhere)
	float TimeBetweenMeleeAttacks = 2;
	UPROPERTY(EditDefaultsOnly)
	float MeleePriorityAttackAngle = 90;
	UPROPERTY(EditDefaultsOnly)
	float InvalidAttackChance = 10;
	UPROPERTY(EditDefaultsOnly)
	float FlankMaxRange = 2000;
	UPROPERTY(EditDefaultsOnly)
	float FlankMinRange = 500;
	UPROPERTY(EditDefaultsOnly)
	float DistanceWeight = 1;

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetFlankToFrontRoute(FVector Origin);

	UFUNCTION(BlueprintCallable)
	bool GetRangedSpot(FVector Origin, FVector& Result, bool ForceResult = false);

	UFUNCTION(BlueprintCallable)
	void AddMeleeAttacker(AEnemybase* Attacker);
	UFUNCTION(BlueprintCallable)
	void RemoveMeleeAttacker(AEnemybase* Attacker);

};
