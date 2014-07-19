#Krono
Krono is the working name a deferred graphics engine I am working one. The name Krono comes from some games I made before called Kronologic. The aim of this graphics engine is to offer a modern and flexible graphics engine for game development. Initially, the graphics engine will focus on a deferred rendering pipeline. It will be kept flexible to allow forward rendering later on.

## Design Goals
Krono will strive to only include what is necessary to render graphics. This means that the graphics engine will not include a hierarchal scene graph using nodes with positions and rotations. This is meant to be a graphics engine only. Instead a transform matrix must be supplied for each object. It is up to the user to decide how to create this transform. This method is compatible with a scene graph external to the graphics engine, but is not required. 

Krono will also not include any singletons. This way any and all dependencies are clear by evaluating a classes interface. This also means that seemingly unrelated code will not be having hidden relationships through global state.