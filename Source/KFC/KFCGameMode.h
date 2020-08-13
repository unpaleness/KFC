// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "KFCGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogKFCGameMode, Log, All)

UCLASS(minimalapi)
class AKFCGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AKFCGameMode();

	void ProcessDieChicken();
	bool ProcessStartGame();
	bool ProcessResetGame();

private:
	virtual void StartPlay() override;
	virtual void StartMatch() override;
	virtual void EndMatch() override;
};
