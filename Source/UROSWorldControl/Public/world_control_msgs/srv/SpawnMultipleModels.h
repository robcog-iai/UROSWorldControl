#pragma once

#include "ROSBridgeSrv.h"
#include "ModelDescription.h"

using namespace unreal_world_control_msgs;

class UROSBRIDGE_API FROSBridgeSpawnMultipleModelsSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSBridgeSpawnMultipleModelsSrv(FString Type_)
	{
		Type = Type_;
	}

	class Request : public SrvRequest
	{
	private:
		TArray<ModelDescription> ModelDescriptions;

	public:
		Request()
		{
		}

		TArray<ModelDescription>* GetModelDescriptions() { return &ModelDescriptions; }

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			TArray<TSharedPtr<FJsonValue>> MeshesPtrArray = JsonObject->GetArrayField(TEXT("model_descriptions"));
			for (auto& ptr : MeshesPtrArray)
			{
				ModelDescription Description;
				Description.FromJson(ptr->AsObject());
				ModelDescriptions.Add(Description);
			}
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request req;
			req.FromJson(JsonObject);
			return req;
		}

		FString ToString() const override
		{
			return TEXT("RosWorldControlSpawnMultipleModelsSrv::Request { Number of Models to Spawn: %s }"),
				FString::FromInt(ModelDescriptions.Num());
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			TArray<TSharedPtr<FJsonValue>> MeshesPtrArray;
			for (auto& Tag : ModelDescriptions)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Tag.ToJsonObject()));
				MeshesPtrArray.Add(Ptr);
			}
			Object->SetArrayField(TEXT("model_descriptions"), MeshesPtrArray);


			return Object;
		}
	};


	class Response : public SrvResponse
	{
	private:
		TArray<bool> SuccessList;
		TArray<InstanceId> InstanceIds;
	public:
		Response()
		{
		}

		Response(TArray<bool> InSuccessList, TArray<InstanceId> InInstanceIds)
		{
			SuccessList = InSuccessList;
			InstanceIds = InInstanceIds;
		}

		TArray<bool> GetSuccessList() const { return SuccessList; }
		void SetSuccessList(TArray<bool> InSuccessList) { SuccessList = InSuccessList; }

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			SuccessList.Empty();
			TArray<TSharedPtr<FJsonValue>> SuccessPtrArray = JsonObject->GetArrayField(TEXT("success"));
			for (auto& ptr : SuccessPtrArray)
			{
				SuccessList.Add(ptr->AsBool());
			}

			InstanceIds.Empty();
			TArray<TSharedPtr<FJsonValue>> IdsPtrArray = JsonObject->GetArrayField(TEXT("Instance_ids"));
			for (auto& ptr : IdsPtrArray)
			{
				InstanceId Entry;
				Entry.FromJson(ptr->AsObject());
				InstanceIds.Add(Entry);
			}
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JSonObject)
		{
			Response resp;
			resp.FromJson(JSonObject);
			return resp;
		}

		FString ToString() const override
		{
			return TEXT("RosWorldControlSpawnMultipleModelsSrv::Response");
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			TArray<TSharedPtr<FJsonValue>> SuccessPtrArray;
			for (auto& Entry : SuccessList)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueBoolean(Entry));
				SuccessPtrArray.Add(Ptr);
			}
			Object->SetArrayField("success", SuccessPtrArray);

			TArray<TSharedPtr<FJsonValue>> IdsPtrArray;
			for (auto& Entry : InstanceIds)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Entry.ToJsonObject()));
				IdsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("instance_ids", IdsPtrArray);

			return Object;
		}
	};
};
