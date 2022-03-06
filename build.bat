@echo off
echo Starting Build
g++ main.cpp tigr.c -o main -lopengl32 -lgdi32 -lWinmm
echo Build Complete