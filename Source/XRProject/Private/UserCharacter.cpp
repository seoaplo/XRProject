// Fill out your copyright notice in the Description page of Project Settings.


#include "UserCharacter.h"

// Sets default values
AUserCharacter::AUserCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUserCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AUserCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUserCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

