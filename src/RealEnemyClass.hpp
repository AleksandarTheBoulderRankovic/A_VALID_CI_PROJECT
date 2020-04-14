#ifndef _REAL_ENEMY_CLASS_HPP
#define _REAL_ENEMY_CLASS_HPP

#include "GameObject.hpp"

using namespace sf;

//Real Enemy class where we store all the relevant information on enemies
class RealEnemyClass : public GameObject{
public:
    RealEnemyClass(Sprite sprite, Sprite gold_sprite,
    double x_pos, double y_pos, int enemy_position, int platform_end)
    : GameObject(x_pos, y_pos), sprite_(sprite), gold_sprite_(gold_sprite),
      enemy_position_(enemy_position), platform_end_(platform_end){

        sprite_.setPosition((platform_end-3)*50, y_pos_);
        scale_ = 4;
        sprite_.setScale(scale_, scale_);

        gold_collected_ = false;

        enemy_hp_ = 100;
        enemy_dead_ = false;
        facing_left_ = true;
        attacking_ = false;

        //we use these to patrol the enemy
        phase_clock_.restart();
        move_phase_ = 0;
        phase_delta_ = 3000;

        //when two sprites intersect that connection lasts - so one hit can deal -80 damage witch we dont want
        first_hit_laser_ = true;
        first_hit_shooting_ = true;


     }

    static void patrolling(RealEnemyClass &deviant,
               std::vector<IntRect> &rectangles_deviant_idle_,
               std::vector<IntRect> &rectangles_deviant_walk_left_,
               std::vector<IntRect> &rectangles_deviant_walk_right_,
               int rectangles_index_deviant_idle_,
               int rectangles_index_deviant_walk_
               ){

    if(deviant.phase_clock_.getElapsedTime().asMilliseconds() > deviant.phase_delta_){
                deviant.phase_clock_.restart();
                deviant.move_phase_++;
    }

    if(deviant.move_phase_ == 4){
        deviant.move_phase_ = 0;
    }

    //marching left then right - phase 0 and 2 are idle - phase 1 walk left - phase 2 walk right
    if(deviant.move_phase_ == 0 || deviant.move_phase_ == 2){

        deviant.sprite_.setTextureRect(rectangles_deviant_idle_[rectangles_index_deviant_idle_]);

        }
    else if(deviant.move_phase_ == 1){

        deviant.facing_left_ = true;
        deviant.sprite_.setPosition(Vector2f(deviant.sprite_.getPosition().x-0.25f, deviant.sprite_.getPosition().y));
        deviant.sprite_.setTextureRect(rectangles_deviant_walk_left_[rectangles_index_deviant_walk_]);

        if(deviant.sprite_.getPosition().x < (deviant.enemy_position_+4)*50)
            deviant.sprite_.setPosition(Vector2f((deviant.enemy_position_+4)*50, deviant.sprite_.getPosition().y));

    }else if(deviant.move_phase_ == 3){

        deviant.facing_left_ = false;
        deviant.sprite_.setPosition(Vector2f(deviant.sprite_.getPosition().x+0.25f, deviant.sprite_.getPosition().y));
        deviant.sprite_.setTextureRect(rectangles_deviant_walk_right_[rectangles_index_deviant_walk_]);

        if(deviant.sprite_.getPosition().x + deviant.sprite_.getGlobalBounds().width > (deviant.platform_end_-3)*50)
            deviant.sprite_.setPosition(Vector2f((deviant.platform_end_-3)*50, deviant.sprite_.getPosition().y));

    }
}

    Sprite sprite_;
    Sprite gold_sprite_;
    bool gold_collected_;
    double scale_;
    double x_vel_;
    double y_vel_;
    int enemy_position_;
    int platform_end_;

    double enemy_hp_;
    bool facing_left_;
    bool attacking_;
    Clock phase_clock_;
    int move_phase_;
    int phase_delta_;
    bool first_hit_laser_;
    bool first_hit_shooting_;
    bool enemy_dead_;

};

#endif // _REAL_ENEMY_CLASS_HPP

