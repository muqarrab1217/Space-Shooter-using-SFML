#include "game.h"
#include<fstream>
class Menu{
    Texture bg;
    Sprite back;
    Text title;
    Text button[4];
    Text instructions[6];
    int selected;
    int current_Selected;
    Font font;
public:
//add menu attributes here

Menu(float height){
    bg.loadFromFile("img/start.jpeg");
    back.setTexture(bg);
    back.setScale(1.31, 1.16);
    font.loadFromFile("img/font.otf");

    title.setFont(font);
    title.setFillColor(Color::Red);
    title.setString("Space Invader");
    title.setCharacterSize(80);
    title.setPosition(50, 30);

    for(int i=0 ; i<6 ; i++){
        instructions[i].setFont(font);
        instructions[i].setFillColor(Color::Yellow);
        instructions[i].setCharacterSize(30);
    }
    
    instructions[0].setString("INSTRUCTIONS");
    instructions[1].setString(" 1.  For  movement ,  use  keys :  Up ,  Down ,  \n Right ,  Left");
    instructions[2].setString(" 2.  If  player  hits  enemy ,  enemy  dies  and\n player  scores");
    instructions[3].setString(" 3.  Dodge  the  danger  meteor ,  fiery  dragon  \n and  the  monsters  lasers  to  score  and  \n destroy  enemies.");
    instructions[4].setString(" 4.  There  are  a  total  of 3  levels  with\n  2  bosses ,  complete   the  game  and  destroy \n  them  all"); 
    instructions[5].setString("\"PRESS  ESC  TO  RETURN  TO MAIN MENU\"");

    instructions[0].setCharacterSize(60);
    
    instructions[0].setPosition(150, 0);
    instructions[1].setPosition(0, 100);
    instructions[2].setPosition(0, 200);
    instructions[3].setPosition(0, 300);
    instructions[4].setPosition(0, 430);
    instructions[5].setPosition(50, 700);


    button[0].setFont(font);
    button[0].setFillColor(Color::Red);
    button[0].setString("Play");
    button[0].setCharacterSize(50);
    button[0].setPosition(300, height/ (5)+100);

    button[1].setFont(font);
    button[1].setFillColor(Color::White);
    button[1].setString("Instructions");
    button[1].setCharacterSize(50);
    button[1].setPosition(190, height/ (5)+250);

    button[2].setFont(font);
    button[2].setFillColor(Color::White);
    button[2].setString("Highscore");
    button[2].setCharacterSize(50);
    button[2].setPosition(220, height/ (5)+400);

    button[3].setFont(font);
    button[3].setFillColor(Color::White);
    button[3].setString("Exit");
    button[3].setCharacterSize(50);
    button[3].setPosition(300, height/ (5)+550);

    selected=0;
    current_Selected=-1;
}
void Move_up(){
    if(selected-1 > -2){
        button[selected].setFillColor(Color::White);
        selected--;
        if(selected==-1)
            selected=3;
        button[selected].setFillColor(Color::Red);
    }
}
void Move_down(){
    if(selected + 1 <=4){
        button[selected].setFillColor(Color::White);
        selected++;
        if(selected==4)
            selected=0;
        button[selected].setFillColor(Color::Red);
    }
}
void set_button(int x){
    selected = x;
}
void set_current(int x){
    current_Selected = x;
}
void display_menu(){    //display menu screen here
    Game g;
    RenderWindow window(VideoMode(700, 900), "Space Invaders Menu");
    // add functionality of all the menu options here
    while(window.isOpen()){
        Event e;
        while (window.pollEvent(e))
        {  
            if(e.type == Event::Closed){ // If cross/close is clicked/pressed
                window.close(); //close the game 
                break;
            }

            if(e.type == Event::KeyPressed){
                if(e.key.code == Keyboard::Up){
                    Move_up();
                }
                if(e.key.code == Keyboard::Down){
                    Move_down();
                }
                if(e.key.code == Keyboard::Enter){
                    window.close();
                    set_current(selected);
                }
            }                      	    
        }
        
    if(current_Selected==0 || current_Selected==1 || current_Selected==2 || current_Selected==3)
        break;

        window.clear(Color::Black);
        window.draw(back);
        window.draw(title);

            for(int i=0 ; i<4 ; i++){
                window.draw(button[i]);
            }

            window.display();
    }

        if(current_Selected==0){    //if Start game option is chosen
            g.score=0;
            g.start_game();
            current_Selected=-1;
            display_menu();
        }
        else if(current_Selected==1){   //if instructions option is pressed
            window.close();
                Texture over;
                Sprite game;
                over.loadFromFile("img/background2.jpg");
                game.setTexture(over);
                game.setScale(2, 2.6);
                RenderWindow gameover(VideoMode(750, 780), "Instructions");
                while(gameover.isOpen()){
                Event gameover_events;
                    while (gameover.pollEvent(gameover_events))
                    {
                        if(gameover_events.type == Event::Closed){ // If cross/close is clicked/pressed
                            gameover.close(); //close the game
                        }
                        if(Keyboard::isKeyPressed(Keyboard::Escape))
                            gameover.close();
                    }
                    gameover.clear(Color::Black);
                    gameover.draw(game);
                    gameover.draw(instructions[0]);
                    gameover.draw(instructions[1]);
                    gameover.draw(instructions[2]);
                    gameover.draw(instructions[3]);
                    gameover.draw(instructions[4]);
                    gameover.draw(instructions[5]);
                    gameover.display();
                }
            current_Selected=-1;
            display_menu();

        }
        else if(current_Selected==2){
            string text;
            Text txt[10];
            for(int i=0 ; i<6 ; i++){
                txt[i].setFont(font);
                txt[i].setFillColor(Color::Yellow);
                txt[i].setCharacterSize(30);
                txt[i].setPosition(50, i*50 + 100);
            }
            txt[0].setString("HIGH SCORE");
            txt->setCharacterSize(80);
            txt->setPosition(170, 20);
            ifstream file("highscore.txt", ios::in);
            for(int i=0 ; i<9 ; i++){
            if(file.is_open()){
                getline(file, text);
            }
                txt[i+1].setString(text);
            }
            Text temp;
            for(int i=1 ; i<9 ; i++){
                for(int j=1 ; j<9-i ; j++){
                    if(txt[j].getString() < txt[j+1].getString()){
                        temp.setString(txt[j].getString());
                        txt[j].setString(txt[j+1].getString());
                        txt[j+1].setString(temp.getString());
                    }
                }
            }
            Texture over;
                Sprite game;
                over.loadFromFile("img/game over.jpeg");
                game.setTexture(over);
                game.setScale(2.7, 2);
                RenderWindow gameover(VideoMode(750, 780), "Game Over");
                while(gameover.isOpen()){
                Event gameover_events;
                    while (gameover.pollEvent(gameover_events))
                    {
                        if(gameover_events.type == Event::Closed){ // If cross/close is clicked/pressed
                            gameover.close(); //close the game
                            current_Selected=-1;
                            display_menu();
                        }
                        if(Keyboard::isKeyPressed(Keyboard::Escape)){
                            gameover.close(); //close the game
                            current_Selected=-1;
                            display_menu();
                        }
                    }
                    gameover.clear(Color::Black);
                    for(int i=0 ; i<4 ; i++)
                        gameover.draw(txt[i]);

                    gameover.display();
                }

        }
        else if(current_Selected==3)    //if exit option is pressed
            window.close();
}


};
