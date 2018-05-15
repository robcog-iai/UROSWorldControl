#pragma once
#include "ROSBridgeMsg.h"


namespace unreal_world_control_msgs
{
	class LinearLimits : public FROSBridgeMsg
	{
		uint8 XMotion;
		uint8 YMotion;
		uint8 ZMotion;
		float Limit;

		bool bUseAdvanced;
		//Advanced
		bool bSoftConstrained;
		float Stiffness;
		float Damping;


	public:
		LinearLimits()
		{
		}

		LinearLimits(uint8 InXMotion, uint8 InYMotion, uint8 InZMotion, float InLimit,
		             bool InbUseAdvanced,
		             bool InSoftConstrained, float InStiffness, float InDamping)
		{
			LinearLimits();
			XMotion = InXMotion;
			YMotion = InYMotion;
			ZMotion = InZMotion;
			Limit = InLimit;
			bUseAdvanced = InbUseAdvanced;
			bSoftConstrained = InSoftConstrained;
			Stiffness = InStiffness;
			Damping = InDamping;
		}

		uint8 GetXMotion()
		{
			return XMotion;
		}

		uint8 GetYMotion()
		{
			return YMotion;
		}

		uint8 GetZMotion()
		{
			return ZMotion;
		}

		float GetLimit()
		{
			return Limit;
		}

		bool GetSoftConstrained()
		{
			return bSoftConstrained;
		}

		float GetStiffness()
		{
			return Stiffness;
		}

		float GetDamping()
		{
			return Damping;
		}

		bool GetUseAdvanced()
		{
			return bUseAdvanced;
		}


		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			XMotion = (uint8)(JsonObject->GetNumberField("x_motion"));
			YMotion = (uint8)(JsonObject->GetNumberField("y_motion"));
			ZMotion = (uint8)(JsonObject->GetNumberField("z_motion"));
			Limit = JsonObject->GetNumberField("limit");
			bUseAdvanced = JsonObject->GetBoolField("use_advanced");
			bSoftConstrained = JsonObject->GetBoolField("soft_constrained");
			Stiffness = JsonObject->GetNumberField("stiffness");
			Damping = JsonObject->GetNumberField("damping");
		}

		static LinearLimits GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			LinearLimits Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		FString ToString() const override
		{
			return TEXT(
					"LinearLimits {x_motion = %d, y_motion = %d z_motion = %d, limit = %s, soft_constrained = %s, stiffness = %s, damping = %s"
				),
				XMotion, YMotion, ZMotion, FString::SanitizeFloat(Limit), bSoftConstrained ? TEXT("True") : TEXT("False"),
				FString::SanitizeFloat(Stiffness), FString::SanitizeFloat(Damping);
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetNumberField(TEXT("x_motion"), XMotion);
			Object->SetNumberField(TEXT("y_motion"), YMotion);
			Object->SetNumberField(TEXT("z_motion"), ZMotion);
			Object->SetNumberField(TEXT("limit"), Limit);
			Object->SetBoolField(TEXT("use_advanced"), bUseAdvanced);
			Object->SetBoolField(TEXT("soft_constrained"), bSoftConstrained);
			Object->SetNumberField(TEXT("stiffness"), Stiffness);
			Object->SetNumberField(TEXT("damping"), Damping);
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
