# UROSWorldControl
Unreal World Control from ROS using UROSBridge.

## Features
Currently there are three ROS services supported:
* spawn_model - Spawns a StaticMeshComponent in the world with given material at given location, rotation and with given UTags.
* relocate_model - Relocates the Actor with given UTagID, to given location and rotation.
* delete_model - Destroys the Actor with given UTagID from Unreal-World.

## Installation
### Unreal
For this Plugin to work on the unreal side of things, you also need to the Plugins [UROSBridge](https://github.com/robcog-iai/UROSBridge) and [UTags](https://github.com/robcog-iai/UTags). Copy all three plugins into your project Plugin folder and activate them.

### ROS
On the ROS side you need the srv and msg files, for that you need to get the package `unreal_msgs`, which can be found in the [unreal_ros_pkgs](https://github.com/robcog-iai/unreal_ros_pkgs) repository.

## Usage
Place an instance of "ROSWorldControlManager" in your Unreal-World and set the ServerAdress and Port in the properties. The Controller will spawn everything else needed at the beginning of the game. In the properties of the "ROSWorldControlManager" you can also set a namespace under which the services will be published in ROS, the default is "unreal". If everything is set up correctly the services will be published in ROS and can be used as if they were regular ROS-services.
