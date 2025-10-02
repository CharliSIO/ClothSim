// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ClothParticle;

/**
 * 
 */
class GD2P02_CLOTH_API ClothConstraint
{
public:
	ClothConstraint(ClothParticle* _particleA, ClothParticle* _particleB);
	~ClothConstraint();

	void Update(float _deltaTime);

protected:
	ClothParticle* ParticleA = nullptr;
	ClothParticle* ParticleB = nullptr;

	float RestingDistance = 10.0f;

	bool Interwoven = false;
};
