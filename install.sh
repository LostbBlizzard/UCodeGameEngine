#!/usr/bin/env bash
set -e

uenginedir="$HOME/.ucodelang"
uenginebindir="$ucodelangdir/bin"
uenginedocdir="$ucodelangdir/doc"

mkdir -p "$ucodelangdir"
mkdir -p "$ucodebindir"

if [ "$(uname)" = "Darwin" ];  
then

curl -LJO https://github.com/LostbBlizzard/UCodeGameEngine/releases/download/Release-#VersionMajor#.#VersionMinor#.#VersionPatch#/MacOSUCodeEditor

mv MacOSUCodeEditor $ucodebindir/UCodeEditor

elif [ "$(uname)" = "Linux" ]; 
then

curl -LJO https://github.com/LostbBlizzard/UCodeGameEngine/releases/download/Release-#VersionMajor#.#VersionMinor#.#VersionPatch#/LinuxUCodeEditor

mv LinuxUCodeEditor $ucodebindir/UCodeEditor

fi

curl -LJO https://github.com/LostbBlizzard/UCodeGameEngine/releases/download/Release-#VersionMajor#.#VersionMinor#.#VersionPatch#/UFiles.data

mv UFiles.data $ucodebindir/UFiles.data

