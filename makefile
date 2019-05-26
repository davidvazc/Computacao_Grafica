
exe:	visualizar.cpp RgbImage.cpp
	g++ visualizar.cpp RgbImage.cpp -o exe materiais.h RgbImage.h -lglut -lGLU -lGL -lm
