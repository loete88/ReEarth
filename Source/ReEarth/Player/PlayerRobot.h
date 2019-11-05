// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBullet.h"
#include "Player/PlayerHoming.h"
#include "Enemy/Character/EnemyBase.h"
#include "Player/UI/Aim/HomingAimWidgetBase.h"
#include "PlayerRobot.generated.h"


//Robot 상태에 대한 Enum값
//----------------------------------------------
//Off 상태에서는 Robot의 모든 움직임을 막는다.
//Normal 상태에서는 움직임 공격 모두 가능하다.
//Dead 상태에서는 Off와 마찬가지로 작동한다.
UENUM(BlueprintType)
enum class ERobotState : uint8
{
	Normal	= 0		UMETA(DisplayName = "Normal"),
	Off		= 1		UMETA(DisplayName = "Off"),
	Dead	= 2		UMETA(DisplayName = "Dead")
};
//----------------------------------------------

//Robot 손에 대한 Enum값
//----------------------------------------------
//Attackable : Controller를 쥐었지만 트리거는 누르지 않은 상태
//Off : Controller를 놓은 상태
//Attack : Attackable에서 트리거까지 누른 상태
UENUM(BlueprintType)
enum class EHandState : uint8
{
	Attackable	= 0		UMETA(DisplayName = "Attackable"),
	Off			= 1		UMETA(DisplayName = "Off"),
	Attack		= 2		UMETA(DisplayName = "Attack")

};
//----------------------------------------------

//Homing 위치에 대한 Enum값(HomingArray의 index로 사용)
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


	//상태 관련 멤버 변수
	//---------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="State")
	ERobotState RobotState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EHandState LeftAttackState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EHandState RightAttackState;
	//---------------------------------------------------
	//상태 관련 멤버 변수


	//체력 관련 멤버 변수
	//---------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "HP")
	float MaxHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
	float CurrentHP = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
	class UMainUIBase * MainUIUMG;
	//---------------------------------------------------
	//체력 관련 멤버 변수

	//Bullet으로 사용할 blueprint 가져오기
	//---------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Bullet")
	TSubclassOf<class APlayerBullet> Bullet_Template;
	//---------------------------------------------------

	//Homing으로 사용할 blueprint 가져오기
	//---------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Bullet")
	TSubclassOf<class APlayerHoming> Homing_Template;
	//---------------------------------------------------
	
	//Fire sound로 사용할 SoundCue 가져오기
	//---------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * FireSound;
	//---------------------------------------------------

	//Homing Ready sound로 사용할 SoundCue 가져오기
	//---------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * ReadyHomingSound;
	//---------------------------------------------------

	//Controller로 사용할 blueprint 가져오기
	//---------------------------------------------------
	UPROPERTY(EditAnywhere,Category="Controller")
	TSubclassOf<class ARemoteController> LeftController_Template;
	
	UPROPERTY(EditAnywhere, Category = "Controller")
	TSubclassOf<class ARemoteController> RightController_Template;
	//---------------------------------------------------

	//기본 Aim으로 사용할 blueprint 가져오기
	//----------------------------------------------------
	UPROPERTY(EditAnywhere, Category = "Aim")
	TSubclassOf<AActor> BasicLeftAim_Template;

	UPROPERTY(EditAnywhere, Category = "Aim")
	TSubclassOf<AActor> BasicRightAim_Template;
	//----------------------------------------------------

	//Homing Aim으로 사용할 blueprint 가져오기
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


	//입력 Event 처리 함수
	//---------------------------------------------------
	
	//이동관련 함수--------------------------------------
	//앞,뒤로 이동
	void MoveForward(float Value);

	//좌,우로 이동
	void MoveRight(float Value);

	//Robot몸체 Yaw 회전
	void ViewYaw(float Value);
	//이동관련 함수--------------------------------------


	//왼쪽 총
	void LeftShot();
	void StopLeftShot();


	//오른쪽 총
	void RightShot();
	void StopRightShot();

	//Jump
	void Jump();
	void StopJump();

	//HomingShot
	void HomingShot();

	//Vr없을 때 테스트할 수 있도록 만든 함수
	void LockOff();
	//---------------------------------------------------
	//입력 Event 처리 함수

	
	//Damage처리 함수
	//----------------------------------------------------
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void DamageProc(float Damage);
	//----------------------------------------------------
	//Damage처리 함수

	//EnemyArray에 추가,삭제하는 함수
	//---------------------------------------------
	void AddEnemy(AEnemyBase * pNewEnemey);
	void RemoveEnemy(AEnemyBase * pRemoveEnemy);
	//---------------------------------------------


private:
	//총 발사시 텀을 주기위한 private변수----------
	//발사 후 누적 시간을 저장한다.
	float LeftAccumulation	= 0;
	float RightAccumulation = 0;
	//---------------------------------------------

	//기본 공격 Aim관련 변수-----------------------
	bool IsLeftAimOn = false;
	bool IsRightAimOn = false;

	class ANormalAim * LeftBasicAim = nullptr;
	class ANormalAim * RightBasicAim = nullptr;
	//---------------------------------------------


	//유도 미사일 관련 private변수-----------------
	TArray<class APlayerHoming*> HomingArray;

	//Target으로 가능한 Enemy Array
	TArray<AEnemyBase*>	EnemyArray;

	//Target으로할 Enemy Array
	TArray<AEnemyBase*>	TargetArray;

	//현재 미사일 개수
	int				CurrentHomingNum = 4;

	//미사일 최대 범위
	float			MinHitDist = 100000000.0f;

	//생성할 HomingAim 배열
	//파괴시키기 위해서 들고있는다.
	TArray<AActor *>	HomingAimArray;

	//미사일 개수가 4개 미만이고
	//마지막 미사일을 쏜 후 5초가 지나면 새로운 미사일을 생성하기 위해
	//필요한 Check Time
	float			HomingTimeCheck =0.0f;

	//미사일을 추가할 위치
	EHomingLocation CurAddHomingPosition = EHomingLocation::LeftUp;

	//---------------------------------------------


	//---------------------------------------------
	void ClearTargetArray();

	//-----------------------------------------------------------------------------------
	//Shot함수																			//
	//HandState : 오른쪽 or 왼쪽의 HandState를 넣어주면 된다.							//
	//DeltaTime : Tick의 DeltaTime을 넣어주면 된다.										//
	//Left		: 왼쪽의 HandState면 true, 오른쪽의 HandState면 false를 넣어주면 된다.	//	
	//define으로 df_FIRE_DURATION을 0.12f로 정의했는데									//
	//0.12초 마다 한 번씩 발사해주는 함수다.											//
	void Shot(EHandState HandState,float DeltaTime, bool Left);							//
	//-----------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------
	//InitSpawnHoming함수																//
	//시작할 때 Homing을 Setting해준다.													//
	void InitSpawnHoming();																//
	//-----------------------------------------------------------------------------------

	//-----------------------------------------------------------------------------------
	//AddSpawnHoming함수
	void AddSpawnHoming();
	//-----------------------------------------------------------------------------------


	//-----------------------------------------------------------------------------------
	//Update Aim함수
	//기본 공격 Aim을 생성해야하면 생성, 소멸시켜야하면 소멸시킨다.
	void UpdateAim(FName SocketName,bool & AimState, class ANormalAim *& AimSide,bool IsLeft);
	//-----------------------------------------------------------------------------------
};
