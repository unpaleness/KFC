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
  AChickenCharacter();
  virtual void Tick(float DeltaTime) override;
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
  virtual void OnConstruction(const FTransform& Transform) override;

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

protected:
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chicken")
  FVector CameraOffset {0.f, 0.f, 0.f};

  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Chicken")
  float CameraArmLength {700.f};
};
