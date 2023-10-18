// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintConsideration.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

float UBlueprintConsideration::GetValue(UBehaviorTreeComponent& OwnerComp) const
{
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	APawn* ControlledPawn = nullptr;

	if (OwnerController)
	{
		ControlledPawn = OwnerController->GetPawn();
	}

	return BlueprintGetValue(OwnerController, ControlledPawn);
}
