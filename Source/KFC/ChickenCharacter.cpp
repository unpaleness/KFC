#include "ChickenCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UObject/ConstructorHelpers.h"

#include "ChickenPlayerController.h"

DEFINE_LOG_CATEGORY(LogChickenCharacter)

// Sets default values
AChickenCharacter::AChickenCharacter() {
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshCube(TEXT("SkeletalMesh'/Engine/EngineMeshes/SkeletalCube.SkeletalCube'"));

	if (SkeletalMeshCube.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SkeletalMeshCube.Object);
	}
	//GetMesh()->SetRelativeScale3D(FVector(0.5f));

	GetCapsuleComponent()->InitCapsuleSize(25.f, 25.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->SetUsingAbsoluteLocation(true);
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 700.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 0.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	//GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	//GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	//GetCharacterMovement()->AirControl = 0.80f;
	//GetCharacterMovement()->JumpZVelocity = 1000.f;
	//GetCharacterMovement()->GroundFriction = 3.f;
	//GetCharacterMovement()->MaxWalkSpeed = 600.f;
	//GetCharacterMovement()->MaxFlySpeed = 600.f;

	OnActorHit.AddDynamic(this, &AChickenCharacter::ProcessChickenHit);
}

void AChickenCharacter::ProcessChickenHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) {
	UE_LOG(LogChickenPlayerController, Log, TEXT("Chicken hit"));

	auto ChickenController = Cast<AChickenPlayerController>(GetController());
	if (IsValid(ChickenController)) {
		ChickenController->ProcessChickenHit();
	}
}

// Called when the game starts or when spawned
void AChickenCharacter::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void AChickenCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AChickenCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
