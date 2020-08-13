#include "KFCGameMode.h"

#include "ChickenPlayerController.h"
#include "ChickenCharacter.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(LogKFCGameMode)

AKFCGameMode::AKFCGameMode() {
	static ConstructorHelpers::FClassFinder<APawn> BP_Chicken(TEXT("/Game/BP_Chicken"));
	if (BP_Chicken.Class != nullptr) {
		DefaultPawnClass = BP_Chicken.Class;
	} else {
		DefaultPawnClass = AChickenCharacter::StaticClass();
	}
	PlayerControllerClass = AChickenPlayerController::StaticClass();
}

void AKFCGameMode::ProcessDieChicken() {
	EndMatch();
}

bool AKFCGameMode::ProcessResetGame() {
	if (MatchState == MatchState::WaitingPostMatch) {
		StartMatch();
		return true;
	}
	return false;
}

void AKFCGameMode::StartPlay() {
	Super::StartPlay();

	UE_LOG(LogKFCGameMode, Log, TEXT("Game starts"));
}

void AKFCGameMode::StartMatch() {
	Super::StartMatch();

	UE_LOG(LogKFCGameMode, Log, TEXT("Match begins"));

	StartMatchDelegate.Broadcast();
}

void AKFCGameMode::EndMatch() {
	Super::EndMatch();

	UE_LOG(LogKFCGameMode, Log, TEXT("Match ends"));

	EndMatchDelegate.Broadcast();
}
