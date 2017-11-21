serializer: 
	g++ -g src/serialize_image.cpp `pkg-config --libs --cflags opencv`-o serialize_image
	./serialize_image image.jpg

palette: 
	cd src && g++ serialize_vgapallet.cpp `pkg-config --libs --cflags opencv`-o ../serialize_vgapallet 
	./serialize_vgapallet


