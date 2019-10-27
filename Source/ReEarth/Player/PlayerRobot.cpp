// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRobot.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Player/RemoteController/RemoteController.h"
#include "Engine/EngineTypes.h"
#include "AIModule/Classes/AIController.h"

#define df_FIRE_DURATION 0.12f


// Sets default values
APlayerRobot::APlayerRobot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Capsule ����
	//---------------------------------------------------------------------------
	GetCapsuleComponent()->SetCapsuleSize(150.0f, 300.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	//---------------------------------------------------------------------------

	//Mesh ��ġ ���� �� ũ�� ����
	//---------------------------------------------------------------------------
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0,
		-225.0f), FRotator(0, -90.0f, 0));
	GetMesh()->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	//---------------------------------------------------------------------------

	//Character Movement ����
	//---------------------------------------------------------------------------
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	//---------------------------------------------------------------------------


	//SpringArm ����
	//---------------------------------------------------------------------------
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	SpringArm->SetupAttachment(GetMesh(), TEXT("seatposition"));
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->SetRelativeLocation(FVector(0, 3, 40));
	SpringArm->SetRelativeRotation(FRotator(0, 90.0f, 0));
	//---------------------------------------------------------------------------

	//VROrgin ����
	//---------------------------------------------------------------------------
	//Vr Hand�� ������ ��ġ
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrgin"));

	VROrigin->SetupAttachment(SpringArm);
	//---------------------------------------------------------------------------

	//Camera ����
	//---------------------------------------------------------------------------
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(VROrigin);
	//---------------------------------------------------------------------------

	//Seat(�÷��̾� å��&����) ����
	//---------------------------------------------------------------------------
	Seat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Seat"));

	Seat->SetupAttachment(GetMesh(), TEXT("seatposition"));
	//---------------------------------------------------------------------------

}

// Called when the game starts or when spawned
void APlayerRobot::BeginPlay()
{
	Super::BeginPlay();
	

	//�ʱ� Robot���� ����
	//-------------------------------------
	//�ʱ� ���´� Off���·� ����
	RobotState = ERobotState::Off;
	LeftAttackState = EHandState::Off;
	RightAttackState = EHandState::Off;
	//-------------------------------------



	//Controller �����ϱ�
	//------------------------------------------
	LeftController = GetWorld()->SpawnActor<ARemoteController>(LeftController_Template, FTransform());
	RightController = GetWorld()->SpawnActor<ARemoteController>(RightController_Template, FTransform());

	LeftController->AttachToComponent(Seat,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("LeftControllerPosition"));

	RightController->AttachToComponent(Seat,
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("RightControllerPosition"));


	InitSpawnHoming();

}

// Called every frame
void APlayerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�÷��̾� �� �߻� Logic
	//-------------------------------------------
	Shot(LeftAttackState,DeltaTime,true);
	Shot(RightAttackState,DeltaTime,false);

	//-------------------------------------------
	//�÷��̾� �� �߻� Logic
}

// Called to bind functionality to input
void APlayerRobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerRobot::MoveForward(float Value)
{
	if (Value != 0)
	{
		//RobotState�� Normal���� �� ���� �����̵��� ������
		if (RobotState == ERobotState::Normal)
		{
			AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)),Value);
		}
	}
}

void APlayerRobot::MoveRight(float Value)
{
	if (Value != 0)
	{
		//RobotState�� Normal���� �� ���� �����̵��� ������
		if (RobotState == ERobotState::Normal)
		{
			AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
		}
	}
}

void APlayerRobot::ViewYaw(float Value)
{
	if (Value != 0)
	{
		if (RobotState == ERobotState::Normal)
		{
			SetActorRotation(FRotator(0.0f, GetActorRotation().Yaw + Value, 0.0f));
		}
	}
}

void APlayerRobot::LeftShot()
{
	//������ ���� ���� ������ ���� ������ ���� ���·� �����Ѵ�.
	if (LeftAttackState == EHandState::Attackable)
	{
		LeftAttackState = EHandState::Attack;
	}
}

void APlayerRobot::StopLeftShot()
{
	//������ ���� ���̿��� ���� ������ ���� ���� ���·� �����Ѵ�.
	//Controller�� �� ���� Ʈ���Ÿ� ������ ��쿡 �ٲ�� ���� ��������
	if (LeftAttackState == EHandState::Attack)
	{
		LeftAttackState = EHandState::Attackable;
	}
}

