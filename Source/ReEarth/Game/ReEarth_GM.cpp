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
	PlayerRobot->SetActorLocation(FVector(8650.000000, -23800.000000, 200.0f));
	PlayerRobot->SetActorRotation(FRotator(0.0f, 90.0f, 0.0));
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


	PlayerRobot->RobotInit();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMap"));


	//PlayerRobot->SetActorLocation(FVector(94840.000000, -19607.250000f, 1122.021729f));
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("TestStart"));
}