#ifndef WET2_SCORES_H
#define WET2_SCORES_H
#define NO_PLAYERS 0
#include "RankAVLTree.h"
#include "LevelRank.h"
#define LEVEL int 

namespace data_structures {
class Scores{
    public:
    Scores(): players_counter(NO_PLAYERS),tree_levels(new RankAVLTree<LEVEL,LevelRank>()){}
    ~Scores(){
        delete tree_levels;
    }
    Scores(const Scores& other)=default;
    Scores& operator=(const Scores& other) = default;
    
    //private:
    int players_counter;
    RankAVLTree<LEVEL,LevelRank>* tree_levels;
};
}

#endif //WET2_SCORES_H