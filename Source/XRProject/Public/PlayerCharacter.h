// Fill out your copyright notice in the Description page of Project Settings.
/*
	�ۼ��� : �ڼ���
*/
#pragma once

#include "Engine.h"
#include "BaseCharacter.h"
#include "ItemEquipment.h"
#include "PlayerCharacterAnimInstance.h"
#include "PlayerCharacterStatComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PlayerCameraShake.h"
#include "PlayerSkillManager.h"
#include "PlayerCharacter.generated.h"

class ANonePlayerCharacter;
class UXRGameInstance;
class UXRPlayerController;
class UHealthBarWidget;
struct FXRDamageEvent;


UENUM()
enum class EEquipmentsType : uint8
{
	BODY = 0,
	HANDS,
	LEGS,
	WEAPON,
};
UENUM()
enum class EPartsType : uint8
{
	HAIR,
	FACE,
	NUDEBODY,
	NUDEHAND,
	NUDELEG,
};

/* 콤보 소드트레일에 사용되는 이펙트 소켓 구조체 */
USTRUCT(BlueprintType)
struct FComboSocket
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "C_SocketName")
		FName FXCombo1;
	UPROPERTY(EditDefaultsOnly, Category = "C_SocketName")
		FName FXCombo2;
	UPROPERTY(EditDefaultsOnly, Category = "C_SocketName")
		FName FXCombo3;
	UPROPERTY(EditDefaultsOnly, Category = "C_SocketName")
		FName FXCombo4;
	UPROPERTY(EditDefaultsOnly, Category = "C_SocketName")
		FName FXGaiaCrush;
	UPROPERTY(EditDefaultsOnly, Category = "C_SocketName")
		FName FxBottom;

	FComboSocket()
	{
		FXCombo1	= "FXCombo1";
		FXCombo2	= "FXCombo2";
		FXCombo3	= "FXCombo3";
		FXCombo4	= "FXCombo4";
		FXGaiaCrush = "FXGaiaCrush";
		FxBottom	= "FxBottom";
	}
};


/*캐릭터 장비 메시컴포넌트, 파츠별 장비 클래스 구조체*/
USTRUCT(BlueprintType)
struct FEquipment
{
	GENERATED_USTRUCT_BODY()
public:
	/*  */
	//UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* BodyComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* HandsComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Equipment")
		class USkeletalMeshComponent* LegsComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "C_Equipment")
		class UStaticMeshComponent* WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* BodyItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* HandsItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* LegsItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C_EquipmentItem")
		UItemEquipment* WeaponItem;

};


/**
 *
 */
UCLASS()
class XRPROJECT_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual ~APlayerCharacter();

	const int32 kMaxComboCount = 4; //최대 4콤보까지
	const float kSprintMovementSpeed = 750.0f;
	const float kNormalMovementSpeed = 450.0f;
	const int32 kCameraWheelSpeed = 40.0f; //Wheel 카메라 조작 이동량
	const int32 kCameraWheelMaxLimit = 550.0f;
	const int32 kCameraWheelMinLimit = 150.0f;
	const int32 kRollStamina = 10;


public:
	UPROPERTY()
		class  UAISenseConfig_Damage* AISenseDamage;

	UPROPERTY(EditInstanceOnly, Category = "C_Movement")
		float RotateSpeed;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera")
		float SpringArmLength;
	UPROPERTY(EditInstanceOnly, Category = "C_Movement")
		float MovementSpeed;
	UPROPERTY()
		bool bIsMale; //남자/여자

