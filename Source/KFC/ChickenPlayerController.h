#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChickenPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChickenPlayerController, Log, All)

class AKFCGameMode;

UCLASS()
class AChickenPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AChickenPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION()
	void ProcessChickenHit();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void JumpAction();
	void StopJumpingAction();
	void StartAction();
	void ResetAction();
	void PauseAction();

	AKFCGameMode* GetGameMode() const;

private:
	bool bIsChickenJumping{false};
	bool bIsChickenDead{ false };

	UPROPERTY(EditAnywhere, Category = "Chicken")
	float JumpAcceleration = 1000.f;
};
