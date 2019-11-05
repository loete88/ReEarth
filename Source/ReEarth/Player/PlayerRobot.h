// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBullet.h"
#include "Player/PlayerHoming.h"
#include "Enemy/Character/EnemyBase.h"
#include "Player/UI/Aim/HomingAimWidgetBase.h"
#include "PlayerRobot.generated.h"


//Robot ���¿� ���� Enum��
//----------------------------------------------
//Off ���¿����� Robot�� ��� �������� ���´�.
//Normal ���¿����� ������ ���� ��� �����ϴ�.
//Dead ���¿����� Off�� ���������� �۵��Ѵ�.
UENUM(BlueprintType)
enum class ERobotState : uint8
{
	Normal	= 0		UMETA(DisplayName = "Normal"),
	Off		= 1		UMETA(DisplayName = "Off"),
	Dead	= 2		UMETA(DisplayName = "Dead")
};
//----------------------------------------------

//Robot �տ� ���� Enum��
//----------------------------------------------
//Attackable : Controller�� ������� Ʈ���Ŵ� ������ ���� ����
//Off : Controller�� ���� ����
//Attack : Attackable���� Ʈ���ű��� ���� ����
UENUM(BlueprintType)
enum class EHandState : uint8
{
	Attackable	= 0		UMETA(DisplayName = "Attackable"),
	Off			= 1		UMETA(DisplayName = "Off"),
	Attack		= 2		UMETA(DisplayName = "Attack")

};
//----------------------------------------------

//Homing ��ġ�� ���� Enum��(HomingArray�� index�� ���)
//-----------------------------------------------
UENUM(BlueprintType)
enum class EHomingLocation : uint8
{
	LeftUp		= 0		UMETA(DisplayName = "LeftUp"),
	LeftDown	= 1		UMETA(DisplayName = "LeftDown"),
	RightUp		= 2		UMETA(DisplayName = "RightUp"),
	RightDown	= 3		UMETA(DisplayName = "RightDown")
};




UCLASS()
class REEARTH_API APlayerRobot : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerRobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	//Component
	//----------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent * SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent * VROrigin;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent * Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent * Seat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent * LeftHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent * RightHandCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent * MainUI;
	//----------------------------------------------
	//Component


	//���� ���� ��� ����
	//---------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	ERobotState RobotState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EHandState LeftAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EHandState RightAttackState;
	//---------------------------------------------------
	//���� ���� ��� ����


	//ü�� ���� ��� ����
	//---------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "HP")
	float MaxHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
	float CurrentHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
	class UMainUIBase * MainUIUMG;
	//---------------------------------------------------
	//ü�� ���� ��� ����

	//Bullet���� ����� blueprint ��������
	//---------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Bullet")
	TSubclassOf<class APlayerBullet> Bullet_Template;
	//---------------------------------------------------

	//Homing���� ����� blueprint ��������
	//---------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Bullet")
	TSubclassOf<class APlayerHoming> Homing_Template;
	//---------------------------------------------------
	
	//Fire sound�� ����� SoundCue ��������
	//---------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * FireSound;
	//---------------------------------------------------

	//Homing Ready sound�� ����� SoundCue ��������
	//---------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * ReadyHomingSound;
	//---------------------------------------------------

	//Controller�� ����� blueprint ��������
	//---------------------------------------------------
	UPROPERTY(EditAnywhere,Category="Controller")
	TSubclassOf<class ARemoteController> LeftController_Template;
	
	UPROPERTY(EditAnywhere, Category = "Controller")
	TSubclassOf<class ARemoteController> RightController_Template;
	//---------------------------------------------------

	//�⺻ Aim���� ����� blueprint ��������
	//----------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Aim")
	TSubclassOf<AActor> BasicLeftAim_Template;

	UPROPERTY(EditAnywhere, Category = "Aim")
	TSubclassOf<AActor> BasicRightAim_Template;
	//----------------------------------------------------

	//Homing Aim���� ����� blueprint ��������
	//----------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Aim")
	TSubclassOf<AActor> HomingAim_Template;
	//----------------------------------------------------

	//Controller
	//---------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ARemoteController * LeftController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ARemoteController * RightController;
	//---------------------------------------------------


	//�Է� Event ó�� �Լ�
	//---------------------------------------------------
	
	//�̵����� �Լ�--------------------------------------
	//��,�ڷ� �̵�
	void MoveForward(float Value);

	//��,��� �̵�
	void MoveRight(float Value);

	//Robot��ü Yaw ȸ��
	void ViewYaw(float Value);
	//�̵����� �Լ�--------------------------------------


	//���� ��
	void LeftShot();
	void StopLeftShot();


	//������ ��
	void RightShot();
	void StopRightShot();

	//Jump
	void Jump();
	void StopJump();

	//HomingShot
	void HomingShot();

	//Vr���� �� �׽�Ʈ�� �� �ֵ��� ���� �Լ�
	void LockOff();
	//---------------------------------------------------
	//�Է� Event ó�� �Լ�

	
	//Damageó�� �Լ�
	//----------------------------------------------------
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void DamageProc(float Damage);
	//----------------------------------------------------
	//Damageó�� �Լ�

	//EnemyArray�� �߰�,�����ϴ� �Լ�
	//---------------------------------------------
	void AddEnemy(AEnemyBase * pNewEnemey);
	void RemoveEnemy(AEnemyBase * pRemoveEnemy);
	//---------------------------------------------


