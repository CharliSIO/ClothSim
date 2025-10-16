// Fill out your copyright notice in the Description page of Project Settings.


#include "Cloth.h"

#include "ClothParticle.h"
#include "ClothConstraint.h"
#include "Kismet/GameplayStatics.h"
#include "KismetProceduralMeshLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACloth::ACloth()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>("Root Scene Component");
	RootComponent = RootSceneComponent;

	ClothMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh");
	ClothMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACloth::BeginPlay()
{
	Super::BeginPlay();

	ClothMesh->SetMaterial(0, ClothMaterial);

	CreateParticles();
	CreateConstraints();

	GenerateMesh();

	// Create a looping timer that calls the fixed update function
	GetWorldTimerManager().SetTimer(SimulationTimer, this, &ACloth::FixedUpdate, TimeStep, true);
}

void ACloth::CreateParticles()
{
	float HorDist = ClothWidth / (NumHorizontalParticles - 1);
	float VertDist = ClothHeight / (NumVerticalParticles - 1);

	FVector StartPos
	{
		-ClothWidth / 2,
		0.0f,
		ClothHeight / 2,
	};

	for (int Vert = 0; Vert < NumVerticalParticles; Vert++)
	{
		TArray<ClothParticle*> ParticleRow;

		for (int Horiz = 0; Horiz < NumHorizontalParticles; Horiz++)
		{
			FVector ParticlePos = { StartPos.X + Horiz * HorDist, StartPos.Y, StartPos.Z - Vert * VertDist };

			DrawDebugPoint(GetWorld(), ParticlePos, 5.0f, FColor::Green, true);

			bool FixedInPlace = Vert == 0 && (Horiz == 0 || Horiz == NumHorizontalParticles - 1); // Update to make sure not whole top row is fixed in place

			ClothParticle* NewParticle = new ClothParticle(ParticlePos, FixedInPlace);

			ParticleRow.Add(NewParticle);
		}

		ClothParticles.Add(ParticleRow);
	}
}

void ACloth::CreateConstraints()
{
	for (int Vert = 0; Vert < NumVerticalParticles; Vert++)
	{
		for (int Horiz = 0; Horiz < NumHorizontalParticles; Horiz++)
		{
			if (Vert < NumVerticalParticles - 1)
			{
				ClothConstraint* NewConstraint = new ClothConstraint(ClothParticles[Vert][Horiz], ClothParticles[Vert + 1][Horiz]);
				ClothConstraints.Add(NewConstraint);

				ClothParticles[Vert][Horiz]->AddConstraint(NewConstraint);
				ClothParticles[Vert + 1][Horiz]->AddConstraint(NewConstraint);

				DrawDebugLine(GetWorld(), ClothParticles[Vert][Horiz]->GetPosition(), ClothParticles[Vert + 1][Horiz]->GetPosition(), FColor::Blue, true);
			}

			if (Horiz < NumHorizontalParticles - 1)
			{
				ClothConstraint* NewConstraint = new ClothConstraint(ClothParticles[Vert][Horiz], ClothParticles[Vert][Horiz + 1]);
				ClothConstraints.Add(NewConstraint);

				ClothParticles[Vert][Horiz]->AddConstraint(NewConstraint);
				ClothParticles[Vert][Horiz + 1]->AddConstraint(NewConstraint);

				DrawDebugLine(GetWorld(), ClothParticles[Vert][Horiz]->GetPosition(), ClothParticles[Vert][Horiz + 1]->GetPosition(), FColor::Blue, true);
			}

			// Interwoven constraints
			if (Vert < NumVerticalParticles - 2)
			{
				ClothConstraint* NewConstraint = new ClothConstraint(ClothParticles[Vert][Horiz], ClothParticles[Vert + 2][Horiz], true);
				ClothConstraints.Add(NewConstraint);

				ClothParticles[Vert][Horiz]->AddConstraint(NewConstraint);
				ClothParticles[Vert + 2][Horiz]->AddConstraint(NewConstraint);

				DrawDebugLine(GetWorld(), ClothParticles[Vert][Horiz]->GetPosition(), ClothParticles[Vert + 2][Horiz]->GetPosition(), FColor::Green, true);
			}

			if (Horiz < NumHorizontalParticles - 2)
			{
				ClothConstraint* NewConstraint = new ClothConstraint(ClothParticles[Vert][Horiz], ClothParticles[Vert][Horiz + 2], true);
				ClothConstraints.Add(NewConstraint);

				ClothParticles[Vert][Horiz]->AddConstraint(NewConstraint);
				ClothParticles[Vert][Horiz + 2]->AddConstraint(NewConstraint);

				DrawDebugLine(GetWorld(), ClothParticles[Vert][Horiz]->GetPosition(), ClothParticles[Vert][Horiz + 2]->GetPosition(), FColor::Green, true);
			}
			if (Vert < NumVerticalParticles - 3)
			{
				ClothConstraint* NewConstraint = new ClothConstraint(ClothParticles[Vert][Horiz], ClothParticles[Vert + 3][Horiz], true);
				ClothConstraints.Add(NewConstraint);

				ClothParticles[Vert][Horiz]->AddConstraint(NewConstraint);
				ClothParticles[Vert + 3][Horiz]->AddConstraint(NewConstraint);

				DrawDebugLine(GetWorld(), ClothParticles[Vert][Horiz]->GetPosition(), ClothParticles[Vert + 3][Horiz]->GetPosition(), FColor::Red, true);
			}

			if (Horiz < NumHorizontalParticles - 3)
			{
				ClothConstraint* NewConstraint = new ClothConstraint(ClothParticles[Vert][Horiz], ClothParticles[Vert][Horiz + 3], true);
				ClothConstraints.Add(NewConstraint);

				ClothParticles[Vert][Horiz]->AddConstraint(NewConstraint);
				ClothParticles[Vert][Horiz + 3]->AddConstraint(NewConstraint);

				DrawDebugLine(GetWorld(), ClothParticles[Vert][Horiz]->GetPosition(), ClothParticles[Vert][Horiz + 3]->GetPosition(), FColor::Red, true);
			}
		}
	}
}

