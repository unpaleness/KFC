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

	void ProcessChickenHit();

	UFUNCTION()
	void OnStartMatch();

	UFUNCTION()
	void OnEndMatch();

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
	float JumpAcceleration{ 750.f };

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chicken")
	FVector StartPoint{0.f, 0.f, 0.f};

private:
	bool bIsChickenJumping_ { false };
	bool bIsChickenDead_ { false };

};
