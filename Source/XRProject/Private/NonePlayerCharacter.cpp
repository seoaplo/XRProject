// Fill out your copyright notice in the Description page of Project Settings.


#include "NonePlayerCharacter.h"
#include "XRAIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/AISenseConfig_Sight.h"


ANonePlayerCharacter::ANonePlayerCharacter()
{
	EnermyStatComponent = CreateDefaultSubobject<UCharacterStatComponent>(TEXT("EnermyStat"));
	EnermyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnermySensing"));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	AIControllerClass = AXRAIController::StaticClass();

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 700.f, 0.0f);


	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = 500.f+50.f;
	SightConfig->PeripheralVisionAngleDegrees = 75.f;
	SightConfig->SetMaxAge(5.f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	EnermyPerceptionComponent->ConfigureSense(*SightConfig);

}


void ANonePlayerCharacter::PostInitializeComponents()
{
	ABaseCharacter::PostInitializeComponents();
	EnermyPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ANonePlayerCharacter::DetectTarget);


}

void ANonePlayerCharacter::BeginPlay()
{
	ABaseCharacter::BeginPlay();
	auto AICon = Cast<AXRAIController>(GetController());
	AICon->RunAI();
}


void ANonePlayerCharacter::Tick(float DeltaTime)
{
	ABaseCharacter::Tick(DeltaTime);
}

void ANonePlayerCharacter::PossessedBy(AController* Cntr)
{
	Super::PossessedBy(Cntr);

	XRLOG(Warning,TEXT("%s PossessedBy %s"),*GetName(),*Cntr->GetName())
}

float ANonePlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	XRLOG(Warning, TEXT("OnAttack"));
	return 0.f;
}

void ANonePlayerCharacter::DetectTarget(const TArray<AActor*>& DetectingPawn)
{

 	for (int DetectedNum = 0; DetectedNum < DetectingPawn.Num(); DetectedNum++ )
	{
		XRLOG(Warning, TEXT("Detected  : %s"), *DetectingPawn[DetectedNum]->GetName());
	}
}
