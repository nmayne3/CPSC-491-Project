// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SingleDoor.h"
#include "DoubleDoor.generated.h"

/**
 * 
 */
UCLASS()
class IMBAD_API ADoubleDoor : public ASingleDoor
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category="Components")
	USceneComponent* DoorRoot2;
	
protected:
	virtual void TickTimeline(float delta) override;
};
