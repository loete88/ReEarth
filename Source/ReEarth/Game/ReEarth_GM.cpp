// Fill out your copyright notice in the Description page of Project Settings.


#include "ReEarth_GM.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Player/PlayerRobot.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Engine/Classes/Kismet/StereoLayerFunctionLibrary.h"

void AReEarth_GM::BeginPlay()
{
	//Loading 화면 설정하기
	//----------------------------------------------------------------
	UStereoLayerFunctionLibrary::EnableAutoLoadingSplashScreen(true);
	UStereoLayerFunctionLibrary::SetSplashScreen(LoadTexture);
	UStereoLayerFunctionLibrary::HideSplashScreen();
	//----------------------------------------------------------------
}

void AReEarth_GM::GameStart()
{
	APlayerPawn * Player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	APlayerRobot * PlayerRobot = Player->pRobot;
	PlayerRobot->SetActorLocation(FVector(7450.0f, -21800.0f, 200.0f));
	PlayerRobot->RobotInit();
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));
}

void AReEarth_GM::GameExit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		EQuitPreference::Quit, false);
}

void AReEarth_GM::GoToLobby()
{
	APlayerPawn * Player = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	APlayerRobot * PlayerRobot = Player->pRobot;
	PlayerRobot->SetActorLocation(FVector(10900.0f, -47757.0f, 942.0f));
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestStart"));
}