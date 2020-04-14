#ifndef MAIN_LOOP_HPP
#define MAIN_LOOP_HPP

#include "Sprites.hpp"
#include <sstream>

#define CONTROL_TIME_STEP 1000
using namespace sf;

extern double window_width;
extern double window_height;

Clock uniform_move_clock;
Clock SM_timer_clock;
int SM_control_counter = 0;
float uniform_time = 0.003;
extern int chromosome_size;
extern int game_mode;

extern int death_by_enemies;
extern int death_by_stuck;
extern int death_by_falling;
extern int game_finished;
extern int death_by_time_running_out;


//platform states for game mode 1:
// 1 - empty platform
// 2 - bat to the right
// 3 - bat to the left
// 4 - gold on the platform
// 5 - imp to the right
// 6 - imp to the left
// 7 - im on the imp
#define DRAW
//A function that transforms a number into a function call

void number_to_function(int func_ind, std::vector<int> &level_map, PlayerClass &player,
                         sf::RenderWindow &window, sf::Thread &mana_thread, std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps, float uniform_time){
    switch(func_ind){
                case 0:
                    move_right(level_map, player, uniform_time);
                    break;
                case 1:
                    move_left(level_map, player, uniform_time);
                    break;
                case 2:
                    move_left_then_jump(window, level_map, player, uniform_time);
                    break;
                case 3:
                    move_right_then_jump(window, level_map, player, uniform_time);
                    break;
                case 4:
                    PANIC(window, level_map, player, uniform_time);
                    break;
                case 5:
                    jump(window, player, uniform_time);
                    break;
                case 6:
                    shoot_shoot(window, player, uniform_time);
                    break;
                case 7:
                    shoot_laser(window, mana_thread, bats, imps, player, uniform_time);
                    break;
                case 8:
                    shoot_laser_then_shoot_shoot(window, mana_thread, bats, imps, player, uniform_time);
                    break;
                case 9:
                    jump_n_shoot(level_map, window, player, uniform_time);
                    break;
                case 10:
                    jump_n_shoot_n_panic(window, level_map, player, uniform_time);
                    break;
                case 11:
                    collect_gold(level_map, player, uniform_time);
                    break;
                default:
                    PANIC(window, level_map, player, uniform_time);
                    break;
            }
}


//A function that transforms a number into a function call
void SM_number_to_function(int func_ind, std::vector<int> &level_map, PlayerClass &player,
                         sf::RenderWindow &window, sf::Thread &mana_thread, std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps, float uniform_time){
    switch(func_ind){
                case 0:
                    SM_move_right(player, uniform_time);
                    break;
                case 1:
                    SM_move_left(player, uniform_time);
                    break;
                case 2:
                    SM_jump(window, player, uniform_time);
                    break;
                case 3:
                    SM_jump_right(window, player, uniform_time);
                    break;
                case 4:
                    SM_jump_left(window, player, uniform_time);
                    break;
                case 5:
                    SM_shoot_shoot(window, player, uniform_time);
                    break;
                case 6:
                    SM_shoot_laser(window, mana_thread, bats, imps, player, uniform_time);
                    break;
                case 7:
                    SM_jump_shoot(window, player, uniform_time);
                    break;
                default:
                    SM_jump_right(window, player, uniform_time);
                    break;
            }
}

// A function that moves the player based on the player status
void SM_state_handler(std::vector<int> &genetic_pattern, std::vector<int> &level_map, PlayerClass &player,
                    sf::RenderWindow &window, sf::Thread &mana_thread, std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps, float uniforme_time){

                if(SM_timer_clock.getElapsedTime().asMilliseconds() > CONTROL_TIME_STEP){
                    SM_control_counter++;
                    SM_timer_clock.restart();
                }

                //std::cout << "SM_COUNTER: " << SM_control_counter << std::endl;
                SM_number_to_function(genetic_pattern[SM_control_counter], level_map, player, window, mana_thread, bats, imps, uniforme_time);


}

