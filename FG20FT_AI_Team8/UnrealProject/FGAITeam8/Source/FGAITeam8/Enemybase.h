// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "FGNavMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Enemybase.generated.h"

class AMasterAIManger;
//class UFGNavMovementComponent;
class AEnemyBaseAIController;
UCLASS()
class FGAITEAM8_API AEnemybase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemybase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsAllowedToAttack = false;

	FVector velocity;

	FVector LastPosition;

	FVector Target;

	TArray<FVector> TargetPath;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	float MovementSpeed = 200;
	UPROPERTY(EditDefaultsOnly)
	float HealthPoints = 100;

	bool CanMove = true;
	bool Flanking = false;
	bool TargetDiscovered = false;
	bool IsPlayerInSight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFGNavMovementComponent* NavigationComponent;

	UPROPERTY(EditAnywhere)
	AMasterAIManger* AIManager;

	UFUNCTION(BlueprintCallable)
	void MoveToTargetedLocation(FVector Position);
	
	AActor* PlayerCharacter;

	AActor* GetPlayerCharacter();

	UFUNCTION(BlueprintCallable)
	AMasterAIManger* GetAIManager();
	
	UFUNCTION(BlueprintCallable)
	bool GetAllowedToAttack();
	
	UFUNCTION(BlueprintCallable)
	bool GetFlanking();

	UFUNCTION(BlueprintCallable)
	void SetAllowedToAttack(bool state);

	UFUNCTION(BlueprintImplementableEvent)
	void Move(FVector Position);
	
	UFUNCTION(BlueprintCallable)
	FVector GetTarget();
	
	UFUNCTION(BlueprintCallable)
	void DamageEnemy(float Damage);
	
	UFUNCTION(BlueprintCallable)
	void DiscoverPlayer();
	
	UFUNCTION(BlueprintCallable)
	void SetPlayerInSight(bool State);
	
	UFUNCTION(BlueprintCallable)
	FVector GetMovementDirection();
	UFUNCTION(BlueprintCallable)

	FVector GetMovementVelocity();
	UFUNCTION(BlueprintCallable)
	float GetMovementSpeed();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDeath();


	UFUNCTION(BlueprintCallable)
	void ChangeMovementSpeed(float Speed);

	void FlankPlayerFromFront();
};	
