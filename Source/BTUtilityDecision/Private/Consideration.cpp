// Fill out your copyright notice in the Description page of Project Settings.


#include "Consideration.h"

float UConsideration::EvaluateScore(UBehaviorTreeComponent& OwnerComp) const
{
	if (!ResponseCurve)
	{
		UE_LOG(LogTemp, Warning, TEXT("Response curve has not been assigned for the consideration %s!"), *this->GetName());
		return 0.f;
	}

	// Return the value of the curve at the point in time specified by the input value.
	const float Score = ResponseCurve->GetFloatValue(FMath::Clamp(GetValue(OwnerComp), 0.f, 1.f));
	
	// Clamp the score between 0 and 1, on the off-chance that the response curve is not set between 0 and 1.
	return FMath::Clamp(Score, 0.f, 1.f);
}

float UConsideration::GetValue(UBehaviorTreeComponent& OwnerComp) const
{
	return 0.f;
}