// A function that moves the player based on the player status
void state_handler(std::vector<int> &genetic_pattern, std::vector<int> &level_map, PlayerClass &player,
                    sf::RenderWindow &window, sf::Thread &mana_thread, std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps, float uniforme_time){

                if(player.platform_state_ == 1){
                    number_to_function(genetic_pattern[0], level_map, player, window, mana_thread, bats, imps, uniforme_time);
                }
                else if(player.platform_state_ == 2){
                    player.facing_left_ = false;
                    number_to_function(genetic_pattern[1], level_map, player, window, mana_thread, bats, imps, uniforme_time);
                }
                else if(player.platform_state_ == 3){
                    player.facing_left_ = true;
                    number_to_function(genetic_pattern[2], level_map, player, window, mana_thread, bats, imps, uniforme_time);
                }else if(player.platform_state_ == 4){
                    number_to_function(genetic_pattern[3], level_map, player, window, mana_thread, bats, imps, uniforme_time);
                }else if(player.platform_state_ == 5){
                    player.facing_left_ = false;
                    number_to_function(genetic_pattern[4], level_map, player, window, mana_thread, bats, imps, uniforme_time);
                }else if(player.platform_state_ == 6){
                    player.facing_left_ = true;
                    number_to_function(genetic_pattern[5], level_map, player, window, mana_thread, bats, imps, uniforme_time);
                }else if(player.platform_state_ == 7){
                    number_to_function(genetic_pattern[6], level_map, player, window, mana_thread, bats, imps, uniforme_time);
                }
}

void Sleep(int milisec){
    Clock sleep_time;

    while(sleep_time.getElapsedTime().asMilliseconds() < milisec){

    }
    return;
}

void level_makeup(sf::RenderWindow &window, Sprites &sprites, PlayerClass &player){
        sprites.portal_sprite.setPosition(5050, 152);
        sprites.portal_sprite.setScale(4, 8);
        window.draw(sprites.portal_sprite);

        sprites.gold_numerator.setPosition(player.sprite_.getPosition().x + 0.8*window_width/2, player.sprite_.getPosition().y - 0.98*window_height/2);
        std::stringstream ss;
        ss << player.player_gold;

        sprites.gold_numerator.setString(ss.str().c_str());
        sprites.gold_numerator.setFillColor(Color(255, 215, 0));
        window.draw(sprites.gold_numerator);
        sprites.gold_sprite.setPosition(player.sprite_.getPosition().x + 0.8*window_width/2 - 60, player.sprite_.getPosition().y - 0.98*window_height/2);
        sprites.gold_sprite.setScale(2, 2);
        window.draw(sprites.gold_sprite);
}


void init_level(int level_length, std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps,
                Sprites &sprites, std::vector<int> &level_map){
    level_map.push_back(0);
    int i = 1;

    int plat_or_hole_len = 0;
    // true plat false hole - what a great comment - clearly im not phoning this one in
    bool plat_or_hole = true;
    while(i < level_length){
        if(plat_or_hole){
            plat_or_hole = false;
            plat_or_hole_len = 10 + rand() % 8;

            if(rand() % 101 < 50){
                bats.push_back(BatsyEnemyClass(sprites.batsy_sprite, sprites.sonic_sprite, sprites.gold_sprite, (i+1)*50, 434, i, i + plat_or_hole_len - 1));
                level_map.push_back(2);
                i++;
            }else if(rand() % 101 <= 100){
                imps.push_back(ImpEnemyClass(sprites.fireball_sprite, sprites.imp_sprite, sprites.gold_sprite, (i+1)*50, 440, i, i + plat_or_hole_len - 1));
                level_map.push_back(5);
                i++;
            }
            else{
                level_map.push_back(1);
                i++;
            }
            for(int k = 0; k < plat_or_hole_len-1; k++){
                level_map.push_back(1);
                i++;
            }

        }
        else{
            plat_or_hole = true;
            plat_or_hole_len = 2 + rand() % 2;

            for(int k = 0; k < plat_or_hole_len; k++){
                level_map.push_back(0);
                i++;
            }
        }

    }

    int plat_length;
    for(int i = 0; i < level_length;){
        if(level_map[i] == 1 || level_map[i] == 2 || level_map[i] == 5)
        {
            plat_length = 0;
            while(level_map[i] != 0){
                i++;
                plat_length++;
            }
            big_platforms.push_back(BigPlatform((i-plat_length)*50, 500, plat_length, sprites.platform_sprite));
        }
        else{
            i++;
        }
    }

}

