#ifndef _PLAYER_MOVEMENT_
#define _PLAYER_MOVEMENT_
#include "BatsyEnemyClass.hpp"
#include "ImpEnemyClass.hpp"
#include "DrawTrails.hpp"

using namespace sf;
#define DRAW
extern std::vector<BigPlatform> big_platforms;
bool panic_indicator = false;

void jump(sf::RenderWindow &window, PlayerClass &player, float uniform_time){

    player.laser_ = true;
    player.laser_setting_ = true;

    int transition_parameter = 2*player.platform_transition_;
    if(player.platform_state_ == 1){
        transition_parameter = 2;
    }

    player.construct_move_ ^= 4;
    player.construct_move_ ^= transition_parameter;
    player.update(big_platforms, uniform_time);

    player.construct_move_ ^= 4;

#ifdef DRAW
    //jumping animation
    if(!player.on_ground_ && (player.construct_move_ & 1)){

        player.sprite_.setTextureRect(IntRect(433, 0, 9, 25));
        draw_left_trail(player, window);
    }
    else if(!player.on_ground_ && (player.construct_move_ & 2)){

        player.sprite_.setTextureRect(IntRect(483, 0, 9, 25));
        draw_right_trail(player, window);
    }
    else if(!player.on_ground_ && !((player.construct_move_ & 1) || (player.construct_move_ & 2))){

        player.sprite_.setTextureRect(IntRect(477, 50, 21, 25));
        draw_middle_trail(player, window);
    }

    player.construct_move_ ^= transition_parameter;
#endif // DRAW
}

void move_right(std::vector<int> &level_map, PlayerClass &player, float uniform_time){

    player.laser_ = true;
    player.laser_setting_ = true;

    player.facing_left_ = false;
    player.construct_move_ ^= 2;
    player.update(big_platforms, uniform_time);
    player.construct_move_ ^= 2;
    player.sprite_.setTextureRect(player.rectangles_move_right_[player.rectangles_index_move_]);
}

void move_left(std::vector<int> &level_map, PlayerClass &player, float uniform_time){

    player.laser_ = true;
    player.laser_setting_ = true;

    player.facing_left_ = true;
    player.construct_move_ ^= 1;
    player.update(big_platforms, uniform_time);
    player.construct_move_ ^= 1;
    player.sprite_.setTextureRect(player.rectangles_move_left_[player.rectangles_index_move_]);
}

void move_right_then_jump(sf::RenderWindow &window, std::vector<int> &level_map, PlayerClass &player, float uniform_time){

    if(level_map[player.position_] != 0){
        if(level_map[player.position_ - 1] == 0){
            player.platform_state_ = level_map[player.position_];
        }
        move_right(level_map, player, uniform_time);
    }
    else if(level_map[player.position_] == 0){

        jump(window, player, uniform_time);

    }
}

void move_left_then_jump(sf::RenderWindow &window, std::vector<int> &level_map, PlayerClass &player, float uniform_time){

    if(level_map[player.position_] == 1){

        if(level_map[player.position_ - 1] == 0){
          player.platform_state_ = level_map[player.position_];
        }

        move_left(level_map, player, uniform_time);
    }
    else{

        if(player.construct_move_ & 2){
             player.construct_move_ ^= 2;
        }if(player.construct_move_ & 1){
             player.construct_move_ ^= 1;
        }
        player.construct_move_ ^= 1;
        player.construct_move_ ^= 2;
        jump(window, player, uniform_time);
        player.construct_move_ ^= 2;
        player.construct_move_ ^= 1;
    }
}

void PANIC(sf::RenderWindow &window, std::vector<int> &level_map, PlayerClass &player, float uniform_time){

    if(player.on_ground_){
        if(level_map[player.position_] != 0 && !panic_indicator){
            move_right(level_map, player, uniform_time);
        }
        else if(level_map[player.position_] == 0 && !panic_indicator){
            panic_indicator = true;
            move_left(level_map, player, uniform_time);
        }
        else if(level_map[player.position_-1] != 0 && panic_indicator){
            move_left(level_map, player, uniform_time);
        }
        else if(level_map[player.position_-1] == 0 && panic_indicator){
            panic_indicator = false;
            move_right(level_map, player, uniform_time);
        }

    }else if(player.platform_transition_){
        jump(window, player, uniform_time);
    }else if(!player.on_ground_){
        player.update(big_platforms, uniform_time);
    }

}

