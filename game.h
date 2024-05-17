#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include<fstream>
using namespace std;
#include "player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "powerup.h"
#include "AddOns.h"
const char title[] = "OOP-Project, Spring-2023";
using namespace sf;

class Game{
public:
int score;
int level;
int difficulty;
float timer=0;
    //for powerups
    int powerup_timer=15, powerup_pickup_timer=0;
    bool powerup_pickup=false, powerup_fired=false;
    
    //for bullets
    bool fired=false, enemy_move=false;
    //Bonus
    bool destroyed=false, explode=false;
    bool set=false;
Sprite background; //Game background sprite
Texture bg_texture;

Texture pause;  //Pause background
Sprite Pause;

Texture over;   //gameover background
Sprite game;

Font font;
Text game_score, sc, level_comp, wave;
Player* p; //player 
// add other game attributes
Bullet* bullet;

//Enemies
int alpha_size=4, beta_size=4, gamma_size=4;
Alpha* alpha=new Alpha[4]{Alpha(1000, 30),Alpha(1080,30),Alpha(1160,30),Alpha(1240,30)};
Beta* beta=new Beta[4]{Beta(-540,90),Beta(-460,90),Beta(-380,90),Beta(-300,90)};
Gamma* gamma=new Gamma[4]{Gamma(1000, 170),Gamma(1080,170),Gamma(1160,170),Gamma(1240,170)};

//Bosses
Monster* monster;
bool monster_spawn=false;
int monster_timer=10;
bool mons=false;

Dragon* dragon;
bool dragon_spawn=false;
int dragon_spawn_timer=0;
int dragon_timer=0;

//Power ups
Powerup* shield;
Powerup* powerup;
Powerup* powerup_fire;
AddOns* fire;
AddOns* life;
AddOns* danger;

Game(){
score=0, level=0, difficulty=1;

font.loadFromFile("img/font.otf");
game_score.setPosition(100, 35);
game_score.setCharacterSize(28);
game_score.setFont(font);
game_score.setFillColor(sf::Color::White);

sc.setPosition(0, 35);
sc.setCharacterSize(28);
sc.setFont(font);
sc.setFillColor(sf::Color::White);
sc.setString("Score: ");

level_comp.setPosition(170, 100);
level_comp.setCharacterSize(50);
level_comp.setFont(font);
level_comp.setFillColor(sf::Color::Yellow);
level_comp.setString("LEVEL COMPLETED");

wave.setPosition(230, 170);
wave.setCharacterSize(50);
wave.setFont(font);
wave.setFillColor(sf::Color::Yellow);
wave.setString("First Wave");

p=new Player("img/player_ship.png");

bullet = new Bullet("img/laserBlue16.png", p->sprite.getPosition().x+10, p->sprite.getPosition().y-20);

bg_texture.loadFromFile("img/background.jpg");
background.setTexture(bg_texture);
background.setScale(2, 1.5);

pause.loadFromFile("img/PAUSED.jpg");
Pause.setTexture(pause);
Pause.setScale(1, 1);

over.loadFromFile("img/game over.jpeg");
game.setTexture(over);
game.setScale(2.7, 2);

alpha[0].alpha_timer=2;
alpha[1].alpha_timer=5;
alpha[2].alpha_timer=9;
alpha[3].alpha_timer=7;

beta[0].beta_timer=3;
beta[1].beta_timer=6;
beta[2].beta_timer=4;
beta[3].beta_timer=8;

gamma[0].gamma_timer=5;
gamma[1].gamma_timer=7;
gamma[2].gamma_timer=9;
gamma[3].gamma_timer=11;

powerup = new Powerup ("img/shield_gold.png");
powerup_fire = new Powerup ("img/laserBlue16.png", p->sprite.getPosition().x+30, p->sprite.getPosition().y+30);
fire = new AddOns ("img/powerupRed_star.png");
life = new AddOns ("img/pill_red.png");
danger = new AddOns ("img/meteorBrown_big1.png");

monster = new Monster ("img/dragon.png", 0, 900);
dragon = new Dragon ("img/monster1.png", 0, 900); 
}

void start_game()
{
    srand(time(0));
    RenderWindow window(VideoMode(780, 780), title, sf::Style::Close);
    Clock clock;
    ofstream file("highscore.txt", ios::app);
    bool g_over=false;
    bool pause=false;
    bool drag=false;
    bool nextlevel=false;

        bool left=false;
        bool right=false;

    SoundBuffer buffer;
    buffer.loadFromFile("sounds/explosion.wav");
    Sound explosion;
    explosion.setBuffer(buffer);

    Music music;
    music.openFromFile("sounds/space.ogg");
    music.play();

    sf::Clock clk;

    sf::Time jumpTime;
    
    sf::RectangleShape floor;
    floor.setPosition(0, 324);
    floor.setSize(sf::Vector2f(640, 16));

    sf::Time jumpMaxTime = sf::seconds(0.1f);

    sf::Vector2f speed;

    sf::Time lastTime;

    sf::Vector2f hover(0.f, .5f);

    sf::RectangleShape Shape;
    Shape.setFillColor(sf::Color(140, 36, 58));
    Shape.setPosition(32, 0);
    Shape.setSize(sf::Vector2f(16, 64));
    
    sf::Texture texture;
    sf::Sprite Background;

    if (texture.loadFromFile("img/background.jpg"))
    {
        texture.setRepeated(true);
        background.setTexture(texture);
    }

        const float MAX_VELOCITY = 1.0f;

    while (window.isOpen())
    {
        p->set_texture("img/player_ship.png");    // sets the spaceship at 0' degrees.

        if(!fired){   //Resets the bullet
            left=false;
            right=false;
            bullet->sprite.setRotation(0.0f);
        }

        sf::Time currentTime = clk.restart();
        lastTime = currentTime;

        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;
        
        Event e;
        
        while (window.pollEvent(e))
        {  
            if (e.type == Event::Closed){ // If cross/close is clicked/pressed
                
                if(file.is_open()){
                    file<<score<<"\n";
                }
                    window.close(); //close the game 
            } 
        }

        speed.y += hover.y*lastTime.asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            // Immediate stop
            if (speed.x > 0)
                speed.x = 0;
            speed.x -= .2f*lastTime.asSeconds();
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // Immediate stop
            if (speed.x < 0)
                speed.x = 0;
            speed.x += .2f*lastTime.asSeconds();
        }
        else
            speed.x *= .75f;

        if(speed.x < -MAX_VELOCITY)
            speed.x = -MAX_VELOCITY;
        else if(speed.x > MAX_VELOCITY)
            speed.x = MAX_VELOCITY;
        if(speed.y < -MAX_VELOCITY)
            speed.y = -MAX_VELOCITY;
        else if (speed.y > MAX_VELOCITY)
            speed.y = MAX_VELOCITY;

        background.setOrigin(background.getLocalBounds().width/2, background.getLocalBounds().height/2);
        background.setPosition(background.getLocalBounds().width/2,  background.getLocalBounds().height/2);
        background.setTextureRect(sf::IntRect(-((Shape.getPosition().x*.125f)), -(Shape.getPosition().y*.25f), 780, 780));

            Shape.move(speed);

        if (Shape.getPosition().x < 0)
            Shape.setPosition(0, Shape.getPosition().y);
        if (Shape.getPosition().x + Shape.getSize().x > window.getSize().x)
            Shape.setPosition(window.getSize().x - Shape.getSize().x, Shape.getPosition().y);

        window.clear(sf::Color::Magenta);
        window.draw(Shape);
        window.draw(floor);
        window.draw(background);

    if (Keyboard::isKeyPressed(Keyboard::Escape)){
        window.close();
    }
    
    //resuming the game if paused
    if (Keyboard::isKeyPressed(Keyboard::R)){
            pause=false;
            window.clear(Color::Black);
            window.draw(background);
    } 

    if(g_over){   //Checks for Gameover
        window.clear(Color::Black);
        window.draw(game);
        window.display();
            continue;
    }

    //Pausing the game
    if(Keyboard::isKeyPressed(Keyboard::P)){
        pause=true;
    }
    if(pause){
        window.clear(Color::Black);
        window.draw(Pause);
        window.display();
        continue;
    }

//Player and bullet controls
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
if(!nextlevel){
	if (Keyboard::isKeyPressed(Keyboard::Left)){ //If left key is pressed
        p->set_texture("img/player_ship_rotated_left.png");
        p->move("l");   // Player will move to left
        if(!fired){
           bullet->move("l");
           left=true;
           bullet->sprite.setRotation(330.0f);
        }
    }        
	else if (Keyboard::isKeyPressed(Keyboard::Right)){ // If right key is pressed
        p->set_texture("img/player_ship_rotated_right.png");
        p->move("r");  //player will move to right
        if(!fired){
            bullet->move("r");
            right=true;
            bullet->sprite.setRotation(30.0f);
        }
    }
	if (Keyboard::isKeyPressed(Keyboard::Up)){ //If up key is pressed
        p->move("u");    //playet will move upwards
        if(!fired)
            bullet->move("u");
    }
	if (Keyboard::isKeyPressed(Keyboard::Down)){ // If down key is pressed
        p->move("d");  //player will move downwards
        if(!fired)
            bullet->move("d");
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //Bullet Fire
    if(Keyboard::isKeyPressed(Keyboard::Space)){
        fired=true;
    }
    if (fired)
    {
        if(left){
            bullet->diag_firing(-1);
        }
        else if(right){
            bullet->diag_firing(1);
        }
        else{
            bullet->firing();
        }
    }
    
    //Update Bullet
    if(bullet->get_pos_y() < 0){
        fired=false;
        left=false;
        right=false;
        bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+40);
    }
    if(bullet->get_pos_x() < 0 && bullet->get_pos_y() > 0){
        left=false;
        right=true;
        bullet->sprite.setRotation(30.0f);
    }
    if(bullet->get_pos_x() > 780 && bullet->get_pos_y() > 0){
        right=false;
        left=true;
        bullet->sprite.setRotation(330.0f);
    }

    //Monster Functionality
    if(level==5){
        if(!set){
            set=true;
            monster->f1=timer;
            monster->f2=timer;
            monster->f3=timer;
            monster->f4=timer;
            monster->f5=timer;
        }
        //monster->movement();
        monster->fire(timer);
        monster->check_bounds(monster->sprite.getPosition().x);
        if(monster->check_hit(p->sprite.getPosition().x, p->sprite.getPosition().y)){
            if(p->lf3)
                p->lf3=false;
            else if(p->lf2)
                p->lf2=false;
            else
                p->lf1=false;
            //player gets hit and is sent back to its starting position
            p->set_pos(340, 700);
            bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
        }
    }

    //dragon functionality
    if(level==11){
    if(dragon_spawn){    //if dragon fired
            if(timer > dragon_timer){
                dragon_timer += 3;
                dragon->dragon_fired=true;
                dragon->bullet->sprite.setScale(1, 15);
            }
            else if(timer > dragon_timer-2){
               dragon->dragon_fired=false;
                dragon->bullet->sprite.setScale(0.75, 0.75);
            }
    }
    dragon->movement();
    if(dragon->check_hit(p->get_pos_x(), p->get_pos_y())){
        if(p->lf3)
                p->lf3=false;
            else if(p->lf2)
                p->lf2=false;
            else
                p->lf1=false;
            //player gets hit and is sent back to its starting position
            p->set_pos(dragon->sprite.getPosition().x, 700);
            bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
    }
    }

    //collision with window
        if(p->get_pos_x() < 0){  //for left
        	p->set_pos(750, p->get_pos_y());
            if(!fired)
            	bullet->set_pos(780, p->get_pos_y()+40);
        }
        else if(p->get_pos_x() > 750){   //for right
        	p->set_pos(0, p->get_pos_y());
            if(!fired)
            	bullet->set_pos(30, p->get_pos_y()+40);
        }
        else if(p->get_pos_y() < 0){   //for up
        	p->set_pos(p->get_pos_x(), 700);
            if(!fired)
            	bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+30);
        }
        else if(p->get_pos_y() > 700){   //for bottom
        	p->sprite.setPosition(p->get_pos_x(), p->get_pos_y()-5);
            if(!fired)
            	bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+30);
        }

