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
	
	if (PhysicsHandle == nullptr) { return; }
	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		/// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	
}

void UGrabber::Grab()
{
	/// Try to reach any actors with physics body collision channel set
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();  // gets the mesh component
	AActor* ActorHit = HitResult.GetActor();
	
	if (ActorHit != nullptr) {
		if (PhysicsHandle == nullptr) { return; }
		PhysicsHandle->GrabComponent(ComponentToGrab
										,NAME_None  // No bones needed
										,ComponentToGrab->GetOwner()->GetActorLocation()
										,true);
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle == nullptr) { return; }
	PhysicsHandle->ReleaseComponent();
	UE_LOG(LogTemp, Warning, TEXT("Grab released."));
}

void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle attached to %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	
	if (InputComponent != nullptr) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("No InputComponent attached to %s"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FHitResult HitResult;

	/// Setting ray-trace query params
	FCollisionObjectQueryParams PhysicsBodyQueryParams = FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody);
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	
	// Line trace (AKA ray-cast) out to reach distance
	GetWorld()->LineTraceSingleByObjectType(HitResult
											,GetReachLineStart()
											,GetReachLineEnd()
											,PhysicsBodyQueryParams
											,TraceParams);

	/// See what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Ray trace hit %s."), *ActorHit->GetName());
	}
	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetActorEyesViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
