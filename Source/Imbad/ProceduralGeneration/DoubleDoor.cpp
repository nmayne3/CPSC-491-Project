// Fill out your copyright notice in the Description page of Project Settings.


#include "DoubleDoor.h"

void ADoubleDoor::TickTimeline(float delta)
{
	FRotator DeltaRotation = {0, delta * DirectionOffset, 0};
	DoorRoot->SetRelativeRotation(DeltaRotation);
	DoorRoot2->SetRelativeRotation(-1*DeltaRotation);
}
