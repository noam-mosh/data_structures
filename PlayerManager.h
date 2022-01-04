#ifndef WET2_PLAYER_MANAGER_H
#define WET2_PLAYER_MANAGER_H
#include "UnionFind.h"
#include "RankAVLTree.h"
#include "HashTable.h"
#include "Player.h"
#include "Group.h"
#include"Scores.h"
#include "library2.h"
#define LEVEL int 

namespace data_structures {
    class PlayerManager
    {
    public:
        PlayerManager(int k, int scale);
        ~PlayerManager();
        StatusType MergeGroups(int GroupID1, int GroupID2);
        StatusType AddPlayer(int PlayerID, int GroupID, int score);
        StatusType RemovePlayer(int PlayerID);
        StatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
        StatusType ChangePlayerIDScore(int PlayerID, int NewScore);
        StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                            double * players);
        StatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
        /*StatusType GetPlayersBound(int GroupID, int score, int m,
                                         int * LowerBoundPlayers, int * HigherBoundPlayers);*/
        //utilitie function to calculate Level in bound in
        double PlayerManager::GetPlayersInLevelBound(RankAVLTree<LEVEL,LevelRank>* LevelTree ,int player_counter, int lowerLevel, int higherLevel );
        //utilitie function to calculate average on a tree of m bigger
        StatusType AverageHighestPlayerLevelByGroupHelp(RankAVLTree<LEVEL,LevelRank> levelTree,int m, double* avg);
        //utilitie function to change a level by one in our ranked tree
        void updateLevel(RankAVLTree<LEVEL,LevelRank>* LevelTree, int level, int change); 

    //private:
    int all_players_counter;
    int scale;
    int group_number;
    UnionFind groups;
    Group** group_array;
    RankAVLTree<LEVEL,LevelRank> levels;
    Scores* scores_array;
    HashTable<Player> players;
    };
    
}

#endif