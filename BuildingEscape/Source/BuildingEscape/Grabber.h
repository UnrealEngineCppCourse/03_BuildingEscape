// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	
	float Reach = 100.0f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Ray-cast and grab what's in reach
	void Grab();
	
	// Called when grab is released
	void Release();

	// Finds attached physics handle
	void FindPhysicsHandleComponent();
	
	// Setup attached input component
	void SetupInputComponent();

	// Return hit for
	const FHitResult GetFirstPhysicsBodyInReach();

	//Returns current start of reach line
	FVector GetReachLineStart();

	//Returns current end of reach line
	FVector GetReachLineEnd();
};
