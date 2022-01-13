#include "PlayerManager.h"
#define NO_PLAYERS 0
using namespace data_structures;
#define MIN_PLAYER_ID 0
#define MIN_GROUP_ID 0
#define MIN_SCORE 0


PlayerManager::PlayerManager(int k,int scale):all_players_counter(NO_PLAYERS),scale(scale),group_number(k),groups(k),group_array(new Group* [k]),
    levels(new RankAVLTree<LEVEL,LevelRank>()),scores_array(new Scores[scale]){
        /*UnionFind group(k);
        groups= group;*/
        for(int i=1; i<=k;i++){
        groups.MakeSet(i);
        }
        for (int i = 0; i < k; i++)
        {
            group_array[i]=nullptr;
        }
        
    }

PlayerManager::~PlayerManager(){
    for(int i=0; i<group_number; i++){
        if(group_array[i]!=nullptr){
            delete group_array[i];
        }
    }
    delete [] group_array;
    delete [] scores_array;
    delete levels;
}

StatusType PlayerManager::MergeGroups(int GroupID1, int GroupID2){
    if(GroupID2<=0 || GroupID1<=0 || GroupID2>group_number || GroupID1>group_number){
        return INVALID_INPUT;
    }
    int group1 = groups.Find(GroupID1);
    int group2 = groups.Find(GroupID2);
    if (group1 == -1 || group2 == -1)
        return INVALID_INPUT;
    if(group1==group2){//already in the same group
        return SUCCESS;
    }
    int groupToMergeTo = groups.Union(group1,group2);
    int groupToMerge=0;
    if (groupToMergeTo==group1){
        groupToMerge=group2;
     }
    else{
         groupToMerge=group1;
     }
     //if 1 of the group is empty
     if(!group_array[groupToMergeTo-1]){
         group_array[groupToMergeTo-1]=group_array[groupToMerge-1];
         group_array[groupToMerge-1]=nullptr;
         return SUCCESS;
     }
     if(!group_array[groupToMerge-1]){
         return SUCCESS;
     }
     //merge level group trees
     if(group_array[group1-1]->tree_levels->tree_root!=nullptr && group_array[group2-1]->tree_levels->tree_root!=nullptr){
         auto* newLevel = new RankAVLTree<LEVEL,LevelRank>(MergeTwoAVLTrees(group_array[group1-1]->tree_levels,group_array[group2-1]->tree_levels));
         if(!newLevel){
             return ALLOCATION_ERROR;
             }
        delete group_array[groupToMergeTo-1]->tree_levels;
        group_array[groupToMergeTo-1]->tree_levels=newLevel;
     }
     else if(group_array[groupToMerge-1]->tree_levels->tree_root!=nullptr){ //only tree in group to merge
            delete group_array[groupToMergeTo-1]->tree_levels;
          group_array[groupToMergeTo-1]->tree_levels= group_array[groupToMerge-1]->tree_levels;
          group_array[groupToMerge-1]->tree_levels=nullptr;
     }
     
     //merge scores array
     for(int i = 0; i<scale;i++){
         group_array[groupToMergeTo-1]->array_scores[i].players_counter+=group_array[groupToMerge-1]->array_scores[i].players_counter;
         if((group_array[groupToMerge-1]->array_scores[i]).tree_levels->tree_root!=nullptr && (group_array[groupToMergeTo-1]->array_scores[i]).tree_levels->tree_root!=nullptr){
             {
                 auto *newLevelScore = new RankAVLTree<LEVEL, LevelRank>(
                         MergeTwoAVLTrees((group_array[group1-1]->array_scores[i].tree_levels),
                                          (group_array[group2-1]->array_scores[i].tree_levels)));
                 if (!newLevelScore) {
                     return ALLOCATION_ERROR;
                 }
                 delete group_array[groupToMergeTo-1]->array_scores[i].tree_levels;
                 group_array[groupToMergeTo-1]->array_scores[i].tree_levels = newLevelScore;
             }
         }
         else if (group_array[groupToMerge-1]->array_scores[i].tree_levels->tree_root!=nullptr && (group_array[groupToMergeTo-1]->array_scores[i]).tree_levels->tree_root==nullptr){
             delete group_array[groupToMergeTo-1]->array_scores[i].tree_levels;
             group_array[groupToMergeTo-1]->array_scores[i].tree_levels=group_array[groupToMerge-1]->array_scores[i].tree_levels;
             group_array[groupToMerge-1]->array_scores[i].tree_levels=nullptr;
         }
     }
     group_array[groupToMergeTo-1]->playerCounter=group_array[group1-1]->playerCounter+group_array[group2-1]->playerCounter;
     delete group_array[groupToMerge-1];
     group_array[groupToMerge-1]=nullptr;
     return SUCCESS;
}

