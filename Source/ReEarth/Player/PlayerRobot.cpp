// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRobot.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetArrayLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Player/RemoteController/RemoteController.h"
#include "Engine/EngineTypes.h"
#include "AIModule/Classes/AIController.h"
#include "Player/PlayerHoming.h"
#include "TimerManager.h"
#include "Player/UI/Aim/NormalAimWidgetBase.h"
#include "Player/UI/NormalAim.h"
#include "Player/UI/MainUI/MainUIBase.h"
#include "Player/UI/HomingAim.h"
#include "Game/ReEarth_GM.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "MovieScene/Public/MovieSceneSequencePlayer.h"
#include "DrawDebugHelpers.h"


//�⺻ ���� �߻� �ֱ�(�ʴ���)
#define df_FIRE_DURATION 0.12f

//�̻��� ���� �ֱ�(�ʴ���)
#define df_HOMINGSPAWN_DURATION 5.0f

//�̻��� ���� ��Ÿ�� UI ������Ʈ �ֱ�
#define df_HOMINGCOOLTIMEUI_UPDATE_DURATION 0.2f

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

	SpringArm->SetupAttachment(GetMesh(), TEXT("PlayerPosition"));
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

	//LeftHand ����
	//---------------------------------------------------------------------------
	LeftHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetCollisionProfileName(TEXT("Player"));
	LeftHandCollision->SetupAttachment(GetMesh(), TEXT("hand_l"));
	LeftHandCollision->SetRelativeLocation(FVector(8.0f, 16.0f, 0.0f));
	LeftHandCollision->SetSphereRadius(45.0f);
	//---------------------------------------------------------------------------

	//RightHand ����
	//---------------------------------------------------------------------------
	RightHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetCollisionProfileName(TEXT("Player"));
	RightHandCollision->SetupAttachment(GetMesh(), TEXT("hand_r"));
	RightHandCollision->SetRelativeLocation(FVector(-8.0f, -16.0f, 0.0f));
	RightHandCollision->SetSphereRadius(45.0f);
	//---------------------------------------------------------------------------

	//Widget(ü��, �̻��� �� �� UI) ����
	//---------------------------------------------------------------------------
	MainUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("MainUI"));

	MainUI->SetupAttachment(Seat,TEXT("MainUIPosition"));
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
			EAttachmentRule::KeepWorld, false),TEXT("RightControllerPosition"));


	InitSpawnHoming();

	MainUIUMG = Cast<UMainUIBase>(MainUI->GetUserWidgetObject());

}

// Called every frame
void APlayerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAim(TEXT("LeftShotPosition"), IsLeftAimOn, LeftBasicAim,true);
	UpdateAim(TEXT("RightShotPosition"), IsRightAimOn, RightBasicAim,false);
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
			AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
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
			AddActorLocalRotation(FRotator(0.0f, Value, 0.0f));
			//SetActorRotation(FRotator(0.0f, GetActorRotation().Yaw + Value, 0.0f));
		}
	}
}