void SM_init_level(int level_length, std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps,
                Sprites &sprites, std::vector<int> &level_map){
    level_map.push_back(0);
    int i = 1;

    int plat_or_hole_len = 0;
    // true plat false hole - what a great comment - clearly im not phoning this one in
    bool plat_or_hole = true;
    bool bat_or_imp = true;
    while(i < level_length){
        if(plat_or_hole){
            plat_or_hole = false;
            plat_or_hole_len = 17;

            if(bat_or_imp){
                bat_or_imp = false;
                bats.push_back(BatsyEnemyClass(sprites.batsy_sprite, sprites.sonic_sprite, sprites.gold_sprite, (i+1)*50, 434, i, i + plat_or_hole_len - 1));
                level_map.push_back(2);
                i++;
            }else if(!bat_or_imp){
                bat_or_imp = true;
                imps.push_back(ImpEnemyClass(sprites.fireball_sprite, sprites.imp_sprite, sprites.gold_sprite, (i+1)*50, 440, i, i + plat_or_hole_len - 1));
                level_map.push_back(5);
                i++;
            }
            else{
                level_map.push_back(1);
                i++;
            }
            for(int k = 0; k < plat_or_hole_len-1; k++){
                level_map.push_back(1);
                i++;
            }

        }
        else{
            plat_or_hole = true;
            plat_or_hole_len = 2;

            for(int k = 0; k < plat_or_hole_len; k++){
                level_map.push_back(0);
                i++;
            }
        }

    }

    int plat_length;
    for(int i = 0; i < level_length;){
        if(level_map[i] == 1 || level_map[i] == 2 || level_map[i] == 5)
        {
            plat_length = 0;
            while(level_map[i] != 0){
                i++;
                plat_length++;
            }
            big_platforms.push_back(BigPlatform((i-plat_length)*50, 500, plat_length, sprites.platform_sprite));
        }
        else{
            i++;
        }
    }

}


