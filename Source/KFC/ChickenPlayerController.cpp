#include "ChickenPlayerController.h"

#include <GameFramework/Character.h>
#include <GameFramework/Pawn.h>


DEFINE_LOG_CATEGORY(LogChickenPlayerController)


AChickenPlayerController::AChickenPlayerController() {
	AutoReceiveInput = EAutoReceiveInput::Player0;
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

void AChickenPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	this->InputComponent->BindAction("Jump", IE_Pressed, this, &AChickenPlayerController::Jump);
	this->InputComponent->BindAction("Jump", IE_Released, this, &AChickenPlayerController::StopJumping);
}

void AChickenPlayerController::Jump() {
	if (!bIsJumping) {
		bPressedJump = true;
		bIsJumping = true;
	}
}

void AChickenPlayerController::StopJumping() {
	bPressedJump = false;
}
