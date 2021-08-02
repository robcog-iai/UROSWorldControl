#include "SrvCallbacks/DeleteAllServer.h"
#include "AssetModifier.h"
#include "Tags.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSDeleteAllServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSDeleteAllSrv::Request> Request_ =
		MakeShareable(new FROSDeleteAllSrv::Request());
	Request_->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request_);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSDeleteAllServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSDeleteAllSrv::Request> RemoveAllRequest =
		StaticCastSharedPtr<FROSDeleteAllSrv::Request>(Request);

	// Do all calculations that don't need a game thread first.

	// Get the important values from the Request.
	FString Type = RemoveAllRequest->GetType();
	FString Key = RemoveAllRequest->GetKey();
	FString Value = RemoveAllRequest->GetValue();
	bool ignoreValue = RemoveAllRequest->GetIgnore();

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			ServiceSuccess = true;

			// Start of finding all matching actors, taking bool in consideration if value included or not.
			TArray<AActor*> AllMatchingActors;

			if (ignoreValue)
			{
				FTags::GetActorsToKeyValue(World, Type, Key).GenerateKeyArray(AllMatchingActors);
			}
			else
			{
				AllMatchingActors = FTags::GetActorsWithKeyValuePair(World, Type, Key, Value);
			}

			for (AActor* Actor : AllMatchingActors)
			{
				ServiceSuccess = ServiceSuccess && Actor->Destroy();
			}
		}, TStatId(), nullptr, ENamedThreads::GameThread);

	// Wait code above to finish
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

	return TSharedPtr<FROSBridgeSrv::SrvResponse>
		(new FROSDeleteAllSrv::Response(ServiceSuccess));
};
