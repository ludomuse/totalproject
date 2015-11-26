#! /bin/bash
cocos compile -p android
cp bin/debug/android/LudoMuse-debug.apk /mnt/
echo "APK successfully moved to shared folder /mnt"