void APlayerRobot::LeftShot()
{
	//������ ���� ���� ������ ���� ������ ���� ���·� �����Ѵ�.
	if (LeftAttackState == EHandState::Attackable)
	{
		LeftAttackState = EHandState::Attack;
		Shot(LeftAttackState, true);
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
		Shot(RightAttackState, false);
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

void APlayerRobot::HomingShot()
{
	int iLen = EnemyArray.Num();


	//TargetArray Setting
	//--------------------------------------------------
	//1. ��ü EnemyArray ��ȯ
	for (int iCnt = 0; iCnt < iLen; ++iCnt)
	{

		//2. �̻����� ������ break
		if (0 == CurrentHomingNum)
		{
			break;
		}



		//�������� ���ֽ� �κ�-------------------------------
		//�������� ���ֽ� �κ�-------------------------------
		//�������� ���ֽ� �κ�-------------------------------
		//�þ� ����.
		if (IsEnemyInSight(EnemyArray[iCnt]) == true)
		{
			TargetArray.Add(EnemyArray[iCnt]);

			//5. �̻��� ���� ����
			--CurrentHomingNum;
		}
		//�������� ���ֽ� �κ�-------------------------------
		//�������� ���ֽ� �κ�-------------------------------
		//�������� ���ֽ� �κ�-------------------------------


		//3. �ش� ������ �Ÿ��� ���� ���ϱ�(ũ�� �񱳸��ϴϱ� ��������)
		//float fDistance = GetSquaredDistanceTo(EnemyArray[iCnt]);
		//UE_LOG(LogClass, Warning, TEXT("<Distance: %f> / <sightDistance2: %f>"), fDistance, sightDistance * sightDistance);
		
		//if (fDistance <= MinHitDist)
		//{
		//	/*
		//	//FVector enemyLocation = EnemyArray[iCnt]->GetActorLocation();
		//	//FVector fromMe2EnemyVector = enemyLocation - GetActorLocation();
		//	//fromMe2EnemyVector.Normalize();

		//	//// Debugging.
		//	////DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 50000.0f, FColor::Red, false, 2.0f);

		//	//float dot = GetActorForwardVector().X * fromMe2EnemyVector.X + GetActorForwardVector().Y * fromMe2EnemyVector.Y + GetActorForwardVector().Z * fromMe2EnemyVector.Z;
		//	//float angle = FMath::Abs(FMath::RadiansToDegrees(FMath::Acos(dot)));

		//	//// ���� Ȯ��.
		//	//if (angle <= sightRadius * 0.5f)
		//	//{
		//	//	// Ray �߻� �� �տ� ��ü �ִ��� Ȯ��.
		//	//	FHitResult OutResult;
		//	//	FCollisionObjectQueryParams QueryParams = FCollisionObjectQueryParams::AllStaticObjects;
		//	//	bool bIsHit = GetWorld()->LineTraceSingleByObjectType(OutResult, GetActorLocation(), enemyLocation, QueryParams);

		//	//	// �þ� ���� �ȿ� ���� ���� ������ ��ü�� �ִ��� �ѹ� �� Ȯ��.
		//	//	if (bIsHit)
		//	//	{
		//	//		UE_LOG(LogClass, Warning, TEXT("<result: %s> / <angle: %f>"), TEXT("true"), angle);
		//	//		UE_LOG(LogClass, Warning, TEXT("HitResult: %s <HitActorName: %s>"), TEXT("Hit"), *OutResult.Actor->GetName());
		//	//	}

		//	//	// ������ ��ü�� ���� �� �þ� �ȵ� ���Դٰ� ����.
		//	//	else
		//	//	{
		//	//		TargetArray.Add(EnemyArray[iCnt]);

		//	//		//5. �̻��� ���� ����
		//	//		--CurrentHomingNum;

		//	//		UE_LOG(LogClass, Warning, TEXT("<result: %s> / <angle: %f>"), TEXT("true"), angle);
		//	//	}
		//	//}

		//	//// ���� ���.
		//	//else
		//	//{
		//	//	UE_LOG(LogClass, Warning, TEXT("<result: %s> / <angle: %f>"), TEXT("false"), angle);
		//	//}*/
		//}

		///*if (fDistance <= sightDistance * sightDistance)
		//{
		//	FVector enemyLocation = EnemyArray[iCnt]->GetActorLocation();
		//	FVector fromMe2EnemyVector = enemyLocation - GetActorLocation();
		//	float angle = FMath::Atan2(fromMe2EnemyVector.Y, fromMe2EnemyVector.X);

		//	if (angle <= sightRadius * 0.5f)
		//	{
		//		UE_LOG(LogClass, Warning, TEXT("<result: %s> / <angle: %f>"), "true", angle);
		//	}
		//	else
		//	{
		//		UE_LOG(LogClass, Warning, TEXT("<result: %s> / <angle: %f>"), "false", angle);
		//	}
		//}*/

		//if (EnemyArray[iCnt]->WasRecentlyRendered(0.0f))
		//{
		//	UE_LOG(LogClass, Warning, TEXT("true"));
		//}
		//else
		//{
		//	UE_LOG(LogClass, Warning, TEXT("false"));
		//}

		//4. �̻��� ��� �����ȿ� ������
		//���� ī�޶� ���̴� ���¸� TargetArray�� �߰�
		//if (fDistance < MinHitDist && EnemyArray[iCnt]->WasRecentlyRendered(0.0f))
		//{
		//	TargetArray.Add(EnemyArray[iCnt]);

		//	//5. �̻��� ���� ����
		//	--CurrentHomingNum;
		//}
	}
	//--------------------------------------------------
	//TargetArray Setting



	//Homing Setting
	//---------------------------------------------------
	//1. ��ü TargetArray ��ȯ
	iLen = TargetArray.Num();

	for (int iCnt = 0; iCnt < iLen; ++iCnt)
	{
		//1-1. �̻��� Detach
		HomingArray[0]->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

		//1-2. Play Sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReadyHomingSound, GetActorLocation());

		//1-3. �̻��Ͽ��� Shot���
		HomingArray[0]->Shot(TargetArray[iCnt]);

		//1-4. �߻��� �̻��� HomingArray���� ����
		APlayerHoming * pTargetHoming = HomingArray[0];
		HomingArray.Remove(pTargetHoming);

		//1-5. UI�� Missile ���� ���̱�
		MainUIUMG->UpdateRemoveMissile();


		TargetArray[iCnt]->HomingOn();
	}
	//---------------------------------------------------
	//Homing Setting



	//�ʿ��� & ����� ������ ����
	//----------------------------------------------------
	//Homing Spawn Timer Handle �۵�
	GetWorldTimerManager().SetTimer(HomingSpawnTimerHandle, this, &APlayerRobot::AddSpawnHoming, df_HOMINGSPAWN_DURATION, false);
	UpdateHomingCoolTime();

	//TargetArrayŬ����
	TargetArray.Empty();
	//----------------------------------------------------
	//�ʿ��� & ����� ������ ����
}

bool APlayerRobot::IsEnemyInSight(AEnemyBase* targetEnemy)
{
	//3. �ش� ������ �Ÿ��� ���� ���ϱ�(ũ�� �񱳸��ϴϱ� ��������)
	float fDistance = GetSquaredDistanceTo(targetEnemy);
	//UE_LOG(LogClass, Warning, TEXT("<Distance: %f> / <sightDistance2: %f>"), fDistance, sightDistance * sightDistance);

	if (fDistance <= MinHitDist)
	{
		FVector enemyLocation = targetEnemy->GetActorLocation();
		FVector fromMe2EnemyVector = enemyLocation - GetActorLocation();
		fromMe2EnemyVector.Normalize();

		// Debugging.
		//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 50000.0f, FColor::Red, false, 2.0f);

		float dot = GetActorForwardVector().X * fromMe2EnemyVector.X + GetActorForwardVector().Y * fromMe2EnemyVector.Y + GetActorForwardVector().Z * fromMe2EnemyVector.Z;
		float angle = FMath::Abs(FMath::RadiansToDegrees(FMath::Acos(dot)));

		// ���� Ȯ��.
		if (angle <= sightRadius * 0.5f)
		{
			// Ray �߻� �� �տ� ��ü �ִ��� Ȯ��.
			FHitResult OutResult;
			FCollisionObjectQueryParams QueryParams = FCollisionObjectQueryParams::AllStaticObjects;
			bool bIsHit = GetWorld()->LineTraceSingleByObjectType(OutResult, GetActorLocation(), enemyLocation, QueryParams);

			// �þ� ���� �ȿ� ���� ���� ������ ��ü�� �ִ��� �ѹ� �� Ȯ��.
			if (bIsHit)
			{
				/*UE_LOG(LogClass, Warning, TEXT("<result: %s> / <angle: %f>"), TEXT("true"), angle);
				UE_LOG(LogClass, Warning, TEXT("HitResult: %s <HitActorName: %s>"), TEXT("Hit"), *OutResult.Actor->GetName());*/

				return false;
			}

			// ������ ��ü�� ���� �� �þ� �ȵ� ���Դٰ� ����.
			else
			{
				//UE_LOG(LogClass, Warning, TEXT("<result: %s> / <angle: %f>"), TEXT("true"), angle);
				return true;
			}
		}

		// ���� ���.
		else
		{
			//UE_LOG(LogClass, Warning, TEXT("<result: %s> / <angle: %f>"), TEXT("false"), angle);
		}
	}

	return false;
}


void APlayerRobot::LockOff()
{
	//vr���� �� test��
	RobotState = ERobotState::Normal;
	LeftAttackState = EHandState::Attackable;
	RightAttackState = EHandState::Attackable;

}

float APlayerRobot::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageProc(DamageAmount);

	return DamageAmount;
}

void APlayerRobot::DamageProc(float Damage)
{
	if (RobotState == ERobotState::Dead)
	{
		return;
	}

	CurrentHP -= Damage;

	float HPRate = CurrentHP / MaxHP;

	//UI HP Bar ����
	MainUIUMG->SetHPBar(HPRate);

	//UI HP Bar ����



	//�������
	if (CurrentHP == 0)
	{
		ALevelSequenceActor * Temp;
		//Fade Out ����
		ULevelSequencePlayer * LSP = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(),
			FadeOut, FMovieSceneSequencePlaybackSettings(),Temp);
		
		LSP->Play();
		//Fade Out ����
	
		//FadeOut ������ Lobby�� �̵�
		GetWorldTimerManager().SetTimer(GotoLobbyTimerHandle, this, &APlayerRobot::GoToLobby, 2.0f, false);
	}

}

