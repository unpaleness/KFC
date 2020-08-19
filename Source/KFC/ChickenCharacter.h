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

 public:
  AChickenCharacter();
  virtual void Tick(float DeltaTime) override;
  virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
  virtual void OnConstruction(const FTransform& Transform) override;

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

 protected:
  UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Camera")
  UCameraComponent* SideViewCameraComponent;

  UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Camera")
  USpringArmComponent* CameraBoom;

  UPROPERTY(EditDefaultsOnly, Category = "Chicken")
  FVector CameraOffset{0.f, 0.f, 0.f};

  UPROPERTY(EditDefaultsOnly, Category = "Chicken")
  float CameraArmLength{700.f};
};
