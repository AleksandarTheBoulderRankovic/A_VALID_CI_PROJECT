#ifndef _GOLD_CLASS_HPP
#define _GOLD_CLASS_HPP

class GoldClass{
public:
    GoldClass(Sprite gold_sprite) : gold_sprite_(gold_sprite)
    {}

    void drop_gold(double x_pos, double y_pos){
        gold_sprite_.setScale(4, 4);
        gold_sprite_.setPosition(x_pos, y_pos);
    }

    void draw_gold(sf::RenderWindow window){

        window.draw(gold_sprite_);
    }

Sprite gold_sprite_;

}
