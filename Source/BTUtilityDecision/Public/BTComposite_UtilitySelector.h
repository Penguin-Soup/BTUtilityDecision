// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BTComposite_UtilitySelector.generated.h"

struct FBTUtilitySelectorMemory : FBTCompositeMemory
{
	/** An array of child node indices, ordered by their utility scores in descending order */
	TArray<uint8> UtilityOrdering;
};

/**
 * Utility selector composite node.
 * A composite node that behaves like a traditional selector,
 * except that children are executed based on their utility score in descending order.
 */
UCLASS(Meta = (DisplayName = "Utility Selector", Category = "Utility"))
class BTUTILITYDECISION_API UBTComposite_UtilitySelector : public UBTCompositeNode
{
	GENERATED_BODY()

public:
	UBTComposite_UtilitySelector();

	virtual uint16 GetInstanceMemorySize() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

	virtual int32 GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;

protected:
	/** Determines the ordering of child nodes via sorting them based on their utility scores */
	virtual void DetermineUtilityOrdering(FBehaviorTreeSearchData& SearchData) const;
};
