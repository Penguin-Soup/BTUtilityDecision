// Copyright Penguin-Soup. All Rights Reserved.

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
