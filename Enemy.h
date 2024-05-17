#include <SFML/Graphics.hpp>
#include<string.h>
#include<iostream>
using namespace sf;
class Enemy{
public:
Texture tex;
float speed=5;
Sprite sprite;
Enemy(std::string png_path)
{
    tex.loadFromFile(png_path);
    sprite.setTexture(tex);
}
Enemy(std::string png_path, int X, int Y)
{
    tex.loadFromFile(png_path);
    sprite.setTexture(tex);
    sprite.setPosition(X, Y);
    sprite.setScale(0.7, 0.7);
}
~Enemy(){}
};

class Alpha:public Enemy{
public:
Texture tex;
Bullet* bullet;
bool s, b, alpha_fired, alpha_move=false;
int alpha_timer;
int x,y;
Alpha():Enemy("img/enemy_1.png"){}
Alpha(int X,int Y):Enemy("img/enemy_1.png"){
srand(time(NULL));
    s=true,b=true, alpha_fired=false;
        sprite.setPosition(sf::Vector2f(X, Y));
        sprite.setScale(0.5,0.5);   

        bullet= new Bullet ("img/enemy_laser.png",X-10,Y-30);
}
void set_pos(int x, int y){
	sprite.setPosition(sf::Vector2f(x,y));
}
void move(int X, int Y){
	sprite.move(X, Y);
}
bool alpha_collide(int X, int Y){
    if((X >= (sprite.getPosition().x) && X <= (sprite.getPosition().x + 99.0f)) &&
    (Y >= (sprite.getPosition().y) && Y <= (sprite.getPosition().y + 99.0f))){
        s=false;
        b=false;
        sprite.setPosition(0, 920);
        if(!alpha_fired)
            bullet->sprite.setPosition(20, 920);
        return true;
    }
    return false;
}
void alpha_movement(int difficulty){
    if(s){
        if(sprite.getPosition().x < 0)
            alpha_move=true;
        if(sprite.getPosition().x >700)
            alpha_move=false;
        if(alpha_move){
            sprite.move(difficulty, 0);
            if(!alpha_fired)
                bullet->sprite.move(difficulty, 0);
        }
        else{
            sprite.move(-difficulty, 0);
            if(!alpha_fired)
                bullet->sprite.move(-difficulty, 0);
        }
    }
}
};

class Beta:public Enemy{
public:
Texture tex;
Bullet* bullet;
bool s, b, beta_fired, beta_move=false;
int beta_timer=8;
int x,y;
Beta():Enemy("img/enemy_2.png"){}
Beta(int X,int Y):Enemy("img/enemy_2.png"){
    srand(time(NULL));
    //Display the rectangle format
    s=true, b=true, beta_fired=false;
        sprite.setPosition(sf::Vector2f(X, Y));
        sprite.setScale(0.5,0.5);   

        bullet= new Bullet ("img/enemy_laser.png",X-10,Y-20);

}
void set_pos(int x, int y){
	sprite.setPosition(sf::Vector2f(x,y));
}
void move(int X, int Y){
	sprite.move(X, Y);
}
bool beta_collide(int X, int Y){
    if((X >= (sprite.getPosition().x) && X <= (sprite.getPosition().x + 99.0f)) &&
    (Y >= (sprite.getPosition().y) && Y <= (sprite.getPosition().y + 99.0f))){
        s=false;
        b=false;
        sprite.setPosition(0, 920);
        if(!beta_fired)
            bullet->sprite.setPosition(20, 920);
        return true;
    }
    return false;
}
void beta_movement(int difficulty){
    if(s){
        if(sprite.getPosition().x < 0)
            beta_move=true;
        if(sprite.getPosition().x >700)
            beta_move=false;
        if(beta_move){
            sprite.move(difficulty, 0);
            if(!beta_fired)
                bullet->sprite.move(difficulty, 0);
        }
        else{
            sprite.move(-difficulty, 0);
            if(!beta_fired)
                bullet->sprite.move(-difficulty, 0);
        }
    }
}

};