    //enemy movement
if(level==8){
    for(int i=0 ; i<4 ; i++){   //Bonus movement
        if(!destroyed){
            if(alpha[i].sprite.getPosition().x < 350)
                alpha[i].sprite.move(3, 0);
            if(beta[i].sprite.getPosition().x > 350)
                beta[i].sprite.move(-3.7, 0);
            if(gamma[i].sprite.getPosition().y < 160)
                gamma[i].sprite.move(0, 1.2);
        }
    }
    //Bonus enemies
    if(alpha[0].sprite.getPosition().x > 80){    //explosion sound effect
        if(!explode){
            explode=true;
            explosion.play();
        }
    }
    if((beta[0].sprite.getPosition().x >= (alpha[0].sprite.getPosition().x) && beta[0].sprite.getPosition().x <= (alpha[0].sprite.getPosition().x + 99.0f)) &&
    (beta[0].sprite.getPosition().y >= (alpha[0].sprite.getPosition().y) && beta[0].sprite.getPosition().y <= (alpha[0].sprite.getPosition().y + 99.0f))){
        
        destroyed=true;

         alpha[0].s=true, alpha[1].s=true, alpha[2].s=true, alpha[3].s=true;
            alpha[0].b=true, alpha[1].b=true, alpha[2].b=true, alpha[3].b=true;
                alpha[0].sprite.setPosition(300, 30);
                alpha[1].sprite.setPosition(260, 90);
                alpha[2].sprite.setPosition(340, 90);
                alpha[3].sprite.setPosition(300, 160);

            beta[0].s=true, beta[1].s=true, beta[2].s=true, beta[3].s=true;
            beta[0].b=true, beta[1].b=true, beta[2].b=true, beta[3].b=true;
                beta[0].sprite.setPosition(220, 160);
                beta[1].sprite.setPosition(380, 160);
                beta[2].sprite.setPosition(220, 240);
                beta[3].sprite.setPosition(380, 240);

            gamma[0].s=true, gamma[1].s=true, gamma[2].s=true, gamma[3].s=true;
            gamma[0].b=true, gamma[1].b=true, gamma[2].b=true, gamma[3].b=true;
                gamma[0].sprite.setPosition(260, 300);
                gamma[1].sprite.setPosition(340, 300);
                gamma[2].sprite.setPosition(300, 370);
                gamma[3].sprite.setPosition(300, 240);

        
        level++;
    }
}
else{
for(int i=0 ; i<alpha_size ; i++){
    alpha[i].alpha_movement(difficulty);
}
for(int i=0 ; i<beta_size ; i++){
    beta[i].beta_movement(difficulty);
}
for(int i=0 ; i<gamma_size ; i++){
    gamma[i].gamma_movement(difficulty);
}
}   
    if(level==0)
        difficulty=1;
    else if(level==3)
        difficulty=2;
    else if(level==6)
        difficulty=3;

