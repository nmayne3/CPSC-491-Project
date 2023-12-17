// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImbadProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AImbadProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* HitFX;
	
public:
	AImbadProjectile();

	/** called when projectile hits something */
	UFUNCTION(BlueprintCallable, Category = Collision)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	/** Set an Actor to be ignored by collision */
	UFUNCTION(BlueprintCallable, Category = Collision)
	void IgnoreWhenMoving(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = Collision)
	void IgnoreOwner();

	/** Amount of Damage applied on Hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile)
	float Damage;
	
	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};
