#ifndef _IMP_ENEMY_CLASS_HPP
#define _IMP_ENEMY_CLASS_HPP

#include "RealEnemyClass.hpp"

class ImpEnemyClass : public RealEnemyClass{
public:
    ImpEnemyClass(Sprite fireball_sprite, Sprite sprite, Sprite gold_sprite,
                   double x_pos, double y_pos, int enemy_position, int platform_end)
     : RealEnemyClass(sprite, gold_sprite, x_pos, y_pos, enemy_position, platform_end),
      fireball_sprite_(fireball_sprite){

        scale_ = 4;
        fireball_sprite_.setScale(scale_, scale_);
        on_my_platform_ = false;

        first_hit_fireball_ = true;
        fireball_damage_ = 5;
     }

void handle_imp(EnemyClass &enemy, PlayerClass &player, sf::RenderWindow &window){


    if(!enemy_dead_  && enemy_hp_ > 0){

        if(player.sprite_.getPosition().x > sprite_.getPosition().x - 150
        && player.sprite_.getPosition().x < sprite_.getPosition().x + 150){
            player.platform_state_ = 7;
        }else if(player.position_ >= enemy_position_ - 1 && player.position_ <= platform_end_){
            if(player.sprite_.getPosition().x > sprite_.getPosition().x){
                player.platform_state_ = 6;
            }
            else
                player.platform_state_ = 5;

        }

        //laser collision
        if(player.laser_ && on_my_platform_){

                if(first_hit_laser_ && player.laser_sprite_.getGlobalBounds().intersects(sprite_.getGlobalBounds())){
                        //if we hit the imp reduce his hp
                        enemy_hp_ -= player.laser_damage_;
                        enemy.rectangles_index_death_ = 0;
                        first_hit_laser_ = false;
                }
        }
        if(player.shooting_ && on_my_platform_){
            if(player.rectangles_index_shooting_ == 1){
                    first_hit_shooting_ = true;
            }

            //check for collision with the imp
            if(enemy_hp_ > 0 && first_hit_shooting_ && player.shooting_sprite_.getGlobalBounds().intersects(sprite_.getGlobalBounds())){
                    enemy_hp_ -= player.shooting_damage_*!(player.rectangles_index_shooting_ == 1);
                    enemy.rectangles_index_death_ = 0;
                    first_hit_shooting_ = false;
            }
        }
        if(player.y_vel_ > 0 && player.first_hit_jumping_imp_ && !player.on_ground_ && player.sprite_.getGlobalBounds().intersects(sprite_.getGlobalBounds())){
                player.first_hit_jumping_imp_ = false;
                enemy_hp_ -= player.jump_damage_;
        }


        imp_attack(enemy, player, window);

        //fireball collision - maybe optimize this at a later date
        if(enemy.rectangles_index_attack_ == 0)
            first_hit_fireball_ = true;

        if(enemy_hp_ <= 0){
            first_hit_fireball_ = false;
            attacking_ = false;

        }

        if(first_hit_fireball_ && fireball_sprite_.getGlobalBounds().intersects(player.sprite_.getGlobalBounds())){
                    first_hit_fireball_ = false;
                    player.construct_hp_ -= fireball_damage_*!(enemy.rectangles_index_attack_ == 0 && attacking_);
        }


        }else if(!enemy_dead_ && enemy_hp_ <= 0){
            sprite_.setTextureRect(enemy.rectangles_imp_death_[5*(facing_left_) + enemy.rectangles_index_death_]);

            gold_collected_ = false;
            player.first_hit_gold = true;

            if(enemy.rectangles_index_death_ == 5){
                enemy_dead_ = true;

            }
        }
        else if(enemy_dead_){

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
                player.player_gold += 100;

            }
        }
}
void imp_attack(EnemyClass &enemy, PlayerClass &player, sf::RenderWindow &window){
    //attack phase checker - if the construct is near enough we switch to attack phase
    if(enemy_hp_ > 0
        && player.sprite_.getPosition().x > sprite_.getPosition().x - 400
        && player.sprite_.getPosition().x < sprite_.getPosition().x + 400
        && player.sprite_.getPosition().y < sprite_.getPosition().y + sprite_.getGlobalBounds().height
        && player.sprite_.getPosition().y > sprite_.getPosition().y - 200){

        // facing left can only be updated on te first frame of the animation
        if(enemy.rectangles_index_attack_ == 0)
            facing_left_ = player.sprite_.getPosition().x < sprite_.getPosition().x;
        attacking_ = true;
        phase_clock_.restart();

    }else{
        attacking_ = false;
    }

    if(attacking_){
        if(facing_left_){
            if(enemy.rectangles_index_attack_ < 2)
                fireball_sprite_.setPosition(sprite_.getPosition().x + 48, sprite_.getPosition().y + 20);
            else if(enemy.rectangles_index_attack_ == 2)
                fireball_sprite_.setPosition(sprite_.getPosition().x + -28, sprite_.getPosition().y + 20);
            else
                fireball_sprite_.move(Vector2f(-4, 0.0));


            sprite_.setTextureRect(enemy.rectangles_imp_attack_left_[enemy.rectangles_index_attack_]);
            fireball_sprite_.setTextureRect(enemy.rectangles_imp_fireBall_left_[enemy.rectangles_index_attack_]);
        }else{
            if(enemy.rectangles_index_attack_ < 2)
                fireball_sprite_.setPosition(sprite_.getPosition().x, sprite_.getPosition().y + 20);
            else if(enemy.rectangles_index_attack_ == 2)
                fireball_sprite_.setPosition(sprite_.getPosition().x + 28, sprite_.getPosition().y + 20);
            else
                fireball_sprite_.move(Vector2f(4, 0.0));


            sprite_.setTextureRect(enemy.rectangles_imp_attack_right_[enemy.rectangles_index_attack_]);
            fireball_sprite_.setTextureRect(enemy.rectangles_imp_fireBall_right_[enemy.rectangles_index_attack_]);
            }
        window.draw(fireball_sprite_);
    }
}



    Sprite fireball_sprite_;
    double scale_;
    int fireball_damage_;
    bool first_hit_fireball_;
    bool on_my_platform_;

};
#endif // _IMP_ENEMY_CLASS_HPP