void APlayerRobot::RightShot()
{
	//�������� ���� ���� ������ ���� �������� ���� ���·� �����Ѵ�.
	if (RightAttackState == EHandState::Attackable)
	{
		RightAttackState = EHandState::Attack;
	}
}

void APlayerRobot::StopRightShot()
{
	//�������� ���� ���̿��� ���� �������� ���� ���� ���·� �����Ѵ�.
	//Controller�� �� ���� Ʈ���Ÿ� ������ ��쿡 �ٲ�� ���� ��������
	if (RightAttackState == EHandState::Attack)
	{
		RightAttackState = EHandState::Attackable;
	}
}

void APlayerRobot::Jump()
{
	if (RobotState == ERobotState::Normal)
	{
		ACharacter::Jump();
	}
}

void APlayerRobot::StopJump()
{
	StopJumping();
}

void APlayerRobot::LockOff()
{
	//vr���� �� test��
	RobotState = ERobotState::Normal;
	LeftAttackState = EHandState::Attackable;
	RightAttackState = EHandState::Attackable;

}

void APlayerRobot::AddEnemy(Enemy * pNewEnemey)
{
	if (nullptr != pNewEnemey)
	{
		EnemyArray.Add(pNewEnemey);
	}
}

void APlayerRobot::RemoveEnemy(Enemy * pRemoveEnemy)
{
	if (nullptr != pRemoveEnemy)
	{
		EnemyArray.Remove(pRemoveEnemy);
	}
}

void APlayerRobot::Shot(EHandState HandState, float DeltaTime,bool Left)
{
	//���� ������ ���� �Ѿ��� �����Ѵ�.
	if (HandState == EHandState::Attack)
	{
		//1. ������ ���
		if (Left)
		{
			//1-1 ���� �� �߻翡 ���� ���� �� ���� ���
			if (LeftAccumulation < df_FIRE_DURATION)
			{
				LeftAccumulation += DeltaTime;
				return;
			}

			//1-2 ���� �߻��ϴϱ� �����ð� 0���� �ٲٱ�
			LeftAccumulation = 0;
			//--------------------------------------

			//1-3 �Ѿ� ���� ��ġ ���� �� ����
			FTransform ShotTransform = GetMesh()->GetSocketTransform(TEXT("LeftShotPosition"));
			FVector ShotLocation = ShotTransform.GetLocation();
			FRotator ShotRotator = ShotTransform.GetRotation().Rotator();
			GetWorld()->SpawnActor<APlayerBullet>(Bullet_Template, ShotLocation, ShotRotator);
			//--------------------------------------------------------------------------------
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ShotLocation);
		}
		
		//2. �������� ���
		else
		{
			if (RightAccumulation < df_FIRE_DURATION)
			{
				RightAccumulation += DeltaTime;
				return;
			}

			RightAccumulation = 0;

			FTransform ShotTransform = GetMesh()->GetSocketTransform(TEXT("RightShotPosition"));
			FVector ShotLocation = ShotTransform.GetLocation();
			FRotator ShotRotator = ShotTransform.GetRotation().Rotator();
			GetWorld()->SpawnActor<APlayerBullet>(Bullet_Template, ShotLocation, ShotRotator);
		}
	}
}

void APlayerRobot::InitSpawnHoming()
{
	//���� �� Homing ����
	HomingArray[(int)EHomingLocation::LeftUp] = GetWorld()->SpawnActor<APlayerHoming>(Homing_Template);
	HomingArray[(int)EHomingLocation::LeftUp]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("LeftUpHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------


	//���� �Ʒ� Homing ����
	HomingArray[(int)EHomingLocation::LeftDown] = GetWorld()->SpawnActor<APlayerHoming>(Homing_Template);
	HomingArray[(int)EHomingLocation::LeftDown]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("LeftDownHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------


	//������ �� Homing ����
	HomingArray[(int)EHomingLocation::RightUp] = GetWorld()->SpawnActor<APlayerHoming>(Homing_Template);
	HomingArray[(int)EHomingLocation::RightUp]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("RightUpHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------


	//���� �Ʒ� Homing ����
	HomingArray[(int)EHomingLocation::RightDown] = GetWorld()->SpawnActor<APlayerHoming>(Homing_Template);
	HomingArray[(int)EHomingLocation::RightDown]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("RightDownHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------
}