private:
	UPROPERTY(EditInstanceOnly, Category = "C_Collision")
		class UCapsuleComponent* HitCapsule;//Hitbox
	UPROPERTY(EditInstanceOnly, Category = "C_Widget")
		class UWidgetComponent* NameTag;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera", Meta = (AllowPrivateAccess = true))
		class UCameraComponent* CameraComponent;
	UPROPERTY(EditInstanceOnly, Category = "C_Camera")
		class USpringArmComponent* SpringArmComponent;
	UPROPERTY(EditInstanceOnly, Category = "C_Parts")
		class USkeletalMeshComponent* FaceComponent;
	UPROPERTY(EditInstanceOnly, Category = "C_Parts")
		class USkeletalMeshComponent* HairComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C_AnimInstance")
		TSubclassOf<UAnimInstance> AnimInstance; //임시 애님인스턴스
	UPROPERTY(EditDefaultsOnly, Category = "C_AnimInstance")
		TSubclassOf<UAnimInstance> FemaleAnimInstance;
	UPROPERTY(EditDefaultsOnly, Category = "C_AnimInstance")
		TSubclassOf<UAnimInstance> RemoteAnimInstance;
	UPROPERTY(EditDefaultsOnly, Category = "C_AnimInstance")
		TSubclassOf<UAnimInstance> FemaleRemoteAnimInstance;
	UPROPERTY(EditInstanceOnly, Category = "C_TEST")
		FVector ScaleVector;
	UPROPERTY(EditInstanceOnly, Category = "C_COLLISION")
		FVector2D CapsuleSize;
	UPROPERTY(EditInstanceOnly, Category = "C_COLLLISION")
		FVector2D RollingHitCapsuleSize;
	UPROPERTY(EditInstanceOnly, Category = "C_VALUE")
		float RollingCapsuleOffset;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UCameraShake> MyShake;
	UPROPERTY(VisibleAnywhere, Category = "C_CharacterStatus")
		UAIPerceptionStimuliSourceComponent* PlayerAIPerceptionStimul;
	UPROPERTY(EditInstanceOnly, Category = "C_TEST")
		FCameraShakeInfo ShakeInfo;
	UPROPERTY(EditDefaultsOnly, Category = "C_Particle")
		FComboSocket ComboParticleSocketName;
	UPROPERTY(EditDefaultsOnly, Category = "C_Particle")
		UParticleSystemComponent* BerserkBuffStart;
	UPROPERTY(EditDefaultsOnly, Category = "C_Particle")
		UParticleSystemComponent* BerserkBuffLoop;
	UPROPERTY(EditDefaultsOnly, Category = "C_Particle")
		UParticleSystemComponent* BloodEffect;
	UPROPERTY(EditDefaultsOnly, Category = "C_Particle")
		TArray<UParticleSystemComponent*> ParticleArray;
	UPROPERTY(EditDefaultsOnly, Category = "C_Particle")
		TArray<UParticleSystemComponent*> AttackEffectList;
	UPROPERTY(EditDefaultsOnly, Category = "C_KnockBack")
		FVector KnockBackVector; //넉백으로 인해 튕겨나가는 벡터값
	UPROPERTY(EditDefaultsOnly, Category = "C_PostProcess")
		UMaterialInstance* BlurMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "C_PostProcess")
		UMaterialInstanceDynamic* DynamicBlurMaterial;
	UPROPERTY(EditDefaultsOnly, Category = "C_AI")
<<<<<<< Updated upstream
		bool bUsePathFinding;
=======
		bool bUsePathFinding; //미사용
	UPROPERTY(EditDefaultsOnly, Category = "C_Widget")
		UNickNameWidget* NickNameWidget;
>>>>>>> Stashed changes
public:
	UPROPERTY(EditAnywhere)
		FEquipment Equipments;
	UPROPERTY(VisibleAnywhere, Category = "C_CharacterStatus")
		UPlayerCharacterStatComponent* PlayerStatComp;
	UPROPERTY(EditDefaultsOnly, Category = "C_AnimInstance")
		UPlayerCharacterAnimInstance* MyAnimInstance; //AnimInstance를 통해 초기화
	UPROPERTY(VisibleAnywhere, Category = "C_GameInstance")
		UXRGameInstance* CurGameInstance;
	UPROPERTY(VisibleAnywhere, Category = "C_CharacterSkill")
		TArray<UPlayerSkill*> SkillList;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsAttack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		/*순수하게 구르는 동작만 수행하는가? : 구르기 시 액터 이동을 체크하기 위해 사용*/
		bool bIsRolling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		/*구르기 전체 동작을 수행중인가? true : false*/
		bool bIsOverallRollAnimPlaying;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bIsSprint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		bool bSavedCombo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Character", Meta = (AllowPrivateAccess = true))
		int32 ComboCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C_Sound", Meta = (AllowPrivateAccess = true))
		USoundAttenuation* Attenuation; 
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "C_TEST", Meta = (AllowPrivateAccess = true))
		float RotSpeed;

	/* 캐릭터 상태를 bool로 정의 */
	bool bIsMove;
	bool bIsCharacterDead;
	bool bIsHit;
	bool bIsSkillMove;
	bool bIsSkillPlaying;
	bool bIsKnockBackMoving;
	bool bIsMoveLocked;
	bool bIsInvisible;

	/*TEST*/
