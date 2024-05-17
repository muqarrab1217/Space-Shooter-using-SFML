#include <SFML/Graphics.hpp>
#include<string.h>
using namespace sf;
class Player{
public:
Texture tex;
Sprite sprite;
Sprite l1;
Sprite l2;
Sprite l3;
float speed=5;
int x,y;
bool lf1, lf2, lf3;
Player(std::string png_path)
{
	lf1=true, lf2=true, lf3=true;

	tex.loadFromFile("img/playerlife_red.png");
	l1.setTexture(tex);
	l1.setPosition(0, 0);
	l1.setScale(0.5, 0.5);

	l2.setTexture(tex);
	l2.setPosition(40, 0);
	l2.setScale(0.5, 0.5);

	l3.setTexture(tex);
	l3.setPosition(80, 0);
	l3.setScale(0.5, 0.5);

	tex.loadFromFile(png_path);
	sprite.setTexture(tex);
	x=340;y=700;
	sprite.setPosition(x, y);
	sprite.setScale(0.75,0.75);
}
void set_texture(std::string ss){
	tex.loadFromFile(ss);
	sprite.setTexture(tex);
}
int get_pos_x(){
	return sprite.getPosition().x;
}
int get_pos_y(){
	return sprite.getPosition().y;
}
void set_pos(int x, int y){
	sprite.setPosition(sf::Vector2f(x,y));
}
void move(std::string s){
float delta_x=0,delta_y=0;
if(s=="l")
	delta_x=-1;	//move the player left
else if(s=="r")
	delta_x=1;	//move the player right
else if(s=="u")
	delta_y=-1;	//move the player up
else if(s=="d")
	delta_y=1;	//move the player down

delta_x*=speed;
delta_y*=speed;

sprite.move(delta_x, delta_y);

}

};
