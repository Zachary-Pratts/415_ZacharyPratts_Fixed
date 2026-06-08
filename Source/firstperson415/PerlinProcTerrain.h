// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinProcTerrain.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class FIRSTPERSON415_API APerlinProcTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerlinProcTerrain();

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0)) // The size of the terrain in the X direction
	int XSize = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0)) // The size of the terrain in the Y direction
	int YSize = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0)) // The multiplier for the height of the terrain in the Z direction
	float ZMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0)) // The scale of the noise used to generate the terrain
	float NoiseScale = 1.0f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001)) // The scale of the terrain in the X and Y directions
	float Scale = 0;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001)) // The scale of the UV coordinates for texturing the terrain
	float UVScale = 0;

	UPROPERTY(EditAnywhere) // The material to apply to the terrain
	float radius;

	UPROPERTY(EditAnywhere) // The depth of the terrain
	FVector Depth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere) 
	UMaterialInterface* Mat; // The material to apply to the terrain

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AlterMesh(FVector impactPoint); // Function to alter the mesh based on an impact point

private:
	UProceduralMeshComponent* ProcMesh; // The procedural mesh component that will be used to create the terrain
	TArray<FVector> Vertices; // The vertices of the terrain mesh
	TArray<int> Triangles; // The indices of the vertices that make up the triangles of the terrain mesh
	TArray<FVector2D> UV0; // The UV coordinates for texturing the terrain mesh
	TArray<FVector> Normals; // The normals for the terrain mesh
	TArray<FColor> UpVertexColors; // The vertex colors for the terrain mesh

	int sectionID = 0;

	void CreateVertices();
	void CreateTriangles();

};
