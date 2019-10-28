// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerBullet.h"
#include "Player/PlayerHoming.h"
#include "PlayerRobot.generated.h"


#define Enemy AActor


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

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UWidgetComponent * HPBar;*/
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

	//Controller로 사용할 blueprint 가져오기
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




	//EnemyArray에 추가,삭제하는 함수
	//---------------------------------------------
	void AddEnemy(Enemy * pNewEnemey);
	void RemoveEnemy(Enemy * pRemoveEnemy);
	//---------------------------------------------


private:
	//총 발사시 텀을 주기위한 private변수----------
	//발사 후 누적 시간을 저장한다.
	float LeftAccumulation	= 0;
	float RightAccumulation = 0;
	//---------------------------------------------


	//유도 미사일 관련 private변수-----------------
	APlayerHoming * HomingArray[4];

	//Target으로 가능한 Enemy Array
	TArray<Enemy*>	EnemyArray;

	//Target으로할 Enemy Array
	TArray<Enemy*>	TargetArray;

	//현재 미사일 개수
	int				CurrentHomingNum = 4;

	//미사일 최대 범위
	float			MinHitDist = 1000.0f;
	//---------------------------------------------




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
};
