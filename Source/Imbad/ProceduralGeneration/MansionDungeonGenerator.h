// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DungeonGenerator.h"
#include "RoomData.h"
#include "MansionDungeonGenerator.generated.h"

/**
 * 
 */
UCLASS()
class IMBAD_API AMansionDungeonGenerator : public ADungeonGenerator
{
	GENERATED_BODY()

public:
	//AMansionDungeonGenerator();

	virtual void BeginPlay() override;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Data")
	URoomData* StartingRoomData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Data")
	URoomData* EndRoomData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Data")
	TSubclassOf<ADoor> NormalDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Data")
	TArray<URoomData*> NormalRooms;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation")
	int MinRooms; 
*/
	UFUNCTION(BlueprintCallable)
	virtual URoomData* ChooseFirstRoomData_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual URoomData* ChooseNextRoomData_Implementation(const URoomData* CurrentRoom,
		const FDoorDef& DoorData) override;

	UFUNCTION(BlueprintCallable)
	virtual bool ContinueToAddRoom_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsValidDungeon_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual TSubclassOf<ADoor> ChooseDoor_Implementation(const URoomData* CurrentRoom,
		const URoomData* NextRoom, const UDoorType* DoorType) override;

};
