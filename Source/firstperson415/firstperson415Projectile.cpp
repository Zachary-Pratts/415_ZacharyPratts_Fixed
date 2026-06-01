// Copyright Epic Games, Inc. All Rights Reserved.

#include "firstperson415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <Kismet/GameplayStatics.h>

Afirstperson415Projectile::Afirstperson415Projectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &Afirstperson415Projectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball Mesh")); //week 2 - Makes the 3D ball shape.

	// Set as root component
	RootComponent = CollisionComp; //week2 - Sets the main collision piece.

	ballMesh->SetupAttachment(CollisionComp); //week 2 - Glues ball to collision.

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void Afirstperson415Projectile::BeginPlay() //week2 - Runs when game starts.
{
	Super::BeginPlay(); //week2 - Does normal start stuff.
	randColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 1.f); //week2 - Picks a random paint color.

	dmiMat = UMaterialInstanceDynamic::Create(projMat, this); //week2 - Makes a changeable color material.
	ballMesh->SetMaterial(0, dmiMat); //week2 - Paints the ball mesh.

	dmiMat->SetVectorParameterValue("ProjColor", randColor); //week2 - Sets the ball's color.
}

void Afirstperson415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) //week2 - Runs when hitting something.
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics()) //week2 - Did it hit movable object?
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation()); //week2 - Pushes the hit object hard.

		Destroy(); //week2 - Deletes the ball completely.
	}

	if (OtherActor != nullptr)
	{
		if (colorP)
		{
			UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, HitComp, NAME_None, FVector(-20.f, 0.f, 0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);
			particleComp->SetNiagaraVariableLinearColor(FString("RandomColor"), randColor);
			ballMesh->DestroyComponent();
			CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
		}

		float frameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f); //week2 - Picks random splatter shape.

		auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.f, 40.f)), Hit.Location, Hit.Normal.Rotation(), 0.f); //week2 - Spawns paint splatter Texture.
		auto MatInstance = Decal->CreateDynamicMaterialInstance(); //week2 - Makes texture color changeable.

		MatInstance->SetVectorParameterValue("Color", randColor); //week2 - Paints texture random color.
		MatInstance->SetScalarParameterValue("Frame", frameNum); //week2 - Sets random splatter shape.



	}
}