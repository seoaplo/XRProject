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
	CurrentStamina = FMath::Max(CurrentStamina, MaxStamina);
	OnStatChange.Broadcast();
}

void UPlayerCharacterStatComponent::SubtractStamina(float Value)
{
	CurrentStamina -= Value;
	CurrentStamina = FMath::Max(CurrentStamina, 0.0f);
	OnStatChange.Broadcast();
}

void UPlayerCharacterStatComponent::SetMaxStamina(float Stamina)
{

	MaxStamina = FMath::Max(Stamina, 0.0f);
	if (UHealthBarWidget::GetInatance())
		UHealthBarWidget::GetInatance()->SetMaxStamina(Stamina);

	OnStatChange.Broadcast();
}

const float UPlayerCharacterStatComponent::GetCurrentStamina() const
{
	return CurrentStamina;
}

void UPlayerCharacterStatComponent::SetCurrentStamina(float Stamina)
{
	CurrentStamina = FMath::Clamp(Stamina, 0.f, MaxStamina);
	XRLOG(Warning, TEXT("Stamina Set %f"), Stamina);
	if(UHealthBarWidget::GetInatance())
		UHealthBarWidget::GetInatance()->ApplyStamina(Stamina);
	OnStatChange.Broadcast();
}

void UPlayerCharacterStatComponent::AddExp(int32 Value)
{
	CurrentExp += Value;
	CurrentExp = FMath::Max(CurrentExp, MaxExp);
	OnStatChange.Broadcast();
}

void UPlayerCharacterStatComponent::SubtractExp(int32 Value)
{
	CurrentExp -= Value;
	CurrentExp = FMath::Max(CurrentExp, 0);
	OnStatChange.Broadcast();
}

int32 UPlayerCharacterStatComponent::GetCurrentExp()
{
	return CurrentExp;
}

void UPlayerCharacterStatComponent::SetCurrentExp(int32 Exp)
{
	CurrentExp = FMath::Clamp(Exp, 0, MaxExp);
	if(UExpBar::GetInstance())
		UExpBar::GetInstance()->SetCurrentExp(Exp);
	OnStatChange.Broadcast();
}

void UPlayerCharacterStatComponent::SetMaxExp(int32 Exp)
{
	MaxExp = FMath::Max(Exp, 0);
	if (UExpBar::GetInstance())
		UExpBar::GetInstance()->SetMaxExp(Exp);
	OnStatChange.Broadcast();
}

int32 UPlayerCharacterStatComponent::GetMaxExp()
{
	return MaxExp;
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