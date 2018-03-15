#pragma once
#include "ROSBridgeMsg.h"


class UTagMsg : public FROSBridgeMsg
{
	FString TagType;
	FString Key;
	FString Value;
	

public:
	UTagMsg(){}

	UTagMsg(FString InTagType, FString InKey, FString InValue)
	{
		UTagMsg();
		TagType = InTagType;
		Key = InKey;
		Value = InValue;
	}

	~UTagMsg() override {}

	FString GetTagType()
	{
		return TagType;
	}

	FString GetKey()
	{
		return Key;
	}

	FString GetValue()
	{
		return Value;
	}

	void SetKey(FString InKey)
	{
		Key = InKey;
	}

	void SetValue(FString InValue)
	{
		Value = InValue;
	}

	void SetTagType(FString InTagType)
	{
		TagType = InTagType;
	}

	virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
	{
		Key = JsonObject->GetStringField("Key");
		Value = JsonObject->GetStringField("Value");
		TagType = JsonObject->GetStringField("TagType");
	}

	virtual FString ToString() const override
	{
		return TEXT("String { TagType = \"" + TagType + "\", Key = \"" + Key + "\" and Value = \"" + Value + "\" }");
	}

	virtual TSharedPtr<FJsonObject> ToJsonObject() const override
	{
		TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
		Object->SetStringField(TEXT("Key"), Key);
		Object->SetStringField(TEXT("Value"), Value);
		Object->SetStringField(TEXT("TagType"), TagType);
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