void APlayerRobot::AddEnemy(AEnemyBase * pNewEnemey)
{
	if (nullptr != pNewEnemey)
	{
		EnemyArray.Add(pNewEnemey);
	}
}

void APlayerRobot::RemoveEnemy(AEnemyBase * pRemoveEnemy)
{
	if (nullptr != pRemoveEnemy)
	{
		EnemyArray.Remove(pRemoveEnemy);
	}
}

void APlayerRobot::RobotOnSoundPlay()
{
	if (RobotOnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), RobotOnSound, GetActorLocation());
	}
}

void APlayerRobot::RobotInit()
{
}


void APlayerRobot::LeftShotCallBack()
{
	Shot(LeftAttackState, true);
}

void APlayerRobot::RightShotCallBack()
{
	Shot(RightAttackState, false);
}



void APlayerRobot::Shot(EHandState HandState, bool Left)
{
	//���� ������ ���� �Ѿ��� �����Ѵ�.
	if (HandState == EHandState::Attack)
	{
		//1. ������ ���
		if (Left)
		{
			//1-1 �Ѿ� ���� ��ġ ���� �� ����
			FTransform ShotTransform = GetMesh()->GetSocketTransform(TEXT("LeftShotPosition"));
			FVector ShotLocation = ShotTransform.GetLocation();
			FRotator ShotRotator = ShotTransform.GetRotation().Rotator();
			GetWorld()->SpawnActor<APlayerBullet>(Bullet_Template, ShotLocation, ShotRotator);
			//--------------------------------------------------------------------------------
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ShotLocation);


			//1-2 ���� �� ���� Aim animation play
			if (nullptr != LeftBasicAim)
				LeftBasicAim->PlayAnimation();

			//1-3 ������ �߻縦 ���� Timer����
			GetWorldTimerManager().SetTimer(LeftShotTimerHandle, this, &APlayerRobot::LeftShotCallBack, df_FIRE_DURATION, false);
		}

		//2. �������� ���
		else
		{
			FTransform ShotTransform = GetMesh()->GetSocketTransform(TEXT("RightShotPosition"));
			FVector ShotLocation = ShotTransform.GetLocation();
			FRotator ShotRotator = ShotTransform.GetRotation().Rotator();
			GetWorld()->SpawnActor<APlayerBullet>(Bullet_Template, ShotLocation, ShotRotator);

			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ShotLocation);

			if (nullptr != RightBasicAim)
				RightBasicAim->PlayAnimation();

			GetWorldTimerManager().SetTimer(RightShotTimerHandle, this, &APlayerRobot::RightShotCallBack, df_FIRE_DURATION, false);
		}		
	}
}