class Gamma:public Enemy{
public:
Texture tex;
Bullet* bullet;
bool s, b, gamma_fired, gamma_move=false;
int gamma_timer;
int x,y;
Gamma():Enemy("img/enemy_3.png"){}
Gamma(int X,int Y):Enemy("img/enemy_3.png"){
s=true, b=true, gamma_fired=false;
    
    srand(time(NULL));
        sprite.setPosition(sf::Vector2f(X, Y));
        sprite.setScale(0.4,0.4);   

        bullet= new Bullet ("img/enemy_laser.png",X-10,Y-20);
}
void set_pos(int x, int y){
	sprite.setPosition(sf::Vector2f(x,y));
}
void move(int X, int Y){
	sprite.move(X, Y);
}
bool gamma_collide(int X, int Y){
    if((X >= (sprite.getPosition().x) && X <= (sprite.getPosition().x + 99.0f)) &&
    (Y >= (sprite.getPosition().y) && Y <= (sprite.getPosition().y + 99.0f))){
        s=false;
        b=false;
        sprite.setPosition(0, 920);
        if(!gamma_fired)
            bullet->sprite.setPosition(20, 920);
        return true;
    }
    return false;
}
void gamma_movement(int difficulty){
    if(s){
        if(sprite.getPosition().x < 0)
            gamma_move=true;
        if(sprite.getPosition().x >700)
            gamma_move=false;
        if(gamma_move){
            sprite.move(difficulty, 0);
            if(!gamma_fired)
                bullet->sprite.move(difficulty, 0);
        }
        else{
            sprite.move(-difficulty, 0);
            if(!gamma_fired)
                bullet->sprite.move(-difficulty, 0);
        }
    }
}
};

