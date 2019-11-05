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

#define df_FIRE_DURATION 0.12f


// Sets default values
APlayerRobot::APlayerRobot()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Capsule 설정
	//---------------------------------------------------------------------------
	GetCapsuleComponent()->SetCapsuleSize(150.0f, 300.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));
	//---------------------------------------------------------------------------
	

	//Mesh 위치 조정 및 크기 설정
	//---------------------------------------------------------------------------
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0,
		-225.0f), FRotator(0, -90.0f, 0));
	GetMesh()->SetWorldScale3D(FVector(3.0f, 3.0f, 3.0f));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	//---------------------------------------------------------------------------

	//Character Movement 설정
	//---------------------------------------------------------------------------
	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	GetCharacterMovement()->JumpZVelocity = 700.0f;
	//---------------------------------------------------------------------------


	//SpringArm 생성
	//---------------------------------------------------------------------------
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	SpringArm->SetupAttachment(GetMesh(), TEXT("PlayerPosition"));
	SpringArm->TargetArmLength = 0.0f;
	SpringArm->SetRelativeLocation(FVector(0, 3, 40));
	SpringArm->SetRelativeRotation(FRotator(0, 90.0f, 0));
	//---------------------------------------------------------------------------

	//VROrgin 생성
	//---------------------------------------------------------------------------
	//Vr Hand가 생성될 위치
	VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrgin"));

	VROrigin->SetupAttachment(SpringArm);
	//---------------------------------------------------------------------------

	//Camera 생성
	//---------------------------------------------------------------------------
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	Camera->SetupAttachment(VROrigin);
	//---------------------------------------------------------------------------

	//Seat(플레이어 책상&의자) 생성
	//---------------------------------------------------------------------------
	Seat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Seat"));

	Seat->SetupAttachment(GetMesh(), TEXT("seatposition"));
	//---------------------------------------------------------------------------

	//LeftHand 생성
	//---------------------------------------------------------------------------
	LeftHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandCollision"));
	LeftHandCollision->SetCollisionProfileName(TEXT("Player"));
	LeftHandCollision->SetupAttachment(GetMesh(), TEXT("hand_l"));
	LeftHandCollision->SetRelativeLocation(FVector(8.0f, 16.0f, 0.0f));
	LeftHandCollision->SetSphereRadius(45.0f);
	//---------------------------------------------------------------------------

	//RightHand 생성
	//---------------------------------------------------------------------------
	RightHandCollision = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandCollision"));
	RightHandCollision->SetCollisionProfileName(TEXT("Player"));
	RightHandCollision->SetupAttachment(GetMesh(), TEXT("hand_r"));
	RightHandCollision->SetRelativeLocation(FVector(-8.0f, -16.0f, 0.0f));
	RightHandCollision->SetSphereRadius(45.0f);
	//---------------------------------------------------------------------------

	//Widget(체력, 미사일 수 등 UI) 생성
	//---------------------------------------------------------------------------
	MainUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("MainUI"));

	MainUI->SetupAttachment(Seat,TEXT("MainUIPosition"));
	//---------------------------------------------------------------------------
}

// Called when the game starts or when spawned
void APlayerRobot::BeginPlay()
{
	Super::BeginPlay();
	

	//초기 Robot상태 설정
	//-------------------------------------
	//초기 상태는 Off상태로 시작
	RobotState = ERobotState::Off;
	LeftAttackState = EHandState::Off;
	RightAttackState = EHandState::Off;
	//-------------------------------------


	//Controller 생성하기
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

	UE_LOG(LogClass, Warning, TEXT("Beginplay"));

}

// Called every frame
void APlayerRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//플레이어 총 발사 Logic
	//-------------------------------------------
	Shot(LeftAttackState,DeltaTime,true);
	Shot(RightAttackState,DeltaTime,false);
	//-------------------------------------------
	//플레이어 총 발사 Logic


	//플레이어 미사일 생성 Logic
	//-------------------------------------------
	AddSpawnHoming();
	//-------------------------------------------
	//플레이어 미사일 생성 Logic


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
		//RobotState가 Normal상태 일 때만 움직이도록 만들자
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
		//RobotState가 Normal상태 일 때만 움직이도록 만들자
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
	//왼쪽이 공격 가능 상태일 때만 왼쪽을 공격 상태로 변경한다.
	if (LeftAttackState == EHandState::Attackable)
	{
		LeftAttackState = EHandState::Attack;
	}
}

void APlayerRobot::StopLeftShot()
{
	//왼쪽이 공격 중이였을 때만 왼쪽을 공격 가능 상태로 변경한다.
	//Controller를 안 집고 트리거를 누르는 경우에 바뀌는 것을 막기위해
	if (LeftAttackState == EHandState::Attack)
	{
		LeftAttackState = EHandState::Attackable;
	}
}

