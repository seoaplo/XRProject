#include "PlayerCharacter.h"
#include "Components/InputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	RotateSpeed = 1.0f;
	NextRotation = FRotator(0.0f, 0.0f, 0.0f);
	SpringArmLocation = FVector(100.0f, 0.0f, 0.0f);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetRelativeLocation(SpringArmLocation);
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bInheritYaw = false;
	SpringArmComponent->bInheritPitch = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

#pragma region TESTCODE
	Equipments.BodyComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Equipments.BodyComponent->SetupAttachment(RootComponent);
	auto Mesh = 
		ConstructorHelpers::FObjectFinder<USkeletalMesh>(TEXT("SkeletalMesh'/Game/Resources/Character/PlayerCharacter/Mesh/Body/SK_human_body_newbie_male.SK_human_body_newbie_male'"));
	Equipments.BodyComponent->SetSkeletalMesh(Mesh.Object);
#pragma endregion
}

APlayerCharacter::~APlayerCharacter()
{
}

void APlayerCharacter::ChangeEquipment(EEquipmentsType Types, UItemEquipment * Item)
{



}

void APlayerCharacter::Tick(float deltatime)
{
	DeltaRotation = NextRotation - GetActorRotation();
	
	if (DeltaRotation.IsNearlyZero() == false)
	{
		AdditionalRotationValue = DeltaRotation * deltatime * RotateSpeed + GetActorRotation();
		SetActorRotation(AdditionalRotationValue.Quaternion()); //nextrotation * deltatime + current_rotation
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &APlayerCharacter::MoveForward);

}

void APlayerCharacter::MoveForward(float Value)
{
	float sign = 1.0f;
	if (Value < 0.0f)
		sign = -1.0f;

	if((FMath::IsNearlyZero(Value)  == false))
		NextRotation = (CameraComponent->GetForwardVector() * sign).Rotation();

	AddMovementInput(GetActorForwardVector(), Value);
}

void APlayerCharacter::MoveRight(float Value)
{
	float sign = 1.0f;
	if (Value < 0.0f)
		sign = -1.0f;

	if ((FMath::IsNearlyZero(Value) == false))
		NextRotation = (CameraComponent->GetRightVector() * sign).Rotation();
	AddMovementInput(GetActorRightVector(), Value);
}