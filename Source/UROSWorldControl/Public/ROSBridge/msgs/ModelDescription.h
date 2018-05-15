#pragma once
#include "ROSBridgeMsg.h"
#include "InstanceId.h"
#include "MeshDescription.h"
#include "Pose.h"
#include "Tag.h"


namespace unreal_world_control_msgs
{
	class ModelDescription : public FROSBridgeMsg
	{
		InstanceId InstanceId;
		MeshDescription MeshDescription;
		geometry_msgs::Pose Pose;
		TArray<Tag> Tags;
		bool bIsStatic;


	public:
		ModelDescription()
		{
		}

		ModelDescription(unreal_world_control_msgs::InstanceId InInstanceId,
		                 unreal_world_control_msgs::MeshDescription InMeshDescription, geometry_msgs::Pose InPose,
		                 TArray<Tag> InTags, bool InbIsStatic)
		{
			ModelDescription();
			InstanceId = InInstanceId;
			MeshDescription = InMeshDescription;
			Pose = InPose;
			Tags = InTags;
			bIsStatic = InbIsStatic;
		}

		unreal_world_control_msgs::InstanceId GetInstanceId()
		{
			return InstanceId;
		}

		unreal_world_control_msgs::MeshDescription GetMeshDescription()
		{
			return MeshDescription;
		}

		geometry_msgs::Pose GetPose()
		{
			return Pose;
		}

		TArray<Tag> GetTags()
		{
			return Tags;
		}

		bool IsStatic()
		{
			return bIsStatic;
		}

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			InstanceId.FromJson(JsonObject->GetObjectField("instance_id"));
			MeshDescription.FromJson(JsonObject->GetObjectField("mesh_description"));
			Pose.FromJson(JsonObject->GetObjectField("pose"));
			bIsStatic = JsonObject->GetBoolField("is_static");
			Tags.Empty();
			TArray<TSharedPtr<FJsonValue>> TagsPtrArray = JsonObject->GetArrayField(TEXT("tags"));
			for (auto& ptr : TagsPtrArray)
			{
				Tag Entry;
				Entry.FromJson(ptr->AsObject());
				Tags.Add(Entry);
			}
		}

		static ModelDescription GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			ModelDescription Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		FString ToString() const override
		{
			return TEXT("ModelDescription {instance_id = %s, mesh_description = %s, pose = %s,% tags size = %s"), InstanceId.
				ToString(), MeshDescription.ToString(), Pose.ToString(), FString::FromInt(Tags.Num());
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetObjectField(TEXT("instance_id"), InstanceId.ToJsonObject());
			Object->SetObjectField(TEXT("mesh_description"), MeshDescription.ToJsonObject());
			Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
			Object->SetBoolField(TEXT("is_static"), bIsStatic);
			TArray<TSharedPtr<FJsonValue>> TagsPtrArray;
			for (auto& Entry : Tags)
			{
				TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueObject(Entry.ToJsonObject()));
				TagsPtrArray.Add(Ptr);
			}
			Object->SetArrayField("tags", TagsPtrArray);

			return Object;
		}

		FString ToYamlString() const override
		{
			FString OutputString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(ToJsonObject().ToSharedRef(), Writer);
			return OutputString;
		}
	};
}
