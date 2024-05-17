#include <SFML/Graphics.hpp>
#include<string.h>
#include<iostream>
using namespace sf;
class Bullet{
public:
Texture tex;
Sprite sprite;
float speed=5;
int x,y;
Bullet(){
    tex.loadFromFile("img/laserBlue16.png");
    sprite.setTexture(tex);
    x=390;y=420;
    sprite.setPosition(sf::Vector2f(x, y));
    sprite.setScale(0.75,0.75);
}
Bullet(std::string png_path, int X, int Y){
    tex.loadFromFile(png_path);
    sprite.setTexture(tex);
    sprite.setPosition(sf::Vector2f(X+27, Y+40));
    sprite.setScale(0.75,0.75);
    
}
void firing(){
float delta_x=0,delta_y=-1;

delta_x*=7;
delta_y*=7;
        sprite.move(sf::Vector2f(delta_x, delta_y));
}
void diag_firing(int X){
float delta_x=X,delta_y=-1;

delta_x*=7;
delta_y*=7;
        sprite.move(sf::Vector2f(delta_x, delta_y));
}
void set_textue(std::string ss){
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
void move(int X, int Y){
float delta_x=X,delta_y=Y;
delta_x*=speed;
delta_y*=speed;
	sprite.move(X, Y);
}

void move(std::string s){
float delta_x=0,delta_y=0;
if(s=="l")
	delta_x=-1;	//move the bullet left
else if(s=="r")
	delta_x=1;  //move the bullet right
else if(s=="u")
    delta_y=-1; //move the bullet up
else if(s=="d")
	delta_y=1;	//move the bullet down
else if(s=="dl"){
    delta_x=-1;
    delta_y=-1;
}
else if(s=="rl"){
    delta_x=1;
    delta_y=1;
}

delta_x*=speed;
delta_y*=speed;

sprite.move(sf::Vector2f(delta_x, delta_y));

}

~Bullet(){}

};