void shoot_laser(sf::RenderWindow &window, sf::Thread &mana_thread, std::vector<BatsyEnemyClass> &bats,
                 std::vector<ImpEnemyClass> &imps, PlayerClass &player, float uniform_time){
        player.laser_ = true;
        if(player.rectangles_index_laser_ == 7){
                    player.laser_ = false;
                    player.laser_setting_ = true;
                }
        if(player.on_ground_){
            if(player.laser_){

            if(player.laser_setting_){
                    player.laser_setting_ = false;
            for(auto &batsy : bats){
                    batsy.first_hit_laser_ = true;
                }
            for(auto &imp : imps){
                    imp.first_hit_laser_ = true;
                }
            }
            if(player.construct_mp_ == 100){
                player.rectangles_index_laser_ = 0;
                player.construct_mp_ = 0;
                mana_thread.launch();
            }

            player.sprite_.setTextureRect(player.rectangles_laser_[9*(player.facing_left_) + 7 + player.rectangles_index_laser_ % 2]);
            player.laser_sprite_.setTextureRect(player.rectangles_laser_[9*(player.facing_left_) + player.rectangles_index_laser_]);
            player.laser_sprite_.setPosition(player.sprite_.getPosition().x + 56 - 685*(player.facing_left_), player.sprite_.getPosition().y + 36);
            player.laser_sprite_.setScale(2.5, 4);
    #ifdef DRAW
            window.draw(player.laser_sprite_);
    #endif // DRAW
        }
    }else{
        jump(window, player, uniform_time);
    }

}

void shoot_shoot(sf::RenderWindow &window, PlayerClass &player, float uniform_time){


    if(player.on_ground_){
        player.shooting_ = true;
        if(player.shooting_ && player.construct_hp_ > 0){
            //draw the cunstruct while shooting - specail case left or right
            player.sprite_.setTextureRect(player.rectangles_shooting_[4*(player.facing_left_) + player.rectangles_index_shooting_]);
            //draw and move the plasma bullet
            player.shooting_sprite_.move(pow(-1, player.facing_left_)*1500*uniform_time, 0);
            if(player.rectangles_index_shooting_ == 1){
                player.shooting_sprite_.setPosition(player.sprite_.getPosition().x + 52 -(player.facing_left_)*60, player.sprite_.getPosition().y + 48);
            }
            player.shooting_sprite_.setScale(4, 4);
    #ifdef DRAW
            window.draw(player.shooting_sprite_);
    #endif // DRAW
    }
    }else{
        player.shooting_ = false;
        jump(window, player, uniform_time);
    }
}

void shoot_laser_then_shoot_shoot(sf::RenderWindow &window, sf::Thread &mana_thread,
                                   std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps, PlayerClass &player, float uniform_time){
                                    if(player.laser_)
                                       shoot_laser(window, mana_thread, bats, imps, player, uniform_time);
                                    else
                                        shoot_shoot(window, player, uniform_time);
}

void jump_n_shoot(std::vector<int> &level_map, sf::RenderWindow &window, PlayerClass &player, float uniform_time){

    if(!player.platform_transition_){
        player.shooting_ = true;
        player.jump_time_ = 800;

        player.construct_move_ ^= 4;

        player.update(big_platforms, uniform_time);

        player.construct_move_ ^= 4;

        player.jump_time_ = 500;

        if(player.shooting_ && player.construct_hp_ > 0){
            //draw the cunstruct while shooting - specail case left or right
            player.sprite_.setTextureRect(player.rectangles_shooting_[4*(player.facing_left_) + player.rectangles_index_shooting_]);
            //draw and move the plasma bullet
            player.shooting_sprite_.move(pow(-1, player.facing_left_)*1500*uniform_time, 0);
            if(player.rectangles_index_shooting_ == 1){
                player.shooting_sprite_.setPosition(player.sprite_.getPosition().x + 52 -(player.facing_left_)*60, player.sprite_.getPosition().y + 48);
            }
            player.shooting_sprite_.setScale(4, 4);
    #ifdef DRAW
            window.draw(player.shooting_sprite_);
    #endif // DRAW
    }
    }else{
        player.shooting_ = false;
        jump(window, player, uniform_time);
    }
}

