// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBullet.h"
#include "Player/PlayerHoming.h"
#include "PlayerRobot.generated.h"


#define Enemy AActor


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

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent * HPBar;*/
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

	//Controller�� ����� blueprint ��������
	//---------------------------------------------------
	UPROPERTY(EditAnywhere,Category="Controller")
	TSubclassOf<class ARemoteController> LeftController_Template;

	UPROPERTY(EditAnywhere, Category = "Controller")
	TSubclassOf<class ARemoteController> RightController_Template;
	//---------------------------------------------------


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




	//EnemyArray�� �߰�,�����ϴ� �Լ�
	//---------------------------------------------
	void AddEnemy(Enemy * pNewEnemey);
	void RemoveEnemy(Enemy * pRemoveEnemy);
	//---------------------------------------------


private:
	//�� �߻�� ���� �ֱ����� private����----------
	//�߻� �� ���� �ð��� �����Ѵ�.
	float LeftAccumulation	= 0;
	float RightAccumulation = 0;
	//---------------------------------------------


	//���� �̻��� ���� private����-----------------
	APlayerHoming * HomingArray[4];

	//Target���� ������ Enemy Array
	TArray<Enemy*>	EnemyArray;

	//Target������ Enemy Array
	TArray<Enemy*>	TargetArray;

	//���� �̻��� ����
	int				CurrentHomingNum = 4;

	//�̻��� �ִ� ����
	float			MinHitDist = 1000.0f;
	//---------------------------------------------




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
};
