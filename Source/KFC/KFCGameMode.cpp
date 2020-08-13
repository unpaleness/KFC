#include "KFCGameMode.h"

#include "ChickenPlayerController.h"
#include "ChickenCharacter.h"

DEFINE_LOG_CATEGORY(LogKFCGameMode)

AKFCGameMode::AKFCGameMode()
{
	bDelayedStart = true;
	PlayerControllerClass = AChickenPlayerController::StaticClass();
	DefaultPawnClass = AChickenCharacter::StaticClass();
}

void AKFCGameMode::ProcessDieChicken() {
	EndMatch();
}

bool AKFCGameMode::ProcessStartGame() {
	if (MatchState == MatchState::WaitingToStart) {
		StartMatch();
		return true;
	}
	return false;
}
bool AKFCGameMode::ProcessResetGame() {
	if (MatchState == MatchState::WaitingPostMatch) {
		StartPlay();
		return true;
	}
	return false;
}


void AKFCGameMode::StartPlay() {
	if (MatchState == MatchState::EnteringMap) {
		SetMatchState(MatchState::WaitingToStart);
	}

	UE_LOG(LogKFCGameMode, Log, TEXT("Game starts"));
}

void AKFCGameMode::StartMatch() {
	Super::StartMatch();

	UE_LOG(LogKFCGameMode, Log, TEXT("Match begins"));
}

void AKFCGameMode::EndMatch() {
	Super::EndMatch();

	UE_LOG(LogKFCGameMode, Log, TEXT("Match ends"));
}
