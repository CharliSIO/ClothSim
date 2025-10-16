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

	void ApplyForce(FVector _force);

	void ApplyGravity(float _deltaTime);

	void Update(float _deltaTime);

	void OffsetPosition(FVector _offset);


	bool SharesBaseConstraint(ClothParticle* _otherParticle);
	inline FVector GetPosition() const { return Position; }
	inline TArray<ClothConstraint*> GetConstraints() const { return Constraints; }
	inline bool IsFixedInPlace() const { return FixedInPlace; }

	inline void SetFixedInPlaced(bool _isFixed) { FixedInPlace = _isFixed; }

protected:

	TArray<ClothConstraint*> Constraints;

	FVector Position = { 0.0f, 0.0f, 0.0f };
	FVector OldPosition = { 0.0f, 0.0f, 0.0f };
	FVector Acceleration = { 0.0f, 0.0f, 0.0f };

	float Mass = 1.0f;
	float Damping = 0.01f;
	bool FixedInPlace = false;
	bool OnGround = false;
};
