#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChickenPlayerController.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChickenPlayerController, Log, All)

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FChickenKaputtDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartGameDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseGameDelegate);

/**
 *
 */
UCLASS()
class AChickenPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AChickenPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION()
	void ProcessChickenHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	FChickenKaputtDelegate ChickenKaputtDelegate;
	FStartGameDelegate StartGameDelegate;
	FPauseGameDelegate PauseGameDelegate;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Jump();
	void StopJumping();
	void Start();
	void Pause();

private:
	bool bPressedJump{false};
	bool bIsJumping{false};

	UPROPERTY(EditAnywhere, Category = "Chicken")
	float JumpAcceleration = 1000.f;
};
