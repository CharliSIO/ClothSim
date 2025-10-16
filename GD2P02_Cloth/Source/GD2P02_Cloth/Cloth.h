// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"

#include "Cloth.generated.h"

// Forward declarations
class ClothParticle;
class ClothConstraint;

UCLASS()
class GD2P02_CLOTH_API ACloth : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACloth();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateParticles();
	void CreateConstraints();

	// Run on tick to generate mesh
	void GenerateMesh();

	void TryCreateTriangles(ClothParticle* _topLeft, ClothParticle* _topRight,
		ClothParticle* _bottomLeft, ClothParticle* _bottomRight, int _topLeftIndex);

	void FixedUpdate();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void DropCloth();

	//TODO: Tear cloth. Release constraints between particles
protected:

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* RootSceneComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UProceduralMeshComponent* ClothMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Materials)
	UMaterial* ClothMaterial = nullptr;


	TArray<FVector>				ClothVertices;
	TArray<int32>				ClothTriangles; // Indices
	TArray<FVector>				ClothNormals;
	TArray<FProcMeshTangent>	ClothTangents;
	TArray<FVector2D>			ClothUVs;
	TArray<FLinearColor>		ClothColours;

	TArray<TArray<ClothParticle*>> ClothParticles;
	TArray <ClothConstraint*> ClothConstraints;


	float ClothWidth = 150.0f;
	float ClothHeight = 200.0f;
	int NumHorizontalParticles = 15;
	int NumVerticalParticles = 20;

	// Simulation properties ---
	FTimerHandle SimulationTimer;

	UPROPERTY(EditDefaultsOnly, Category = Simulation)
	float TimeStep = 0.01666f; // 1/60 time step
	UPROPERTY(EditAnywhere, Category = Simulation)
	int ConstraintIterations = 3;

	UPROPERTY(BlueprintReadWrite)
	FRotator WindAngle = { 0.0f, 0.0f, 0.0f };

	UPROPERTY(BlueprintReadWrite)
	float WindStrength = 30.0f;
};
