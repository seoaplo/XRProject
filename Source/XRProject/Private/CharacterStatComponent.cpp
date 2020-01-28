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
	 if (Current_HP >= 0.f)
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
	 Max_HP = FMath::Min(NewHP, 0.f); 
	 OnStatChange.Broadcast();
 }


 const float UCharacterStatComponent::GetAttack_Min() const { return Attack_Min; }

 void UCharacterStatComponent::SetAttack_Min(float NewAttack_Min) { 
	 Attack_Min = FMath::Min(NewAttack_Min, 0.f); 
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetAttack_Max() const { return Attack_Max; }

 void UCharacterStatComponent::SetAttack_Max(float NewAttack_Max) { 
	 Attack_Max = NewAttack_Max; 
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetAttack_Range() const { return Attack_Range; }

 void UCharacterStatComponent::SetAttack_Range(float NewAttack_Range) { 
	 Attack_Range = FMath::Min(NewAttack_Range, 0.f);
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetAttack_Speed() const { return Attack_Speed; }

 void UCharacterStatComponent::SetAttack_Speed(float NewAttack_Speed) { 
	 Attack_Speed = FMath::Min(NewAttack_Speed, 0.f);
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetSpeed() const { return Speed; }

 void UCharacterStatComponent::SetSpeed(float NewSpeed) { 
	 Speed = FMath::Min(NewSpeed, 0.f); 
	 OnStatChange.Broadcast();
 }

 const float UCharacterStatComponent::GetDefence() const { return Defence; }

 void UCharacterStatComponent::SetDefence(float NewDefence) { 
	 Defence = NewDefence; 
	 OnStatChange.Broadcast();
 }


// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