class Monster:public Enemy{
public:
    Bullet* bullet1;
    Bullet* bullet2;
    Bullet* bullet3;
    Bullet* bullet4;
    Bullet* bullet5;
    Bullet* bullet6;
    Bullet* bullet7;
    bool monster_move=false;
    float f1=10, f2=10, f3=10, f4=10, f5=10, f6=10, f7=10;
    bool b1=false, b2=false, b3=false, b4=false, b5=false, b6=false, b7=false;

Monster():Enemy("img/monster1.png"){}
Monster(std::string png_path,int X, int Y):Enemy(png_path){
    srand(time(NULL));
    sprite.setPosition(sf::Vector2f(X, Y));
    sprite.setScale(2, 2);

        bullet1= new Bullet ("img/enemy_laser.png", X+50, Y-20);
        bullet2= new Bullet ("img/enemy_laser.png", X+50, Y-20);
        bullet3= new Bullet ("img/enemy_laser.png", X+50, Y-20);
        bullet4= new Bullet ("img/enemy_laser.png", X+50, Y-20);
        bullet5= new Bullet ("img/enemy_laser.png", X+50, Y-20);
        bullet6= new Bullet ("img/enemy_laser.png", X+50, Y-20);
        bullet7= new Bullet ("img/enemy_laser.png", X+50, Y-20);
}
void movement(){
    if(sprite.getPosition().x < -200)
        monster_move=false;
    if(sprite.getPosition().x > 500)
        monster_move=true;
    if(monster_move){
        sprite.move(-5, 0);
        if(!b1)
            bullet1->move(-5, 0);
        if(!b2)
            bullet2->move(-5, 0);
        if(!b3)
            bullet3->move(-5, 0);
        if(!b4)
            bullet4->move(-5, 0);
        if(!b5)
            bullet5->move(-5, 0);
    }
    else{
        sprite.move(5, 0);
        if(!b1)
            bullet1->move(5, 0);
        if(!b2)
            bullet2->move(5, 0);
        if(!b3)
            bullet3->move(5, 0);
        if(!b4)
            bullet4->move(5, 0);
        if(!b5)
            bullet5->move(5, 0);
    }
}
void fire(float timer){
    if(timer > f1+0.5){
        b1=true;
        bullet1->sprite.move(2, 3);
    }
    if(timer > f2+ 1.2){
        b2=true;
        bullet2->sprite.move(3, 3);
    }
    if(timer > f3 + 1.7){
        b3=true;
        bullet3->sprite.move(0, 3);
    }
    if(timer > f4 + 2.1){
        b4=true;
        bullet4->sprite.move(-3, 3);
    }
    if(timer > f5 + 2.6){
        b5=true;
        bullet5->sprite.move(-0.5, 3);
    }
    if(timer > f6 +3.2){
        b6=true;
        bullet6->sprite.move(2, 3);
    }
    if(timer > f7 + 3.7){
        b7=true;
        bullet7->sprite.move(-1, 3);
    }
}
void check_bounds(int X){
    if(bullet1->sprite.getPosition().y > 790){
        b1=false;
        f1+=3;
        bullet1->sprite.setPosition(X+210, 300);
    }
    if(bullet2->sprite.getPosition().y > 790){
        b2=false;
        f2+=3;
        bullet2->sprite.setPosition(X+210, 300);
    }
    if(bullet3->sprite.getPosition().y > 790){
        b3=false;
        f3+=3;
        bullet3->sprite.setPosition(X+210, 300);
    }
    if(bullet4->sprite.getPosition().y > 790){
        b4=false;
        f4+=3;
        bullet4->sprite.setPosition(X+210, 300);
    }
    if(bullet5->sprite.getPosition().y > 790){
        b5=false;
        f5+=3;
        bullet5->sprite.setPosition(X+210, 300);
    }
    
    if(bullet6->sprite.getPosition().y > 790){
        b6=false;
        f6+=3;
        bullet6->sprite.setPosition(X+210, 300);
    }
    if(bullet7->sprite.getPosition().y > 790){
        b7=false;
        f7+=3;
        bullet7->sprite.setPosition(X+210, 300);
    }
}
bool check_hit(int X, int Y){
    if((bullet1->sprite.getPosition().x >= (X) && bullet1->sprite.getPosition().x <= (X + 99.0f)) &&
        (bullet1->sprite.getPosition().y >= (Y) && bullet1->sprite.getPosition().y <= (Y + 99.0f))){
            b1=false;
            f1+=3;
            bullet1->sprite.setPosition(sprite.getPosition().x+150, 300);
            return true;
        }
    if((bullet2->sprite.getPosition().x >= (X) && bullet2->sprite.getPosition().x <= (X + 99.0f)) &&
        (bullet2->sprite.getPosition().y >= (Y) && bullet2->sprite.getPosition().y <= (Y + 99.0f))){
            b2=false;
            f2+=3;
            bullet2->sprite.setPosition(sprite.getPosition().x+200, 300);
            return true;
        }
    if((bullet3->sprite.getPosition().x >= (X) && bullet3->sprite.getPosition().x <= (X + 99.0f)) &&
        (bullet3->sprite.getPosition().y >= (Y) && bullet3->sprite.getPosition().y <= (Y + 99.0f))){
            b3=false;
            f3+=3;
            bullet3->sprite.setPosition(sprite.getPosition().x+200, 300);
            return true;
        }
    if((bullet4->sprite.getPosition().x >= (X) && bullet4->sprite.getPosition().x <= (X + 99.0f)) &&
        (bullet4->sprite.getPosition().y >= (Y) && bullet4->sprite.getPosition().y <= (Y + 99.0f))){
            b4=false;
            f4+=3;
            bullet4->sprite.setPosition(sprite.getPosition().x+200, 300);
            return true;
        }
    if((bullet5->sprite.getPosition().x >= (X) && bullet5->sprite.getPosition().x <= (X + 99.0f)) &&
        (bullet5->sprite.getPosition().y >= (Y) && bullet5->sprite.getPosition().y <= (Y + 99.0f))){
            b5=false;
            f5+=3;
            bullet5->sprite.setPosition(sprite.getPosition().x+200, 300);
            return true;
        }
    if((bullet6->sprite.getPosition().x >= (X) && bullet6->sprite.getPosition().x <= (X + 99.0f)) &&
        (bullet6->sprite.getPosition().y >= (Y) && bullet6->sprite.getPosition().y <= (Y + 99.0f))){
            b6=false;
            f6+=3;
            bullet6->sprite.setPosition(sprite.getPosition().x+200, 300);
            return true;
        }
    if((bullet7->sprite.getPosition().x >= (X) && bullet7->sprite.getPosition().x <= (X + 99.0f)) &&
        (bullet7->sprite.getPosition().y >= (Y) && bullet7->sprite.getPosition().y <= (Y + 99.0f))){
            b7=false;
            f7+=3;
            bullet7->sprite.setPosition(sprite.getPosition().x+200, 300);
            return true;
        }
    return false;
}
};

class Dragon:public Enemy{
public:
    Bullet* bullet;
    bool dragon_fired=false;
    bool dragon_move=false;

Dragon():Enemy("img/dragon.png"){}
Dragon(std::string png_path,int X, int Y):Enemy(png_path){
    srand(time(NULL));
    sprite.setPosition(sf::Vector2f(X, Y));
    sprite.setScale(0.7, 0.7);

        bullet= new Bullet ("img/beam.png",X-50,Y-20);
}
void movement(){    //dragon movement
    if(sprite.getPosition().x < -150)
        dragon_move=false;
    else if(sprite.getPosition().x > 500)
        dragon_move=true;
    if(!dragon_move){
        sprite.move(3, 0);
        bullet->sprite.move(3, 0);
    }
    else{
        sprite.move(-3, 0);
        bullet->sprite.move(-3, 0);
    }
}
bool check_hit(int X, int Y){
    if(dragon_fired){
        if(((X) >= bullet->sprite.getPosition().x  && X <= (bullet->sprite.getPosition().x + 99.0f)) &&
        ((Y) >= bullet->sprite.getPosition().y  && Y <= (bullet->sprite.getPosition().y + 500.0f))){
            return true;
        }
    }
return false;
}

};