void jump_n_shoot_n_panic(sf::RenderWindow &window, std::vector<int> &level_map, PlayerClass &player, float uniform_time){


    if(!player.platform_transition_){
        player.shooting_ = true;

        if(player.phase_clock_mv_.getElapsedTime().asMilliseconds() > player.phase_delta_time_mv_
           && ((player.phase_counter_mv_ % 2 == 0 && player.on_ground_) || player.phase_counter_mv_ % 2 == 1)){
            player.phase_clock_mv_.restart();
            player.phase_counter_mv_++;
        }

        if(player.phase_counter_mv_ == 0)
        {
        player.jump_time_ = 800;

        player.construct_move_ ^= 4;

        player.update(big_platforms, uniform_time);

        player.construct_move_ ^= 4;

        player.jump_time_ = 500;

        }else if(player.phase_counter_mv_ == 1){
            shoot_shoot(window, player, uniform_time);
        }else if(player.phase_counter_mv_ == 2){
            player.shooting_ = false;
            player.phase_counter_mv_ = 0;
        }

    }else{
        player.shooting_ = false;
        jump(window, player, uniform_time);
    }

}

void collect_gold(std::vector<int> &level_map, PlayerClass &player, float uniform_time){

    if(player.on_ground_){
        if(player.facing_left_){
            if(level_map[player.position_] != 0){
                move_left(level_map, player, uniform_time);
            }
            else{
                player.facing_left_ = false;
            }
        }else{
            if(level_map[player.position_] != 0){
                move_right(level_map, player, uniform_time);
            }
            else{
                player.facing_left_ = true;
            }
        }
    }else{
        player.update(big_platforms, uniform_time);
    }

}
void SM_move_right(PlayerClass &player, float uniform_time){

    player.shooting_ = false;
    player.laser_ = true;
    player.laser_setting_ = true;

    player.facing_left_ = false;
    player.construct_move_ ^= 2;
    player.update(big_platforms, uniform_time);
    player.construct_move_ ^= 2;
    player.sprite_.setTextureRect(player.rectangles_move_right_[player.rectangles_index_move_]);
}

void SM_move_left(PlayerClass &player, float uniform_time){

    player.shooting_ = false;
    player.laser_ = true;
    player.laser_setting_ = true;

    player.facing_left_ = true;
    player.construct_move_ ^= 1;
    player.update(big_platforms, uniform_time);
    player.construct_move_ ^= 1;
    player.sprite_.setTextureRect(player.rectangles_move_left_[player.rectangles_index_move_]);
}
void SM_jump(sf::RenderWindow &window, PlayerClass &player, float uniform_time){

    player.shooting_ = false;
    player.laser_ = true;
    player.laser_setting_ = true;

    player.construct_move_ ^= 4;
    player.update(big_platforms, uniform_time);
    player.construct_move_ ^= 4;

#ifdef DRAW
    //jumping animation
    if(!player.on_ground_ && (player.construct_move_ & 1)){

        player.sprite_.setTextureRect(IntRect(433, 0, 9, 25));
        draw_left_trail(player, window);
    }
    else if(!player.on_ground_ && (player.construct_move_ & 2)){

        player.sprite_.setTextureRect(IntRect(483, 0, 9, 25));
        draw_right_trail(player, window);
    }
    else if(!player.on_ground_ && !((player.construct_move_ & 1) || (player.construct_move_ & 2))){

        player.sprite_.setTextureRect(IntRect(477, 50, 21, 25));
        draw_middle_trail(player, window);
    }
#endif // DRAW
}
void SM_jump_right(sf::RenderWindow &window, PlayerClass &player, float uniform_time){

    player.shooting_ = false;
    player.construct_move_ ^= 2;
    SM_jump(window, player, uniform_time);
    player.construct_move_ ^= 2;
}

