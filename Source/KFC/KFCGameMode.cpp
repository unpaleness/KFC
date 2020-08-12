// Copyright Epic Games, Inc. All Rights Reserved.

#include "KFCGameMode.h"
#include "ChickenPlayerController.h"
#include "ChickenCharacter.h"

DEFINE_LOG_CATEGORY(LogKFCGameMode)

AKFCGameMode::AKFCGameMode()
{
	PlayerControllerClass = AChickenPlayerController::StaticClass();
	DefaultPawnClass = AChickenCharacter::StaticClass();
}

void AKFCGameMode::ProcessChickenKaputt() {
	EndMatch();
}

void AKFCGameMode::ProcessStartGame() {
	if (MatchState == MatchState::WaitingToStart) {
		StartMatch();
	}
}

void AKFCGameMode::StartPlay() {
	if (MatchState == MatchState::EnteringMap) {
		SetMatchState(MatchState::WaitingToStart);
	}

	UE_LOG(LogKFCGameMode, Log, TEXT("Game starts"));

	auto PlayerController = Cast<AChickenPlayerController>(GetWorld()->GetFirstPlayerController());
	if (IsValid(PlayerController)) {
		PlayerController->ChickenKaputtDelegate.AddDynamic(this, &AKFCGameMode::ProcessChickenKaputt);
		PlayerController->StartGameDelegate.AddDynamic(this, &AKFCGameMode::ProcessStartGame);
	}
	else {
		UE_LOG(LogKFCGameMode, Warning, TEXT("Player controller is invalid!"));
	}
}

void AKFCGameMode::StartMatch() {
	Super::StartMatch();

	UE_LOG(LogKFCGameMode, Log, TEXT("Match begins"));
}

void AKFCGameMode::EndMatch() {
	Super::EndMatch();

	UE_LOG(LogKFCGameMode, Log, TEXT("Match ends"));
}
