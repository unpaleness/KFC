#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChickenPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChickenPlayerController, Log, All)

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
	void ResetAction();
	void PauseAction();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chicken")
	float JumpAcceleration = 750.f;

private:
	bool bIsChickenJumping_ { false };
	bool bIsChickenDead_ { false };

};
