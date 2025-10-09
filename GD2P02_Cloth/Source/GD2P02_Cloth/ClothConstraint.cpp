// Fill out your copyright notice in the Description page of Project Settings.


#include "ClothConstraint.h"
#include "ClothParticle.h"

ClothConstraint::ClothConstraint(ClothParticle* _particleA, ClothParticle* _particleB)
{
	ParticleA = _particleA;
	ParticleB = _particleB;

	RestingDistance = FVector::Dist(_particleA->GetPosition(), _particleB->GetPosition());
}

ClothConstraint::~ClothConstraint()
{
}

void ClothConstraint::Update(float _deltaTime)
{
	FVector currentOffset = ParticleB->GetPosition() - ParticleA->GetPosition();

	FVector correction = currentOffset * (1.0f - RestingDistance / currentOffset.Size());
	FVector halfCorrection = correction * 0.5f;

	if (!ParticleA->IsFixedInPlace() && !ParticleB->IsFixedInPlace())
	{
		ParticleA->OffsetPosition(halfCorrection);
		ParticleB->OffsetPosition(-halfCorrection);
	}
	else if (!ParticleA->IsFixedInPlace())
	{
		ParticleA->OffsetPosition(correction);
	}
	else if (!ParticleB->IsFixedInPlace())
	{
		ParticleB->OffsetPosition(-correction);
	}
}
