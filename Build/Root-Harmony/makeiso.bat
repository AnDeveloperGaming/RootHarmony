@echo off
title Root Harmony ISO Builder
echo ===================================================
echo   AUTOMATED ISO BUILD ENGINE - ROOT HARMONY OS     
echo ===================================================
echo.

echo [1/4] Compiling fresh kernel binaries...
wsl make
if %errorlevel% neq 0 goto ERROR

echo.
echo [2/4] Setting up native ISO staging folders...
wsl mkdir -p iso/boot/grub/themes/RootHarmonyTheme

echo.
echo [3/4] Forcing injection of fonts, configs, and Figma assets...
wsl grub-mkfont --output=iso/boot/grub/themes/RootHarmonyTheme/font.pf2 --size=14 /usr/share/fonts/truetype/dejavu/DejaVuSans.ttf
wsl cp grub.cfg iso/boot/grub/grub.cfg
wsl cp theme.txt background.png select_m.png iso/boot/grub/themes/RootHarmonyTheme/ 2^> /dev/null
wsl cp kern iso/boot/kern 2^> /dev/null

echo.
echo [4/4] Packing everything manually into RootHarmony.iso...
wsl grub-mkrescue iso --output=RootHarmony.iso
if %errorlevel% neq 0 goto ERROR

echo.
echo ===================================================
echo  SUCCESS: RootHarmony.iso has been built perfectly!
echo ===================================================
pause
exit

:ERROR
echo.
echo ---------------------------------------------------
echo  CRITICAL ERROR: Compilation or packaging failed.  
echo ---------------------------------------------------
pause
exit
