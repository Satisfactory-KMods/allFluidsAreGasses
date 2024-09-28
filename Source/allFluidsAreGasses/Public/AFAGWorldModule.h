// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KBFLGameInstanceModule.h"
#include "KBFLWorldModule.h"
#include "AFAGWorldModule.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ALLFLUIDSAREGASSES_API UAFAGWorldModule : public UKBFLGameInstanceModule
{
	GENERATED_BODY()

public:
	UAFAGWorldModule();

	virtual void ConstructionPhase_Delayed() override;
	void OverwriteItems();
};
