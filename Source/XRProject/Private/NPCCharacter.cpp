// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"


ANPCCharacter::ANPCCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

