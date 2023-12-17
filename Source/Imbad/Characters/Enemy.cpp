// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if(MaxHealth == NULL)
		MaxHealth = 100;

	Health = MaxHealth;
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::OnAttackAnimationEnd(UAnimMontage* AnimMontage, bool Interrupted)
{
	if(AnimMontage == AttackAnimation)
	{
		if(Interrupted)
		{
			UE_LOG(LogTemp, Warning, TEXT("Animation interrupted :("));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Animation Blending tf out!!"))
		}
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AttackTarget(AActor* Target)
{
	
}

void AEnemy::Attack()
{
	// Play Animation if Possible
	if(AttackAnimation != nullptr)
	{
		PlayAnimMontage(AttackAnimation);
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		// AnimInstance->OnMontageStarted
		//AnimInstance->OnMontageBlendingOut.AddDynamic(this, &AEnemy::OnAttackAnimationEnd);
	}
	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                         AActor* DamageCauser)
{
	// Modify Health
	Health -= DamageAmount;
	
	// No Negative Health and destroy actor
	if (Health <= 0)
	{
		Health = 0;
		//TODO: Add Gib Particle System
		Destroy();
	}
	
	// Spawn Blood Emitter from impact point
	if(DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		// What the fuck man, why wont you just work?
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		
		if(BloodSystem != nullptr && DamageCauser != nullptr)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodSystem, PointDamageEvent->ShotDirection, DamageCauser->GetActorRotation());
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

