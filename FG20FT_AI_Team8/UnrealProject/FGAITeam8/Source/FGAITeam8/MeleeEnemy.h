// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemybase.h"
#include "MeleeEnemy.generated.h"

/**
 * 
 */
UCLASS()
class FGAITEAM8_API AMeleeEnemy : public AEnemybase
{
	GENERATED_BODY()

	protected:
	//FVector
public:
	UPROPERTY(EditAnywhere)
	float AttackRange = 200;

	UPROPERTY(EditAnywhere)
	float EngageJumpSpeed = 200;
	UPROPERTY(EditAnywhere)
	float ArcHeight = 50;
	UPROPERTY(EditAnywhere)
	float MaxJumpDistance = 1000;
	UPROPERTY(EditAnywhere)
	float MinJumpDistance = 700;
	UPROPERTY(EditAnywhere)
	float LungeAttackRange = 100;
	
	UPROPERTY(EditAnywhere)
	float NormalAttackRange = 100;
	UPROPERTY(EditAnywhere)
	float NormalAttackRate = 2;

	float AttackTimer = 0;

	float JumpCompletion = 0;

	bool UseCustomLungeHeight;
	float CustomLungeHeight;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> TraceChannel;
	
	FVector JumpStartPos;
	FVector JumpTargetPoint;
	bool IsJumping = false;
	bool HasLungeDamaged = false;

	void ExecuteJump(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void StartLunge(FVector TargetPos, float Height = -1);

	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION(BlueprintCallable)
	float GetAttackRange();
	UFUNCTION(BlueprintCallable)
	bool GetAllowedLunge();
	UFUNCTION(BlueprintCallable)
	bool GetIsLunging();
	UFUNCTION(BlueprintCallable)
	void DestroyEnemy();

	//Called when the normal attacks begins
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttack(AActor* PlayerActor);
	//Called when the normal attack damages the player
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackDamage(AActor* PlayerActor);
	//Called when the lunge begins
	UFUNCTION(BlueprintImplementableEvent)
	void OnLunge();
	//Called when the lunge Damages the player
	UFUNCTION(BlueprintImplementableEvent)
	void OnLungeDamage(AActor* PlayerActor);
	
	void EvasiveMovement(FVector EnemyLocation, float DeltaTime);
};