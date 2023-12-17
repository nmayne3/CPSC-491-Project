// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Adds Item if Current Item doesn't exist or is at capacity
bool UInventoryComponent::Add(UItemDataAsset* ItemData)
{
	if(UItemDataAsset* Existing = GetItemByName(ItemData->ItemName))
	{
		if(Existing->IsFull())
		{
			return false;
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Adding To Existing Item"));
		const uint8 Max = Existing->MaxStack;
		Existing->CurrentStack += ItemData->CurrentStack;
		if(Existing->CurrentStack > Max)
		{
			Existing->CurrentStack = Max;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Adding NEW Item"));
		UItemDataAsset* NewItem = DuplicateObject(ItemData, nullptr, ItemData->ItemName);
		Inventory.Add(NewItem);
	}
	return true;
}

// Subtracts Item from stack and removes Item if Stack is Empty
bool UInventoryComponent::Subtract(const FName &ItemName, uint8 Count)
{
	if(UItemDataAsset* Existing = GetItemByName(ItemName))
	{
		Existing->CurrentStack = (Existing->CurrentStack < Count) ? 0 : Existing->CurrentStack-Count;
		if(Existing->CurrentStack <= 0)
		{
			Inventory.Remove(Existing);
		}
		return true;
	}
	return false;
}

// Returns Pointer to first item found with matching name
UItemDataAsset* UInventoryComponent::GetItemByName(const FName &ItemName)
{
	for(UItemDataAsset* Item: Inventory)
	{
		if(Item->ItemName == ItemName)
		{
			return Item;
		}
	}
	return nullptr;
}

