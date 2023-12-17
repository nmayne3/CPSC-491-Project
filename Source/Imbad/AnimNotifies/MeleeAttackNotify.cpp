// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttackNotify.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"


void UMeleeAttackNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	HitActors.Empty();
}

void UMeleeAttackNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    float FrameDeltaTime)
{
	// Trace Parameters for Unarmed Animation
	FVector TraceBegin = MeshComp->GetSocketLocation(FName(TEXT("Hand_RSocket")));
	FVector TraceEnd = MeshComp->GetSocketLocation(FName(TEXT("Fingertip_RSocket")));

	// Should make this Editable in BP 
	float Radius = 20;

	// Type of Objects to Trace
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; 
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));	// Tracing for pawns
	FHitResult HitResult;
	if(UKismetSystemLibrary::SphereTraceSingleForObjects(MeshComp->GetWorld(), TraceBegin, TraceEnd, Radius, ObjectTypes, true, {MeshComp->GetOwner()}, EDrawDebugTrace::None, HitResult, true))
	{
		if(HitResult.GetActor())
		{
			if(HitActors.Find(HitResult.GetActor()) == INDEX_NONE)
			{
				HitActors.Add(HitResult.GetActor());
				FPointDamageEvent DamageEvent(20, HitResult, HitResult.ImpactNormal, NULL);
				// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, HitResult.GetActor()->GetName());
				// Only damage Characters :(
				ACharacter* HitCharacter = Cast<ACharacter>(HitResult.GetActor());
				if(HitCharacter->CanBeDamaged())
				{
					HitCharacter->TakeDamage(20, DamageEvent, NULL, NULL);
				}
			}
		}
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Swing and a Miss :("));
	}
}