StatusType PlayerManager::AddPlayer(int PlayerID, int GroupID, int score){
    if(PlayerID<=MIN_PLAYER_ID || GroupID> group_number || GroupID<=MIN_GROUP_ID || score>scale || score<= MIN_SCORE){
        return INVALID_INPUT;
    }
    Player player(PlayerID,GroupID,score);
    HashStatus status= players_.Insert(player);
    if (status==ALREADY_EXISTS){
        return FAILURE;
    }
    if (status==ALLOC_ERROR){
        return ALLOCATION_ERROR;
    }
    all_players_counter++;
    scores_array[score-1].players_counter++;
    int group= groups.Find(GroupID);
    if (group_array[group-1] == nullptr){
        group_array[group-1]= new Group(scale);
        group_array[group-1]->playerCounter++;
        group_array[group-1]->array_scores[score-1].players_counter++;
    }
    else{
         group_array[group-1]->playerCounter++;
         group_array[group-1]->array_scores[score-1].players_counter++;
    }
    return SUCCESS;
}

void PlayerManager::updateLevel(RankAVLTree<LEVEL,LevelRank>* LevelTree, int level, int change){//change -1 or 1
    if(LevelTree == nullptr){
//        || LevelTree->tree_root == nullptr
        return;
    }
    AVLNode<LEVEL,LevelRank>* levelToChange = LevelTree->Find(LevelTree->tree_root,level);
    if(!levelToChange && change==1){//then chanfe suppose to be 1
        LevelRank newLevel = LevelRank(level,1);
        LevelTree->Insert(level,newLevel);
        return;
    }
    else if(levelToChange && levelToChange->w_info.num_of_players_in_level==1 && change==-1){
        LevelTree->RemoveNode(level);
        return;
    }
    else if(levelToChange && levelToChange->w_info.num_of_players_in_level>=1){
        int num_of_players=levelToChange->w_info.num_of_players_in_level+change;
        LevelTree->RemoveNode(level);
        LevelRank newLevel = LevelRank(level,num_of_players);
        LevelTree->Insert(level,newLevel);
    }
}

StatusType PlayerManager::RemovePlayer(int PlayerID){
    if(PlayerID<=MIN_PLAYER_ID){
        return INVALID_INPUT;
    }
    Player player(PlayerID);
    Node<Player>* MyPlayer=players_.Find(player);
    if(!MyPlayer){
        return FAILURE;
    }
    int player_level=MyPlayer->element.level;
    int player_group=MyPlayer->element.groupID;
    int player_score=MyPlayer->element.score;
    player_group = groups.Find(player_group);
    // if level is not zero delete from all trees
    if(player_level>0){
        //delete from level tree in group
        updateLevel(group_array[player_group-1]->tree_levels,player_level,-1);   
        // delete from score array in group
        updateLevel(group_array[player_group-1]->array_scores[player_score-1].tree_levels,player_level,-1);
        //delete from level tree
        updateLevel(levels,player_level,-1);
        // delete from score array
        updateLevel(scores_array[player_score-1].tree_levels,player_level,-1);
    }
    //change counters and delete from hash anyway
    group_array[player_group-1]->playerCounter--;
    group_array[player_group-1]->array_scores[player_score-1].players_counter--;
    scores_array[player_score-1].players_counter--;
    all_players_counter--;
    players_.remove(player);
    return SUCCESS;
}

