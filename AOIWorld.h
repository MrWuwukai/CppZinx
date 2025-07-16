#pragma once

#include <list>
#include <vector>

class Player {
public:
    virtual int GetX() = 0;
    virtual int GetY() = 0;
};

class Grid {
public:
    std::list<Player*> m_players;
};

class AOIWorld {
public:  
    AOIWorld(int xB, int xE, int yB, int yE, int xC, int yC);
	virtual ~AOIWorld();

    /*��ȡ��Χ���*/
    std::list<Player*> GetSurroundPlayers(Player* _player);

    /*�����ҵ�A0I����*/
    bool AddPlayer(Player* _player);
    /*ժ�����*/
    void DelPlayer(Player* _player);
public:
    std::vector<Grid> m_grids;
private:
    int x_begin = 0;
    int x_end = 0;
    int y_begin = 0;
    int y_end = 0;
    int x_count = 0;
    int y_count = 0;
    int x_width = 0;
    int y_width = 0;
};

