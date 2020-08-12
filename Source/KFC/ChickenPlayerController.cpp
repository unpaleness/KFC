#include "ChickenPlayerController.h"

#include <GameFramework/Character.h>
#include <GameFramework/Pawn.h>

DEFINE_LOG_CATEGORY(LogChickenPlayerController)

AChickenPlayerController::AChickenPlayerController() {
	//AutoReceiveInput = EAutoReceiveInput::Player0;
}

void AChickenPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);

	auto ControlledPawn = Cast<ACharacter>(GetPawn());
	if (IsValid(ControlledPawn)) {
		if (bIsJumping) {
			UE_LOG(LogChickenPlayerController, Log, TEXT("%s jumped"), *ControlledPawn->GetName());
			ControlledPawn->LaunchCharacter({0.0f, 0.0f, JumpAcceleration}, false, false);
			bIsJumping = false;
		}
	}
	else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Controlled pawn is invalid!"));
	}
}

void AChickenPlayerController::ProcessChickenHit(AActor*, AActor*, FVector, const FHitResult&) {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Chicken hit"));

	ChickenKaputtDelegate.Broadcast();
}

void AChickenPlayerController::BeginPlay() {
	auto ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn)) {
		ControlledPawn->OnActorHit.AddDynamic(this, &AChickenPlayerController::ProcessChickenHit);
	}
	else {
		UE_LOG(LogChickenPlayerController, Warning, TEXT("Controlled pawn is invalid!"));
	}
}

void AChickenPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	this->InputComponent->BindAction("Jump", IE_Pressed, this, &AChickenPlayerController::Jump);
	this->InputComponent->BindAction("Jump", IE_Released, this, &AChickenPlayerController::StopJumping);
	this->InputComponent->BindAction("Start", IE_Pressed, this, &AChickenPlayerController::Start);
	this->InputComponent->BindAction("Pause", IE_Pressed, this, &AChickenPlayerController::Pause);
}

void AChickenPlayerController::Jump() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Jump button pressed"));
	if (!bIsJumping) {
		bPressedJump = true;
		bIsJumping = true;
	}
}

void AChickenPlayerController::StopJumping() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Jump button released"));
	bPressedJump = false;
}

void AChickenPlayerController::Start() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Start button pressed"));
	StartGameDelegate.Broadcast();
}

void AChickenPlayerController::Pause() {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Pause button pressed"));
	PauseGameDelegate.Broadcast();
}
