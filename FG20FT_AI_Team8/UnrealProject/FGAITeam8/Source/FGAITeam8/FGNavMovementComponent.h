#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "FGNavMovementComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UFGNavMovementComponent : public UPawnMovementComponent
{
    GENERATED_BODY()

public:
    UFGNavMovementComponent();
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;
    virtual void StopActiveMovement() override;

    virtual bool CanStartPathFollowing() const override;
    virtual bool CanStopPathFollowing() const override;

    virtual float GetPathFollowingBrakingDistance(float MaxSpeed) const override;
    bool FindNavFloor(const FVector& TestLocation, FNavLocation& NavFloorLocation) const;

    const INavigationDataInterface* GetNavData() const;

    UFUNCTION(BlueprintPure)
    bool IsMoving() const;

    UPROPERTY(EditAnywhere, Category=Movement)
    float MovementSpeed = 400.0f;

    UFUNCTION(BlueprintCallable)
    void ChangeMovementSpeed(float Speed);

    bool bHasRequestedVelocity;
    FVector RequestedVelocity = FVector::ZeroVector;
};
