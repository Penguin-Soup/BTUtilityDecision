// Fill out your copyright notice in the Description page of Project Settings.


#include "BTComposite_UtilitySelector.h"

#include "BTDecorator_UtilityEvaluator.h"

UBTComposite_UtilitySelector::UBTComposite_UtilitySelector()
{
	NodeName = "Utility Selector";
}

uint16 UBTComposite_UtilitySelector::GetInstanceMemorySize() const
{
	return sizeof(FBTUtilitySelectorMemory);
}

#if WITH_EDITOR
FName UBTComposite_UtilitySelector::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}
#endif

int32 UBTComposite_UtilitySelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild,
                                                        EBTNodeResult::Type LastResult) const
{
	FBTUtilitySelectorMemory* UtilitySelectorMemory = GetNodeMemory<FBTUtilitySelectorMemory>(SearchData);
	
	// Set quitting and returning to parent as default
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (PrevChild == BTSpecialChild::NotInitialized)
	{
		// Newly activated - sort children based on their utility scores
		DetermineUtilityOrdering(SearchData);
		// Set the next child index as the value of the first index in the custom ordering
		NextChildIdx = UtilitySelectorMemory->UtilityOrdering[0];
	}
	else if (LastResult == EBTNodeResult::Failed)
	{
		// Failed - get the next index of the custom ordering
		const int32 UtilityNextIndex = UtilitySelectorMemory->UtilityOrdering.IndexOfByKey(PrevChild) + 1;

		// Make sure that we haven't gone through all of the children yet
		if (UtilityNextIndex < UtilitySelectorMemory->UtilityOrdering.Num())
		{
			// Set the next child index as the value of the next index in the custom ordering
			NextChildIdx = UtilitySelectorMemory->UtilityOrdering[UtilityNextIndex];
		}
	}

	return NextChildIdx;
}

void UBTComposite_UtilitySelector::DetermineUtilityOrdering(FBehaviorTreeSearchData& SearchData) const
{
	FBTUtilitySelectorMemory* UtilitySelectorMemory = GetNodeMemory<FBTUtilitySelectorMemory>(SearchData);

	// Initialize the array
	UtilitySelectorMemory->UtilityOrdering = TArray<uint8>();
	// Resize the array to the number of children (can access index without needing to add new elements anymore)
	UtilitySelectorMemory->UtilityOrdering.SetNum(GetChildrenNum());

	// Define a struct that represents each child node of the selector
	struct FBTUtilityChild
	{
		int32 Index;
		float Score;
	};

	// Declare the array of utility children
	TArray<FBTUtilityChild> UtilityChildren;
	// Resize the array to the number of children
	UtilityChildren.SetNum(GetChildrenNum());

	// Loop through all the child nodes of this selector
	for (int i = 0; i < GetChildrenNum(); i++)
	{
		// Save the default index of each child in their Index variable (before sorting happens)
		UtilityChildren[i].Index = i;
		// Get the child info (reference variable to not copy the entire struct)
		const FBTCompositeChild& CompositeChild = Children[i];

		// Loop through all the decorators
		for (auto Decorator : CompositeChild.Decorators)
		{
			// If a utility decorator is found, then get its utility score and save it
			if (const auto UtilityEvaluator = Cast<UBTDecorator_UtilityEvaluator>(Decorator))
			{
				UtilityChildren[i].Score = UtilityEvaluator->GetUtility(SearchData.OwnerComp,
					UtilityEvaluator->GetNodeMemory<uint8>(SearchData));
				// Exit for loop (there should be only one utility decorator for each child)
				break;
			}
		}
	}

	// Sort the utility children in descending order from the highest utility score to the lowest
	UtilityChildren.Sort([](const FBTUtilityChild& A, const FBTUtilityChild& B)
	{
		return A.Score > B.Score;
	});

	// Set the new children ordering
	for (int i = 0; i < GetChildrenNum(); i++)
	{
		UtilitySelectorMemory->UtilityOrdering[i] = UtilityChildren[i].Index;
	}
}