int main_game_loop(std::vector<int> &gene_pattern, sf::RenderWindow &window,
                    PlayerClass &player, sf::View &view, int fitness, std::vector<BatsyEnemyClass> &bats,
                    std::vector<ImpEnemyClass> &imps,  std::vector<int> &level_map, int level_length,
                    sf::Thread &mana_thread, Sprites &sprites, EnemyClass &enemy, int &index_of_death){

while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Clear screen
#ifdef DRAW
        window.clear(Color(161, 242, 236));

        for(int i = 0; i < window_width/sprites.background_sprite.getGlobalBounds().width; i++){
                sprites.background_sprite.setPosition(i*sprites.background_sprite.getGlobalBounds().width + (player.sprite_.getPosition().x - window_width/2), player.sprite_.getPosition().y - window_height/2);
                window.draw(sprites.background_sprite);
                sprites.background_sprite_2.setPosition(i*sprites.background_sprite_2.getGlobalBounds().width + (player.sprite_.getPosition().x - window_width/2), player.sprite_.getPosition().y + window_height/2);
                window.draw(sprites.background_sprite_2);
            }

        level_makeup(window, sprites, player);

        //here we center the view on the player
        view.setCenter(Vector2f(player.sprite_.getPosition().x, player.sprite_.getPosition().y));
        window.setView(view);
#endif // DRAW

        //PLAYER

        if(game_mode == 1 && player.stuck_clock_.getElapsedTime().asMilliseconds() > player.stuck_time_){
            if(player.last_player_position_ + 5 >= player.position_ && player.last_player_position_ - 5 <= player.position_){
                //std::cout << "stuck mu je presudio" << std::endl;
                fitness += round(player.position_*1.0/level_length*1000);
                fitness += player.player_gold;
                fitness -= 500;
                death_by_stuck++;
                return fitness;
            }
            player.last_player_position_ = player.position_;
            player.stuck_clock_.restart();
        }

        if(player.construct_hp_ > 0){
            if(game_mode == 1){


            uniform_time = uniform_move_clock.getElapsedTime().asSeconds();
            state_handler(gene_pattern, level_map, player, window, mana_thread, bats, imps, uniform_time);
            uniform_move_clock.restart();

            }else if(game_mode == 2){
                uniform_time = uniform_move_clock.getElapsedTime().asSeconds();
                SM_state_handler(gene_pattern, level_map, player, window, mana_thread, bats, imps, uniform_time);
                uniform_move_clock.restart();
                if(SM_control_counter >= chromosome_size){
                    fitness += round(player.position_*1.0/level_length*1000);
                    fitness += player.player_gold;
                    index_of_death = 30;
                    std::cout << "in main loop: " << index_of_death << std::endl;
                    death_by_time_running_out++;
                    return fitness;
                }
            }

        }else if(player.construct_hp_ <= 0){

                //std::cout << "prirodna smrt" << std::endl;
                fitness += round(player.position_*1.0/level_length*1000);
                fitness += player.player_gold;
                fitness -= 500;
                index_of_death = SM_control_counter - 1 > 0 ? SM_control_counter - 1 : 0;
                std::cout << "in main loop: " << index_of_death << std::endl;
                death_by_enemies++;
                return fitness;

         }
        if(player.position_ > level_length+1){
            //std::cout << "pobeda" << std::endl;
            fitness += round(player.position_*1.0/level_length*1000);
            fitness += player.player_gold;
            fitness += 500;//POBEDA
            index_of_death = SM_control_counter;
            std::cout << "in main loop: " << index_of_death << std::endl;
            game_finished++;
            return fitness;
        }
        if(player.sprite_.getPosition().y > 600){
            //std::cout << "ko pimpuje zivo jaje dumre" << std::endl;
                fitness += round(player.position_*1.0/level_length*1000);
                fitness += player.player_gold;
                fitness -= 500;
                index_of_death = SM_control_counter - 1 > 0 ? SM_control_counter - 1 : 0;
                std::cout << "in main loop: " << index_of_death << std::endl;
                death_by_falling++;
                return fitness;
        }
#ifdef DRAW
        draw_player_hp_mp(window, player, sprites.hp_sprite);

        if(!player.RIP_construct_){
            window.draw(player.sprite_);
        }

        //draw platforms
        for(const auto& bp : big_platforms){
            for(const auto& plat : bp.platforms_){
                window.draw(plat.sprite_);
            }
        }
#endif // DRAW
        //draw bats
        for(BatsyEnemyClass& batsy : bats){
            if(!batsy.gold_collected_){

                if(player.position_ >= batsy.enemy_position_ - 1 && player.position_ <= batsy.platform_end_){
                    batsy.on_my_platform_ = true;
                }

                batsy.bat_handler_(window, enemy, player);

                if(!batsy.enemy_dead_){
                draw_batsy_hp(window, batsy, sprites.hp_sprite);
#ifdef DRAW
                window.draw(batsy.sprite_);
                window.draw(batsy.sonic_sprite_);
#endif // DRAW
                }else if(batsy.enemy_dead_){
                    level_map[batsy.enemy_position_] = 4;
                    if(player.position_ >= batsy.enemy_position_ - 1 && player.position_ <= batsy.platform_end_){
                        player.platform_state_ = 4;
                        player.shooting_ = false;
                    }
                }
            }else{
                level_map[batsy.enemy_position_] = 1;
                if(player.position_ >= batsy.enemy_position_ - 1 && player.position_ <= batsy.platform_end_){
                        player.platform_state_ = 1;
                    }

            }
        }

        //draw imps
        for(ImpEnemyClass& imp : imps){
            if(!imp.gold_collected_){

                if(player.position_ >= imp.enemy_position_ - 1 && player.position_ <= imp.platform_end_){
                    imp.on_my_platform_ = true;
                }

                if(!imp.attacking_ && imp.enemy_hp_ > 0){
                    imp.patrolling(imp, enemy.rectangles_imp_idle_, enemy.rectangles_imp_walk_left_,
                    enemy.rectangles_imp_walk_right_, enemy.rectangles_index_, enemy.rectangles_index_walk_);
                }
                imp.handle_imp(enemy, player, window);

                if(!imp.enemy_dead_){
#ifdef DRAW
                    draw_imp_hp(window, imp, sprites.hp_sprite);
                    window.draw(imp.sprite_);
#endif // DRAW
                }else if(imp.enemy_dead_){
                    level_map[imp.enemy_position_] = 4;
                    if(player.position_ >= imp.enemy_position_ - 1 && player.position_ <= imp.platform_end_){
                        player.platform_state_ = 4;
                        player.shooting_ = false;
                    }
                }
            }else{
                level_map[imp.enemy_position_] = 1;
                if(player.position_ >= imp.enemy_position_ - 1 && player.position_ <= imp.platform_end_){
                        player.platform_state_ = 1;
                    }
            }
        }
#ifdef DRAW
        window.display();
#endif // DRAW
    }
    return 0;
}





#endif // MAIN_LOOP_HPP
