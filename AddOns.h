#include <SFML/Graphics.hpp>
#include<string.h>
using namespace sf;
class AddOns{
public:
Texture tex;
Sprite sprite;
//fire
int fire_timer=5, fire_pickup_timer=0;
bool fire_pickup=false;

int life_timer=15;

int danger_timer=5;
bool danger_pickup=false;

float speed=5;
int x,y;
AddOns(std::string png_path)
{
	tex.loadFromFile(png_path);
	sprite.setTexture(tex);
	sprite.setPosition(rand()%780, -60);
	sprite.setScale(0.75,0.75);
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
//for fire
void movement_fire(float timer){
	if(timer > fire_timer ){
       sprite.move(0, 1);
    }
}
void check_bounds_fire(){
	if(sprite.getPosition().y > 790){
        sprite.setPosition(rand()%700, -30);
		fire_timer += 19;
    }
}
bool check_collision(int X, int Y, float timer){
	if((sprite.getPosition().x >= (X) && sprite.getPosition().x <= (X + 99.0f)) &&
    (sprite.getPosition().y >= (Y) && sprite.getPosition().y <= (Y + 99.0f))){
        sprite.setPosition(rand()%700, -80);
		return true;
    }
	return false;
}
//for life
void movement_life(float timer){
	if(timer > life_timer){
         sprite.move(0, 1);
    }
}
void check_bounds_life(){
	if(sprite.getPosition().y > 790){
        life_timer += 20;
        sprite.setPosition(rand()%700, -30);
    }
}

//for danger
void movement_danger(float timer){
	if(timer > danger_timer){
        sprite.move(0, 1);
        sprite.rotate(0.5f);
    }
}

};
