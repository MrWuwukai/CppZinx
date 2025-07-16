#include "AOIWorld.h"

AOIWorld::AOIWorld(int xB, int xE, int yB, int yE, int xC, int yC)
    : x_begin(xB), x_end(xE), y_begin(yB), y_end(yE), x_count(xC), y_count(yC) {
	x_width = (x_end - x_begin) / x_count;
	y_width = (y_end - y_begin) / y_count;

	/*创建格子们*/
	for (int i = 0; i < x_count * y_count; i++)
	{
		Grid tmp;
		m_grids.push_back(tmp);
	}
}

std::list<Player*> AOIWorld::GetSurroundPlayers(Player* _player) {
	std::list<Player*> ret;
	int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY()) / y_width * x_count;

    //计算当前网格横着数和纵着数的个数
    int x_index = grid_id % x_count;
    int y_index = grid_id / x_count;

    if (x_index > 0 && y_index > 0){
        list<Player*>& cur_list = m_grids[grid_id - 1 - x_count].m_players;
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if (y_index > 0){
        list<Player*>& cur_list = m_grids[grid_id - x_count].m_players;
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if (x_index < x_count - 1 && y_index > 0){
        list<Player*>& cur_list = m_grids[grid_id - x_count + 1].m_players;
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if (x_index > 0){
        list<Player*>& cur_list = m_grids[grid_id - 1].m_players;
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    list<Player*>& cur_list = m_grids[grid_id].m_players;
    ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    if (x_index < x_count - 1){
        list<Player*>& cur_list = m_grids[grid_id + 1].m_players;
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if (x_index > 0 && y_index < y_count - 1){
        list<Player*>& cur_list = m_grids[grid_id - 1 + x_count].m_players;
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if (y_index < y_count - 1){
        list<Player*>& cur_list = m_grids[grid_id + x_count].m_players;
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }
    if (x_index < x_count - 1 && y_index < y_count - 1){
        list<Player*>& cur_list = m_grids[grid_id + 1 + x_count].m_players;
        ret.insert(ret.begin(), cur_list.begin(), cur_list.end());
    }

	return ret;
}

bool AOIWorld::AddPlayer(Player* _player) {
    /*计算所属网格号*/
    int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY()) / y_width * x_count;

	/*添加到该网格中*/
	m_grids[grid_id].m_players.push_back(_player);
	return true;
}

void AOIWorld::DelPlayer(Player* _player)
{
	int grid_id = (_player->GetX() - x_begin) / x_width + (_player->GetY()) / y_width * x_count;
	m_grids[grid_id].m_players.remove(_player);
}