StatusType PlayerManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease){
    if(PlayerID<=MIN_PLAYER_ID || LevelIncrease<=0){
        return INVALID_INPUT;
    }
    Player player(PlayerID);
    Node<Player>* MyPlayer=players_.Find(player);
    if(!MyPlayer){
        return FAILURE;
    }
    int player_level=MyPlayer->element.level;
    int player_group=MyPlayer->element.groupID;
    int player_score=MyPlayer->element.score;
    player_group = groups.Find(player_group);
    // if level is not zero delete from all trees
    if(player_level>0){
        //delete from level tree in group
        updateLevel(group_array[player_group-1]->tree_levels,player_level,-1);
        // delete from score array in group
        updateLevel(group_array[player_group-1]->array_scores[player_score-1].tree_levels,player_level,-1);
        //delete from level tree
        updateLevel(levels,player_level,-1);
        // delete from score array
        updateLevel(scores_array[player_score-1].tree_levels,player_level,-1);
    }
    //add new level to all trees
    //add to level tree in group
    updateLevel(group_array[player_group-1]->tree_levels,player_level+LevelIncrease,1);   
    //add to score array in group
    updateLevel(group_array[player_group-1]->array_scores[player_score-1].tree_levels,player_level+LevelIncrease,1);
    //add to level tree
    updateLevel(levels,player_level+LevelIncrease,1);        
    //add to score array
    updateLevel(scores_array[player_score-1].tree_levels,player_level+LevelIncrease,1);
    MyPlayer->element.level=player_level+LevelIncrease;
    return SUCCESS;
}

StatusType PlayerManager::ChangePlayerIDScore(int PlayerID, int NewScore){
    if(PlayerID<=MIN_PLAYER_ID || NewScore<=MIN_SCORE || NewScore >scale){
        return INVALID_INPUT;
    }
    Player player(PlayerID);
    Node<Player>* MyPlayer=players_.Find(player);
    if(!MyPlayer){
        return FAILURE;
    }
    int player_level=MyPlayer->element.level;
    int player_group=MyPlayer->element.groupID;
    int player_score=MyPlayer->element.score;
    player_group = groups.Find(player_group);
    // if level is not zero delete and add to score trees
    if(player_level>0){
        // delete from score array in group
        updateLevel(group_array[player_group-1]->array_scores[player_score-1].tree_levels,player_level,-1);
        //add to score array in group
        updateLevel(group_array[player_group-1]->array_scores[NewScore-1].tree_levels,player_level,1);
        // delete from score array
        updateLevel(scores_array[player_score-1].tree_levels,player_level,-1);
        //add to score array
        updateLevel(scores_array[NewScore-1].tree_levels,player_level,1);
    }
    //change number of player in score anyway
    // delete from score array in group
    group_array[player_group-1]->array_scores[player_score-1].players_counter--;
    //add to score array in group
    group_array[player_group-1]->array_scores[NewScore-1].players_counter++;
    // delete from score array
    scores_array[player_score-1].players_counter--;
    //add to score array
    scores_array[NewScore-1].players_counter++;
    MyPlayer->element.score=NewScore;
    return SUCCESS;
}

double PlayerManager::GetPlayersInLevelBound(RankAVLTree<LEVEL,LevelRank>* LevelTree ,int player_counter, int lowerLevel, int higherLevel ){
    if(LevelTree->tree_root==nullptr){
        if(lowerLevel>0)
            return 0;
        return player_counter;
    }
    AVLNode<LEVEL,LevelRank>* max=LevelTree->GetMax(LevelTree->tree_root);
    if(max->data<lowerLevel){
        return 0;
    }
    if(max->data==lowerLevel){
        return max->w_info.num_of_players_in_level;
    }
    int CountLow=0, CountHigh=0, LowerPlayer=0;
    AVLNode<LEVEL,LevelRank>* MinLow =LevelTree->MinBigger(LevelTree->tree_root,lowerLevel); 
    LevelRank MinRank = LevelTree->Rank(LevelTree->tree_root,MinLow->data);
    CountLow= MinRank.num_of_players_in_level-MinLow->w_info.num_of_players_in_level;
    if(max->data<=higherLevel){
        LevelRank MaxRank = LevelTree->Rank(LevelTree->tree_root,max->data);
        CountHigh= MaxRank.num_of_players_in_level;
    }
    else{
        AVLNode<LEVEL,LevelRank>* MinHigh =LevelTree->MinBigger(LevelTree->tree_root,higherLevel); 
        LevelRank MaxRank = LevelTree->Rank(LevelTree->tree_root,MinHigh->data);
        CountHigh= MaxRank.num_of_players_in_level-MinHigh->w_info.num_of_players_in_level;
    }
    if(lowerLevel>0){
        AVLNode<LEVEL,LevelRank>* lower =LevelTree->Find(LevelTree->tree_root,lowerLevel);
        if(lower!=nullptr){
            LowerPlayer=lower->w_info.num_of_players_in_level;
        }
    }
    else if(lowerLevel<=0){
        LevelRank Maximum = LevelTree->Rank(LevelTree->tree_root,max->data);
        LowerPlayer=player_counter-Maximum.num_of_players_in_level;
    }
    return CountHigh-CountLow+LowerPlayer;

    
}

