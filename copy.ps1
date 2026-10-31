echo off
cd E:/Six/SixEngine
copy Engine/out/x64/Debug/Engine.dll ./x64/Debug -Force                
copy Engine/out/x64/Debug/Engine.lib ./x64/Debug -Force
copy Engine/out/x64/Debug/Engine.exp ./x64/Debug -Force
copy Engine/out/x64/Debug/Engine.pdb ./x64/Debug -Force
pause