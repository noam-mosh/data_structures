#include"library2.h"
#include"PlayerManager.h"
#define MAX_SCALE 200
using namespace data_structures;

void *Init(int k, int scale){
    if(k<=0 || scale >MAX_SCALE || scale <=0){
        return NULL;
    }
    return new PlayerManager(k,scale);
}

StatusType MergeGroups(void *DS, int GroupID1, int GroupID2){
    if (!DS){
        return INVALID_INPUT;
    }
    return ((PlayerManager*)(DS))->MergeGroups(GroupID1,GroupID2);
}

StatusType AddPlayer(void *DS, int PlayerID, int GroupID, int score){
    if (!DS){
        return INVALID_INPUT;
    }
    return ((PlayerManager*)(DS))->AddPlayer(PlayerID,GroupID,score);
}

StatusType RemovePlayer(void *DS, int PlayerID){
    if (!DS){
        return INVALID_INPUT;
    }
    return ((PlayerManager*)(DS))->RemovePlayer(PlayerID);
}

StatusType IncreasePlayerIDLevel(void *DS, int PlayerID, int LevelIncrease){
    if (!DS){
        return INVALID_INPUT;
    }
    return ((PlayerManager*)(DS))->IncreasePlayerIDLevel(PlayerID,LevelIncrease);
}

StatusType ChangePlayerIDScore(void *DS, int PlayerID, int NewScore){
    if (!DS){
        return INVALID_INPUT;
    }
    return ((PlayerManager*)(DS))->ChangePlayerIDScore(PlayerID, NewScore);
}

StatusType GetPercentOfPlayersWithScoreInBounds(void *DS, int GroupID, int score, int lowerLevel, int higherLevel,
                                            double * players){
    if (!DS){
        return INVALID_INPUT;
    }
    return ((PlayerManager*)(DS))->GetPercentOfPlayersWithScoreInBounds(GroupID,score,lowerLevel,higherLevel,players);   
}

StatusType AverageHighestPlayerLevelByGroup(void *DS, int GroupID, int m, double * level){
    if (!DS){
        return INVALID_INPUT;
    }
    return ((PlayerManager*)(DS))->AverageHighestPlayerLevelByGroup(GroupID,m,level);
}

StatusType GetPlayersBound(void *DS, int GroupID, int score, int m,
                                         int * LowerBoundPlayers, int * HigherBoundPlayers){
     if (!DS){
        return INVALID_INPUT;
    }
    return ((PlayerManager*)(DS))->GetPlayersBound(GroupID,score,m,LowerBoundPlayers,HigherBoundPlayers);
}

void Quit(void** DS){
    if(!DS){
        return;
    }
    delete (PlayerManager*)(*DS);
    *DS=NULL;
}
