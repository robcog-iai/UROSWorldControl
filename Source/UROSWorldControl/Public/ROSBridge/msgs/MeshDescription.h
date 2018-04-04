#pragma once
#include "ROSBridgeMsg.h"
#include "string.h"



namespace unreal_msgs
{
	class MeshDescription : public FROSBridgeMsg
	{
		FString PathToMesh;
		FString PathToMaterial;


	public:
		MeshDescription() {}

		MeshDescription(FString InPathToMesh, FString InPathToMaterial)
		{
			MeshDescription();
			PathToMesh = InPathToMesh;
			PathToMaterial = InPathToMaterial;
		}

		FString GetPathToMesh()
		{
			return PathToMesh;
		}

		FString GetPathToMaterial()
		{
			return PathToMaterial;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			PathToMesh = JsonObject->GetStringField("path_to_mesh");
			PathToMaterial = JsonObject->GetStringField("path_to_material");
		}

		static MeshDescription GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			MeshDescription Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return TEXT("MeshDescription {path_to_mesh = %s, path_to_material = %s"), PathToMesh, PathToMaterial;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("path_to_mesh"), PathToMesh);
			Object->SetStringField(TEXT("path_to_material"), PathToMaterial);
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