// Fill out your copyright notice in the Description page of Project Settings.

#include "Pickup_Weapon.h"
#include "Imbad/Widgets/WeaponUI.h"

// Sets default values
APickup_Weapon::APickup_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TP_PickUp = CreateDefaultSubobject<UTP_PickUpComponent>(TEXT("TP_Pickup"));
	TP_PickUp->InitSphereRadius(32.0f);
	TP_PickUp->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Block);
	RootComponent = Mesh;
}

void APickup_Weapon::Interact(APawn& Caller)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Broadcasting Pickup"));
	if(AImbadCharacter* Character = Cast<AImbadCharacter>(&Caller))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Gonna Pick stuff up now"));
		if(ItemData)
		{
			if(UInventoryComponent* InventoryComponent = Character->InventoryComponent)
			{
				if(InventoryComponent->Add(ItemData))
				{
					Character->WeaponUI->UpdateAmmo();
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("We Equipping stuff "));
					if(ItemData->ItemMeshComponent)
					{
						Character->EquipItem(ItemData);
						//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("EQUIPPED"));
					}
					Destroy();
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("INVENTORY COMPONENT NULL"));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Item Data NULL"));
		}		
    }
}

// Called when the game starts or when spawned
void APickup_Weapon::BeginPlay()
{
	Super::BeginPlay();
	if(!ItemArray.IsEmpty() && !ItemData)
	{
		int index = FMath::RandRange(0, ItemArray.Num()-1);
		ItemData = ItemArray[index];
	}
	if(ItemData && Mesh)
	{
		Mesh->SetStaticMesh(ItemData->ItemMesh);
		if(ItemData->ItemMeshComponent != nullptr)
		{
			if(UTP_WeaponComponent* WeaponComponent = Cast<UTP_WeaponComponent>(ItemData->ItemMeshComponent))
				Mesh->SetWorldScale3D({ WeaponComponent->GetScale(), WeaponComponent->GetScale(), WeaponComponent->GetScale() });
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Item Data NULL"));
	}

}

// Called every frame
void APickup_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

