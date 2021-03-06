#ifndef BARS_HPP
#define BARS_HPP
#include "BatsyEnemyClass.hpp"


extern double window_height;
extern double window_width;

void draw_player_hp_mp(RenderWindow &window, PlayerClass &player, Sprite hp_sprite){

        //we draw the hp bar propartionally to the amount of hp left
        if(player.construct_hp_ > 0){
            hp_sprite.setTextureRect(IntRect(8, 14, 64*player.construct_hp_/100.0, 9));
            hp_sprite.setScale(2, 2);
            hp_sprite.setPosition(player.sprite_.getPosition().x - window_width/2 + 16, player.sprite_.getPosition().y - 0.495*window_height);
            window.draw(hp_sprite);
        }

        hp_sprite.setTextureRect(IntRect(8, 0, 64*player.construct_mp_/100.0, 9));
        hp_sprite.setScale(2, 2);
        hp_sprite.setPosition(player.sprite_.getPosition().x - window_width/2 + 16, player.sprite_.getPosition().y - 0.455*window_height);
        window.draw(hp_sprite);

        hp_sprite.setTextureRect(IntRect(0, 42, 80, 9));
        hp_sprite.setScale(2, 2);
        hp_sprite.setPosition(player.sprite_.getPosition().x - window_width/2, player.sprite_.getPosition().y - 0.495*window_height);
        window.draw(hp_sprite);

        hp_sprite.setTextureRect(IntRect(0, 54, 80, 9));
        hp_sprite.setScale(2, 2);
        hp_sprite.setPosition(player.sprite_.getPosition().x - window_width/2, player.sprite_.getPosition().y - 0.455*window_height);
        window.draw(hp_sprite);
}

void draw_imp_hp(RenderWindow &window, RealEnemyClass &enemy, Sprite hp_sprite){

        if(enemy.enemy_hp_ > 0){
            hp_sprite.setTextureRect(IntRect(0, 65, 1 + 110*enemy.enemy_hp_/100.0, 8));
            hp_sprite.setScale(0.6, 1);
            hp_sprite.setPosition(enemy.sprite_.getPosition().x, enemy.sprite_.getPosition().y - 20);
            window.draw(hp_sprite);
        }
        hp_sprite.setTextureRect(IntRect(0, 81, 112, 8));
        hp_sprite.setScale(0.6, 1);
        hp_sprite.setPosition(enemy.sprite_.getPosition().x, enemy.sprite_.getPosition().y - 20);
        window.draw(hp_sprite);

}

void draw_batsy_hp(RenderWindow &window, BatsyEnemyClass &batsy, Sprite hp_sprite){

        if(batsy.enemy_hp_ > 0){
            hp_sprite.setTextureRect(IntRect(8, 29, 64*batsy.enemy_hp_/100.0, 9));
            hp_sprite.setScale(1, 1);
            hp_sprite.setPosition(batsy.sprite_.getPosition().x - 2, batsy.sprite_.getPosition().y - 10);
			window.draw(hp_sprite);
		}
			hp_sprite.setTextureRect(IntRect(0, 42, 80, 9));
			hp_sprite.setScale(1, 1);
			hp_sprite.setPosition(batsy.sprite_.getPosition().x - 10, batsy.sprite_.getPosition().y - 11);
			window.draw(hp_sprite);
}

#endif // BARS_HPP
