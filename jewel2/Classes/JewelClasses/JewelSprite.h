//
//  JewelSprite.h
//  jewel2
//
//  Created by CpyShine on 13-5-22.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef _JewelSprite_H
#define _JewelSprite_H

#include "JewelConfig.h"
#include "cocos2d.h"
#include "CCWaitAction.h"


using namespace cocos2d;


class JewelSprite :public CCSprite {
    
    int m_x;
    int m_y;
    int m_index;
    
    int m_moveCount;
    
    int m_moveActionTag;
    
    float m_w;
    float m_h;
    
    bool m_selected;
    bool m_disappear;
    bool m_lastDisappear;
    
    SKILLTYPE m_type;
    
    CCPoint m_startPos;
public:
    
    void f_start(int x,int y,int index,float width,float height,SKILLTYPE type);//
    
    void f_appearAndDropDown(int y);//
    
    void f_appearAgain(bool callFunc);
    void f_dropDown(bool callFunc);
    void f_dropEndCallFunc();
    
    void f_doExchange(int x,int y,int index,bool doAction,bool addcount);//

    inline int getX(){return m_x;}
    inline int getY(){return m_y;}
    inline int getIndex(){return m_index;}
    inline SKILLTYPE getSkillType(){return m_type;}
    inline bool getSelected(){return m_selected;}
    inline bool getDisappear(){return m_disappear;}
    
#pragma mark - 
    
    void f_disappear(bool doCallF);// 调用他消失
    void f_disappearCallFunc();
    
    void f_setType(SKILLTYPE type);// 设置类型
    
    void f_selected(){
        
        if (m_selected) {
            return;
        }
        
        m_selected = true;
        setZOrder(TOTALX*TOTALY + m_index);
    }
    
    inline void f_cancelSelect(){
        
        setPosition(f_calcPos());
        setZOrder(m_index);
        m_selected = false;
    }
    
    inline CCPoint f_calcPos(){// 计算位置
        
        float width = m_x * m_w + m_w * this->getAnchorPoint().x;
        float height = m_y * m_h + m_h * this->getAnchorPoint().y;
        
        return ccp(width,height);
    }

    inline void f_setDisappear(bool d){
        
        m_disappear = d;
    }
    
#pragma mark - 
    virtual bool init(void);
    
    CREATE_FUNC(JewelSprite);
    
    JewelSprite(){
        m_moveCount = 0;
        m_index = -1;
        m_selected = false;
        m_disappear = false;
        m_lastDisappear = false;
    }
};

#endif