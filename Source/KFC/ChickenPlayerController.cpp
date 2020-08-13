#include "ChickenPlayerController.h"

#include "GameFramework/Character.h"
#include "GameFramework/Pawn.h"

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

	bIsChickenDead = true;

	auto GameMode = GetGameMode();
	if (IsValid(GameMode)) {
		GameMode->ProcessDieChicken();
	}
	else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Game mode is invalid!"));
	}
}

void AChickenPlayerController::BeginPlay() {
	Super::BeginPlay();

	bIsChickenDead = false;
}

void AChickenPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	this->InputComponent->BindAction("Jump", IE_Pressed, this, &AChickenPlayerController::JumpAction);
	this->InputComponent->BindAction("Jump", IE_Released, this, &AChickenPlayerController::StopJumpingAction);
	this->InputComponent->BindAction("Start", IE_Pressed, this, &AChickenPlayerController::StartAction);
	this->InputComponent->BindAction("Reset", IE_Pressed, this, &AChickenPlayerController::ResetAction);
	this->InputComponent->BindAction("Pause", IE_Pressed, this, &AChickenPlayerController::PauseAction);
}

void AChickenPlayerController::JumpAction() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Jump button pressed"));
	if (!bIsChickenJumping && !bIsChickenDead) {
		bIsChickenJumping = true;
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
	bIsChickenJumping = false;
}

void AChickenPlayerController::StartAction() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Start button pressed"));

	auto GameMode = GetGameMode();
	if (IsValid(GameMode)) {
		GameMode->ProcessStartGame();
	} else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Game mode is invalid!"));
	}
}

void AChickenPlayerController::ResetAction() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Reset button pressed"));

	auto GameMode = GetGameMode();
	if (IsValid(GameMode)) {
		const auto bIsGameReset = GameMode->ProcessResetGame();
		if (bIsGameReset) {
			bIsChickenDead = false;
		}
	}
	else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Game mode is invalid!"));
	}
}

void AChickenPlayerController::PauseAction() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Pause button pressed"));
}

AKFCGameMode* AChickenPlayerController::GetGameMode() const {
	return Cast<AKFCGameMode>(GetWorld()->GetAuthGameMode());
}