void APlayerRobot::InitSpawnHoming()
{
	//���� �� Homing ����
	HomingArray.Add(GetWorld()->SpawnActor<APlayerHoming>(Homing_Template));
	HomingArray[(int)EHomingLocation::LeftUp]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("LeftUpHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------


	//���� �Ʒ� Homing ����
	HomingArray.Add(GetWorld()->SpawnActor<APlayerHoming>(Homing_Template, GetActorTransform()));
	HomingArray[(int)EHomingLocation::LeftDown]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("LeftDownHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------


	//������ �� Homing ����
	HomingArray.Add(GetWorld()->SpawnActor<APlayerHoming>(Homing_Template, GetActorTransform()));
	HomingArray[(int)EHomingLocation::RightUp]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("RightUpHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------


	//���� �Ʒ� Homing ����
	HomingArray.Add(GetWorld()->SpawnActor<APlayerHoming>(Homing_Template, GetActorTransform()));
	HomingArray[(int)EHomingLocation::RightDown]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("RightDownHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------
}

void APlayerRobot::AddSpawnHoming()
{
	//���� �̻��� ������ 4�� �̸��� �� ����
	if (CurrentHomingNum < 4)
	{
		APlayerHoming * pNewHoming = GetWorld()->SpawnActor<APlayerHoming>(Homing_Template);

		//���� �߰��ؾ��ϴ� ��ġ�� �߰��ϱ� ���� switch��
		FName HomingPosition;
		switch (CurAddHomingPosition)
		{
		case EHomingLocation::LeftUp:
		{
			HomingPosition = TEXT("LeftUpHomingPosition");
			CurAddHomingPosition = EHomingLocation::LeftDown;
		}
		break;

		case EHomingLocation::LeftDown:
		{
			HomingPosition = TEXT("LeftDownHomingPosition");
			CurAddHomingPosition = EHomingLocation::RightUp;
		}
		break;

		case EHomingLocation::RightUp:
		{
			HomingPosition = TEXT("RightUpHomingPosition");
			CurAddHomingPosition = EHomingLocation::RightDown;
		}
		break;

		case EHomingLocation::RightDown:
		{
			HomingPosition = TEXT("RightDownHomingPosition");
			CurAddHomingPosition = EHomingLocation::LeftUp;
		}
		break;

		default:
			break;

		}

		//�̻��� Mesh�� Attach��Ű��
		pNewHoming->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), HomingPosition);

		//�̻��� ���� Update
		++CurrentHomingNum;
		
		//�迭�� �߰�
		HomingArray.Add(pNewHoming);

		//UI Update
		MainUIUMG->UpdateAddMissile();

		GetWorldTimerManager().SetTimer(HomingSpawnTimerHandle, this, &APlayerRobot::AddSpawnHoming, df_HOMINGSPAWN_DURATION, false);
	}
}

void APlayerRobot::UpdateAim(FName SocketName, bool & AimState, ANormalAim *& AimSide,bool IsLeft)
{
	//Check�� ������ ���� Setting
	FVector SocketPosition = GetMesh()->GetSocketLocation(SocketName);
	FRotator SocketRotator = GetMesh()->GetSocketRotation(SocketName);
	FVector SocketForwardVector = UKismetMathLibrary::GetForwardVector(SocketRotator);

	SocketForwardVector = SocketForwardVector * 10000.0f;
	FVector EndLocation = SocketPosition + SocketForwardVector;

	//Check�� ��ü Ÿ�� ����
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));


	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(this);

	FHitResult OutHit;

	bool IsResult = UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(), SocketPosition,
		EndLocation,
		ObjectType,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		OutHit,
		true);
	//���� Check ���� �� Aim ����
	if (IsResult)
	{
		AActor * HitActor = OutHit.GetActor();
		FRotator HitActorRotator = HitActor->GetActorRotation();
		FVector HitActorLocation = OutHit.Location;

		FRotator LookPlayerRotation = UKismetMathLibrary::FindLookAtRotation(HitActorLocation, GetActorLocation());
		FVector LookPlayerVector = UKismetMathLibrary::GetForwardVector(LookPlayerRotation);
		FVector SpawnAimLocation = LookPlayerVector * 200 + HitActorLocation;

		//Aim�� �̹� �������ִ� ���¸� Aim ��ġ Update
		if (AimState)
		{
			AimSide->SetActorTransform(UKismetMathLibrary::MakeTransform(SpawnAimLocation,
				LookPlayerRotation, FVector(1.0f, 1.0f, 1.0f)));
		}

		//Aim ����
		else
		{
			if(IsLeft)
				AimSide = GetWorld()->SpawnActor<ANormalAim>(BasicLeftAim_Template, SpawnAimLocation, LookPlayerRotation);
			else
				AimSide = GetWorld()->SpawnActor<ANormalAim>(BasicRightAim_Template, SpawnAimLocation, LookPlayerRotation);
			
			AimState = true;
		}
	}
	else
	{
		if (AimState)
		{
			AimSide->Destroy();
			AimSide = nullptr;
			AimState = false;
		}
	}

}

void APlayerRobot::UpdateHomingCoolTime()
{
	if (MainUIUMG->HomingCoolTimeBar && CurrentHomingNum < 4)
	{
		float RemainTime = UKismetSystemLibrary::K2_GetTimerRemainingTimeHandle(GetWorld(), HomingSpawnTimerHandle);

		MainUIUMG->SetHomingCoolTimeBar(1 - (RemainTime / df_HOMINGSPAWN_DURATION));

		GetWorldTimerManager().SetTimer(HomingCoolTimeUITimerHandle, this,
			&APlayerRobot::UpdateHomingCoolTime, df_HOMINGCOOLTIMEUI_UPDATE_DURATION, false);
	}
	else if (CurrentHomingNum == 4)
	{
		MainUIUMG->SetHomingCoolTimeBar();
	}
}


void APlayerRobot::GoToLobby()
{
	AReEarth_GM * GM = Cast<AReEarth_GM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->GoToLobby();
	}
}