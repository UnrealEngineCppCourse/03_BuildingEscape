// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
		
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// move the object that we're holding
		GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
		LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
		PhysicsHandle->SetTargetLocation(LineTraceEnd);

	}
	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed."));
	
	/// Try to reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	/// If we hit something then attach a physics handle
	if (ActorHit != nullptr) {
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
	
	// TODO attach physics handle
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab released."));
	// TODO release physics handle
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached PhysicsHandle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		// PhysicsHandle is found
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle attached to %s"), *GetOwner()->GetName());
	}
	else {
		// PhysicsHandle not found
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle attached to %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	
	if (InputComponent) {
		// InputComponent is found
		UE_LOG(LogTemp, Warning, TEXT("InputComponent attached to %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		// InputComponent not found
		UE_LOG(LogTemp, Error, TEXT("No InputComponent attached to %s"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Get player view point for this tick
	GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	/// Draw a red trace in the world to visualize
	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// Line trace (AKA ray-cast) out to reach distance
	GetWorld()->LineTraceSingleByObjectType(Hit, PlayerViewPointLocation, LineTraceEnd, PhysicsBodyQueryParams, TraceParams);

	/// See what we hit
	ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Ray trace hit %s."), *ActorHit->GetName());
	}
	return Hit;
}
