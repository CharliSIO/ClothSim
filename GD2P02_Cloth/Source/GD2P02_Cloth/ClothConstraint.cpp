// Fill out your copyright notice in the Description page of Project Settings.


#include "ClothConstraint.h"
#include "ClothParticle.h"

ClothConstraint::ClothConstraint(ClothParticle* _particleA, ClothParticle* _particleB, bool _interwoven)
{
	ParticleA = _particleA;
	ParticleB = _particleB;

	Interwoven = _interwoven;
	if (Interwoven) ConstraintStrength = 0.75f;

	RestingDistance = FVector::Dist(_particleA->GetPosition(), _particleB->GetPosition());
}

ClothConstraint::~ClothConstraint()
{
}

void ClothConstraint::Update(float _deltaTime)
{
	FVector currentOffset = ParticleB->GetPosition() - ParticleA->GetPosition();
	float displacement = currentOffset.Size() - RestingDistance;

	//currentOffset.Normalize();
	//FVector correction = (currentOffset * displacement) * -Tightness;

	FVector correction = currentOffset * (1.0f - RestingDistance / currentOffset.Size()) * ConstraintStrength;
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
