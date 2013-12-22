//
//  JewelDatahandle.h
//  jewel2
//
//  Created by CpyShine on 13-5-22.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef _JewelDatahandle_H
#define _JewelDatahandle_H

#include "cocos2d.h"

#include "JewelConfig.h"

using namespace cocos2d;
//using namespace std;

class JewelSprite;

class JewelDatahandle :public CCNode {
    
    JewelSprite * m_jewelSpriteArray;
    
    CCArray * m_stateArray;
    
    std::list<int> m_checkDisappearList;// 检查需要消除的数组
    
public:
    
    virtual bool init(void);
    virtual void onEnter(void);
    virtual void onExit(void);
    virtual void onEnterTransitionDidFinish(void);
    
    CREATE_FUNC(JewelDatahandle);
    virtual ~JewelDatahandle();
    
#pragma mark - do data handles
    
    int     f_getStateArrayCount(){
        return m_stateArray->count();
    }
    
    void    f_showDebugStateArray();
    
    void    f_initJewelSprites( float width, float height);//初始化 jewelsprite
    
    void    f_doCacleDissappearAgain();
    
    bool    f_dropdown();
    
    bool    f_exchangeTwoJewelSprite(int indexA,bool a,int indexB,int b,bool addCount);//交换两个元素
    
    JewelSprite * f_getCurrentSelectJewel(const CCPoint &touchL);//找到当前选着的 jewel
    
private:
    
    //spawn 
    
    SKILLTYPE f_randSkillType(int *a,int ac, int *b,int bc,int x);//随机SkillType
    
    void    f_spawnTabel(int *a,int ac, int *b,int bc);// 生成table1 和table2
    
    void    f_spawnNewJewelSprite();//生成新的sprite
    //disappear need functions
    
    int     f_calcDisappear();//计算需要被进行消除计算的index
    
    void    f_findSameJewelAtIndexNearBy(int index,std::list<int> &resultList,
                                         std::list<int> &closeList);
    void    f_doCheckSameJewelSprite(int x,int y,SKILLTYPE type,std::list<int> &resultList,
                                     std::list<int> &closeList);
    bool    f_judgeListExitNumber(std::list<int> &list,int value);
    
    void    f_calcCloseListNoUseIndex(std::list<int> &list);
    bool    f_xListIsContinue(std::list<int> &list);
    bool    f_yListIsContinue(std::list<int> &list);
    
    int     f_calcIndex(int x,int y);
    int     f_doDisappear(int index);//计算index 位置消除情况
    bool    f_doDisappearColoum(int index,std::list<int> &list);//计算 coloum消除情况
    void    f_doDisappearWithList(std::list<int> &list);
    //掉落
    void    f_calcDropDown(int index);
    void    f_doDropDown();
};

#endif
