// Copyright Epic Games, Inc. All Rights Reserved.

#include "KFCGameMode.h"
#include "ChickenPlayerController.h"
#include "ChickenPawn.h"
//#include "UObject/ConstructorHelpers.h"

AKFCGameMode::AKFCGameMode()
{
	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}
	PlayerControllerClass = AChickenPlayerController::StaticClass();
	DefaultPawnClass = AChickenPawn::StaticClass();
}
