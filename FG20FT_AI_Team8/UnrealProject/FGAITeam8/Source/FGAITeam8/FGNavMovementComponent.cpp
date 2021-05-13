#include "FGNavMovementComponent.h"
#include "AI/Navigation/NavigationDataInterface.h"
#include "Kismet/KismetMathLibrary.h"

UFGNavMovementComponent::UFGNavMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UFGNavMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if(!bHasRequestedVelocity)
        return;

    GetOwner()->AddActorWorldOffset(RequestedVelocity * DeltaTime);
    
    FRotator Rot = UKismetMathLibrary::MakeRotFromX(RequestedVelocity);
    GetOwner()->SetActorRotation(FRotator(0.0f, Rot.Yaw, 0.0f));

    FNavLocation NavFloorLocation;
    if(FindNavFloor(GetOwner()->GetActorLocation(), NavFloorLocation))
    {
        float FloorHeight = NavFloorLocation.Location.Z;
        FVector CurrentLocation = GetOwner()->GetActorLocation();
        CurrentLocation.Z = FloorHeight + 100;
        GetOwner()->SetActorLocation(CurrentLocation);
    }
}

void UFGNavMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
    RequestedVelocity = MoveVelocity.GetSafeNormal() * MovementSpeed;
    bHasRequestedVelocity = true;
}

void UFGNavMovementComponent::StopActiveMovement()
{
    Super::StopActiveMovement();

    RequestedVelocity = FVector::ZeroVector;
    bHasRequestedVelocity = false;
}
bool UFGNavMovementComponent::CanStartPathFollowing() const
{
    return true;
}

bool UFGNavMovementComponent::CanStopPathFollowing() const
{
    return true;
}

float UFGNavMovementComponent::GetPathFollowingBrakingDistance(float MaxSpeed) const
{
    return 1.0f;
}

bool UFGNavMovementComponent::FindNavFloor(const FVector& TestLocation, FNavLocation& NavFloorLocation) const
{
    const INavigationDataInterface* NavData = GetNavData();
    if(!NavData)
    {
        return false;
    }

    const FNavAgentProperties& AgentProps = GetPawnOwner()->GetNavAgentPropertiesRef();
    const float SearchRadius = AgentProps.AgentRadius * 2.0f;
    const float SearchHeight = AgentProps.AgentHeight * AgentProps.NavWalkingSearchHeightScale;

    return  NavData->ProjectPoint(TestLocation - FVector(0.0f, 0.0f, GetPawnOwner()->GetDefaultHalfHeight()),
        NavFloorLocation, FVector(SearchRadius, SearchRadius, SearchHeight));
}

const INavigationDataInterface* UFGNavMovementComponent::GetNavData() const
{
    const UWorld* World = GetWorld();
    if(!World || !World->GetNavigationSystem())
    {
        return nullptr;
    }

    const INavigationDataInterface* NavData = FNavigationSystem::GetNavDataForActor(*GetOwner());
    return NavData;
}

bool UFGNavMovementComponent::IsMoving() const
{
    return bHasRequestedVelocity;
}

void UFGNavMovementComponent::ChangeMovementSpeed(float Speed)
{
    MovementSpeed =Speed;
}