    //enemy bullet movement and if enemy bullet hits the player
if(!nextlevel){
for(int i=0 ; i<4 ; i++){
    if(alpha[i].s){
        if(timer > alpha[i].alpha_timer){
            alpha[i].alpha_fired=true;
            alpha[i].bullet->sprite.move(0, 1);
        }
        if(alpha[i].bullet->sprite.getPosition().y > 790){
            alpha[i].alpha_fired=false;
            alpha[i].alpha_timer = timer;
            alpha[i].alpha_timer += 5;
            alpha[i].bullet->sprite.setPosition(alpha[i].sprite.getPosition().x+20, alpha[i].sprite.getPosition().y);
        }
        if((alpha[i].bullet->sprite.getPosition().x >= (p->sprite.getPosition().x) && alpha[i].bullet->sprite.getPosition().x <= (p->sprite.getPosition().x + 99.0f)) &&
        (alpha[i].bullet->sprite.getPosition().y >= (p->sprite.getPosition().y) && alpha[i].bullet->sprite.getPosition().y <= (p->sprite.getPosition().y + 99.0f))){
            if(p->lf3)
                p->lf3=false;
            else if(p->lf2)
                p->lf2=false;
            else
                p->lf1=false;
            alpha[i].alpha_fired=false;
            alpha[i].alpha_timer = timer;
            alpha[i].alpha_timer += 5;
            alpha[i].bullet->sprite.setPosition(alpha[i].sprite.getPosition().x+20, alpha[i].sprite.getPosition().y);
            //player gets hit and is sent back to its starting position
            p->set_pos(340, 700);
            bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
        }
    }
        else if(!alpha[i].s){
            if(timer > alpha[i].alpha_timer){
            alpha[i].alpha_fired=true;
            alpha[i].bullet->sprite.move(0, 1);
            }
            if((alpha[i].bullet->sprite.getPosition().x >= (p->sprite.getPosition().x) && alpha[i].bullet->sprite.getPosition().x <= (p->sprite.getPosition().x + 99.0f)) &&
            (alpha[i].bullet->sprite.getPosition().y >= (p->sprite.getPosition().y) && alpha[i].bullet->sprite.getPosition().y <= (p->sprite.getPosition().y + 99.0f))){
                if(p->lf3)
                    p->lf3=false;
                else if(p->lf2)
                    p->lf2=false;
                else
                    p->lf1=false;
                alpha[i].alpha_fired=false;
                alpha[i].alpha_timer = timer;
                alpha[i].alpha_timer += 5;
                alpha[i].b=false;
                //player gets hit and is sent back to its starting position
                p->set_pos(340, 700);
                bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
            }
            else if(alpha[i].alpha_fired){
                if(alpha[i].bullet->get_pos_y() > 790){
                alpha[i].alpha_fired=false;
                alpha[i].alpha_timer = timer;
                alpha[i].alpha_timer += 5;
                }
            }
            else{
                alpha[i].alpha_fired=true;
                alpha[i].bullet->sprite.move(0, 1);
            }
        }  
}
for(int i=0 ; i<4 ; i++){
    if(beta[i].s){
        if(timer > beta[i].beta_timer){
            beta[i].beta_fired=true;
            beta[i].bullet->sprite.move(0, 1);
        }
        if(beta[i].bullet->sprite.getPosition().y > 790){
            beta[i].beta_fired=false;
            beta[i].beta_timer = timer;
            beta[i].beta_timer += 5;
            beta[i].bullet->sprite.setPosition(beta[i].sprite.getPosition().x+20, beta[i].sprite.getPosition().y+20);
        }
        if((beta[i].bullet->sprite.getPosition().x >= (p->sprite.getPosition().x) && beta[i].bullet->sprite.getPosition().x <= (p->sprite.getPosition().x + 99.0f)) &&
        (beta[i].bullet->sprite.getPosition().y >= (p->sprite.getPosition().y) && beta[i].bullet->sprite.getPosition().y <= (p->sprite.getPosition().y + 99.0f))){
            if(p->lf3)
                p->lf3=false;
            else if(p->lf2)
                p->lf2=false;
            else
                p->lf1=false;
            beta[i].beta_fired=false;
            beta[i].beta_timer = timer;
            beta[i].beta_timer += 5;
            beta[i].bullet->sprite.setPosition(beta[i].sprite.getPosition().x+20, beta[i].sprite.getPosition().y+20);
            //player gets hit and is sent back to its starting position
            p->set_pos(340, 700);
            bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
        }
    }
        else if(!beta[i].s){
            if(timer > beta[i].beta_timer){
            beta[i].beta_fired=true;
            beta[i].bullet->sprite.move(0, 1);
            }
            if((beta[i].bullet->sprite.getPosition().x >= (p->sprite.getPosition().x) && beta[i].bullet->sprite.getPosition().x <= (p->sprite.getPosition().x + 99.0f)) &&
            (beta[i].bullet->sprite.getPosition().y >= (p->sprite.getPosition().y) && beta[i].bullet->sprite.getPosition().y <= (p->sprite.getPosition().y + 99.0f))){
                if(p->lf3)
                    p->lf3=false;
                else if(p->lf2)
                    p->lf2=false;
                else
                    p->lf1=false;
                beta[i].beta_fired=false;
                beta[i].beta_timer = timer;
                beta[i].beta_timer += 5;
                beta[i].b=false;
                //player gets hit and is sent back to its starting position
                p->set_pos(340, 700);
                bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
            }
            else if(beta[i].beta_fired){
                if(beta[i].bullet->get_pos_y() > 790){
                beta[i].beta_fired=false;
                beta[i].beta_timer = timer;
                beta[i].beta_timer += 5;
                }
            }
            else{
                beta[i].beta_fired=true;
                beta[i].bullet->sprite.move(0, 1);
            }
        }  
}
for(int i=0 ; i<4 ; i++){
    if(gamma[i].s){
        if(timer > gamma[i].gamma_timer){
            gamma[i].gamma_fired=true;
            gamma[i].bullet->sprite.move(0, 1);
        }
        if(gamma[i].bullet->sprite.getPosition().y > 790){
            gamma[i].gamma_fired=false;
            gamma[i].gamma_timer = timer;
            gamma[i].gamma_timer += 5;
            gamma[i].bullet->sprite.setPosition(gamma[i].sprite.getPosition().x+20, gamma[i].sprite.getPosition().y+20);
        }
        if((gamma[i].bullet->sprite.getPosition().x >= (p->sprite.getPosition().x) && gamma[i].bullet->sprite.getPosition().x <= (p->sprite.getPosition().x + 99.0f)) &&
        (gamma[i].bullet->sprite.getPosition().y >= (p->sprite.getPosition().y) && gamma[i].bullet->sprite.getPosition().y <= (p->sprite.getPosition().y + 99.0f))){
            if(p->lf3){
                p->lf3=false;
            }
            else if(p->lf2){
                p->lf2=false;
            }
            else{
                p->lf1=false;
            }
            gamma[i].gamma_fired=false;
            gamma[i].gamma_timer = timer;
            gamma[i].gamma_timer += 5;
            gamma[i].bullet->sprite.setPosition(gamma[i].sprite.getPosition().x+20, gamma[i].sprite.getPosition().y+20);
            //player gets hit and is sent back to its starting position
            p->set_pos(340, 700);
            bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
        }
    }
        else if(!gamma[i].s){
            if(timer > gamma[i].gamma_timer){
                gamma[i].gamma_fired=true;
                gamma[i].bullet->sprite.move(0, 1);
            }
            if((gamma[i].bullet->sprite.getPosition().x >= (p->sprite.getPosition().x) && gamma[i].bullet->sprite.getPosition().x <= (p->sprite.getPosition().x + 99.0f)) &&
            (gamma[i].bullet->sprite.getPosition().y >= (p->sprite.getPosition().y) && gamma[i].bullet->sprite.getPosition().y <= (p->sprite.getPosition().y + 99.0f))){
                if(p->lf3){
                    p->lf3=false;
                }
                else if(p->lf2){
                    p->lf2=false;
                }
                else{
                    p->lf1=false;
                }
                gamma[i].gamma_fired=false;
                gamma[i].gamma_timer = timer;
                gamma[i].gamma_timer += 5;
                gamma[i].b=false;
                gamma[i].bullet->sprite.setPosition(-700+20, 20);
                //player gets hit and is sent back to its starting position
                p->set_pos(340, 700);
                bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
            }
            else if(gamma[i].gamma_fired){
                if(gamma[i].bullet->get_pos_y() > 790){
                gamma[i].gamma_fired=false;
                gamma[i].gamma_timer = timer;
                gamma[i].gamma_timer += 5;
                }
            }
            else{
                gamma[i].gamma_fired=true;
                gamma[i].bullet->sprite.move(0, 1);
            }
        }
}
}
    //if player collides with enemy, it dies
for(int i=0 ; i<4 ; i++){
    if(alpha[i].alpha_collide(p->sprite.getPosition().x, p->sprite.getPosition().y)){
            if(p->lf3)
                p->lf3=false;
            else if(p->lf2)
                p->lf2=false;
            else
                p->lf1=false;
        p->set_pos(340, 700);
        bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
    }   
}
for(int i=0 ; i<4 ; i++){
    if(beta[i].beta_collide(p->sprite.getPosition().x, p->sprite.getPosition().y)){
            if(p->lf3)
                p->lf3=false;
            else if(p->lf2)
                p->lf2=false;
            else
                p->lf1=false;
        p->set_pos(340, 700);
        bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
    }
}
for(int i=0 ; i<4 ; i++){ 
    if(gamma[i].gamma_collide(p->sprite.getPosition().x, p->sprite.getPosition().y)){
            if(p->lf3)
                p->lf3=false;
            else if(p->lf2)
                p->lf2=false;
            else
                p->lf1=false;
        p->set_pos(340, 700);
        bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
    }
}

