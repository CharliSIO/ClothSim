// Fill out your copyright notice in the Description page of Project Settings.


#include "ClothCollidableSphere.h"

// Sets default values
AClothCollidableSphere::AClothCollidableSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("Sphere Mesh");
	RootComponent = SphereMesh;
}

// Called when the game starts or when spawned
void AClothCollidableSphere::BeginPlay()
{
	Super::BeginPlay();

	SphereMesh->SetWorldScale3D(FVector(Radius * 0.02));
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 32, FColor::Green, true, -1.0f, (uint8)0U, 6.0f);
}

// Called every frame
void AClothCollidableSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

