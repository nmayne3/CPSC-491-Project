// Copyright Epic Games, Inc. All Rights Reserved.

#include "ImbadProjectile.h"
#include "ImbadCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"


AImbadProjectile::AImbadProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AImbadProjectile::OnHit);		// set up a notification for when this component hits something blocking
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AImbadProjectile::AImbadProjectile::OverlapBegin);

	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	
	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	IgnoreOwner();

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AImbadProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
	}
	// Bullet hits Character
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherActor != Owner) && (Cast<ACharacter>(OtherActor)))
	{
		if(Damage != NULL)
		{
			OtherActor->TakeDamage(Damage, FPointDamageEvent(Damage, Hit, Hit.ImpactPoint, nullptr), nullptr, this);
		}
	} 
	Destroy();
}


void AImbadProjectile::IgnoreWhenMoving(AActor* Actor)
{
	if(Actor == nullptr)
	{
		UE_LOG(LogClass, Warning, TEXT("Actor is NULL???"));
	}
	else
		CollisionComp->IgnoreActorWhenMoving(Actor, true);
	
}

void AImbadProjectile::IgnoreOwner()
{
	CollisionComp->IgnoreActorWhenMoving(GetOwner(), true);
}
