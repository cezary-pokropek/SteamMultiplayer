// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"
#include "TimerManager.h"

AMovingPlatform::AMovingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);

	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);

	bInterping = false;

	InterpSpeed = 4.0f;
	InterpTime = 1.f;
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);

		StartPoint = GetActorLocation();
		EndPoint += StartPoint;

		bInterping = false;

		GetWorldTimerManager().SetTimer(InterpTimer, this, &AMovingPlatform::ToggleInterping, InterpTime);

		Distance = (EndPoint - StartPoint).Size();

	}

}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveTriggers > 0)
	{
		/*Runs only on the server*/
		if (HasAuthority())
		{

			if (bInterping)
			{
				FVector CurrentLocation = GetActorLocation();
				FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
				SetActorLocation(Interp);

				float DistanceTravelled = (GetActorLocation() - StartPoint).Size();
				if (Distance - DistanceTravelled <= 1.f)
				{
					ToggleInterping();

					GetWorldTimerManager().SetTimer(InterpTimer, this, &AMovingPlatform::ToggleInterping, InterpTime);
					SwapVectors(StartPoint, EndPoint);
				}

			}

		}
	}

	
}

void AMovingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AMovingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector Temp = VecOne;
	VecOne = VecTwo;
	VecTwo = Temp;
}


void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers++;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	if (ActiveTriggers > 0)
	{
		ActiveTriggers--;
	}
}