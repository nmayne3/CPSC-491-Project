// Fill out your copyright notice in the Description page of Project Settings.


#include "MansionDungeonGenerator.h"
#include "Door.h"
#include "DungeonGraph.h"




void AMansionDungeonGenerator::BeginPlay()
{
	Generate();
	Super::BeginPlay();
}


// Chooses Default Starting Room
URoomData* AMansionDungeonGenerator::ChooseFirstRoomData_Implementation()
{
	return StartingRoomData;
}

// Finds compatible rooms based on door
URoomData* AMansionDungeonGenerator::ChooseNextRoomData_Implementation(const URoomData* CurrentRoom, const FDoorDef& DoorData)
{
	bool Success = false;
	TArray<URoomData*> CompatibleRooms;
	GetCompatibleRoomData(Success, CompatibleRooms, NormalRooms, DoorData);
	
	// Continues adding rooms if Minimum Rooms not met
	if (MinRooms > GetNbRoom() && Success)
	{
		return GetRandomRoomData(CompatibleRooms);
	}
	// Spawns a room with no exits if ending room is incompatible with current room to cap room generation
	if (!EndRoomData->HasCompatibleDoor(DoorData))
	{
		TArray<URoomData*> ClosingRooms;
		for(URoomData* RoomData: CompatibleRooms)
		{
			if(RoomData->GetNbDoor() == 1)
			{
				ClosingRooms.Push(RoomData);
			}
		}
		if (ClosingRooms.Num())
		{
			return GetRandomRoomData(ClosingRooms);
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("No Closing Rooms found :("));
	}
	return EndRoomData;
}

// Decides whether or not to add door or wall
TSubclassOf<ADoor> AMansionDungeonGenerator::ChooseDoor_Implementation(const URoomData* CurrentRoom, const URoomData* NextRoom, const UDoorType* DoorType)
{
	if(IsValid(CurrentRoom) && IsValid(NextRoom))
	{
		for(TSubclassOf<ADoor> Door : NormalDoors)
		{
			if(Door->GetDefaultObject<ADoor>()->GetDoorType() == DoorType)
			{
				return Door;
			}
		}
	}
	return Wall;
}

// Checks if Dungeon has Ending Room Spawned in.
bool AMansionDungeonGenerator::IsValidDungeon_Implementation()
{
	if(HasAlreadyRoomData(EndRoomData))
		return true;
	
	return false;
}

// Allows Dungeon to Naturally Stop Generating
bool AMansionDungeonGenerator::ContinueToAddRoom_Implementation()
{
	return true;
}
