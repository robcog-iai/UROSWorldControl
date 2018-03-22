#pragma once
#include "ROSBridgeMsg.h"
#include "Pose.h"
#include "ROSBridge/msgs/Tag.h"


class UStaticMeshDescription : public FROSBridgeMsg
{
	FString PathToMesh;
	FString PathToMaterial;
	geometry_msgs::Pose Pose;
	TArray<UTagMsg> Tags;


public:
	UStaticMeshDescription() {}

	UStaticMeshDescription(FString InPathToMesh, FString InPathToMaterial, geometry_msgs::Pose InPose, TArray<UTagMsg> InTags)
	{
		UStaticMeshDescription();
		PathToMesh = InPathToMesh;
		PathToMaterial = InPathToMaterial;
		Pose = InPose;
		Tags = InTags;
	}

	~UStaticMeshDescription() override {}

	FString GetPathToMesh()
	{
		return PathToMesh;
	}

	FString GetPathToMaterial()
	{
		return PathToMaterial;
	}

	geometry_msgs::Pose GetPose()
	{
		return Pose;
	}

	FVector GetLocation()
	{
		return Pose.GetPosition().GetVector();
	}

	FRotator GetRotator()
	{
		return FRotator::FRotator(Pose.GetOrientation().GetQuat());
	}

	TArray<UTagMsg> GetTags() {
		return Tags;
	}

	void SetPathToMesh(FString InPathToMesh)
	{
		PathToMesh = InPathToMesh;
	}

	void SetPathToMaterial(FString InPathToMaterial)
	{
		PathToMaterial = InPathToMaterial;
	}

	void SetPose(geometry_msgs::Pose InPose)
	{
		Pose = InPose;
	}
	
	void SetTags(TArray<UTagMsg> InTags)
	{
		Tags = InTags;
	}

	virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
	{
		PathToMesh = JsonObject->GetStringField("path_to_mesh");
		PathToMaterial = JsonObject->GetStringField("path_to_material");
		Pose = Pose.GetFromJson(JsonObject->GetObjectField("pose"));

		Tags.Empty();
		TArray<TSharedPtr<FJsonValue>> TagsPtrArray = JsonObject->GetArrayField(TEXT("tags"));
		for (auto &ptr : TagsPtrArray)
		{
			UTagMsg Tag;
			Tag.FromJson(ptr->AsObject());
			Tags.Add(Tag);
		}

	}

	virtual FString ToString() const override
	{
		return TEXT("String { PathToMesh = \"" + PathToMesh + "\", PathToMaterial = \"" + PathToMaterial + "\" and Pose = \"" + Pose.ToString() + "\" }");
	}

	virtual TSharedPtr<FJsonObject> ToJsonObject() const override
	{
		TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
		Object->SetStringField("path_to_mesh", PathToMesh);
		Object->SetStringField("path_to_material", PathToMaterial);
		Object->SetObjectField("pose", Pose.ToJsonObject());

		TArray<TSharedPtr<FJsonValue>> TagsPtrArray;
		for (auto &Tag : Tags)
		{
			TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Tag.ToJsonObject()));
			TagsPtrArray.Add(Ptr);
		}
		Object->SetArrayField("tags", TagsPtrArray);


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