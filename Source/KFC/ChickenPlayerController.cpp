#include "ChickenPlayerController.h"

#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"

#include "ChickenCharacter.h"
#include "KFCGameMode.h"

DEFINE_LOG_CATEGORY(LogChickenPlayerController)

AChickenPlayerController::AChickenPlayerController() {
	//AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AChickenPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
}

void AChickenPlayerController::ProcessChickenHit() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Chicken hit"));

	bIsChickenDead_ = true;

	auto GameMode = Cast<AKFCGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode)) {
		GameMode->ProcessDieChicken();
	}
	else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Game mode is invalid!"));
	}
}

void AChickenPlayerController::BeginPlay() {
	Super::BeginPlay();

	bIsChickenDead_ = false;
}

void AChickenPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	this->InputComponent->BindAction("Jump", IE_Pressed, this, &AChickenPlayerController::JumpAction);
	this->InputComponent->BindAction("Jump", IE_Released, this, &AChickenPlayerController::StopJumpingAction);
	this->InputComponent->BindAction("Reset", IE_Pressed, this, &AChickenPlayerController::ResetAction);
	this->InputComponent->BindAction("Pause", IE_Pressed, this, &AChickenPlayerController::PauseAction);
}

void AChickenPlayerController::JumpAction() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Jump button pressed"));
	if (!bIsChickenJumping_ && !bIsChickenDead_) {
		bIsChickenJumping_ = true;
		auto ControlledPawn = Cast<ACharacter>(GetPawn());
		if (IsValid(ControlledPawn)) {
			UE_LOG(LogChickenPlayerController, Log, TEXT("%s jumped"), *ControlledPawn->GetName());
			ControlledPawn->LaunchCharacter({ 0.0f, 0.0f, JumpAcceleration }, false, false);
		} else {
			UE_LOG(LogChickenPlayerController, Warning, TEXT("Controlled pawn is invalid!"));
		}
	}
}

void AChickenPlayerController::StopJumpingAction() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Jump button released"));
	bIsChickenJumping_ = false;
}

void AChickenPlayerController::ResetAction() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Reset button pressed"));

	auto GameMode = Cast<AKFCGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode)) {
		const auto bIsGameReset = GameMode->ProcessResetGame();
		if (bIsGameReset) {
			bIsChickenDead_ = false;
			auto ControlledPawn = GetPawn();
			if (IsValid(ControlledPawn)) {
				ControlledPawn->SetActorLocation({0.f, 0.f, 0.f}, false, nullptr, ETeleportType::ResetPhysics);
			} else {
				UE_LOG(LogChickenPlayerController, Warning, TEXT("Controlled pawn is invalid!"));
			}
		}
	}
	else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Game mode is invalid!"));
	}
}

void AChickenPlayerController::PauseAction() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Pause button pressed"));
}
