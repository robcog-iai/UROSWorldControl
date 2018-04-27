#pragma once
#include "ROSBridgeMsg.h"
#include "string.h"
#include "string.h"
#include "string.h"



namespace unreal_world_control_msgs
{
	class Tag : public FROSBridgeMsg
	{
		FString  TagType;
		FString  Key;
		FString  Value;


	public:
		Tag() {}

		Tag(FString  InTagType, FString  InKey, FString  InValue)
		{
			Tag();
			TagType = InTagType;
			Key = InKey;
			Value = InValue;
		}

		FString  GetTagType()
		{
			return TagType;
		}

		FString  GetKey()
		{
			return Key;
		}

		FString  GetValue()
		{
			return Value;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			TagType = JsonObject->GetStringField("tag_type");
			Key = JsonObject->GetStringField("key");
			Value = JsonObject->GetStringField("value");
		}

		static Tag GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Tag Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return TEXT("Tag {tag_type = %s, key = %s, value = %s"), TagType, Key, Value;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("tag_type"), TagType);
			Object->SetStringField(TEXT("key"), Key);
			Object->SetStringField(TEXT("value"), Value);
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