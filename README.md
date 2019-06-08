## KenshiHook

##### If you want to build this, follow these steps:

You will have to remove/change the post-build events in Visual Studio, you can find this in Project properties -> Configuration properties -> Build events -> Post-build event.

You will have to download "directxtk_desktop_2015" using Nuget package manager inside Visual Studio. You can find this in Tools -> Nuget package manager -> Manage Nuget packages for solution -> Browse -> Search for directxtk, beware to download the DirectX11 version. This package is used for loading textures properly.

Make sure "Shaders.hlsl" is not included in the build, you can do this by right clicking it -> Properties -> Configuration properties -> General -> Excluded from build -> "Yes". The present build errors in this file should be ignored, they don't have any effect on the running program.

In order to run the .dll, place it next to the game. Also, currently you need to have the original dxgi.dll in the same folder as the .dll you built, but renamed to "dxgi_.dll". You can find the original dxgi.dll in C:/windows/system32. Simply copy it from there. (Obviously don't cut & paste it, and don't mess with anything else in there)
