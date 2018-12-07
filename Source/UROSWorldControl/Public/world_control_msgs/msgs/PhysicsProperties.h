#pragma once
#include "ROSBridgeMsg.h"

namespace world_control_msgs
{
	class PhysicsProperties : public FROSBridgeMsg
	{
		bool SimulatePhysics;
		bool Gravity;
		bool GenerateOverlapEvents;
		float Mass;


	public:
		PhysicsProperties() {}

		PhysicsProperties(bool InSimulatePhysics, bool InGravity, bool InGenerateOverlapEvents, float InMass)
		{
			PhysicsProperties();
			SimulatePhysics = InSimulatePhysics;
			Gravity = InGravity;
			GenerateOverlapEvents = InGenerateOverlapEvents;
			Mass = InMass;
		}

		bool GetSimulatePhysics()
		{
			return SimulatePhysics;
		}

		bool GetGravity()
		{
			return Gravity;
		}

		bool GetGenerateOverlapEvents()
		{
			return GenerateOverlapEvents;
		}

		float GetMass()
		{
			return Mass;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			SimulatePhysics = JsonObject->GetBoolField("simulate_physics");
			Gravity = JsonObject->GetBoolField("gravity");
			GenerateOverlapEvents = JsonObject->GetBoolField("generate_overlap_events");
			Mass = JsonObject->GetNumberField("mass");
		}

		static PhysicsProperties GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PhysicsProperties Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual FString ToString() const override
		{
			return "PhysicsProperties {simulate_physics = " + (SimulatePhysics ? FString("True") : FString("False")) +
				", gravity = " + (Gravity ? FString("True") : FString("False")) +
				", generate_overlap_events = " + (GenerateOverlapEvents ? FString("True") : FString("False")) +
				", mass = " + FString::SanitizeFloat(Mass) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("simulate_physics"), SimulatePhysics);
			Object->SetBoolField(TEXT("gravity"), Gravity);
			Object->SetBoolField(TEXT("generate_overlap_events"), GenerateOverlapEvents);
			Object->SetNumberField(TEXT("mass"), Mass);
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