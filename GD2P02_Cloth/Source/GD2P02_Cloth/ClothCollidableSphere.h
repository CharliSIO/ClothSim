// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClothCollidableSphere.generated.h"

UCLASS()
class GD2P02_CLOTH_API AClothCollidableSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClothCollidableSphere();

	// Getters
	inline float GetRadius() const { return Radius; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Sphere)
	UStaticMeshComponent* SphereMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Sphere)
	float Radius = 100.0f;

};