void APlayerRobot::RightShot()
{
	//오른쪽이 공격 가능 상태일 때만 오른쪽을 공격 상태로 변경한다.
	if (RightAttackState == EHandState::Attackable)
	{
		RightAttackState = EHandState::Attack;
	}
}

void APlayerRobot::StopRightShot()
{
	//오른쪽이 공격 중이였을 때만 오른쪽을 공격 가능 상태로 변경한다.
	//Controller를 안 집고 트리거를 누르는 경우에 바뀌는 것을 막기위해
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
	//1. 전체 EnemyArray 순환
	for (int iCnt = 0; iCnt < iLen; ++iCnt)
	{

		//2. 미사일이 없으면 break
		if (0 == CurrentHomingNum)
		{
			break;
		}

		//3. 해당 적과의 거리의 제곱 구하기(크기 비교만하니까 제곱으로)
		float fDistance = GetSquaredDistanceTo(EnemyArray[iCnt]);

		UE_LOG(LogClass, Warning, TEXT("%f"), fDistance);
		
		//4. 미사일 허용 범위안에 들어오고
		//현재 카메라에 보이는 상태면 TargetArray에 추가
		if (fDistance < MinHitDist && WasRecentlyRendered(0.0f))
		{
			TargetArray.Add(EnemyArray[iCnt]);

			//5. 미사일 개수 갱신
			--CurrentHomingNum;
		}
	}
	//--------------------------------------------------
	//TargetArray Setting



	//Homing Setting
	//---------------------------------------------------
	//1. 전체 TargetArray 순환
	iLen = TargetArray.Num();

	for (int iCnt = 0; iCnt < iLen; ++iCnt)
	{
		////2. HomingAim 생성
		//	//2-1 HomingPosition 가져오기(미사일 맞을 위치)
		USceneComponent * HomingPosition = TargetArray[iCnt]->HomingPosition;
		FVector ActorHitPosition = HomingPosition->GetComponentLocation();
		//
		//	//2-2 Aim이 Player를 바라보는 Rotator 가져오기
		FRotator LookPlayerRotation = UKismetMathLibrary::FindLookAtRotation(ActorHitPosition, GetActorLocation());
		//
		//	//2-3 Aim을 Transform 생성
		////위치는 HomingPosition + Aim이 Player를 바라보는 Forward Vector
		FVector AimLocation = ActorHitPosition + UKismetMathLibrary::GetForwardVector(LookPlayerRotation) * 300;
		////각도는 Aim이 Player를 바라보도록 그냥 LookPlayerRotation
		FTransform AimTransform = UKismetMathLibrary::MakeTransform(AimLocation, LookPlayerRotation,FVector(1.0f,1.0f,1.0f));

		//	//2-4 HomingAim Spawn
		AHomingAim * pHomingAim = GetWorld()->SpawnActor<AHomingAim>(HomingAim_Template, AimLocation, LookPlayerRotation);

		////3. HomingAimArray에 넣고 나중에 한번에 소멸시킨다.
		HomingAimArray.Add(pHomingAim);


		////4. 미사일 Detach
		HomingArray[0]->GetRootComponent()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

		//5. Play Sound
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReadyHomingSound, GetActorLocation());

		//6. 미사일에게 Shot명령
		HomingArray[0]->Shot(TargetArray[iCnt]);

		//7. 발사한 미사일 HomingArray에서 제거
		APlayerHoming * pTargetHoming = HomingArray[0];
		HomingArray.Remove(pTargetHoming);
	}
	//---------------------------------------------------
	//Homing Setting

	

	//필요한 & 사용한 변수들 갱신
	//----------------------------------------------------
	HomingTimeCheck = UGameplayStatics::GetTimeSeconds(GetWorld());

	//TargetArray클리어
	TargetArray.Empty();

	//Homing Aim을 보여주기위한 약간의 Delay
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerRobot::ClearTargetArray, 1.0f, false);
	//----------------------------------------------------
	//필요한 & 사용한 변수들 갱신
}


