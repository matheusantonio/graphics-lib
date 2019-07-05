glib : main.cpp Color.cpp Color.h Complexos.cpp Complexos.h Desenho.cpp Desenho.h Image.cpp Image.h Vector.cpp Vector.h Quaternions.cpp Quaternions.h Matrix.cpp Matrix.h Surface.cpp Surface.h
	g++ main.cpp Color.cpp Complexos.cpp Desenho.cpp Image.cpp Vector.cpp Quaternions.cpp Matrix.cpp Surface.cpp -o glib

openglib: open.cpp Vector.cpp Color.cpp Matrix.cpp Quaternions.cpp Model.cpp cguff.cpp
	g++ -o openglib open.cpp Vector.cpp Color.cpp Matrix.cpp Quaternions.cpp Model.cpp cguff.cpp -lglut -lGLU -lGL -lGLEW -lassimp