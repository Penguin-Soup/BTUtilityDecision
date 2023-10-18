// Copyright Penguin-Soup. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Consideration.generated.h"

class UBehaviorTreeComponent;

/**
 * A UObject allowing one to create a consideration for an AI agent (e.g. hunger, thirst etc)
 */
UCLASS(BlueprintType, EditInlineNew, DefaultToInstanced, Abstract)
class BTUTILITYDECISION_API UConsideration : public UObject
{
	GENERATED_BODY()

public:
	/** Returns a value of the response curve at the point in time specified by the GetValue() */
	float EvaluateScore(UBehaviorTreeComponent& OwnerComp) const;

protected:
	/** Returns a raw value for the consideration. Value must be normalized between 0 and 1 */
	virtual float GetValue(UBehaviorTreeComponent& OwnerComp) const;

private:
	/** A float curve used for transforming the consideration's value into a score  */
	UPROPERTY(EditAnywhere, Category = ResponseCurve)
	TObjectPtr<UCurveFloat> ResponseCurve;
};
