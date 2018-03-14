# UROSWorldControl
Unreal World Control from ROS using UROSBridge.

## Features
Currently there are three ROS services supported:
* spawn_model - Spawns a StaticMeshComponent in the world with given material at given location, rotation and with given UTags.
* relocate_model - Relocates the Actor with given UTagID, to given location and rotation.
* delete_model - Destroys the Actor with given UTagID from Unreal-World.

## Usage
Place an instance of "ROSWorldControlManager" in your Unreal-World and set the ServerAdress and Port in the properties. The Controller will spawn everything else needed at the beginning of the game. In the properties of the "ROSWorldControlManager" you can also set a namespace under which the services will be published in ROS, the default is "unreal". If everything is set up correctly the services will be published in ROS and can be used as if they were regular ROS-services.
