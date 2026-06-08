// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinProcTerrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
APerlinProcTerrain::APerlinProcTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh")); // Create a new procedural mesh component and attach it to the root component
	ProcMesh->SetupAttachment(GetRootComponent()); // Attach the procedural mesh component to the root component

}

// Called when the game starts or when spawned
void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay();

	CreateVertices(); // Call the function to create the vertices of the terrain mesh
	CreateTriangles(); // Call the function to create the triangles of the terrain mesh
	ProcMesh->CreateMeshSection(sectionID, Vertices, Triangles, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>(), true); // Create a new mesh section using the vertices, triangles, normals, UV coordinates, and vertex colors that were created
	ProcMesh->SetMaterial(0, Mat); // Set the material of the procedural mesh to the material that was specified in the editor
	
}

// Called every frame
void APerlinProcTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APerlinProcTerrain::AlterMesh(FVector impactPoint) //passing through impact point.
{
	for (int i = 0; i < Vertices.Num(); i++) //cycling through for loop checking the number of vertises in the mesh.
	{
		FVector tempVector = impactPoint - this->GetActorLocation(); //setting temporary veriable to the impact point minus the location of the terrain actor, which gives us the relative position of the impact point to the terrain mesh.

		if (FVector(Vertices[i] - tempVector).Size() < radius) //if the distance between the current vertex and the impact point is less than the specified radius, we will alter the vertex by subtracting the depth from its position.
		{
			Vertices[i] = Vertices[i] - Depth; //altering the vertex by subtracting the depth from its position, which creates a depression in the terrain mesh at the location of the impact point.
			ProcMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>()); //update the mesh section with the new vertex positions
		}
	}
}

void APerlinProcTerrain::CreateVertices() //nested for loop to create vertices for the terrain mesh based on the X and Y size of the terrain, using Perlin noise to generate the Z value for each vertex
{
	for (int X = 0; X <= XSize; X++) //loops continuously until it reaches the X size of the terrain, creating vertices for each point in the X direction
	{
		for (int Y = 0; Y <= YSize; Y++) //loops continuously until it reaches the Y size of the terrain, creating vertices for each point in the Y direction
		{
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + 0.1, Y * NoiseScale + 0.1)) * ZMultiplier; //uses float vertex heights based on Perlin noise, which creates a more natural-looking terrain. The noise is scaled by the NoiseScale variable and multiplied by the ZMultiplier to control the height of the terrain
			GEngine->AddOnScreenDebugMessage(-1, 999.0f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z));  //printing the Z value for each vertex to the screen for debugging purposes
			Vertices.Add(FVector(X * Scale, Y * Scale, Z)); // adding a vertex and multiplying the X and Y values by the Scale variable to control the overall size of the terrain in the X and Y directions
			UV0.Add(FVector2D(X * UVScale, Y * UVScale)); // adding UV coordinates for texturing the terrain mesh, multiplying the X and Y values by the UVScale variable to control the scale of the UV coordinates
		}
	}
}

void APerlinProcTerrain::CreateTriangles()
{
	int Vertex = 0;

	for (int X = 0; X < XSize; X++)
	{
		for (int Y = 0; Y < YSize; Y++)
		{
			Triangles.Add(Vertex); //adding to increase the vertex index for each triangle, creating two triangles for each square of the terrain mesh
			Triangles.Add(Vertex + 1);  
			Triangles.Add(Vertex + YSize + 1);
			Triangles.Add(Vertex + 1);
			Triangles.Add(Vertex + YSize + 2);
			Triangles.Add(Vertex + YSize + 1);

			Vertex++;
		}
		Vertex++;
	}
}

