#include "ChickenPlayerController.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	UE_LOG(LogChickenPlayerController, Log, TEXT("Process chicken hit"));

	bIsChickenDead_ = true;

	auto GameMode = Cast<AKFCGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode)) {
		GameMode->ProcessDieChicken();
	}
	else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Game mode is invalid!"));
	}
}

void AChickenPlayerController::OnStartMatch() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("On start match"));

	bIsChickenDead_ = false;
	auto ControlledCharacter = Cast<ACharacter>(GetPawn());
	if (IsValid(ControlledCharacter)) {
		ControlledCharacter->SetActorLocation(StartPoint, false, nullptr, ETeleportType::ResetPhysics);
		ControlledCharacter->GetCharacterMovement()->StopMovementImmediately();
		ControlledCharacter->GetCharacterMovement()->GravityScale = 2.f;
	}
	else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Controlled pawn is invalid!"));
		auto GameMode = Cast<AKFCGameMode>(GetWorld()->GetAuthGameMode());
		if (!IsValid(GameMode)) return;
		auto NewCharacter = Cast<ACharacter>(GetWorld()->SpawnActor(GameMode->DefaultPawnClass));
		if (IsValid(NewCharacter)) {
			NewCharacter->SetActorLocation(StartPoint, false, nullptr, ETeleportType::ResetPhysics);
			NewCharacter->GetCharacterMovement()->StopMovementImmediately();
			NewCharacter->GetCharacterMovement()->GravityScale = 2.f;
			Possess(NewCharacter);
		}
		else {
			UE_LOG(LogChickenPlayerController, Warning, TEXT("Failed to spawn new pawn!"));
		}
	}
}

void AChickenPlayerController::OnEndMatch() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("On end match"));

	auto ControlledCharacter = Cast<ACharacter>(GetPawn());
	if (IsValid(ControlledCharacter)) {
		ControlledCharacter->GetCharacterMovement()->GravityScale = 0.f;
		ControlledCharacter->GetCharacterMovement()->StopMovementImmediately();
	}
}

void AChickenPlayerController::BeginPlay() {
	Super::BeginPlay();

	bIsChickenDead_ = false;

	auto GameMode = Cast<AKFCGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode)) {
		GameMode->StartMatchDelegate.AddDynamic(this, &AChickenPlayerController::OnStartMatch);
		GameMode->EndMatchDelegate.AddDynamic(this, &AChickenPlayerController::OnEndMatch);
	}
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
		auto ControlledCharacter = Cast<ACharacter>(GetPawn());
		if (IsValid(ControlledCharacter)) {
			UE_LOG(LogChickenPlayerController, Log, TEXT("%s jumped"), *ControlledCharacter->GetName());
			ControlledCharacter->LaunchCharacter({ 0.f, 0.f, JumpAcceleration }, false, false);
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
		GameMode->ProcessResetGame();
	}
	else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Game mode is invalid!"));
	}
}

void AChickenPlayerController::PauseAction() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Pause button pressed"));
}
