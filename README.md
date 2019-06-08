KenshiDXHook

If you want to build this, you will have to remove/change the post-build events in Visual Studio, you can find this in Project properties -> Configuration properties -> Build events -> Post-build event.

You will also have to download "directxtk_desktop_2015" using Nuget package manager inside Visual Studio. You can find this in Tools -> Nuget package manager -> Manage Nuget packages for solution -> Browse -> Search for directxtk, beware to download the DirectX11 version. This package is used for loading textures properly.
