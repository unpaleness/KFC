#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ChickenCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogChickenCharacter, Log, All)

class UCharacterMovementComponent;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class AChickenCharacter : public ACharacter {
	GENERATED_BODY()

	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	/** Camera boom positioning the camera beside the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

public:
	// Sets default values for this pawn's properties
	AChickenCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
