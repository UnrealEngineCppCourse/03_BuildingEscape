// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (PressurePlate == nullptr) 
	{ 
		UE_LOG(LogTemp, Error, TEXT("No PressurePlate attached to %s"), *GetOwner()->GetName());
	}
}

void UOpenDoor::OpenDoor()
{
	if (Owner == nullptr) { return; }
	Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Check if ActorThatOpens is in the volume, then open the door
	if (GetTotalMassOnPlate() > TriggerMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}
	if (GetWorld()->GetTimeSeconds() >= LastDoorOpenTime + DoorCloseDelay)
	{
		CloseDoor();
	}
	
}

float UOpenDoor::GetTotalMassOnPlate()
{
	float TotalMass = 0.0f;
	TArray<AActor*> OverlappingActors;

	if (PressurePlate == nullptr) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OverlappingActors);
	for (const auto* Actor : OverlappingActors) {
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}
	return TotalMass;
}