#pragma once
#include "ROSBridgeMsg.h"
#include "String.h"



namespace unreal_msgs
{
	class InstanceId : public FROSBridgeMsg
	{
		FString ClassName;
		FString Id;
		FString Ns;


	public:
		InstanceId() {}

		InstanceId(FString InClassName, FString  InId, FString  InNs)
		{
			InstanceId();
			ClassName = InClassName;
			Id = InId;
			Ns = InNs;
		}

		FString GetModelClassName()
		{
			return ClassName;
		}

		FString  GetId()
		{
			return Id;
		}

		void SetId(FString InId)
		{
			Id = InId;
		}

		FString  GetNs()
		{
			return Ns;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			ClassName = JsonObject->GetStringField("class_name");
			Id = JsonObject->GetStringField("id");
			Ns = JsonObject->GetStringField("ns");
		}

		static InstanceId GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			InstanceId Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return TEXT("InstanceId {class_name = %s, id = %s, ns = %s"), ClassName, Id, Ns;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("class_name"), ClassName);
			Object->SetStringField(TEXT("id"), Id);
			Object->SetStringField(TEXT("ns"), Ns);
			return Object;
		}

		virtual FString ToYamlString() const override
		{
			FString OutputString;
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(ToJsonObject().ToSharedRef(), Writer);
			return OutputString;
		}
	};
}