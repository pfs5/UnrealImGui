Unreal ImGui ++
============
Fork of the UnrealEngine plugin integrating **ImGUI** into the engine.

I have added some small improvements for my personal use and hereby made them public if someone else might find them useful.

The code is far from completed as I will keep adding stuff as I continue using this.

Summary
-------
- Tested with UnrealEngine5.
- Updated imgui lib to 1.90.4.
- Created simple framework to add imgui debug to any class/struct.

Setup
-----
Basic setup is done as for every Unreal plugin.
1. In the root folder of your project (where the .uproject file is located), find or create the `Plugins` folder.
2. In there either clone this repository or paste the downloaded zip. The files in this repository should end up in `<project_root>/Plugins/ImGui`.
3. Double click on your .uproject file. A prompt about rebuilding the `ImGui` module should appear, press **yes**.
4. Add the module in `<project_name>.Build.cs`, for example
    - `PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ImGui" });`
5. Finally, regenerate the Visual Studio project by right-clicking on the .uproject file > `Generate Visual Studio project files`.
6. _Voilà_! Your plugin should work now. For additional setup information please see the original repository. 

How to
------
- The menu bar at the top of the screen will automatically be displayed in the Unreal editor.
- The "Listeners" menu will show all object with debug implemented.

### Adding debug to a class
```c++
#include "ImGui/Public/ImGuiListener.h"
#include "imgui.h"
#include "DummyActor.generated.h"

UCLASS()
class ADummyActor : public AActor
    , public ImGui::TImGuiListener<ADummyActor> // 1. Inherit this class
{
    GENERATED_BODY()

public:
    // 2. Override this function to add custom imgui code.
    void Draw() override
    {
        ImGui::TImGuiListener<ADummyActor>::Draw();  // The base class will already display some basic info in the debug.

        // -- YOUR CODE HERE --
        ImGui::Text("This is the best actor in the world!");
    }

protected:
    // 3. You will need to register/unregister your object in the debug system. For actors, these are the best places.
    void BeginPlay() override
    {
        Super::BeginPlay();
        IImGuiListener::RegisterListener();
    }
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override
    {
        IImGuiListener::UnregisterListener();
        Super::EndPlay(EndPlayReason);
    }

};
```

Note: This is a simple example of how to use the framework for a common `AActor` class. The same principle applies to all other objects:
- Inherit the base class.
- Find good places to register/unregister the object. No debug will work before registering. Make sure you unregister the object before destruction or the system will crash.
- Inherit the Draw function and add your debug code there!

Happy debugging!