private:
	//�� �߻�� ���� �ֱ����� private����----------
	//�߻� �� ���� �ð��� �����Ѵ�.
	float LeftAccumulation	= 0;
	float RightAccumulation = 0;
	//---------------------------------------------

	//�⺻ ���� Aim���� ����-----------------------
	bool IsLeftAimOn = false;
	bool IsRightAimOn = false;

	class ANormalAim * LeftBasicAim = nullptr;
	class ANormalAim * RightBasicAim = nullptr;
	//---------------------------------------------


	//���� �̻��� ���� private����-----------------
	TArray<class APlayerHoming*> HomingArray;

	//Target���� ������ Enemy Array
	TArray<AEnemyBase*>	EnemyArray;

	//Target������ Enemy Array
	TArray<AEnemyBase*>	TargetArray;

	//���� �̻��� ����
	int				CurrentHomingNum = 4;

	//�̻��� �ִ� ����
	float			MinHitDist = 100000000.0f;

	//������ HomingAim �迭
	//�ı���Ű�� ���ؼ� ����ִ´�.
	TArray<AActor *>	HomingAimArray;

	//�̻��� ������ 4�� �̸��̰�
	//������ �̻����� �� �� 5�ʰ� ������ ���ο� �̻����� �����ϱ� ����
	//�ʿ��� Check Time
	float			HomingTimeCheck =0.0f;

	//�̻����� �߰��� ��ġ
	EHomingLocation CurAddHomingPosition = EHomingLocation::LeftUp;

	//---------------------------------------------


	//---------------------------------------------
	void ClearTargetArray();

	//-----------------------------------------------------------------------------------
	//Shot�Լ�																			//
	//HandState : ������ or ������ HandState�� �־��ָ� �ȴ�.							//
	//DeltaTime : Tick�� DeltaTime�� �־��ָ� �ȴ�.										//
	//Left		: ������ HandState�� true, �������� HandState�� false�� �־��ָ� �ȴ�.	//	
	//define���� df_FIRE_DURATION�� 0.12f�� �����ߴµ�									//
	//0.12�� ���� �� ���� �߻����ִ� �Լ���.											//
	void Shot(EHandState HandState,float DeltaTime, bool Left);							//
	//-----------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------
	//InitSpawnHoming�Լ�																//
	//������ �� Homing�� Setting���ش�.													//
	void InitSpawnHoming();																//
	//-----------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------
	//AddSpawnHoming�Լ�
	void AddSpawnHoming();
	//-----------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------
	//Update Aim�Լ�
	//�⺻ ���� Aim�� �����ؾ��ϸ� ����, �Ҹ���Ѿ��ϸ� �Ҹ��Ų��.
	void UpdateAim(FName SocketName,bool & AimState, class ANormalAim *& AimSide,bool IsLeft);
	//-----------------------------------------------------------------------------------
};