StatusType PlayerManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel,
                                            double * players){
    if( !players || GroupID<MIN_GROUP_ID || GroupID> group_number){
    return INVALID_INPUT;
    }
    if (higherLevel<0 || higherLevel<lowerLevel)
    {
        return FAILURE;
    } 
    double denominator=0,counter=0;
    if(GroupID!=0){
        int MyGroup = groups.Find(GroupID);
        if(MyGroup == -1 || group_array[MyGroup-1]==nullptr){
            return FAILURE;
        }
        counter = GetPlayersInLevelBound((group_array[MyGroup-1]->tree_levels) ,group_array[MyGroup-1]->playerCounter,lowerLevel,higherLevel);
        if(counter == 0){
            return FAILURE;
        }
        if(score<=0 || score>scale){
            *players=0;
            return SUCCESS;
            } 
        if(group_array[MyGroup-1]->array_scores[score-1].tree_levels->size==0){
            if(lowerLevel==0){
                denominator=group_array[MyGroup-1]->array_scores[score-1].players_counter;
            }
            else{
                denominator=0;
            }
        }
        else{
            denominator= GetPlayersInLevelBound((group_array[MyGroup-1]->array_scores[score-1].tree_levels) ,group_array[MyGroup-1]->array_scores[score-1].players_counter,lowerLevel,higherLevel);
        }

        *players= 100.00*(denominator/counter);
        return SUCCESS;
    }
    else{
        counter = GetPlayersInLevelBound((levels) ,all_players_counter,lowerLevel,higherLevel);
        if(counter == 0){
            return FAILURE;
        }
        if(score<=0 || score>scale){
            *players=0;
            return SUCCESS;
        } 
        if(scores_array[score-1].tree_levels->size==0){
            if(lowerLevel==0){
                denominator=scores_array[score-1].players_counter;
            }
            else{
                denominator=0;
            }
        }
        else{
            denominator= GetPlayersInLevelBound((scores_array[score-1].tree_levels) ,scores_array[score-1].players_counter,lowerLevel,higherLevel);
        }
        *players= 100.00*(denominator/counter);
        return SUCCESS;
    }
}


StatusType PlayerManager::AverageHighestPlayerLevelByGroupHelp(RankAVLTree<LEVEL,LevelRank>* levelTree,int m, double* avg){
    if (levelTree->tree_root==nullptr)
    { *avg= 0;
        return SUCCESS;
    }
    AVLNode<LEVEL,LevelRank>* max = levelTree->GetMax(levelTree->tree_root);
    LevelRank maxRank = levelTree->Rank(levelTree->tree_root,max->data);
    int max_count_level = maxRank.num_of_players_in_level;
    if(max_count_level-m<=0){
        *avg= ((double)(maxRank.num_of_players_mult_with_level)/m);
        return SUCCESS;
    }
    AVLNode<LEVEL,LevelRank>* m_count_level= levelTree->SelectMinBigger(levelTree->tree_root,max_count_level-m);
    LevelRank mRank = levelTree->Rank(levelTree->tree_root,m_count_level->data);
    *avg= ((double)(maxRank.num_of_players_mult_with_level)-(mRank.num_of_players_mult_with_level)+(m-(maxRank.num_of_players_in_level-mRank.num_of_players_in_level))*m_count_level->data)/m;
    return SUCCESS;
}

StatusType PlayerManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level) {
    if (!level || GroupID < MIN_GROUP_ID || GroupID > group_number || m <= 0) {
        return INVALID_INPUT;
    }
    if (GroupID != 0) {
        int MyGroup = groups.Find(GroupID);
        if (MyGroup == -1 || group_array[MyGroup-1] == nullptr || group_array[MyGroup-1]->playerCounter < m) {
            return FAILURE;
        }
        return AverageHighestPlayerLevelByGroupHelp(group_array[MyGroup-1]->tree_levels, m, level);
    } else {
        if (all_players_counter < m) {
            return FAILURE;
        }
        return AverageHighestPlayerLevelByGroupHelp(levels, m, level);
    }
    return FAILURE;
}