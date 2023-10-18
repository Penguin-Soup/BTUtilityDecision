// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_UtilityEvaluator.generated.h"

UENUM(BlueprintType)
enum class EUtilityEvalMethod : uint8
{
	// Use the multiplicative average of consideration scores
	GeometricMean,
	// Use the additive average of consideration scores
	ArithmeticMean,
	// Use the lowest consideration score
	Min,
	// Use the highest consideration score
	Max
};

struct FBTUtilityEvaluatorMemory
{
	/** Stored score that other methods can access via node memory*/
	float Score;
};

class UConsideration;
/**
 * Utility evaluator decorator node.
 * A decorator node that calculates and returns a utility score based on its considerations and other parameters.
 */
UCLASS(Meta = (DisplayName = "Utility Evaluator", Category = "Utility"), HideCategories = (Condition, FlowControl))
class BTUTILITYDECISION_API UBTDecorator_UtilityEvaluator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_UtilityEvaluator();
	
	virtual uint16 GetInstanceMemorySize() const override;

	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	
	/** Processes the considerations and outputs a utility score for a behaviour */
	virtual float GetUtility(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

protected:
	float GetGeometricMean(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	float GetArithmeticMean(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	float GetMinScore(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	float GetMaxScore(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

	/** The method that the evaluator will use to determine the utility score */
	UPROPERTY(EditAnywhere)
	EUtilityEvalMethod EvaluationMethod;

	/** The amount of randomness to apply to the utility score. 0 = no randomness, 1 = very high randomness */
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "1", UIMin = "0", UIMax = "1"))
	float Randomness;

	/** An array of considerations that will be used to determine the utility score for a behaviour */
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UConsideration>> Considerations;
};
