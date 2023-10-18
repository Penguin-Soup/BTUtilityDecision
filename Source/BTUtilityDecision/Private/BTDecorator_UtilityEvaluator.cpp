// Copyright Penguin-Soup. All Rights Reserved.


#include "BTDecorator_UtilityEvaluator.h"
#include "Consideration.h"

UBTDecorator_UtilityEvaluator::UBTDecorator_UtilityEvaluator()
{
	NodeName = "Utility Evaluator";
}

uint16 UBTDecorator_UtilityEvaluator::GetInstanceMemorySize() const
{
	return sizeof(FBTUtilityEvaluatorMemory);
}

void UBTDecorator_UtilityEvaluator::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                          EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	const FBTUtilityEvaluatorMemory* UtilityEvaluatorMemory = CastInstanceNodeMemory<FBTUtilityEvaluatorMemory>(NodeMemory);

	Values.Add(FString::Printf(TEXT("Utility Score: %f"), UtilityEvaluatorMemory->Score));
}

float UBTDecorator_UtilityEvaluator::GetUtility(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float UtilityScore = 0.f;

	switch (EvaluationMethod)
	{
		case EUtilityEvalMethod::GeometricMean:
			UtilityScore = GetGeometricMean(OwnerComp, NodeMemory);
			break;
		case EUtilityEvalMethod::ArithmeticMean:
			UtilityScore = GetArithmeticMean(OwnerComp, NodeMemory);
			break;
		case EUtilityEvalMethod::Min:
			UtilityScore = GetMinScore(OwnerComp, NodeMemory);
			break;
		case EUtilityEvalMethod::Max:
			UtilityScore = GetMaxScore(OwnerComp, NodeMemory);
			break;
	}

	// Randomise the score specified by the Randomness float. If Randomness was 0, then the score is not affected.
	UtilityScore *= FMath::FRandRange(1.f - Randomness, 1.f);

	// Store the score in the node's memory (so other methods can access the score)
	FBTUtilityEvaluatorMemory* UtilityEvaluatorMemory = CastInstanceNodeMemory<FBTUtilityEvaluatorMemory>(NodeMemory);
	UtilityEvaluatorMemory->Score = UtilityScore;

	return UtilityScore;
}

float UBTDecorator_UtilityEvaluator::GetGeometricMean(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// Set initial score to 1 so that it doesn't affect subsequent multiplications
	float Score = 1.f;

	// Multiply all considerations
	for (const auto Consideration : Considerations)
	{
		Score *= Consideration->EvaluateScore(OwnerComp);
	}

	// Get an average via raising the score to the power of 1 / num of considerations
	Score = FMath::Pow(Score, 1.f / Considerations.Num());

	return Score;
}

float UBTDecorator_UtilityEvaluator::GetArithmeticMean(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float Score = 0.f;

	// Add all considerations
	for (const auto Consideration : Considerations)
	{
		Score += Consideration->EvaluateScore(OwnerComp);
	}

	// Divide the score by the consideration count
	Score /= Considerations.Num();

	return Score;
}

float UBTDecorator_UtilityEvaluator::GetMinScore(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float MinScore = 1.f;

	// Find the minimum value
	for (const auto Consideration : Considerations)
	{
		if (const float Score = Consideration->EvaluateScore(OwnerComp); Score < MinScore)
		{
			MinScore = Score;
		}
	}

	return MinScore;
}

float UBTDecorator_UtilityEvaluator::GetMaxScore(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float MaxScore = 0.f;

	// Find the maximum value
	for (const auto Consideration : Considerations)
	{
		if (const float Score = Consideration->EvaluateScore(OwnerComp); Score > MaxScore)
		{
			MaxScore = Score;
		}
	}

	return MaxScore;
}