void APlayerRobot::LockOff()
{
	//vr없을 때 test용
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

	//UI HP Bar 갱신
	MainUIUMG->SetHPBar(HPRate);

	//UI HP Bar 갱신



	//죽은경우
	if (CurrentHP < 0)
	{
		//Fade Out 실행

		//Fade Out 실행
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

void APlayerRobot::ClearTargetArray()
{
	int iLen = HomingAimArray.Num();

	for (int iCnt = 0; iCnt < iLen; ++iCnt)
	{
		HomingAimArray[iCnt]->K2_DestroyActor();
	}

	HomingAimArray.Empty();
}

void APlayerRobot::Shot(EHandState HandState, float DeltaTime,bool Left)
{
	//공격 상태일 때만 총알을 생성한다.
	if (HandState == EHandState::Attack)
	{
		//1. 왼쪽인 경우
		if (Left)
		{
			//1-1 아직 전 발사에 대한 텀이 안 끝난 경우
			if (LeftAccumulation < df_FIRE_DURATION)
			{
				LeftAccumulation += DeltaTime;
				return;
			}

			//1-2 총을 발사하니까 누적시간 0으로 바꾸기
			LeftAccumulation = 0;
			//--------------------------------------

			//1-3 총알 생성 위치 설정 및 생성
			FTransform ShotTransform = GetMesh()->GetSocketTransform(TEXT("LeftShotPosition"));
			FVector ShotLocation = ShotTransform.GetLocation();
			FRotator ShotRotator = ShotTransform.GetRotation().Rotator();
			GetWorld()->SpawnActor<APlayerBullet>(Bullet_Template, ShotLocation, ShotRotator);
			//--------------------------------------------------------------------------------
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ShotLocation);

	
			//존재 할 때만 Aim animation play
			if(nullptr != LeftBasicAim)
				LeftBasicAim->PlayAnimation();

		}
		
		//2. 오른쪽인 경우
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

			//TESTTEST
			if (nullptr != RightBasicAim)
				RightBasicAim->PlayAnimation();
			//TESTTEST

		}
	}
}

void APlayerRobot::InitSpawnHoming()
{
	//왼쪽 위 Homing 생성
	HomingArray.Add(GetWorld()->SpawnActor<APlayerHoming>(Homing_Template));
	HomingArray[(int)EHomingLocation::LeftUp]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("LeftUpHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------


	//왼쪽 아래 Homing 생성
	HomingArray.Add(GetWorld()->SpawnActor<APlayerHoming>(Homing_Template));
	HomingArray[(int)EHomingLocation::LeftDown]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("LeftDownHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------


	//오른쪽 위 Homing 생성
	HomingArray.Add(GetWorld()->SpawnActor<APlayerHoming>(Homing_Template));
	HomingArray[(int)EHomingLocation::RightUp]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("RightUpHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------


	//왼쪽 아래 Homing 생성
	HomingArray.Add(GetWorld()->SpawnActor<APlayerHoming>(Homing_Template));
	HomingArray[(int)EHomingLocation::RightDown]->AttachToComponent(GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("RightDownHomingPosition"));
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------
}

void APlayerRobot::AddSpawnHoming()
{
	float CurTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	float LastShotTime = HomingTimeCheck;

	//마지막으로 쏜지 5초가 지났고 현재 미사일 개수가 4개 미만일 때 생성
	if (CurTime - LastShotTime >= 5.0f && CurrentHomingNum < 4)
	{
		APlayerHoming * pNewHoming = GetWorld()->SpawnActor<APlayerHoming>(Homing_Template);

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

		pNewHoming->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), HomingPosition);


		HomingTimeCheck = UGameplayStatics::GetTimeSeconds(GetWorld());
		++CurrentHomingNum;
		

		HomingArray.Add(pNewHoming);
	}
}

void APlayerRobot::UpdateAim(FName SocketName, bool & AimState, ANormalAim *& AimSide,bool IsLeft)
{
	//Check할 시작점 끝점 Setting
	FVector SocketPosition = GetMesh()->GetSocketLocation(SocketName);
	FRotator SocketRotator = GetMesh()->GetSocketRotation(SocketName);
	FVector SocketForwardVector = UKismetMathLibrary::GetForwardVector(SocketRotator);

	SocketForwardVector = SocketForwardVector * 10000.0f;
	FVector EndLocation = SocketPosition + SocketForwardVector;

	//Check할 물체 타입 설정
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
	//적이 Check 됐을 때 Aim 생성
	if (IsResult)
	{
		AActor * HitActor = OutHit.GetActor();
		FRotator HitActorRotator = HitActor->GetActorRotation();
		FVector HitActorLocation = OutHit.Location;

		FRotator LookPlayerRotation = UKismetMathLibrary::FindLookAtRotation(HitActorLocation, GetActorLocation());
		FVector LookPlayerVector = UKismetMathLibrary::GetForwardVector(LookPlayerRotation);
		FVector SpawnAimLocation = LookPlayerVector * 200 + HitActorLocation;

		//Aim이 이미 생성돼있는 상태면 Aim 위치 Update
		if (AimState)
		{
			AimSide->SetActorTransform(UKismetMathLibrary::MakeTransform(SpawnAimLocation,
				LookPlayerRotation, FVector(1.0f, 1.0f, 1.0f)));
		}

		//Aim 생성
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

