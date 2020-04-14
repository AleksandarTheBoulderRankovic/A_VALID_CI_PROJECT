#ifndef SPRITES_HPP
#define SPRITES_HPP

class Sprites{
public:
    Sprites(Sprite background_sprite, Sprite background_sprite_2, Sprite hp_sprite, Sprite batsy_sprite,
            Sprite sonic_sprite, Sprite imp_sprite, Sprite fireball_sprite, Sprite gold_sprite,
            Sprite platform_sprite, Sprite portal_sprite, Text gold_numerator)
    : background_sprite(background_sprite),
      background_sprite_2(background_sprite_2),
      hp_sprite(hp_sprite),
      batsy_sprite(batsy_sprite),
      sonic_sprite(sonic_sprite),
      imp_sprite(imp_sprite),
      fireball_sprite(fireball_sprite),
      gold_sprite(gold_sprite),
      platform_sprite(platform_sprite),
      portal_sprite(portal_sprite),
      gold_numerator(gold_numerator)
      {}

    Sprite background_sprite;
    Sprite background_sprite_2;
    Sprite hp_sprite;
    Sprite batsy_sprite;
    Sprite sonic_sprite;
    Sprite imp_sprite;
    Sprite fireball_sprite;
    Sprite gold_sprite;
    Sprite platform_sprite;
    Sprite portal_sprite;
    Text gold_numerator;
};

#endif // SPRITES_HPP