void SM_jump_left(sf::RenderWindow &window, PlayerClass &player, float uniform_time){

    player.shooting_ = false;
    player.construct_move_ ^= 1;
    SM_jump(window, player, uniform_time);
    player.construct_move_ ^= 1;
}


void SM_shoot_laser(sf::RenderWindow &window, sf::Thread &mana_thread, std::vector<BatsyEnemyClass> &bats,
                 std::vector<ImpEnemyClass> &imps, PlayerClass &player, float uniform_time){

    player.shooting_ = false;
    if(player.on_ground_){
            if(player.laser_){

            if(player.laser_setting_){
                    player.laser_setting_ = false;
            for(auto &batsy : bats){
                    batsy.first_hit_laser_ = true;
                }
            for(auto &imp : imps){
                    imp.first_hit_laser_ = true;
                }
            }
            if(player.construct_mp_ == 100){
                player.rectangles_index_laser_ = 0;
                player.construct_mp_ = 0;
                mana_thread.launch();
            }
            player.sprite_.setTextureRect(player.rectangles_laser_[9*(player.facing_left_) + 7 + player.rectangles_index_laser_ % 2]);
            player.laser_sprite_.setTextureRect(player.rectangles_laser_[9*(player.facing_left_) + player.rectangles_index_laser_]);
            player.laser_sprite_.setPosition(player.sprite_.getPosition().x + 56 - 685*(player.facing_left_), player.sprite_.getPosition().y + 36);
            player.laser_sprite_.setScale(2.5, 4);
    #ifdef DRAW
            window.draw(player.laser_sprite_);
    #endif // DRAW
        }
    }else{
        SM_jump(window, player, uniform_time);
    }
}

void SM_shoot_shoot(sf::RenderWindow &window, PlayerClass &player, float uniform_time){
    if(player.on_ground_){
        player.shooting_ = true;
        if(player.shooting_ && player.construct_hp_ > 0){
            //draw the cunstruct while shooting - specail case left or right
            player.sprite_.setTextureRect(player.rectangles_shooting_[4*(player.facing_left_) + player.rectangles_index_shooting_]);
            //draw and move the plasma bullet
            player.shooting_sprite_.move(pow(-1, player.facing_left_)*1500*uniform_time, 0);
            if(player.rectangles_index_shooting_ == 1){
                player.shooting_sprite_.setPosition(player.sprite_.getPosition().x + 52 -(player.facing_left_)*60, player.sprite_.getPosition().y + 48);
            }
            player.shooting_sprite_.setScale(4, 4);
    #ifdef DRAW
            window.draw(player.shooting_sprite_);
    #endif // DRAW
        }
    }else{
        SM_jump(window, player, uniform_time);
    }

}

void SM_jump_shoot(sf::RenderWindow &window, PlayerClass &player, float uniform_time){

    player.construct_move_ ^= 4;
    player.update(big_platforms, uniform_time);
    player.construct_move_ ^= 4;

    player.shooting_ = true;
    if(player.shooting_ && player.construct_hp_ > 0){
        //draw the cunstruct while shooting - specail case left or right
        player.sprite_.setTextureRect(player.rectangles_shooting_[4*(player.facing_left_) + player.rectangles_index_shooting_]);
        //draw and move the plasma bullet
        player.shooting_sprite_.move(pow(-1, player.facing_left_)*1500*uniform_time, 0);
        if(player.rectangles_index_shooting_ == 1){
            player.shooting_sprite_.setPosition(player.sprite_.getPosition().x + 52 -(player.facing_left_)*60, player.sprite_.getPosition().y + 48);
        }
        player.shooting_sprite_.setScale(4, 4);
#ifdef DRAW
        window.draw(player.shooting_sprite_);
#endif // DRAW
    }

}




#endif // _PLAYER_MOVEMENT_
