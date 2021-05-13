// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "AiNavigationComponent.generated.h"

class UNavigationSystemV1;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FGAITEAM8_API UAiNavigationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAiNavigationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TArray<AActor*> Waypoints;

	// UFUNCTION()
	// FVector GetRandomWaypoint(FVector Origin, float radius);
	// UFUNCTION()
	// FVector GetPlayerWaypoint(FVector Origin);



		
};
