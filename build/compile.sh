#!/bin/sh

SRC="../src"
LIBS="-lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl"
INCL="-I../include/"

g++ $SRC/main.cpp $SRC/glad.c $LIBS $INCL -o out