#ifndef _BATSY_ENEMY_CLASS_HPP
#define _BATSY_ENEMY_CLASS_HPP
#include "RealEnemyClass.hpp"
#include "EnemyClass.hpp"

using namespace sf;

class BatsyEnemyClass : public RealEnemyClass{
public:
    BatsyEnemyClass(Sprite sprite, Sprite sonic_sprite, Sprite gold_sprite,
    double x_pos, double y_pos, int enemy_position, int platform_end)
    : RealEnemyClass(sprite,gold_sprite,x_pos,y_pos,enemy_position,platform_end), sonic_sprite_(sonic_sprite){

        sonic_sprite_.setPosition(x_pos_, y_pos_);
        sonic_damage_ = 5;
        first_hit_sonic_ = true;
        on_my_platform_ = false;
        phase_delta_ = 300;
     }

    //PATROLLING
    void bat_patrol_(EnemyClass &enemy){
        sonic_sprite_.setScale(4, 4);

        if(sprite_.getPosition().x < enemy_position_*50){
                move_phase_++;
        }

        if(move_phase_ == 1 && sprite_.getPosition().x > platform_end_*50){
                move_phase_++;
        }

        if(move_phase_ == 2){
            move_phase_ = 0;
        }

        else if(move_phase_ == 0){
            facing_left_ = true;
            sprite_.setPosition(Vector2f(sprite_.getPosition().x-0.6f, sprite_.getPosition().y));
            sprite_.setTextureRect(enemy.rectangles_batsy_fly_[5 + enemy.rectangles_index_batsy_fly_]);

            sonic_sprite_.setPosition(Vector2f(sprite_.getPosition().x-0.6f - 140, sprite_.getPosition().y - 68));
            sonic_sprite_.setTextureRect(enemy.rectangles_sonic_attack_[3 + enemy.rectangles_index_batsy_attack_]);

        }else if(move_phase_ == 1){
            facing_left_ = false;
            sprite_.setPosition(Vector2f(sprite_.getPosition().x+0.6f, sprite_.getPosition().y));
            sprite_.setTextureRect(enemy.rectangles_batsy_fly_[enemy.rectangles_index_batsy_fly_]);

            sonic_sprite_.setPosition(Vector2f(sprite_.getPosition().x+0.6f + 56, sprite_.getPosition().y - 68));
            sonic_sprite_.setTextureRect(enemy.rectangles_sonic_attack_[enemy.rectangles_index_batsy_attack_]);

        }
        }
    //check if the player damaged us
    void bat_handler_(sf::RenderWindow &window, EnemyClass &enemy, PlayerClass &player){

    if(!enemy_dead_ && enemy_hp_ > 0){

        if(player.position_ >= enemy_position_ - 1 && player.position_ <= platform_end_){
            if(player.sprite_.getPosition().x > sprite_.getPosition().x){
                player.platform_state_ = 3;
            }
            else
                player.platform_state_ = 2;

        }

            if(player.laser_ && on_my_platform_){

                if(first_hit_laser_ && player.laser_sprite_.getGlobalBounds().intersects(sprite_.getGlobalBounds())){
                        enemy_hp_ -= player.laser_damage_;
                        enemy.rectangles_index_batsy_death_ = 0;
                        first_hit_laser_ = false;
                }
            }
            if(player.shooting_ && on_my_platform_){
                if(player.rectangles_index_shooting_ == 1){
                        first_hit_shooting_ = true;
                }

                //check for collision with the batsy
                if(player.shooting_ && first_hit_shooting_ &&
                   player.shooting_sprite_.getGlobalBounds().intersects(sprite_.getGlobalBounds())){

                    enemy_hp_ -= player.shooting_damage_*!(player.rectangles_index_shooting_ == 1);
                    enemy.rectangles_index_batsy_death_ = 0;
                    first_hit_shooting_ = false;
                }
            }

            if(player.y_vel_ > 0 && player.first_hit_jumping_bat_ && !player.on_ground_ && player.sprite_.getGlobalBounds().intersects(sprite_.getGlobalBounds())){
                player.first_hit_jumping_bat_ = false;
                enemy_hp_ -= player.jump_damage_;
            }

            bat_patrol_(enemy);

            if(!first_hit_sonic_ && phase_clock_.getElapsedTime().asMilliseconds() > phase_delta_){
                first_hit_sonic_ = true;
            }


            if(first_hit_sonic_ && sonic_sprite_.getGlobalBounds().intersects(player.sprite_.getGlobalBounds())){
                    player.construct_hp_ -= sonic_damage_;
                    first_hit_sonic_ = false;
                    phase_clock_.restart();

            }

    }else if(!enemy_dead_ && enemy_hp_ <= 0){

        sprite_.setTextureRect(enemy.rectangles_batsy_death_[6*(facing_left_) + enemy.rectangles_index_batsy_death_]);

        if(enemy_hp_ <= 0){
                gold_collected_ = false;
                player.first_hit_gold = true;
        }

        if(enemy.rectangles_index_batsy_death_ == 5){
            enemy_dead_ = true;
        }
        }else if(enemy_dead_){
            gold_sprite_.setScale(3,3);
            gold_sprite_.setPosition(sprite_.getPosition().x, sprite_.getPosition().y +
                                     sprite_.getGlobalBounds().height - gold_sprite_.getGlobalBounds().height);

            if(!gold_collected_){
                window.draw(gold_sprite_);

            }

            if(player.first_hit_gold && player.sprite_.getGlobalBounds().intersects(gold_sprite_.getGlobalBounds()))
            {
                gold_collected_ = true;
                player.first_hit_gold = false;
                player.player_gold += 50;

            }
    }

    }

    int sonic_damage_;
    Sprite sonic_sprite_;
    bool first_hit_sonic_;
    bool on_my_platform_;

};
#endif // _BATSY_ENEMY_CLASS_HPP
