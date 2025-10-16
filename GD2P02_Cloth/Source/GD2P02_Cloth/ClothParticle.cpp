// Fill out your copyright notice in the Description page of Project Settings.


#include "ClothParticle.h"
#include "ClothConstraint.h"

ClothParticle::ClothParticle(FVector _position, bool _fixedInPlace)
{
	Position = _position;
	OldPosition = _position;
	FixedInPlace = _fixedInPlace;
}

ClothParticle::~ClothParticle()
{
}

void ClothParticle::AddConstraint(ClothConstraint* _constraint)
{
	Constraints.Add(_constraint);
}

bool ClothParticle::SharesBaseConstraint(ClothParticle* _otherParticle)
{
	for (auto iter : _otherParticle->GetConstraints())
	{
		if (Constraints.Contains(iter))
		{
			if (iter->IsInterwoven())
			{
				return false;
			}

			return true;
		}
	}
	return false;
}

void ClothParticle::ApplyForce(FVector _force)
{
	if (FixedInPlace) return;

	Acceleration += _force / Mass;
}

void ClothParticle::ApplyGravity(float _deltaTime)
{
	ApplyForce(FVector(0.0f, 0.0f, -981.0f) * Mass * _deltaTime);
}

void ClothParticle::Update(float _deltaTime)
{
	if (FixedInPlace) return;

	FVector tempPos = Position;
	Position += (Position - OldPosition) * (1.0f - Damping) + Acceleration * _deltaTime;
	OldPosition = tempPos;

	Acceleration = FVector::ZeroVector;
}

void ClothParticle::OffsetPosition(FVector _offset)
{
	Position += _offset;
}
