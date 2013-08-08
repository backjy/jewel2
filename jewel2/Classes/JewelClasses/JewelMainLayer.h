//
//  JewelMainLayer.h
//  jewel2
//
//  Created by CpyShine on 13-5-22.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef _JewelMainLayer_H
#define _JewelMainLayer_H

#include "cocos2d.h"

#include "JewelConfig.h"

using namespace cocos2d;

class JewelDatahandle;

class JewelSprite;

class JewelMainLayer :public CCLayer {
    
    JewelDisappearSpawnSkill_Protocol * delegate;
    
    JewelSprite * m_currentSelectSprite;
    
    JewelSprite * m_jsAlpha;// 背后半透明的那个
    
    JewelDatahandle * m_dataHandle;
    
    CCPoint m_startPos;     //
    
    float m_farDistance;    //
    
    bool  m_canDoExchange;  //
    
    CCLayerColor * m_pauseLayer;
public:
    virtual bool init(void);
    virtual void onEnter(void);
    virtual void onExit(void);
    virtual void onEnterTransitionDidFinish(void);
    virtual void draw(void);
    
    // default implements are used to call script callback if exist
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    
    // default implements are used to call script callback if exist
    CREATE_FUNC(JewelMainLayer);
    
    virtual ~JewelMainLayer();
    
    
#pragma mark - functions

    EXCHANGEDIRECTION f_theDirectionOfJudgment(CCPoint beginePos,
                                               CCPoint endPos);//return exchange direction The Relation
    
    void f_doJewelBlockExchange(EXCHANGEDIRECTION exchangeDirection);//do exchange with direction
    
    void f_exchangeWithUp();
    void f_exchangeWithLeft();
    void f_exchangeWithDown();
    void f_exchangeWithRight();
    
    void f_gamePause();
    void f_gameResume();
};

#endif