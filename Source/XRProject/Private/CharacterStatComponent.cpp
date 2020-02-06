// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

const float UCharacterStatComponent::GetCurrentHP() const { return Current_HP; }

 void UCharacterStatComponent::SetCurrentHP(float NewHP) { 
	 Current_HP = FMath::Clamp(NewHP, 0.f, Max_HP);
	 if (Current_HP <= 0.f)
	 {
		 OnHPZero.Broadcast();
	 }
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetHPRatio() const 
 { 
	 if (FMath::IsNearlyZero(Max_HP))
	 {
		 XRLOG(Warning, TEXT("MaxHP Attemp to Zero devide, Need Set MaxHP"))
	 }
	 else
	 {
		 return Current_HP / Max_HP;
	 }
	 return 0.f;
 }


 const float UCharacterStatComponent::GetMaxHP() const { return Max_HP; }

 void UCharacterStatComponent::SetMaxHP(float NewHP) {
	 Max_HP = FMath::Max(NewHP, 0.f); 
	 OnStatChange.Broadcast();
 }


 const float UCharacterStatComponent::GetAttack_Min() const { return Attack_Min; }

 void UCharacterStatComponent::SetAttack_Min(float NewAttack_Min) { 
	 Attack_Min = FMath::Max(NewAttack_Min, 0.f);
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetAttack_Max() const { return Attack_Max; }

 void UCharacterStatComponent::SetAttack_Max(float NewAttack_Max) { 
	 Attack_Max = NewAttack_Max; 
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetAttack_Range() const { return Attack_Range; }

 void UCharacterStatComponent::SetAttack_Range(float NewAttack_Range) { 
	 Attack_Range = FMath::Max(NewAttack_Range, 0.f);
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetAttack_Speed() const { return Attack_Speed; }

 void UCharacterStatComponent::SetAttack_Speed(float NewAttack_Speed) { 
	 Attack_Speed = FMath::Max(NewAttack_Speed, 0.f);
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetSpeed() const { return Speed; }

 void UCharacterStatComponent::SetSpeed(float NewSpeed) { 
	 Speed = FMath::Max(NewSpeed, 0.f);
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetDefence() const { return Defence; }

 void UCharacterStatComponent::SetDefence(float NewDefence) { 
	 Defence = NewDefence; 
	 OnStatChange.Broadcast();
 }

 bool UCharacterStatComponent::GetStatDataFromServer(InputStream& input)
 {
	Current_HP = input.ReadFloat32();
	Max_HP = input.ReadFloat32();
	Attack_Min = input.ReadFloat32();
	Attack_Max = input.ReadFloat32();
	Attack_Range = input.ReadFloat32();
	Attack_Speed = input.ReadFloat32();
	Speed = input.ReadFloat32();
	Defence = input.ReadFloat32();

	return true;
 }

 void UCharacterStatComponent::AddHP(float Value)
 {
	 Current_HP += Value;
	 Current_HP = FMath::Max(Value, Max_HP);
	 OnStatChange.Broadcast();
 }

 void UCharacterStatComponent::SubtractHP(float Value)
 {
	 Current_HP -= Value;
	 Current_HP = FMath::Max(Current_HP, 0.0f);
	 if (Current_HP <= 0.0f)
	 {
		 OnHPZero.Broadcast();
	 }
	 OnStatChange.Broadcast();
 }


// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

