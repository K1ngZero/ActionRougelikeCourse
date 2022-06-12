#pragma once

#include "CoreMinimal.h"
#include "K2Node_ConstructObjectFromClass.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_AsyncAction.h"
#include "K2Node.h"
#include "GameplayTasksEditor/Classes/K2Node_LatentGameplayTaskCall.h"
#include "K2Node_SpawnProjectileTest.generated.h"

UCLASS()
class ACTIONROUGELIKEEDITOR_API UK2Node_SpawnProjectileTest : public UK2Node_LatentGameplayTaskCall
{
	GENERATED_BODY()
	
public:
	UK2Node_SpawnProjectileTest(const FObjectInitializer& ObjectInitializer);

	virtual void AllocateDefaultPins() override;


	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;

private:
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual FText GetMenuCategory() const override;
};
