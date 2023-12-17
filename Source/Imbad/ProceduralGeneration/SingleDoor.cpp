// Fill out your copyright notice in the Description page of Project Settings.

#include "SingleDoor.h"


ASingleDoor::ASingleDoor()
{
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline Component"));
}

void ASingleDoor::BeginPlay()
{
	Super::BeginPlay();
	UpdateFunctionFloat.BindDynamic(this, &ASingleDoor::TickTimeline);
	//Timeline->

	if(TimelineCurveFloat)
	{
		Timeline->AddInterpFloat(TimelineCurveFloat, UpdateFunctionFloat);
	}
}


void ASingleDoor::Interact(APawn &Caller)
{
	// Opens Door if closed
	if(!IsOpen())
	{
		// Check Direction Door is facing
		DirectionOffset = (FVector::DotProduct(GetActorForwardVector(), Caller.GetActorForwardVector()) < 0) ? -1 : 1;
		Open(true);
	} // Closes Door
	else
	{
		Open(false);
	}
}
	
void ASingleDoor::OnDoorOpen()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Door Opening"));
	Timeline->Play();
}

void ASingleDoor::OnDoorClose()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Door Closing"));
	Timeline->Reverse();
}

void ASingleDoor::TickTimeline(float delta)
{
	FRotator RotateAmount = { 0, delta * DirectionOffset, 0 };
	DoorRoot->SetRelativeRotation(RotateAmount);
}