    //if bullet hits enemy ,it dies and player scores
for(int i=0 ; i<4 ; i++){
    if((bullet->sprite.getPosition().x >= (alpha[i].sprite.getPosition().x) && bullet->sprite.getPosition().x <= (alpha[i].sprite.getPosition().x + 99.0f)) &&
    (bullet->sprite.getPosition().y >= (alpha[i].sprite.getPosition().y) && bullet->sprite.getPosition().y <= (alpha[i].sprite.getPosition().y + 99.0f))){
        if(alpha[i].s){
            alpha[i].s=false;
            alpha[i].sprite.setPosition(0, 900);
            if(!alpha[i].alpha_fired)
                alpha[i].bullet->sprite.setPosition(20, 920);
            if(!fire->fire_pickup){
                fired=false;
                bullet->sprite.setPosition(p->get_pos_x()+27, p->get_pos_y()+30);
            }
        score+=30*(level/3+1);
        }
    }
}
for(int i=0 ; i<4 ; i++){
    if((bullet->sprite.getPosition().x >= (beta[i].sprite.getPosition().x) && bullet->sprite.getPosition().x <= (beta[i].sprite.getPosition().x + 99.0f)) &&
    (bullet->sprite.getPosition().y >= (beta[i].sprite.getPosition().y) && bullet->sprite.getPosition().y <= (beta[i].sprite.getPosition().y + 99.0f))){
        if(beta[i].s){
            beta[i].s=false;
            beta[i].sprite.setPosition(0, 900);
            if(!beta[i].beta_fired)
                beta[i].bullet->sprite.setPosition(20, 920);
            if(!fire->fire_pickup){
                fired=false;
                bullet->sprite.setPosition(sf::Vector2f(p->get_pos_x()+27, p->get_pos_y()+30));
            }
        score+=20*(level/3+1);
        }
    }
}
for(int i=0 ; i<4 ; i++){
    if((bullet->sprite.getPosition().x >= (gamma[i].sprite.getPosition().x) && bullet->sprite.getPosition().x <= (gamma[i].sprite.getPosition().x + 99.0f)) &&
    (bullet->sprite.getPosition().y >= (gamma[i].sprite.getPosition().y) && bullet->sprite.getPosition().y <= (gamma[i].sprite.getPosition().y + 99.0f))){
        if(gamma[i].s){
            gamma[i].s=false;
            gamma[i].sprite.setPosition(0, 900);
            if(!gamma[i].gamma_fired)
                gamma[i].bullet->sprite.setPosition(20, 920);
            if(!fire->fire_pickup){
                fired=false;
                bullet->sprite.setPosition(sf::Vector2f(p->get_pos_x()+27, p->get_pos_y()+40));
            }
        score+=10*(level/3+1);
        }
    }
}

//powerups
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(!powerup_fired){   //powerup movement
        for(int i=0 ; i<7 ; i++){
            powerup_fire->p[i].setPosition(p->sprite.getPosition().x+27, p->sprite.getPosition().y+27);
        }
    }
    //powerup
    if(timer > powerup_timer){  //moves the powerup icon down
        powerup->sprite.move(0, 1);
    }
    if(timer > powerup_pickup_timer + 5){   //resets the powerup icon
        powerup_pickup=false;
    }
    if(powerup->sprite.getPosition().y > 790){  //respawns the powerup icon and resets the drop down timer
        powerup_timer +=30;
        powerup->sprite.setPosition(rand()%780, -50);
    }
    if((powerup->sprite.getPosition().x >= (p->sprite.getPosition().x) && powerup->sprite.getPosition().x <= (p->sprite.getPosition().x + 99.0f)) &&
    (powerup->sprite.getPosition().y >= (p->sprite.getPosition().y) && powerup->sprite.getPosition().y <= (p->sprite.getPosition().y + 99.0f))){  //checks if powerup has been picked
        powerup_timer +=30;
        powerup_pickup_timer = timer;
        powerup_pickup = true;
        powerup_fired = true;
        powerup->sprite.setPosition(rand()%780, -50);
        for(int i=0 ; i<7 ; i++){
            powerup_fire->arr[i]=true;
        }
    }
    if(powerup_fired){  //moves all 7 bullets when powerup is picked
            if(powerup_fire->arr[0])
                powerup_fire->p[0].move(0, -5);
            if(powerup_fire->arr[1])
                powerup_fire->p[1].move(-1, -5);
            if(powerup_fire->arr[2])
                powerup_fire->p[2].move(1, -5);
            if(powerup_fire->arr[3])
                powerup_fire->p[3].move(-2, -5);
            if(powerup_fire->arr[4])
                powerup_fire->p[4].move(2, -5);
            if(powerup_fire->arr[5])
                powerup_fire->p[5].move(-3, -5);
            if(powerup_fire->arr[6])
                powerup_fire->p[6].move(3, -5);
    }
    for(int i=0 ; i<7 ; i++){   //if powerup bullets reach the top
        if(powerup_fire->p[i].getPosition().y < 0){
            powerup_fired=false;
            powerup_fire->arr[i]=false;
        }
    }
    for(int i=0 ; i<7 ; i++){   //if powerup bullets hit the enemy, it dies
        for(int j=0 ; j<4 ; j++){
            if((powerup_fire->p[i].getPosition().x >= (alpha[j].sprite.getPosition().x) && powerup_fire->p[i].getPosition().x <= (alpha[j].sprite.getPosition().x + 99.0f)) &&
            (powerup_fire->p[i].getPosition().y >= (alpha[j].sprite.getPosition().y) && powerup_fire->p[i].getPosition().y <= (alpha[j].sprite.getPosition().y + 99.0f))){
                if(alpha[j].s){
                    alpha[j].s=false;
                    alpha[j].sprite.setPosition(0, 900);
                    if(!alpha[j].alpha_fired)
                        alpha[j].bullet->sprite.setPosition(20, 920);
                    if(!fire->fire_pickup){
                        fired=false;
                        bullet->sprite.setPosition(p->get_pos_x()+27, p->get_pos_y()+30);
                    }
                score+=30*level;
                powerup_fire->p[i].setPosition(p->sprite.getPosition().x+20, p->sprite.getPosition().y+20);
                powerup_fire->arr[i]=false;
                }
            }
            if((powerup_fire->p[i].getPosition().x >= (beta[j].sprite.getPosition().x) && powerup_fire->p[i].getPosition().x <= (beta[j].sprite.getPosition().x + 99.0f)) &&
            (powerup_fire->p[i].getPosition().y >= (beta[j].sprite.getPosition().y) && powerup_fire->p[i].getPosition().y <= (beta[j].sprite.getPosition().y + 99.0f))){
                if(beta[j].s){
                    beta[j].s=false;
                    beta[j].sprite.setPosition(0, 900);
                    if(!beta[j].beta_fired)
                        beta[j].bullet->sprite.setPosition(20, 920);
                    if(!fire->fire_pickup){
                        fired=false;
                        bullet->sprite.setPosition(sf::Vector2f(p->get_pos_x()+27, p->get_pos_y()+30));
                    }
                score+=20*level;
                powerup_fire->p[i].setPosition(p->sprite.getPosition().x+20, p->sprite.getPosition().y+20);
                powerup_fire->arr[i]=false;
                }
            }
            if((powerup_fire->p[i].getPosition().x >= (gamma[j].sprite.getPosition().x) && powerup_fire->p[i].getPosition().x <= (gamma[j].sprite.getPosition().x + 99.0f)) &&
            (powerup_fire->p[i].getPosition().y >= (gamma[j].sprite.getPosition().y) && powerup_fire->p[i].getPosition().y <= (gamma[j].sprite.getPosition().y + 99.0f))){
                if(gamma[j].s){
                    gamma[j].s=false;
                    gamma[j].sprite.setPosition(0, 900);
                    if(!gamma[j].gamma_fired)
                        gamma[j].bullet->sprite.setPosition(20, 920);
                    if(!fire->fire_pickup){
                        fired=false;
                        bullet->sprite.setPosition(sf::Vector2f(p->get_pos_x()+27, p->get_pos_y()+40));
                    }
                score+=10*level;
                powerup_fire->p[i].setPosition(p->sprite.getPosition().x+20, p->sprite.getPosition().y+20);
                powerup_fire->arr[i]=false;
                }
            }
        }
    }
    //fire
    fire->movement_fire(timer);  //for the movement of fire addOnn
    fire->check_bounds_fire();   //checks for out of bounds and updates the addOnn
    if(timer > fire->fire_pickup_timer + 5){
        bullet->set_textue("img/laserBlue16.png");
        bullet->sprite.setScale(0.7, 0.7);
        fire->fire_pickup=false;
    }
    if(fire->check_collision(p->sprite.getPosition().x, p->sprite.getPosition().y, timer)){
        fire->fire_timer += 15;
        fire->fire_pickup_timer=timer;
        fire->fire_pickup=true;
        bullet->set_textue("img/laserRed06.png");
        bullet->sprite.setScale(1.5, 1.5);
    }
    
