// Fill out your copyright notice in the Description page of Project Settings.


#include "AiNavigationComponent.h"

#include "NavigationData.h"
#include "NavigationSystem.h"
#include "Runtime/Engine/Classes/Engine/TargetPoint.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UAiNavigationComponent::UAiNavigationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAiNavigationComponent::BeginPlay()
{
	Super::BeginPlay();

	FNavLocation Location;
}


// Called every frame
void UAiNavigationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


