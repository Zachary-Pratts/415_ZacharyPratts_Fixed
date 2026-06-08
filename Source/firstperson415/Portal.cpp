// Copyright Epic Games, Inc. All Rights Reserved.

#include "Portal.h"
#include "firstperson415Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Comp"));
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capture"));
	rootArrow = CreateDefaultSubobject<UArrowComponent>("Root Arrow");

	RootComponent = boxComp; // Sets the main collision piece.
	mesh->SetupAttachment(boxComp); // attacthes mesh to collision.
	sceneCapture->SetupAttachment(mesh); //attaches the scene capture to the mesh so it moves with the portal
	rootArrow->SetupAttachment(RootComponent); //attaches the root arrow to the root component so it moves with the portal

	mesh->SetCollisionResponseToAllChannels(ECR_Ignore); //disables collision for the portal mesh so it doesn't interfere with the player movement

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	mesh->SetHiddenInSceneCapture(true);
	//mesh->bCastStaticShadow(false);
	//mesh->bCastDynamicShadow(false);

	if (mat)
	{
		mesh->SetMaterial(0, mat); //cehcks if material is valid and sets it to the portal mesh
	}
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePortals(); //gives the illusion of looking through the portal by updating the scene capture every frame to match the position and rotation of the other portal
}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Afirstperson415Character* playerChar = Cast<Afirstperson415Character>(OtherActor);

	if (playerChar) //check if cast to playercharacter is valid
	{
		if (OtherPortal) //check if the other portal is valid
		{
			if (!playerChar->isTeleporting) //checking if teliporting is false so we don't get stuck in a loop of teleporting back and forth between the two portals
			{
				playerChar->isTeleporting = true; // if false were setting it to true. if we dont do this and i overlap the portal again while teleporting, it will teleport us back to the original portal and create an infinite loop of teleporting back and forth between the two portals
				FVector loc = OtherPortal->rootArrow->GetComponentLocation(); //getting location of other portal.
				playerChar->SetActorLocation(loc);

				FTimerHandle TimerHandle; //creating a timer handle to set the bool back to false after a short delay so we can teleport again if we want to
				FTimerDelegate TimerDelegate; //creating a timer delegate to bind the function that will set the bool back to false after the delay
				TimerDelegate.BindUFunction(this, "SetBool", playerChar); //binding the timer delegate to the SetBool function, which will set the isTeleporting bool back to false after the delay
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false); //setting the timer to call the SetBool function after 1 second, which will set the isTeleporting bool back to false and allow us to teleport again if we want to
			}
		}
	}
}

void APortal::SetBool(Afirstperson415Character* playerChar)
{
	if (playerChar)
	{
		playerChar->isTeleporting = false; //setting the isTeleporting bool back to false after the delay so we can teleport again if we want to
	}
}

void APortal::UpdatePortals() //so we can settup the scene capture component.
{
	FVector Location = this->GetActorLocation() - OtherPortal->GetActorLocation(); //getting the vector between the two portals by subtracting the location of the other portal from the location of this portal. 
	FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation();//getting the location of the platyer camera.
	FRotator camRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation(); //getting the rotation of the player camera.
	FVector CombinedLocation = camLocation + Location; //Combinding them to get the location that the screen capture should give.

	sceneCapture->SetWorldLocationAndRotation(CombinedLocation, camRotation); //setting the location and rotation of the scene capture to match the combined location and the player camera rotation so it gives the illusion of looking through the portal.
}
