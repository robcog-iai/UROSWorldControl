# UROSWorldControl
Unreal World Control from ROS using UROSBridge.

## Features
Currently there are three ROS services supported:
* spawn_model - Spawns a StaticMeshComponent in the world with given material at given location, rotation and with given UTags.
* relocate_model - Relocates the Actor with given UTagID, to given location and rotation.
* delete_model - Destroys the Actor with given UTagID from Unreal-World.

## Usage
Place an instance of "AROSWorldControlManager" in your Unreal-World and set the ServerAdress and Port in the Properties. The Controller will spawn everything else needed at the beginning of the game. If everything is set up correctly the services will be published in ROS and can be used as if they were regular ROS-services.
