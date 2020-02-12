// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterStatComponent.h"

float UPlayerCharacterStatComponent::GetMaxStamina()
{
	return MaxStamina;
}

int32 UPlayerCharacterStatComponent::GetLevel()
{
	return Level;
}

int UPlayerCharacterStatComponent::GetSTR()
{
	return STR;
}

int UPlayerCharacterStatComponent::GetDEX()
{
	return DEX;
}

int UPlayerCharacterStatComponent::GetINT()
{
	return INT;
}

int UPlayerCharacterStatComponent::GetGender()
{
	return Gender;
}

void UPlayerCharacterStatComponent::SetGender(int32 Gender)
{
	this->Gender = Gender;
}

void UPlayerCharacterStatComponent::SetLevel(int32 Level_)
{
	Level = Level_;
}

void UPlayerCharacterStatComponent::SetSTR(int32 STR_)
{
	STR = STR_;
}

void UPlayerCharacterStatComponent::SetDEX(int32 DEX_)
{
	DEX = DEX_;
}

void UPlayerCharacterStatComponent::SetINT(int32 INT_)
{
	INT = INT_;
}

void UPlayerCharacterStatComponent::AddStamina(float Value)
{
	CurrentStamina += Value;
	CurrentStamina = FMath::Max(Value, MaxStamina);
	OnStatChange.Broadcast();
}

void UPlayerCharacterStatComponent::SubtractStamina(float Value)
{
	CurrentStamina -= Value;
	CurrentStamina = FMath::Max(CurrentStamina, 0.0f);
	OnStatChange.Broadcast();
}

const float UPlayerCharacterStatComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

void UPlayerCharacterStatComponent::SetCurrentStamina(float Stamina)
{
	CurrentStamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
	OnStatChange.Broadcast();
}

void UPlayerCharacterStatComponent::SetMaxStamina(float Stamina)
{
	MaxStamina = FMath::Max(Stamina, 0.f);
	OnStatChange.Broadcast();
}


bool UPlayerCharacterStatComponent::GetStatDataFromServer(InputStream& input)
{
	Super::GetStatDataFromServer(input);
	std::string Name = input.ReadCString();
	Level = input.ReadInt32();
	Gender = input.ReadInt32();
	int FaceID = input.ReadInt32();
	int HairID = input.ReadInt32();
	STR = input.ReadInt32();
	DEX = input.ReadInt32();
	INT = input.ReadInt32();
	CurrentStamina = input.ReadFloat32();
	MaxStamina = input.ReadFloat32();

	return true;
}