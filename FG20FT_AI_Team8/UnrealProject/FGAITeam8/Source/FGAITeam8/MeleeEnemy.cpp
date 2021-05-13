// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeEnemy.h"



#include "MasterAIManger.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Math/UnrealMathVectorCommon.h"

void AMeleeEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AttackTimer += DeltaTime;
	ExecuteJump(DeltaTime);
	Attack();
}

void AMeleeEnemy::ExecuteJump(float DeltaTime)
{
	if(!IsJumping) return;
	
	if(JumpCompletion <= 1)
	{
		FVector Jump = FMath::Lerp(JumpStartPos, JumpTargetPoint, JumpCompletion);

		float Height = ArcHeight;
		
		if(UseCustomLungeHeight) Height = CustomLungeHeight;
		
		const FVector JumpHeight = FVector::UpVector * FMath::Sin(JumpCompletion * 3.141592f) * Height;
		Jump += JumpHeight;

		UE_LOG(LogTemp, Warning, TEXT("The Height is: %f"), JumpHeight.Z);

		SetActorLocation(Jump);

		const float LungeRangeSquared = LungeAttackRange * LungeAttackRange;

		if(FVector::DistSquared(GetActorLocation(), GetPlayerCharacter()->GetActorLocation()) < LungeRangeSquared &&
			!HasLungeDamaged)
		{
			HasLungeDamaged = true;
			OnLungeDamage(PlayerCharacter);
		}
		
		JumpCompletion += DeltaTime * EngageJumpSpeed;
	}
	else
	{
		CanMove = true;
		IsJumping = false;
	}
}

void AMeleeEnemy::StartLunge(FVector TargetPos, float Height)
{
	if(!IsJumping)
	{
		CanMove = false;
		IsJumping = true;
		HasLungeDamaged = false;
		IsAllowedToAttack = false;
		
		JumpStartPos = GetActorLocation();
		JumpTargetPoint = TargetPos;
		JumpCompletion = 0;
		
		OnLunge();

		if(Height != - 1)
		{
			UseCustomLungeHeight = true;
			CustomLungeHeight = Height;
		}
		else UseCustomLungeHeight = false;
	}
}

void AMeleeEnemy::Attack()
{
	
	if(!IsJumping && AttackTimer > NormalAttackRate)
	{
		OnAttack(PlayerCharacter);
		
		if(FVector::DistSquared(GetActorLocation(), GetPlayerCharacter()->GetActorLocation()) < AttackRange * AttackRange)
		{
			OnAttackDamage(PlayerCharacter);
			AttackTimer = 0;
		}
	}
}

float AMeleeEnemy::GetAttackRange()
{
	return  AttackRange;
}

bool AMeleeEnemy::GetAllowedLunge()
{
	float Dist = FVector::Distance(GetActorLocation(), GetPlayerCharacter()->GetActorLocation());
	
	return IsAllowedToAttack && Dist < MaxJumpDistance && Dist > MinJumpDistance && IsPlayerInSight;
}

bool AMeleeEnemy::GetIsLunging()
{
	return IsJumping;
}

void AMeleeEnemy::DestroyEnemy()
{
	AIManager->RemoveMeleeAttacker(this);
	Destroy();
}

void AMeleeEnemy::EvasiveMovement(FVector EnemyLocation, float DeltaTime)
{
}
