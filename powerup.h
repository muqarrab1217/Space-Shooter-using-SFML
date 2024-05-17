#include <SFML/Graphics.hpp>
#include<string.h>
using namespace sf;
class Powerup{
public:
Texture tex;
Sprite sprite;
Sprite* p = new Sprite [7];
bool* arr = new bool [7];
float speed=5;
int x,y;
Powerup(std::string png_path){
	tex.loadFromFile(png_path);
	sprite.setTexture(tex);
	sprite.setPosition(rand()%780, -60);
	sprite.setScale(0.75,0.75);
}
Powerup(std::string png_path, int X, int Y)
{
	tex.loadFromFile(png_path);
    for(int i=0 ; i<7 ; i++){
		arr[i]=false;
		p[i].setTexture(tex);
		p[i].setPosition(X, Y);
		p[i].setScale(0.75, 0.75);
	}
	p[1].setRotation(350);
	p[2].setRotation(10);
	p[3].setRotation(340);
	p[4].setRotation(20);
	p[5].setRotation(330);
	p[6].setRotation(30);
}


};