    //life
    life->movement_life(timer);  //for the movement of life addOnn
    life->check_bounds_life();   //checks for out of bounds and updates the addOnn
    if(life->check_collision(p->sprite.getPosition().x, p->sprite.getPosition().y, timer)){
        if(!p->lf1)
            p->lf1=true;
        else if(!p->lf2)
            p->lf2=true;
        else if(!p->lf3)
            p->lf3=true;
        
        life->life_timer += 10;
    }
    //Danger
    danger->movement_danger(timer);  //for the movement of danger addOnn
    if(danger->sprite.getPosition().y > 790){   //checks for out of bounds and updates the addOnn
        danger->danger_timer += 15;
        danger->sprite.setPosition(rand()%700, -80);
        if(!danger->danger_pickup){
            score+=40;
            danger->danger_pickup=false;
        }
    }
    if(!nextlevel){     //checks for collision
        if(danger->check_collision(p->sprite.getPosition().x, p->sprite.getPosition().y, timer)){
            if(p->lf3)
                p->lf3=false;
            else if(p->lf2)
                p->lf2=false;
            else
                p->lf1=false;

                danger->danger_pickup=true;
                danger->danger_timer += 5;
        
            p->set_pos(340, 700);
            bullet->set_pos(p->get_pos_x()+30, p->get_pos_y()+20);
        }
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //checking levels;
    bool check=true;
    for(int i=0 ; i<4 ; i++){
        if(alpha[i].s)
            check=false;
        if(beta[i].s)
            check=false;
        if(gamma[i].s)
            check=false;
    }
    if(level==5){
        if(monster_spawn){
            check=false;
        }
        if(timer > monster_timer + 10)
            monster_spawn=false;
    }
    if(level==11){
        if(dragon_spawn){
            check=false;
        }
        if(timer > dragon_spawn_timer + 20)
            dragon_spawn=false;
    }
    if(check){
        nextlevel=true;
        p->sprite.move(0, -2);
        bullet->sprite.move(0, -2);
    }
    if(p->sprite.getPosition().y >= 600 && p->sprite.getPosition().y <=630){
        nextlevel=false;
    }
    if(check==true && nextlevel==false){

        level++;

            for(int i=0 ; i<4 ; i++){
                alpha[i].alpha_move=false;
                beta[i].beta_move=false;
                gamma[i].gamma_move=false;
            }

        switch(level){      //Displaying of levels
            case 1:{    //display the triangle shape
            wave.setString("third wave");
            alpha[0].s=true, alpha[1].s=true, alpha[2].s=true;
            alpha[0].b=true, alpha[1].b=true, alpha[2].b=true;
                alpha[0].sprite.setPosition(300, 30);
                alpha[1].sprite.setPosition(260, 100);
                alpha[2].sprite.setPosition(340, 100);

            beta[0].s=true, beta[1].s=true;
            beta[0].b=true, beta[1].b=true;
                beta[0].sprite.setPosition(220, 160);
                beta[1].sprite.setPosition(380, 160);

            gamma[0].s=true, gamma[1].s=true, gamma[2].s=true, gamma[3].s=true;
            gamma[0].b=true, gamma[1].b=true, gamma[2].b=true, gamma[3].b=true;
                gamma[0].sprite.setPosition(180, 240);
                gamma[1].sprite.setPosition(260, 240);
                gamma[2].sprite.setPosition(340, 240);
                gamma[3].sprite.setPosition(420, 240);
            }break;
            case 2:{    //displays the cross shape
            wave.setString("fourth wave");
            alpha[0].s=true, alpha[1].s=true;
            alpha[0].b=true, alpha[1].b=true;
                alpha[0].sprite.setPosition(220, 30);
                alpha[1].sprite.setPosition(380, 30);

            beta[0].s=true, beta[1].s=true, beta[2].s=true;
            beta[0].b=true, beta[1].b=true, beta[2].b=true;
                beta[0].sprite.setPosition(260, 90);
                beta[1].sprite.setPosition(340, 90);
                beta[2].sprite.setPosition(300, 160);

            gamma[0].s=true, gamma[1].s=true, gamma[2].s=true, gamma[3].s=true;
            gamma[0].b=true, gamma[1].b=true, gamma[2].b=true, gamma[3].b=true;
                gamma[0].sprite.setPosition(260, 240);
                gamma[1].sprite.setPosition(340, 240);
                gamma[2].sprite.setPosition(220, 300);
                gamma[3].sprite.setPosition(380, 300);
            }break;
            case 3:{    //displays the circle shape
            wave.setString("fifth wave");
            alpha[0].s=true, alpha[1].s=true;
            alpha[0].b=true, alpha[1].b=true;
                alpha[0].sprite.setPosition(260, 30);
                alpha[1].sprite.setPosition(340, 30);

            beta[0].s=true, beta[1].s=true;
            beta[0].b=true, beta[1].b=true;
                beta[0].sprite.setPosition(220, 90);
                beta[1].sprite.setPosition(380, 90);

            gamma[0].s=true, gamma[1].s=true, gamma[2].s=true, gamma[3].s=true;
            gamma[0].b=true, gamma[1].b=true, gamma[2].b=true, gamma[3].b=true;
                gamma[0].sprite.setPosition(220, 160);
                gamma[1].sprite.setPosition(380, 160);
                gamma[2].sprite.setPosition(260, 240);
                gamma[3].sprite.setPosition(340, 240);
            }break;
            case 4:{    //displays the diamond Shape
            wave.setString("Dragon Incoming");
            alpha[0].s=true, alpha[1].s=true, alpha[2].s=true;
            alpha[0].b=true, alpha[1].b=true, alpha[2].b=true;
                alpha[0].sprite.setPosition(300, 30);
                alpha[1].sprite.setPosition(260, 90);
                alpha[2].sprite.setPosition(340, 90);

            beta[0].s=true, beta[1].s=true, beta[2].s=true, beta[3].s=true;
            beta[0].b=true, beta[1].b=true, beta[2].b=true, beta[3].b=true;
                beta[0].sprite.setPosition(220, 160);
                beta[1].sprite.setPosition(380, 160);
                beta[2].sprite.setPosition(220, 240);
                beta[3].sprite.setPosition(380, 240);

            gamma[0].s=true, gamma[1].s=true, gamma[2].s=true;
            gamma[0].b=true, gamma[1].b=true, gamma[2].b=true;
                gamma[0].sprite.setPosition(260, 300);
                gamma[1].sprite.setPosition(340, 300);
                gamma[2].sprite.setPosition(300, 370);
            }break;
            case 5:{    //unleashes the dragon
            wave.setString("Sixth wave");
            monster_spawn=true;
                if(!mons){
                    mons=true;
                    monster_timer=timer;
                }
                    monster->sprite.setPosition(150, -100);

                        monster->bullet1->sprite.setPosition(360, 300);
                    monster->bullet1->sprite.setScale(1.5, 1.5);
                        monster->bullet2->sprite.setPosition(360, 300);
                    monster->bullet2->sprite.setScale(1.5, 1.5);
                        monster->bullet3->sprite.setPosition(360, 300);
                    monster->bullet3->sprite.setScale(1.5, 1.5);
                        monster->bullet4->sprite.setPosition(360, 300);
                    monster->bullet4->sprite.setScale(1.5, 1.5);
                        monster->bullet5->sprite.setPosition(360, 300);
                    monster->bullet5->sprite.setScale(1.5, 1.5);
                        monster->bullet6->sprite.setPosition(360, 300);
                    monster->bullet6->sprite.setScale(1.5, 1.5);
                        monster->bullet7->sprite.setPosition(360, 300);
                    monster->bullet7->sprite.setScale(1.5, 1.5);
            }break;
            case 6:{    //displays full rectangle
            score+=50;
            wave.setString("Seventh wave");
            alpha[0].s=true, alpha[1].s=true, alpha[2].s=true, alpha[3].s=true;
            alpha[0].b=true, alpha[1].b=true, alpha[2].b=true, alpha[3].b=true;
                alpha[0].sprite.setPosition(240, 30);
                alpha[1].sprite.setPosition(300, 30);
                alpha[2].sprite.setPosition(360, 30);
                alpha[3].sprite.setPosition(420, 30);

            beta[0].s=true, beta[1].s=true, beta[2].s=true, beta[3].s=true;
            beta[0].b=true, beta[1].b=true, beta[2].b=true, beta[3].b=true;
                beta[0].sprite.setPosition(240, 90);
                beta[1].sprite.setPosition(300, 90);
                beta[2].sprite.setPosition(360, 90);
                beta[3].sprite.setPosition(420, 90);

            gamma[0].s=true, gamma[1].s=true, gamma[2].s=true, gamma[3].s=true;
            gamma[0].b=true, gamma[1].b=true, gamma[2].b=true, gamma[3].b=true;
                gamma[0].sprite.setPosition(240, 160);
                gamma[1].sprite.setPosition(300, 160);
                gamma[2].sprite.setPosition(360, 160);
                gamma[3].sprite.setPosition(420, 160);
            }break;
            case 7:{    //displays full diamond
            wave.setString("Eigth wave");
            alpha[0].s=true, alpha[1].s=true, alpha[2].s=true, alpha[3].s=true;
            alpha[0].b=true, alpha[1].b=true, alpha[2].b=true, alpha[3].b=true;
                alpha[0].sprite.setPosition(300, 30);
                alpha[1].sprite.setPosition(260, 90);
                alpha[2].sprite.setPosition(340, 90);
                alpha[3].sprite.setPosition(300, 160);

            beta[0].s=true, beta[1].s=true, beta[2].s=true, beta[3].s=true;
            beta[0].b=true, beta[1].b=true, beta[2].b=true, beta[3].b=true;
                beta[0].sprite.setPosition(220, 160);
                beta[1].sprite.setPosition(380, 160);
                beta[2].sprite.setPosition(220, 240);
                beta[3].sprite.setPosition(380, 240);

            gamma[0].s=true, gamma[1].s=true, gamma[2].s=true, gamma[3].s=true;
            gamma[0].b=true, gamma[1].b=true, gamma[2].b=true, gamma[3].b=true;
                gamma[0].sprite.setPosition(260, 300);
                gamma[1].sprite.setPosition(340, 300);
                gamma[2].sprite.setPosition(300, 370);
                gamma[3].sprite.setPosition(300, 240);
            }break;
            case 8:{    //displays the Bonus task
            wave.setString("Final wave");
            alpha[0].s=true, alpha[1].s=true, alpha[2].s=true, alpha[3].s=true;
            alpha[0].b=true, alpha[1].b=true, alpha[2].b=true, alpha[3].b=true;
                alpha[0].sprite.setPosition(-50, 160);
                alpha[1].sprite.setPosition(-50, 160);
                alpha[2].sprite.setPosition(-50, 160);
                alpha[3].sprite.setPosition(-50, 160);

            beta[0].s=true, beta[1].s=true, beta[2].s=true, beta[3].s=true;
            beta[0].b=true, beta[1].b=true, beta[2].b=true, beta[3].b=true;
                beta[0].sprite.setPosition(850, 160);
                beta[1].sprite.setPosition(850, 160);
                beta[2].sprite.setPosition(850, 160);
                beta[3].sprite.setPosition(850, 160);

            gamma[0].s=true, gamma[1].s=true, gamma[2].s=true, gamma[3].s=true;
            gamma[0].b=true, gamma[1].b=true, gamma[2].b=true, gamma[3].b=true;
                gamma[0].sprite.setPosition(350, -50);
                gamma[1].sprite.setPosition(350, -50);
                gamma[2].sprite.setPosition(350, -50);
                gamma[3].sprite.setPosition(350, -50);
            }break;
            case 9:{}break;
            case 10:{    //displays the heart shape
            wave.setString("Monster Incoming");
            alpha[0].s=true, alpha[1].s=true, alpha[2].s=true, alpha[3].s=true;
            alpha[0].b=true, alpha[1].b=true, alpha[2].b=true, alpha[3].b=true;
                alpha[0].sprite.setPosition(220, 30);
                alpha[1].sprite.setPosition(380, 30);
                alpha[2].sprite.setPosition(180, 90);
                alpha[3].sprite.setPosition(420, 90);

            beta[0].s=true, beta[1].s=true, beta[2].s=true, beta[3].s=true;
            beta[0].b=true, beta[1].b=true, beta[2].b=true, beta[3].b=true;
                beta[0].sprite.setPosition(260, 90);
                beta[1].sprite.setPosition(340, 90);
                beta[2].sprite.setPosition(220, 160);
                beta[3].sprite.setPosition(380, 160);

            gamma[0].s=true, gamma[1].s=true, gamma[2].s=true;
            gamma[0].b=true, gamma[1].b=true, gamma[2].b=true;
                gamma[0].sprite.setPosition(260, 240);
                gamma[1].sprite.setPosition(340, 240);
                gamma[2].sprite.setPosition(300, 300);
            }break;
            case 11:{   //displays the monster
                dragon_spawn=true;
                if(!drag){
                    dragon_spawn_timer=timer;
                    drag=true;
                    dragon_timer=timer;
                }
                dragon->sprite.setPosition(150, 0);
                dragon->bullet->set_pos(350, 250);
            }break;
            case 12:{   //restarts the levels
                score+=50;
                level=0;
            }break;
        }
    }

    //All displaying is done here
    if(true){
	
    window.clear(Color::Black); //clears the screen
	window.draw(background);  // setting background

    //Draw and update lives
    if(p->lf1)
        window.draw(p->l1);
    if(p->lf2)
        window.draw(p->l2);
    if(p->lf3)
        window.draw(p->l3);
    
    //Display enemy bullets
    for(int i=0 ; i<4 ; i++){
        window.draw(alpha[i].bullet->sprite);
    }
    for(int i=0 ; i<4 ; i++){
        window.draw(beta[i].bullet->sprite);
    }
    for(int i=0 ; i<4 ; i++){
        window.draw(gamma[i].bullet->sprite);
    }

    //display invader sprites
    for(int i=0 ; i<4 ; i++){
            window.draw(alpha[i].sprite);
    }
    for(int i=0 ; i<4 ; i++){
            window.draw(beta[i].sprite);
    }
    for(int i=0 ; i<4 ; i++){
            window.draw(gamma[i].sprite);
    }

    //Bosses
    if(monster_spawn){
        window.draw(monster->bullet1->sprite);
        window.draw(monster->bullet2->sprite);
        window.draw(monster->bullet3->sprite);
        window.draw(monster->bullet4->sprite);
        window.draw(monster->bullet5->sprite);
        window.draw(monster->bullet6->sprite);
        window.draw(monster->bullet7->sprite);
        window.draw(monster->sprite);
    }
    if(dragon_spawn){
        window.draw(dragon->bullet->sprite);
        window.draw(dragon->sprite);
    }

    //Powerup
        for(int i=0 ; i<7 ; i++){
            window.draw(powerup_fire->p[i]);
        }
        window.draw(powerup->sprite);
        window.draw(fire->sprite);
        window.draw(life->sprite);
        window.draw(danger->sprite);

    //Displays score;
    game_score.setString(std::__cxx11::to_string(score));

    window.draw(sc);
    window.draw(game_score);

    //Displaying wave information
    if(nextlevel){
        window.draw(level_comp);
        window.draw(wave);
    }
    if(timer < 5)
        window.draw(wave);
    else if(timer <6){
        wave.setString("Second Wave");
    }

    window.draw(bullet->sprite);
	window.draw(p->sprite);   // setting player on screen

	window.display();  //Displying all the sprites
    }

    //If all three lives are ended the window closes
    if(!p->lf1){
        if(!p->lf2){
            if(!p->lf3){
                g_over=true;
            }
        }
    }
    
        
    }
}
};