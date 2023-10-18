// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Consideration.h"
#include "BlueprintConsideration.generated.h"

class AAIController;

/**
 * 
 */
UCLASS(BlueprintType, Abstract, Blueprintable)
class BTUTILITYDECISION_API UBlueprintConsideration : public UConsideration
{
	GENERATED_BODY()

public:
	virtual float GetValue(UBehaviorTreeComponent& OwnerComp) const override;

	UFUNCTION(BlueprintImplementableEvent, Category = Utility)
	float BlueprintGetValue(AAIController* OwnerController, APawn* ControlledPawn) const;
};
