// Copyright notice...

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Portal.generated.h"

class firstperson415Character; //so we can access player becaue we are using a local variable to transfer player location to the portal

UCLASS()
class FIRSTPERSON415_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APortal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* mesh; //the portal mesh

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneCaptureComponent2D* sceneCapture; //the scene capture component that will capture the view from the portal and render it to a texture

	UPROPERTY(EditAnywhere)
	UArrowComponent* rootArrow; //the arrow component that will be used to orient the scene capture component to match the other portal

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextureRenderTarget2D* renderTarget; //the render target that the scene capture will render to, which will be used as the material for the portal mesh

	UPROPERTY(EditAnywhere)
	UBoxComponent* boxComp; //the box component that will be used to detect when the player enters the portal

	UPROPERTY(EditAnywhere)
	APortal* OtherPortal; //gives us eye dropper to connect the two portals together in the editor

	UPROPERTY(EditAnywhere)
	UMaterialInterface* mat; //the material that will be used for the portal mesh, which will have a parameter that we will set to the render target

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetBool(Afirstperson415Character* playerChar);

	UFUNCTION()
	void UpdatePortals();

};
