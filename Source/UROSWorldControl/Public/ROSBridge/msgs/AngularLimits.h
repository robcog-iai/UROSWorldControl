#pragma once
#include "ROSBridgeMsg.h"

namespace unreal_world_control_msgs
{
	class AngularLimits : public FROSBridgeMsg
	{
		uint8 Swing1Motion;
		uint8 Swing2Motion;
		uint8 TwistMotion;
		float Swing1LimitAngle;
		float Swing2LimitAngle;
		float TwistLimitAngle;

		bool bUseAdvanced;
		//Advanced
		bool bSwingSoftConstrained;
		float SwingStiffness;
		float SwingDamping;
		bool bTwistSoftConstrained;
		float TwistStiffness;
		float TwistDamping;


	public:
		AngularLimits()
		{
		}

		AngularLimits(uint8 InSwing1Motion, uint8 InSwing2Motion, uint8 InTwistMotion,
		              float InSwing1LimitAngle, float InSwing2LimitAngle, float InTwistLimitAngle,
		              bool InbUseAdvanced,
		              bool InSwingSoftConstrained, float InSwingStiffness, float InSwingDamping,
		              bool InTwistSoftConstrained, float InTwistStiffness, float InTwistDamping)
		{
			AngularLimits();
			Swing1Motion = InSwing1Motion;
			Swing2Motion = InSwing2Motion;
			TwistMotion = InTwistMotion;
			Swing1LimitAngle = InSwing1LimitAngle;
			Swing2LimitAngle = InSwing2LimitAngle;
			TwistLimitAngle = InTwistLimitAngle;
			bUseAdvanced = InbUseAdvanced;
			bSwingSoftConstrained = InSwingSoftConstrained;
			SwingStiffness = InSwingStiffness;
			SwingDamping = InSwingDamping;
			bTwistSoftConstrained = InTwistSoftConstrained;
			TwistStiffness = InTwistStiffness;
			TwistDamping = InTwistDamping;
		}

		uint8 GetSwing1Motion()
		{
			return Swing1Motion;
		}

		uint8 GetSwing2Motion()
		{
			return Swing2Motion;
		}

		uint8 GetTwistMotion()
		{
			return TwistMotion;
		}

		float GetSwing1LimitAngle()
		{
			return Swing1LimitAngle;
		}

		float GetSwing2LimitAngle()
		{
			return Swing2LimitAngle;
		}

		float GetTwistLimitAngle()
		{
			return TwistLimitAngle;
		}

		bool GetSwingSoftConstrained()
		{
			return bSwingSoftConstrained;
		}

		float GetSwingStiffness()
		{
			return SwingStiffness;
		}

		float GetSwingDamping()
		{
			return SwingDamping;
		}

		bool GetTwistSoftConstrained()
		{
			return bTwistSoftConstrained;
		}

		float GetTwistStiffness()
		{
			return TwistStiffness;
		}

		float GetTwistDamping()
		{
			return TwistDamping;
		}

		bool GetUseAdvanced()
		{
			return bUseAdvanced;
		}

		void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Swing1Motion = (uint8)(JsonObject->GetNumberField("swing_1_motion"));
			Swing2Motion = (uint8)(JsonObject->GetNumberField("swing_2_motion"));
			TwistMotion = (uint8)(JsonObject->GetNumberField("twist_motion"));
			Swing1LimitAngle = JsonObject->GetNumberField("swing_1_limit_angle");
			Swing2LimitAngle = JsonObject->GetNumberField("swing_2_limit_angle");
			TwistLimitAngle = JsonObject->GetNumberField("twist_limit_angle");
			bUseAdvanced = JsonObject->GetBoolField("use_advanced");
			bSwingSoftConstrained = JsonObject->GetBoolField("swing_soft_constrained");
			SwingStiffness = JsonObject->GetNumberField("swing_stiffness");
			SwingDamping = JsonObject->GetNumberField("swing_damping");
			bTwistSoftConstrained = JsonObject->GetBoolField("twist_soft_constrained");
			TwistStiffness = JsonObject->GetNumberField("twist_stiffness");
			TwistDamping = JsonObject->GetNumberField("twist_damping");
		}

		static AngularLimits GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			AngularLimits Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		FString ToString() const override
		{
			return TEXT(
					"AngularLimits {swing_1_motion = %d, swing_2_motion = %d, twist_motion = %d, swing_1_limit_angle = %s, swing_2_limit_angle = %s, twist_limit_angle = %s, swing_soft_constrained = %s, swing_stiffness = %s, swing_damping = %s, twist_soft_constrained = %s, twist_stiffness = %s, twist_damping = %s"
				),
				Swing1Motion, Swing2Motion, TwistMotion,
				FString::SanitizeFloat(Swing1LimitAngle), FString::SanitizeFloat(Swing2LimitAngle), FString::SanitizeFloat(
					TwistLimitAngle),
				bSwingSoftConstrained ? TEXT("True") : TEXT("False"), FString::SanitizeFloat(SwingStiffness), FString::
				SanitizeFloat(SwingDamping),
				bTwistSoftConstrained ? TEXT("True") : TEXT("False"), FString::SanitizeFloat(TwistStiffness), FString::
				SanitizeFloat(TwistDamping);
		}

		TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetNumberField(TEXT("swing_1_motion"), Swing1Motion);
			Object->SetNumberField(TEXT("swing_2_motion"), Swing2Motion);
			Object->SetNumberField(TEXT("twist_motion"), TwistMotion);
			Object->SetNumberField(TEXT("swing_1_limit_angle"), Swing1LimitAngle);
			Object->SetNumberField(TEXT("swing_2_limit_angle"), Swing2LimitAngle);
			Object->SetNumberField(TEXT("twist_limit_angle"), TwistLimitAngle);
			Object->SetBoolField(TEXT("use_advanced"), bUseAdvanced);
			Object->SetBoolField(TEXT("swing_soft_constrained"), bSwingSoftConstrained);
			Object->SetNumberField(TEXT("swing_stiffness"), SwingStiffness);
			Object->SetNumberField(TEXT("swing_damping"), SwingDamping);
			Object->SetBoolField(TEXT("twist_soft_constrained"), bTwistSoftConstrained);
			Object->SetNumberField(TEXT("twist_stiffness"), TwistStiffness);
			Object->SetNumberField(TEXT("twist_damping"), TwistDamping);
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
