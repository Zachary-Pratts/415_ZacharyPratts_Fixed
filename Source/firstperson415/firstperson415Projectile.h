// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "firstperson415Projectile.generated.h" 

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS(config = Game)
class Afirstperson415Projectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ballMesh; //week 2 - The 3D ball shape.

	UPROPERTY(EditAnywhere)
	UMaterial* baseMat;  //week 2 - The splatter sticker material.

	UPROPERTY()
	FLinearColor randColor;  //week 2 - The saved random color.

	UPROPERTY(EditAnywhere)
	UMaterialInterface* projMat;  //week 2 - The ball's base material.

	UPROPERTY()
	UMaterialInstanceDynamic* dmiMat;  //week 2 - The changeable color material.

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* colorP; //week 3 - The paint splatter particle system coloring 

public:
	Afirstperson415Projectile();

protected:
	virtual void BeginPlay();

public:

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};