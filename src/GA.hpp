#ifndef GA_HPP
#define GA_HPP

#include "Sprites.hpp"

#include <iostream>
#include <cstdlib>
#include <string>
#include <array>
#include <vector>
#include <time.h>
#include <algorithm>


extern int generation_size;
extern int chromosome_size;
extern int reproduction_size;
extern int max_iterations;
extern double mutation_rate;
extern int tournament_size;
extern std::string selection_type;
extern int crossover_type;
extern Clock SM_timer_clock;
extern Clock uniform_move_clock;
extern int SM_control_counter;
extern int elite_number;
extern double creation_influence;

extern int best_fitness;
extern int game_mode;
int index_of_death = 0;


namespace GA{


class chromosome{
public:
    chromosome(std::vector<int> code) : code_(code){
        index_of_death_ = 0;
    }

    std::vector<int> code_;
    int fitness_;
    int index_of_death_;
    };



// there was no other way for this to be resolved - this code is a stain upon humanity
int calculate_fitness(std::vector<int> &gene_pattern, sf::RenderWindow &window, PlayerClass &player,
                      sf::View &view, std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps,
                      std::vector<int> &level_map, int level_length, sf::Thread &mana_thread, Sprites &sprites,
                      EnemyClass &enemy){


    //fitness - lets count em!
    imps.clear();
    bats.clear();
    big_platforms.clear();
    level_map.clear();
    player.position_ = 3;
    player.last_player_position_ = 3;
    player.platform_state_ = 1;
    player.construct_move_ = 0;
    player.phase_counter_mv_ = 0;
    player.RIP_construct_ = false;
    player.shooting_ = false;
    player.construct_hp_ = 100.0;
    player.construct_mp_ = 100.0;
    player.laser_ = true;
    player.laser_setting_ = true;
    player.facing_left_ = false;
    player.on_ground_ = true;
    player.player_gold = 0;
    player.x_vel_ = 0;
    player.y_vel_ = 0;
    player.sprite_.setPosition(100, 400);
    player.shooting_sprite_.setPosition(player.sprite_.getPosition().x + 52 -(player.facing_left_)*60, player.sprite_.getPosition().y + 48);

    if(game_mode == 1)
        init_level(level_length, bats, imps, sprites, level_map);
    else if(game_mode ==2){
        SM_init_level(level_length, bats, imps, sprites, level_map);
    }
    int fitness = 0;
    player.stuck_clock_.restart();
    uniform_move_clock.restart();
    SM_control_counter = 0;
    SM_timer_clock.restart();
    // Start the game loop

    std::cout << "genetic_pattern: ";
    for(int num : gene_pattern){
        std::cout << num << ", ";
    }
    std::cout << 0 << std::endl;

    Clock fitness_clock;
    fitness = main_game_loop(gene_pattern, window, player, view, fitness, bats, imps,
                            level_map, level_length, mana_thread, sprites, enemy, index_of_death);

    //std::cout << "time it took fitness calculation to finish in seconds: " << fitness_clock.getElapsedTime().asSeconds() << std::endl;

    //std::cout << "fitness " << fitness << std::endl << std::endl;

    return fitness;
}

void simulated_annealing(chromosome &chrom, sf::RenderWindow &window, PlayerClass &player, sf::View &view,
                         std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps,
                         std::vector<int> &level_map, int level_length, sf::Thread &mana_thread,
                         Sprites &sprites, EnemyClass &enemy){


                    int max_iter = 5;
                    std::vector<int> new_code = chrom.code_;
                    double r;
                    int random_index = 0;
                    int new_fitness = 0;
                    double p = 0.5;


                    for(int i = 0; i < max_iter; i++){

                        random_index = std::rand() % 7;

                        do{
                            if(random_index == 0){
                                new_code[random_index] = std::rand()%6;
                            }
                            else if(random_index != 3 && random_index != 6){
                                new_code[random_index] = 4 + std::rand()%7;
                            }else if(random_index == 3){
                                new_code[random_index] = 11 + std::rand()%2;
                            }else if(random_index == 6){
                                new_code[random_index] = 12*(std::rand()%2);
                            }
                        }while(chrom.code_[random_index] == new_code[random_index]);

                        //chromosome new_chrom(new_code);
                        new_fitness = calculate_fitness(new_code, window, player, view, bats, imps, level_map,
                                                        level_length, mana_thread, sprites, enemy);

                        if(new_fitness > chrom.fitness_){
                            chrom.code_ = new_code;
                            chrom.fitness_ = new_fitness;
                            chrom.index_of_death_ = index_of_death;
                        }else{
                            p = 1.0/(pow((i+1), 0.5) + 1);
                            r = ((double) std::rand() /(RAND_MAX));
                            if(r < p){
                                chrom.code_ = new_code;
                                chrom.fitness_ = new_fitness;
                                chrom.index_of_death_ = index_of_death;
                            }else{
                                new_code = chrom.code_;
                            }
                        }
                    }
}

void initial_population(std::vector<GA::chromosome> &init_population, sf::RenderWindow &window,
                    PlayerClass &player, sf::View &view, std::vector<BatsyEnemyClass> &bats,
                    std::vector<ImpEnemyClass> &imps,  std::vector<int> &level_map, int level_length,
                    sf::Thread &mana_thread, Sprites &sprites, EnemyClass &enemy){

        for(int i = 0; i < generation_size; i++)
        {
            std::vector<int> genes{};
            int selected_value = 0;
            double r;

            for(int j = 0; j < chromosome_size; j++){
                if(game_mode == 1){
                    if(j == 0){
                        selected_value = std::rand()%6;
                    }
                    else if(j != 3 && j != 6){
                        selected_value = 4 + std::rand()%7;
                    }else if(j == 3){
                    selected_value = 11 + std::rand()%2;
                    }else if(j == 6){
                    selected_value = 12*(std::rand()%2);
                    }

                    genes.push_back(selected_value);
                }else if(game_mode == 2){
                    r = ((double) std::rand() /(RAND_MAX));
                    if(r < creation_influence){
                        genes.push_back(3);
                    }else{
                        genes.push_back(std::rand() % 8);
                    }
                }
            }

            init_population.push_back(GA::chromosome(genes));
            init_population[i].fitness_ = calculate_fitness(init_population[i].code_, window, player, view, bats, imps, level_map,
                                                            level_length, mana_thread, sprites, enemy);
            init_population[i].index_of_death_ = index_of_death;
        }

        return;
    }

void mutate(std::vector<int> &gene, bool bad_parents_ind, int death_index){

    if(game_mode == 1){
    int new_value = 0;
    double random_value = ((double) std::rand() /(RAND_MAX));

    if(random_value < mutation_rate)
    {
        int random_index = std::rand() % chromosome_size;

        while(1)
        {
            if(random_index == 0){
                new_value = std::rand()%6;
            }
            else if(random_index != 3  && random_index != 6){
                new_value = 4 + std::rand()%7;
            }else if(random_index == 3){
                    new_value = 11 + std::rand()%2;
            }else if(random_index == 6){
                    new_value = 12*(std::rand()%2);
            }

            if(gene[random_index] != new_value)
                break;
        }

        gene[random_index] = new_value;
    }
    if(bad_parents_ind){
        random_value = ((double) std::rand() /(RAND_MAX));
        if(random_value < 0.5)
            gene[0] =  std::rand()%6;
    }
    }else if(game_mode == 2){

        double r;
        int value;

        for(int i = death_index; i < chromosome_size; i++){
            r = ((double) std::rand() /(RAND_MAX));
            if(r < mutation_rate){

                double rand = ((double) std::rand() /(RAND_MAX));
                if(rand < creation_influence){
                        value = 3;
                }else{
                         value = std::rand()%8;
                }
                gene[i] = value;
            }

        }
    }

    return;
}

void crossover(std::vector<int> &child1_code, std::vector<int> &child2_code,
               std::vector<int> &parent1_code, std::vector<int> &parent2_code,
               int death_index_1, int death_index_2){

    if(crossover_type == 1){

        int break_point = 1 + std::rand() % (chromosome_size - 2);

        for(int i = 0; i < break_point; i++)
        {
            child1_code.push_back(parent1_code[i]);
            child2_code.push_back(parent2_code[i]);
        }

        for(int j = break_point; j < chromosome_size; j++)
        {
            child2_code.push_back(parent1_code[j]);
            child1_code.push_back(parent2_code[j]);
        }
    }else if(crossover_type == 2){
                int break_point_1 = 1 + std::rand() % (chromosome_size - 2);
                int break_point_2 = 1 + std::rand() % (chromosome_size - 2);
                while(break_point_2 == break_point_1){
                    break_point_2 = 1 + std::rand() % (chromosome_size - 2);
                }

                if(break_point_1 > break_point_2){
                    int tmp = break_point_1;
                    break_point_1 = break_point_2;
                    break_point_2 = tmp;
                }

                for(int i = 0; i < break_point_1; i++)
                {
                    child1_code.push_back(parent1_code[i]);
                    child2_code.push_back(parent2_code[i]);
                }

                for(int j = break_point_1; j < break_point_2; j++)
                {
                    child2_code.push_back(parent1_code[j]);
                    child1_code.push_back(parent2_code[j]);
                }
                for(int j = break_point_2; j < chromosome_size; j++)
                {
                    child1_code.push_back(parent1_code[j]);
                    child2_code.push_back(parent2_code[j]);
                }


    }else if(crossover_type == 3){
        for(int i = 0; i < chromosome_size; i++){
            if(((double) std::rand() /(RAND_MAX)) < 0.5){
                child1_code.push_back(parent1_code[i]);
            }else{
                child1_code.push_back(parent2_code[i]);
            }

            if(((double) std::rand() /(RAND_MAX)) < 0.5){
                child2_code.push_back(parent1_code[i]);
            }else{
                child2_code.push_back(parent2_code[i]);
            }
        }

    }else if(crossover_type == 4){

        for(int i = 0; i < death_index_1; i++)
        {
            child1_code.push_back(parent1_code[i]);
        }

        for(int j = death_index_1; j < chromosome_size; j++)
        {
            child1_code.push_back(parent2_code[j]);
        }

        for(int i = 0; i < death_index_2; i++)
        {
            child2_code.push_back(parent2_code[i]);
        }

        for(int j = death_index_2; j < chromosome_size; j++)
        {
            child2_code.push_back(parent1_code[j]);
        }
    }
}

void create_generation(std::vector<GA::chromosome> &population, std::vector<GA::chromosome> &generation,
                       sf::RenderWindow &window, PlayerClass &player, sf::View &view,
                       std::vector<BatsyEnemyClass> &bats, std::vector<ImpEnemyClass> &imps,
                       std::vector<int> &level_map, int level_length, sf::Thread &mana_thread, Sprites &sprites,
                       EnemyClass &enemy){

        for(int j = 0; j < elite_number; j++){
            generation.push_back(population[j]);
        }

        int size_of_generation = elite_number;

        while(size_of_generation < generation_size)
        {
            int rand1 = std::rand() % reproduction_size;
            int rand2 = std::rand() % reproduction_size;
            while(rand1 == rand2){
                rand2 = std::rand() % reproduction_size;
            }
            chromosome parent1 = population[rand1];
            chromosome parent2 = population[rand2];

            bool bad_parents_ind = false;
            if(parent1.fitness_ < 0 && parent2.fitness_ < 0){
                bad_parents_ind = true;
            }
            //Crossover

            std::vector<int> child1_code;
            std::vector<int> child2_code;

            crossover(child1_code, child2_code, parent1.code_, parent2.code_, parent1.index_of_death_, parent2.index_of_death_);

            mutate(child1_code, bad_parents_ind, parent1.index_of_death_);
            mutate(child2_code, bad_parents_ind, parent2.index_of_death_);

            chromosome child1(child1_code);
            child1.fitness_ = calculate_fitness(child1.code_, window, player, view, bats, imps, level_map,
                                                level_length, mana_thread, sprites, enemy);
            child1.index_of_death_ = index_of_death;

            chromosome child2(child2_code);
            child2.fitness_ = calculate_fitness(child2.code_, window, player, view, bats, imps, level_map,
                                                level_length, mana_thread, sprites, enemy);
            child2.index_of_death_ = index_of_death;
            generation.push_back(child1);
            generation.push_back(child2);

            size_of_generation += 2;
        }

        return;
    }
    chromosome roulette_selection(std::vector<chromosome> &population){

        int total_fitness = 0;
        int best = 0;

        for(int i = 0; i < generation_size; i++)
        {
            total_fitness += population[i].fitness_;
        }


        int selected_value = std::rand() % total_fitness;

        if(total_fitness < 0){
            selected_value = -selected_value;
        }

        int current_sum = 0;
        for(int i = 0; i < generation_size; i++)
        {
            current_sum += population[i].fitness_;

            if(total_fitness < 0 && current_sum < selected_value){
                best = i;
                break;
            }
            else if(total_fitness < 0 && current_sum > selected_value){
                best = i;
                break;
            }
        }

        return population[best];
    }