private:
	FRotator DeltaRotation;
	FRotator AttackNextRotation; //공격 시에 방향전환에 사용되는 로테이터.
	FRotator AdditionalRotationValue; 
	FVector SpringArmLocation;
	bool bForwardKeyIsNeutral;
	std::vector<ANonePlayerCharacter*> AttackOverlapList;
	bool bIsPlayer;
	bool bInitialized;
	bool bIsMouseShow;
	int32 CurrentAttackID;
	float ForwardValue; //앞 방향키를 누르고 있는가(-1~1)
	float RightValue;  // 오른쪽 방향키를 누르고 있는가?(-1~1)
	float SumSec = 0;
	bool bIsAttackMoving; //공격 시 일보 전진
	float BaseTurnRate;
	float BaseLookUpRate;
	FVector MeshLocationVector;
	FVector WeaponScaleVector;
	FVector NameTagLocation;
	int32 LocationSyncFailCount;
	bool bIsPathFinding;

public:
	/*Test Value*/
	bool bIsTestMode;
public:
	virtual void Tick(float Deltatime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* controller) override;
	
public:
	void InitializeCharacter(bool bIsPlayerCharacter, CharacterData& Data);
	void ChangePartsById(EPartsType Type, int32 ID);
	void ChangeEquipment(UItem* Item, USkeletalMesh* SkMesh);
	void ChangeEquipment(UItem* Item, UStaticMesh* SmMesh);
	void PseudoChangeEquipmentWithoutMesh(UItem * Item);/*아이템 메시를 포함하지 않고, 정보만 일단 업데이트하는 함수*/
	void ChangePartsComponentsMesh(EPartsType Type, FSoftObjectPath PartAsset);
	void SetIsPlayer(bool is);
	bool GetIsPlayer();
	bool GetIsTestMode();
	void SetRollingCapsuleMode(); //구를때 모드 설정. 캡슐뿐아니라 이동속도도 관장함
	void SetNormalCapsuleMode(); //구른 뒤에 모드 설정. 캡슐뿐아니라 이동속도도 관장함
	float GetYawFromArrowKeys(float ForwardValue, float RightValue, bool& Out_ArrowKeyPressed);
	
	/*Setter*/
	void SetbIsSkillMove(bool b);
	void SetbIsSkillPlaying(bool b);
	void SetbIsKnockBackMoving(bool b);
	void SetbIsAttack(bool b);
	void SetbIsInvisible(bool b);
	void SetbSavedCombo(bool b);
	void SetComboCount(int32 NextCombo);
	void SetKnockBackVector(FVector& Vec);
	void SetbIsPathFinding(bool bPathFd);
	
	/*Getter*/
	bool GetbIsRolling();
	bool GetbIsOverallRollAnimPlaying();
	bool GetbIsSkillMove();
	bool GetbIsSkillPlaying();
	bool GetbIsDead();
	bool GetbIsAttack();
	bool GetbSavedCombo();
	bool GetbIsKnockBackMoving();
	bool GetbIsInvisible();
	int32 GetComboCount();
	int32 GetLocationSyncFailCount();
	bool GetbIsPathFinding();
<<<<<<< Updated upstream
	UParticleSystemComponent* GetParticleComponentByName(FString FindStr);
=======
	UParticleSystemComponent* GetParticleComponentByName(FString FindStr); //ParticleArray에서 이름으로 호출
	UNickNameWidget* GetNickNameWidget();
>>>>>>> Stashed changes

	/*Force(해당 동작 바로 정지, 모든 관련 변수 초기화)*/
	void ForceAttackStop();
	void ForceRollStop();
	void ForceSkillStop();
	void ForceKnockbackStop();
	void LockCharacterMove(bool Lock); 
	void AddLocationSyncFailCount(); //PathFinding 조건 카운트 : 미사용



	/*Test Function*/
	void TestPlay();

	/*아이템 장착, 가져오기(주로 UI에서 사용)*/
	UItemEquipment* GetEquippedItem(EEquipmentsType Type);
	void SetEquippedItem(EEquipmentsType Type, UItemEquipment* Item);

	float TakeDamage(float Damage, FXRDamageEvent& DamageEvent, class AController* EventInstigator,
		class AActor* DamageCauser);

	FVector2D GetNormalCapsuleSize();
	FVector2D GetRollingCapsuleSize();
	FComboSocket GetComboSocket();
	
	UFUNCTION()
		/* 공격시 칼 오버랩 */
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnMyMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void ContinueCombo();
	UFUNCTION()
		void StartMoveAttack(); //공격 시 전방 이동거리 조절
	UFUNCTION()
		void EndMoveAttack();
	UFUNCTION()
		void LoadPartsComplete(FSoftObjectPath AssetPath, EPartsType Type);
	UFUNCTION(BlueprintCallable, Category ="C_TEST")
		void TestInitialize();

		virtual	void OnDead() override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void Attack();
	void Roll();
	void Sprint();
	void SprintEnd();
	void ToggleMouseCursor();
	void WheelUp();
	void WheelDown();

};

