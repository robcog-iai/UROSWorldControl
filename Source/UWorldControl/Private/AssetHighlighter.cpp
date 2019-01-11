#include "AssetHighlighter.h" 
#include "Tags.h"
#include "Editor.h"

bool FAssetHighlighter::HighlightAsset(AActor* ActorToBeHighlighted, uint8 Color)
{
	if (ActorToBeHighlighted)
	{
#if WITH_EDITOR
		GEditor->BeginTransaction(FText::FromString(TEXT("Highlight: ") + ActorToBeHighlighted->GetActorLabel()));
#endif
		TArray<UStaticMeshComponent*> Components;
		ActorToBeHighlighted->GetComponents<UStaticMeshComponent>(Components);
		for (auto Component : Components)
		{
#if WITH_EDITOR
			Component->Modify();
#endif

			if (Color == 0)
			{
				Component->SetRenderCustomDepth(false);
			}
			else
			{
				Component->SetRenderCustomDepth(true);
				Component->CustomDepthStencilValue = Color;
			}
		}
#if WITH_EDITOR
		ActorToBeHighlighted->Modify();
		GEditor->EndTransaction();
#endif
		return true;
	}
	else
	{

		return false;
	}
}