    chromosome tournament_selection(std::vector<chromosome> &population){

        std::vector<chromosome> selected_tournament;
        int curr_fitness = 0;
        int max_fitness = 0;
        int best = -1;

        for(int i = 0; i < tournament_size; i++)
        {
            selected_tournament.push_back(population[std::rand() % generation_size]);
        }

        for(int j = 0; j < tournament_size; j++)
        {
            curr_fitness = selected_tournament[j].fitness_;

            if(curr_fitness > max_fitness)
            {
                max_fitness = curr_fitness;
                best = j;
            }

        }
        if(best == -1){
            return selected_tournament[0];
        }
        return selected_tournament[best];
    }


    void selection(std::vector<chromosome> &population, std::vector<chromosome> &selected)
    {
        for(int i = elite_number; i < reproduction_size; i++){
            if(selection_type.compare("roulette") == 0){
                selected.push_back(roulette_selection(population));
            }
            else if(selection_type.compare("tournament") == 0){
                selected.push_back(tournament_selection(population));
            }
        }

        return;
    }



    chromosome max(std::vector<chromosome> &population)
    {
        int maximum = 0;
        int current = 0;
        int best = 0;

        for(int i = 0; i < generation_size; i++)
        {
            current = population[i].fitness_;
            if(current > maximum)
            {
                maximum = current;
                best = i;
            }
        }

        return population[best];
    }
};


#endif // GA_HPP
