// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ClothConstraint;

/**
 * 
 */
class GD2P02_CLOTH_API ClothParticle
{
public:
	ClothParticle(FVector _position, bool _fixedInPlace = false);
	~ClothParticle();

	void AddConstraint(ClothConstraint* _constraint);

	inline FVector GetPosition() const { return Position; }
	inline TArray<ClothConstraint*> GetConstraints() const { return Constraints; }

	bool SharesConstraint(ClothParticle* _otherParticle);

protected:

	TArray<ClothConstraint*> Constraints;

	FVector Position = { 0.0f, 0.0f, 0.0f };
	FVector OldPosition = { 0.0f, 0.0f, 0.0f };
	FVector Acceleration = { 0.0f, 0.0f, 0.0f };

	float Mass = 1.0f;
	float Daming = 0.01f;
	bool FixedInPlace = false;
	bool OnGround = false;
};
