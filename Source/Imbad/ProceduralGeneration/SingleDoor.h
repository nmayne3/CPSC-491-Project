// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Door.h"
#include "Components/TimelineComponent.h"
#include "ImBad/Interfaces/InteractInterface.h"
#include "SingleDoor.generated.h"

/**
 * 
 */
UCLASS()
class IMBAD_API ASingleDoor : public ADoor, public IInteractInterface
{
	GENERATED_BODY()

public:
	ASingleDoor();
	virtual void Interact(APawn &Caller) override;
	virtual void OnDoorOpen() override;
	virtual void OnDoorClose() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Timeline)
	UCurveFloat* TimelineCurveFloat;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category="Components")
	UStaticMesh* Door;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category="Components")
	USceneComponent* DoorRoot;

	int DirectionOffset = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTimelineComponent* Timeline;

	FOnTimelineFloat UpdateFunctionFloat;

	UFUNCTION()
	virtual void TickTimeline(float delta);
};
