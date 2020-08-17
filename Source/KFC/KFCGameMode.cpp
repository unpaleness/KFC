#include "KFCGameMode.h"

#include "ChickenCharacter.h"
#include "ChickenPlayerController.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(LogKFCGameMode)

AKFCGameMode::AKFCGameMode() {
  static const TCHAR* BPPawnName = TEXT("/Game/BP_Chicken");
  static const ConstructorHelpers::FClassFinder<APawn> BPChicken(BPPawnName);
  if (BPChicken.Class != nullptr) {
    DefaultPawnClass = BPChicken.Class;
  } else {
    UE_LOG(LogKFCGameMode, Warning, TEXT("Blueprint Pawn %s not found, falling back to %s"), BPPawnName,
           *AChickenCharacter::StaticClass()->GetName());
    DefaultPawnClass = AChickenCharacter::StaticClass();
  }

  static const TCHAR* BPPlayerControllerName = TEXT("/Game/BP_ChickenController");
  static const ConstructorHelpers::FClassFinder<APlayerController> BPChickenController(BPPlayerControllerName);
  if (BPChickenController.Class != nullptr) {
    PlayerControllerClass = BPChickenController.Class;
  } else {
    UE_LOG(LogKFCGameMode, Warning, TEXT("Blueprint PlayerController %s not found, falling back to %s"),
           BPPlayerControllerName, *AChickenPlayerController::StaticClass()->GetName());
    PlayerControllerClass = AChickenPlayerController::StaticClass();
  }
}

void AKFCGameMode::ProcessDieChicken() {
  EndMatch();
}

bool AKFCGameMode::ProcessResetGame() {
  if (MatchState == MatchState::WaitingPostMatch || MatchState == MatchState::InProgress) {
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
