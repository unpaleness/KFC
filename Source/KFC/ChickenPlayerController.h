#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChickenPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChickenPlayerController, Log, All)

/**
 *
 */
UCLASS()
class AChickenPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AChickenPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void SetupInputComponent() override;

private:
	void Jump();
	void StopJumping();

private:
	bool bPressedJump{false};
	bool bIsJumping{false};

	UPROPERTY(EditAnywhere, Category = "Chicken")
	float JumpAcceleration = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Chicken")
	float Gravity = 2.0f;
};
