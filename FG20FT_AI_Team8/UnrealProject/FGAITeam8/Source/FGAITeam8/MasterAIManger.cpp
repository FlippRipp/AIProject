// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterAIManger.h"

#include <experimental/filesystem>


#include "Kismet/GameplayStatics.h"
#include "Enemybase.h"
#include "Math/UnrealMathUtility.h"
#include "FGAITeam8GameModeBase.h"


// Sets default values
AMasterAIManger::AMasterAIManger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMasterAIManger::BeginPlay()
{
	Super::BeginPlay();
	
	//PlayerCharacter = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	AFGAITeam8GameModeBase * GameMode = static_cast<AFGAITeam8GameModeBase*>(GetWorld()->GetAuthGameMode());

	 GameMode->AIManager = this;
}

void AMasterAIManger::UpdateAttackQueues(float DeltaTime)
{
	MeleeAttackTimer += DeltaTime;

	if(MeleeAttackTimer > TimeBetweenMeleeAttacks)
	{
		MeleeAttackTimer = 0;

		if(MeleeAttackers.Num() <= 0) return;

		TArray<AEnemybase*> ValidAttackers;
		
		TArray<AEnemybase*> InvalidAttackers;

		FVector PlayerLocation = PlayerCharacter->GetActorLocation();

		for(AEnemybase* Attacker : MeleeAttackers)
		{
			if(Attacker == nullptr || PlayerCharacter == nullptr) continue;
			
			Attacker->SetAllowedToAttack(false);

			FVector AttackerLocation = Attacker->GetActorLocation();
			
			FVector Dir = (AttackerLocation - PlayerLocation).GetSafeNormal();

			if(FVector::DotProduct(Dir, PlayerCharacter->GetActorForwardVector()) > (180 - MeleePriorityAttackAngle) / 180)
			{
				ValidAttackers.Add(Attacker);
			}
			else
			{
				InvalidAttackers.Add(Attacker);
			}
		}

		UE_LOG(LogTemp, Warning, TEXT("Finding Attacker"));

		
		if (MeleeAttackers.Num() == 1)
		{
			MeleeAttackers[0]->SetAllowedToAttack(true);
		}
	
		float RandomChance = FMath::RandRange(0, 100);
		
		if(RandomChance < InvalidAttackChance && InvalidAttackers.Num() > 0)
		{
			int RandomAttacker = FMath::RandRange(0, InvalidAttackers.Num() - 1);

			InvalidAttackers[RandomAttacker]->SetAllowedToAttack(true);
		}
		else if (ValidAttackers.Num() > 0)
		{
			int RandomAttacker = FMath::RandRange(0, ValidAttackers.Num() - 1);
			
			ValidAttackers[RandomAttacker]->SetAllowedToAttack(true);

		}
	}
}

// Called every frame
void AMasterAIManger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(PlayerCharacter == nullptr)
	{
		AFGAITeam8GameModeBase * GameMode = static_cast<AFGAITeam8GameModeBase*>(GetWorld()->GetAuthGameMode());

		PlayerCharacter = GameMode->PlayerCharacter;
	}
	UpdateAttackQueues(DeltaTime);
}

TArray<FVector> AMasterAIManger::GetFlankToFrontRoute(FVector Origin)
{

	float RandomDir = FMath::RandRange(0, 100);
	TArray<FVector> Path;
	FVector PlayerSideDir;

	if(FlankPoints.Num() == 0) return Path;
	
	if(RandomDir > 50)
	{
		PlayerSideDir = PlayerCharacter->GetActorRightVector();
	}
	else
	{
		PlayerSideDir = -PlayerCharacter->GetActorRightVector();
	}

	float BestSideScore = -1;
	int BestSideIndex = 0;
	int i = 0;

	float DesiredDist = FMath::RandRange(FlankMinRange, FlankMaxRange);

	for (AActor* FlankPoint : FlankPoints)
	{
		FVector PlayerLoc = PlayerCharacter->GetActorLocation();
		FVector FlankLoc = FlankPoint->GetActorLocation();

		float Dist = FVector::Distance(PlayerLoc, FlankLoc);
		
		if(Dist > FlankMaxRange || Dist < FlankMinRange) continue;


		float DistScore = 1 - FMath::Abs(Dist - DesiredDist) / FMath::Abs(FlankMinRange - FlankMaxRange) * DistanceWeight;

		UE_LOG(LogTemp, Warning, TEXT("Dist score is : %f"), DistScore);
		
		FVector DirToPlayer = FlankLoc - PlayerLoc;
		DirToPlayer.Normalize();
		
		float SideScore = FVector::DotProduct(PlayerSideDir, DirToPlayer);

		if(SideScore > BestSideScore)
		{
			BestSideIndex = i;
			BestSideScore = SideScore;
		}
		i++;
	}

	Path.Add(FlankPoints[BestSideIndex]->GetActorLocation());
	
	float BestFrontScore = -1;
	int BestFrontIndex = 0;
	i = 0;
	
	for (AActor* FlankPoint : FlankPoints)
	{
		if(i == BestSideIndex) continue;
		
		FVector PlayerLoc = PlayerCharacter->GetActorLocation();
		FVector FlankLoc = FlankPoint->GetActorLocation();

		float Dist = FVector::Distance(PlayerLoc, FlankLoc);
		
		if(Dist > FlankMaxRange || Dist < FlankMinRange) continue;
		
		FVector DirToPlayer = FlankLoc - PlayerLoc;
		DirToPlayer.Normalize();
		
		float FrontScore = FVector::DotProduct(PlayerCharacter->GetActorForwardVector() * PlayerSideDir, DirToPlayer);

		if(FrontScore > BestFrontScore)
		{
			BestFrontIndex = i;
			BestFrontScore = FrontScore;
		}
		i++;
	}

	Path.Add(FlankPoints[BestFrontIndex]->GetActorLocation());
	return Path;
}

bool AMasterAIManger::GetRangedSpot(FVector Origin, FVector& Result, bool ForceResult)
{
	for (AActor* RangeSpot : RangedSpots)
	{
		FHitResult Hit;
		ECollisionChannel TraceChannel = ECollisionChannel::ECC_WorldStatic;
		FCollisionResponseParams ResponseParam = FCollisionResponseParams::DefaultResponseParam;
		FCollisionQueryParams& Params  = FCollisionQueryParams::DefaultQueryParam;
		
		Params.AddIgnoredActor(RangeSpot);
		
		bool ReturnHit = GetWorld()->LineTraceSingleByChannel(Hit, RangeSpot->GetActorLocation(),
			PlayerCharacter->GetActorLocation(), TraceChannel, Params, ResponseParam);
		
		if(ReturnHit && Hit.Actor == PlayerCharacter)
		{
			return true;
			Result = RangeSpot->GetActorLocation();
		}
	}
	
	return false;
}

void AMasterAIManger::AddMeleeAttacker(AEnemybase* Attacker)
{
	if(!MeleeAttackers.Contains(Attacker))
	{
		MeleeAttackers.Add(Attacker);
	}
}

void AMasterAIManger::RemoveMeleeAttacker(AEnemybase* Attacker)
{
	if(MeleeAttackers.Contains(Attacker))
	{
		MeleeAttackers.Remove(Attacker);
	}
}