// Regenerate the mesh on tick
void ACloth::GenerateMesh()
{
	ClothVertices.Reset();
	ClothTriangles.Reset();
	ClothNormals.Reset();
	ClothTangents.Reset();
	ClothUVs.Reset();
	ClothColours.Reset();
	
	for (int Vert = 0; Vert < NumVerticalParticles; Vert++)
	{
		for (int Horiz = 0; Horiz < NumHorizontalParticles; Horiz++)
		{
			ClothVertices.Add(ClothParticles[Vert][Horiz]->GetPosition());

			FLinearColor ParticleColour = FLinearColor::Black;
			ClothColours.Add(ParticleColour);

			ClothUVs.Add(FVector2D(float(Horiz) / (NumHorizontalParticles - 1), float(Vert) / (NumVerticalParticles - 1)));

			if (Vert < NumVerticalParticles - 1 && Horiz < NumHorizontalParticles - 1)
			{
				TryCreateTriangles(ClothParticles[Vert][Horiz], ClothParticles[Vert][Horiz + 1],
					ClothParticles[Vert + 1][Horiz], ClothParticles[Vert + 1][Horiz + 1],
					Vert * NumHorizontalParticles + Horiz);
			}
		}
	}

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(ClothVertices, ClothTriangles, ClothUVs, ClothNormals, ClothTangents);

	ClothMesh->CreateMeshSection_LinearColor(0, ClothVertices, ClothTriangles, ClothNormals, ClothUVs, ClothColours, ClothTangents, false);
}

void ACloth::TryCreateTriangles(ClothParticle* _topLeft, ClothParticle* _topRight, ClothParticle* _bottomLeft, ClothParticle* _bottomRight, int _topLeftIndex)
{
	int TopRightIndex = _topLeftIndex + 1;
	int BottomLeftIndex = _topLeftIndex + NumHorizontalParticles;
	int BottomRightIndex = BottomLeftIndex + 1;

	if (_topLeft->SharesBaseConstraint(_topRight) && _topLeft->SharesBaseConstraint(_bottomLeft))
	{
		ClothTriangles.Add(_topLeftIndex);
		ClothTriangles.Add(TopRightIndex);
		ClothTriangles.Add(BottomLeftIndex);

		if (_bottomRight->SharesBaseConstraint(_topRight) && _bottomRight->SharesBaseConstraint(_bottomLeft))
		{
			ClothTriangles.Add(TopRightIndex);
			ClothTriangles.Add(BottomRightIndex);
			ClothTriangles.Add(BottomLeftIndex);
		}
	}
	else if (_topLeft->SharesBaseConstraint(_bottomLeft) && _bottomLeft->SharesBaseConstraint(_bottomRight))
	{
		ClothTriangles.Add(_topLeftIndex);
		ClothTriangles.Add(BottomRightIndex);
		ClothTriangles.Add(BottomLeftIndex);

		if (_topRight->SharesBaseConstraint(_topLeft) && _topRight->SharesBaseConstraint(_bottomRight))
		{
			ClothTriangles.Add(TopRightIndex);
			ClothTriangles.Add(BottomRightIndex);
			ClothTriangles.Add(_topLeftIndex);
		}
	}
}

// Called repeatedly on a timer at 60fps
void ACloth::FixedUpdate()
{
	for (int Vert = 0; Vert < NumVerticalParticles; Vert++)
	{
		for (int Horiz = 0; Horiz < NumHorizontalParticles; Horiz++)
		{
			ClothParticles[Vert][Horiz]->ApplyGravity(TimeStep);

			// Wind
			ClothParticles[Vert][Horiz]->ApplyForce(WindAngle.Vector() * WindStrength);

			// Pulling

			ClothParticles[Vert][Horiz]->Update(TimeStep);
		}
	}

	for (int i = 0; i < ConstraintIterations; i++)
	{
		for (ClothConstraint* currentConstraint : ClothConstraints)
		{
			// TODO: run update function
			currentConstraint->Update(TimeStep);
		}
	}

	GenerateMesh();
}

// Called every frame
void ACloth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACloth::DropCloth()
{
	for (int Vert = 0; Vert < NumVerticalParticles; Vert++)
	{
		for (int Horiz = 0; Horiz < NumHorizontalParticles; Horiz++)
		{
			ClothParticles[Vert][Horiz]->SetFixedInPlaced(false);
		}
	}
}

