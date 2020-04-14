#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "src/BigPlatform.hpp"
#include "src/GameObject.hpp"
#include "src/PlatformClass.hpp"
#include "src/PlayerClass.hpp"
#include "src/PlayerMovement.hpp"
#include "src/BatsyEnemyClass.hpp"
#include "src/RealEnemyClass.hpp"
#include "src/EnemyClass.hpp"
#include "src/Bars.hpp"
#include "src/ImpEnemyClass.hpp"
#include "src/MianLoop.hpp"
#include <unistd.h>
#include "src/GA.hpp"
#include "src/Sprites.hpp"

//#define DRAW
using namespace sf;

int game_mode = 2;
int generation_size = 50;
//int chromosome_size = 7;
int chromosome_size = 45;
int reproduction_size = 50;
int max_iterations = 15;
double mutation_rate = 0.2;
int tournament_size = 5;
int crossover_type = 4;
std::string selection_type = "roulette";
//std::string selection_type = "tournament";
int best_fitness = 0;
int elite_number = 1;
double creation_influence = 0.4;
int death_by_enemies = 0;
int death_by_stuck = 0;
int death_by_falling = 0;
int game_finished = 0;
int death_by_time_running_out = 0;

double window_height = 600;
double window_width = 1200;
std::vector<BigPlatform> big_platforms;

bool compareChromosomes(GA::chromosome &c1, GA::chromosome &c2){
    return c1.fitness_ > c2.fitness_;
}

int main(){

    // Create the main window
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "SFML window");

    Clock uniform_move_clock;
    //Clock situation_clock;

    // define a view
    sf::View view(sf::FloatRect(0.0, 0.0, window_width, window_height));

    srand(time(NULL));

    Texture construct_tex;
    construct_tex.loadFromFile("assets/images/construct_sprite_sheet.png");
    Sprite construct_sprite(construct_tex, IntRect(131, 50, 11, 25));

    // plasma sprite - for jumping
    Sprite plasma_booster_sprite(construct_tex);

    // laser sprite - for disintegrating
    Sprite laser_sprite(construct_tex);

    // construct shooting parameters
    Sprite shooting_sprite(construct_tex, IntRect(26, 152, 4, 4));

    // making a player object

    // health and mana bars
    Texture hp_tex;
    hp_tex.loadFromFile("assets/images/hp_mp_bars.png");
    Sprite hp_sprite(hp_tex);

    // background element
    Texture background;
    background.loadFromFile("assets/images/sky.png");

    Texture background_2;
    background_2.loadFromFile("assets/images/skyReverse.png");

    Sprite background_sprite(background);
    background_sprite.setScale(1, window_height/background_sprite.getLocalBounds().height);

    Sprite background_sprite_2(background_2);
    background_sprite_2.setScale(1, window_height/background_sprite_2.getLocalBounds().height);

    // enemy initialization
    Texture gold;
    gold.loadFromFile("assets/images/money.png");

    Sprite gold_sprite(gold, IntRect(75,125,25,25));

    // Batsy initialization
    Texture batsy_tex;
    batsy_tex.loadFromFile("assets/images/batsy.png");

    Sprite batsy_sprite(batsy_tex, IntRect(1, 27, 13, 11));

    Sprite sonic_sprite(batsy_tex, IntRect(32, 73, 26, 24));

    // imp initialization
    Texture imp_tex;
    imp_tex.loadFromFile("assets/images/imp.png");

    Sprite imp_sprite(imp_tex, IntRect(23, 377, 1, 1));
    Sprite fireball_sprite(imp_tex, IntRect(10, 211, 18, 15));

    EnemyClass enemy{};
    // these threads do all the animation calculations
    sf::Thread enemy_thread(&EnemyClass::Animation, &enemy);
    // start the thread
    enemy_thread.launch();

    // platform initialization
    Texture platform_tex;
    platform_tex.loadFromFile("assets/images/tileset.png");
    Sprite platform_sprite(platform_tex);

    Texture portal_tex;
    portal_tex.loadFromFile("assets/images/Portal.png");
    Sprite portal_sprite(portal_tex, IntRect(26, 79, 13, 41));

    Font arial;
    arial.loadFromFile("assets/fonts/arial.ttf");

    Text gold_numerator("0", arial, 50);


    Sprites sprites(background_sprite, background_sprite_2, hp_sprite, batsy_sprite, sonic_sprite, imp_sprite,
                    fireball_sprite, gold_sprite, platform_sprite, portal_sprite, gold_numerator);

    // level initialization
    int level_length = 100;

    PlayerClass player(laser_sprite, shooting_sprite, plasma_booster_sprite, construct_sprite, 100, 400);

    sf::Thread cunstruct_thread(&PlayerClass::Animation, &player);
    cunstruct_thread.launch();

    sf::Thread mana_thread(&PlayerClass::mana_recovery, &player);


    std::vector<ImpEnemyClass> imps;
    std::vector<BatsyEnemyClass> bats;
    std::vector<int> level_map;
