#include "AssetHighlighter.h" 
#include "Tags.h"

bool FAssetHighlighter::HighlightAsset(AActor* ActorToBeHighlighted, uint8 Color)
{
	if (ActorToBeHighlighted)
	{

		TArray<UStaticMeshComponent*> Components;
		ActorToBeHighlighted->GetComponents<UStaticMeshComponent>(Components);
		for (auto Component : Components)
		{
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
		return true;
	}
	else
	{

		return false;
	}
}
