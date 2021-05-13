// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemybase.h"

#include <concrt.h>


#include "FGAITeam8GameModeBase.h"
#include "FGNavMovementComponent.h"
#include "MasterAIManger.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemybase::AEnemybase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NavigationComponent = CreateDefaultSubobject<UFGNavMovementComponent>(TEXT("NavigationComponent"));
}

// Called when the game starts or when spawned
void AEnemybase::BeginPlay()
{
	Super::BeginPlay();

	LastPosition = GetActorLocation();
	
	AFGAITeam8GameModeBase * GameMode = static_cast<AFGAITeam8GameModeBase*>(GetWorld()->GetAuthGameMode());
	
	GameMode->AddEnemy();
	
	AIManager = GameMode->GetMasterAIManager();
	
	AIManager->AddMeleeAttacker(this);
	
	if(NavigationComponent != nullptr)
	{
		NavigationComponent->ChangeMovementSpeed(MovementSpeed);
	}
}

// Called every frame
void AEnemybase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewPosition = GetActorLocation();

	velocity = (NewPosition - LastPosition) / DeltaTime;

	LastPosition = NewPosition;

	//UE_LOG(LogTemp, Warning, TEXT("Some warning message (%f , %f, %f) (%f, %f, %f)"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z, Target.X, Target.Y, Target.Z);

	if(FVector::Distance(GetActorLocation(), Target) < 250)
	{
		if(Flanking)
		{
			TargetPath.RemoveAt(0);
		
			if(TargetPath.Num() == 0)
			{
				Flanking = false;

				if(TargetDiscovered)
				{
					Target = GetPlayerCharacter()->GetActorLocation();
				}
				else
				{
					FlankPlayerFromFront();
				}
			}
			else Target = TargetPath[0];
		}
		else
		{
			FlankPlayerFromFront();
		}
	}

	if(!Flanking)
	{
		if(TargetDiscovered)
		{
			Target = GetPlayerCharacter()->GetActorLocation();
		}
		else
		{
			FlankPlayerFromFront();
		}
	}
}

// Called to bind functionality to input
void AEnemybase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemybase::MoveToTargetedLocation(FVector Position)
{
	Move(Position);
}

AActor* AEnemybase::GetPlayerCharacter()
{
	if(PlayerCharacter == nullptr)
	{
		AFGAITeam8GameModeBase * GameMode = static_cast<AFGAITeam8GameModeBase*>(GetWorld()->GetAuthGameMode());

		PlayerCharacter = GameMode->PlayerCharacter;
	}

	return PlayerCharacter;
}

AMasterAIManger* AEnemybase::GetAIManager()
{
	return AIManager;
}

bool AEnemybase::GetAllowedToAttack()
{
	return  IsAllowedToAttack;
}

bool AEnemybase::GetFlanking()
{
	return Flanking;
}

void AEnemybase::SetAllowedToAttack(bool state)
{
	IsAllowedToAttack = state;
}

FVector AEnemybase::GetTarget()
{
	return Target;
}

void AEnemybase::DamageEnemy(float Damage)
{
	HealthPoints -= Damage;

	if(HealthPoints <= 0)
	{
		HealthPoints = 0;
		AFGAITeam8GameModeBase * GameMode = static_cast<AFGAITeam8GameModeBase*>(GetWorld()->GetAuthGameMode());
	
		GameMode->RemoveEnemy();
		OnDeath(); 
	}
}

void AEnemybase::DiscoverPlayer()
{
	if(!TargetDiscovered)
	{
		TargetDiscovered = true;
		Flanking = false;
	}
}

void AEnemybase::SetPlayerInSight(bool State)
{
	IsPlayerInSight = State;
}

FVector AEnemybase::GetMovementDirection()
{
	
	return GetTransform().InverseTransformVector(velocity.GetSafeNormal());
}

FVector AEnemybase::GetMovementVelocity()
{
	return velocity;
}

float AEnemybase::GetMovementSpeed()
{
	return velocity.Size();
}

void AEnemybase::ChangeMovementSpeed(float Speed)
{
	NavigationComponent->ChangeMovementSpeed(Speed);
}

void AEnemybase::FlankPlayerFromFront()
{
	TargetPath = AIManager->GetFlankToFrontRoute(GetActorLocation());

	Target = TargetPath[0];

	if(TargetPath.Num() > 0)
	{
		Flanking = true;
	}
}