// here we test our best chromosomes, it has no value in the grand scheme of things
//    std::vector<int> test_gene{0, 3, 3, 5, 3, 7, 1, 3, 3, 7, 3, 2, 6, 3, 3, 3, 5, 3, 2, 3, 4, 3, 4, 2, 3, 3, 4, 5, 0, 7, 3, 3, 0, 3, 7, 3, 3, 4, 1, 1, 4, 3, 3, 6, 1};
//
//    GA::chromosome test_chrom(test_gene);
//    int fin = GA::calculate_fitness(test_gene, window, player, view, bats, imps,
//                            level_map, level_length, mana_thread, sprites, enemy);
//
//    std::cout << fin << std::endl;
    std::cout << "generation size: " << generation_size << std::endl;
    std::cout << "chromosome size: " << chromosome_size << std::endl;
    std::cout << "reproduction size: " << reproduction_size << std::endl;
    std::cout << "max iterations: " << max_iterations << std::endl;
    std::cout << "mutation rate: " << mutation_rate << std::endl;
    std::cout << "tournament size: " << tournament_size << std::endl;
    std::cout << "selection type: " << selection_type << std::endl;
    std::cout << "crossover type: " << crossover_type << std::endl;

    Clock GA_clock;

    std::vector<GA::chromosome> population;
    std::vector<GA::chromosome> selected = {};


    GA::initial_population(population, window, player, view, bats, imps,
                            level_map, level_length, mana_thread, sprites, enemy);
    int counter = 0;
    //GA::chromosome global_best();

    for(int i = 0; i < max_iterations; i++)
    {

        std::cout << std::endl << "Pocetak iteracije: " << i+1 << std::endl << std::endl;

        selected.clear();

        std::sort(population.begin(), population.end(), compareChromosomes);

        for(int j = 0; j < elite_number; j++){
            selected.push_back(population[j]);
        }

        GA::selection(population, selected);

        population.clear();

        GA::create_generation(selected, population, window, player, view, bats, imps,
                            level_map, level_length, mana_thread, sprites, enemy);

        GA::chromosome global_best = max(population);
//        GA::simulated_annealing(global_best, window, player, view, bats, imps,
//                           level_map, level_length, mana_thread, sprites, enemy);

        population[0] = global_best;

            std::cout << "global best: ";
            for(int num : global_best.code_){
                std::cout << num << ", ";
            }
            std::cout << 0 << std::endl;

        int global_best_fitness = global_best.fitness_;

        std::cout << "best fitness: " << global_best_fitness << std::endl;

        if(global_best_fitness > best_fitness)
        {
            best_fitness = global_best_fitness;
            counter = 0;
            std::cout << i+1 << ". iteracija, fitness: " << best_fitness << std::endl;

        }
        else
        {
            counter += 1;
            std::cout << i+1 << ". iteracija, fitness se nije popravio" << std::endl;
        }
    }

    std::cout << "time it took the GA to finish in seconds: " << GA_clock.getElapsedTime().asSeconds() << std::endl;
    std::cout << "death by enemies: " << death_by_enemies << " stats: " << death_by_enemies*1.0/(generation_size + (generation_size-elite_number)*max_iterations) << std::endl;
    std::cout << "death by falling: " << death_by_falling << " stats: " << death_by_falling*1.0/(generation_size + (generation_size-elite_number)*max_iterations) << std::endl;
    if(game_mode == 1)
        std::cout << "death by stuck: " << death_by_stuck << " stats: " << death_by_stuck*1.0/(generation_size + (generation_size-elite_number)*max_iterations) << std::endl;
    if(game_mode == 2)
        std::cout << "death by time running out: " << death_by_time_running_out << " stats: " << death_by_time_running_out*1.0/(generation_size + (generation_size-elite_number)*max_iterations) << std::endl;
    std::cout << "victory: " << game_finished << " stats: " << game_finished*1.0/(generation_size + (generation_size-elite_number)*max_iterations) << std::endl;

    window.close();
return 0;

}
