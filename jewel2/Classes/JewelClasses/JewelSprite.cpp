//
//  JewelSprite.cpp
//  jewel2
//
//  Created by CpyShine on 13-5-22.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#include "JewelSprite.h"

#include "JewelDatahandle.h"

#define MOVEACTIONTAG 1000

bool JewelSprite::init(void){
    if (!CCSprite::init()) {
        return false;
    }
    
    setVisible(false);
    
    return true;
}

void JewelSprite::f_start(int x,int y,int index,float width,float height,
                          SKILLTYPE type){// 最开始设置
    
    m_x = x;
    m_y = TOTALY;
    
    m_w = width;
    m_h = height;
    
    this->setContentSize(CCSizeMake(m_w, m_h));
    
    CCPoint startPos   = f_calcPos();
    m_startPos         = ccp( startPos.x, startPos.y);
    
//    setPosition(m_startPos);//位置复原
    
    m_y = y;
    m_index = index;
    
    f_appearAndDropDown(TOTALY);//重设置位置
    
    f_setType( type);// 重设置 类型
}

void JewelSprite::f_appearAndDropDown(int y){ //重置属性 且执行动作
    
    int absY    = abs(y - m_y);
    
    CCWaitAction *  wait = CCWaitAction::createWithWaitTime(0.2*(TOTALY-absY));
    CCMoveTo    * moveTo = CCMoveTo::create(0.2f, f_calcPos());
    CCSequence * seq    = CCSequence::create(wait,moveTo,NULL);
    
    seq->setTag(MOVEACTIONTAG);
    
    this->runAction(seq);
}

void JewelSprite::f_doExchange(int x,int y,int index,bool doAction,bool addcount){//
    
    if (m_x == x && m_y == y) {
        return;
    }
    
    m_x     = x;
    m_y     = y;
    m_index = index;
    
    if (doAction) {
        this->stopActionByTag(MOVEACTIONTAG);
        CCMoveTo * moveTo = CCMoveTo::create(0.1, f_calcPos());
        moveTo->setTag(MOVEACTIONTAG);
        this->runAction(moveTo);
    }
    if (addcount) {
        m_moveCount ++;
    }
    
//    if (m_x != x) {
        m_startPos = ccp(m_x * m_w + m_w * this->getAnchorPoint().x,
                         TOTALY * m_h + m_h * this->getAnchorPoint().y);
//    }
}


void JewelSprite::f_setType(SKILLTYPE type){
    
    m_type = type;
    setPosition(m_startPos);
    setOpacity(255);

    std:: string fileName;
    
    switch (m_type) {
        case SKILL_ONE:
            fileName = "C0.png";
            break;
        case SKILL_TWO:
            fileName = "C1.png";
            break;
        case SKILL_THREE:
            fileName = "C2.png";
            break;
        case SKILL_FOUR:
            fileName = "C3.png";
            break;
        case SKILL_FIVE:
            fileName = "C4.png";
            break;
        case SKILL_SIX:
            fileName = "C5.png";
            break;
        default:
            return;
            break;
    }
    
//    this->initWithFile(fileName.c_str());
    CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->
                                                spriteFrameByName(fileName.c_str());
    
    if (frame == NULL) {
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->
                             addSpriteFramesWithFile("JewelResources.plist");
        
        frame = CCSpriteFrameCache::sharedSpriteFrameCache()->
                                     spriteFrameByName(fileName.c_str());
    }
    
    this->setDisplayFrame(frame);
    
    this->setVisible(true);
}

void JewelSprite::f_disappear(bool doCallF){
    
    this->stopActionByTag(MOVEACTIONTAG);
    m_disappear = true;
    
//
    m_lastDisappear = doCallF;
    
    CCFadeOut * fadeOut = CCFadeOut::create(0.2);
    CCCallFunc * callfunc = CCCallFunc::create(this, callfunc_selector(JewelSprite::f_disappearCallFunc));
    CCSequence * seq = CCSequence::create(fadeOut, callfunc, NULL);
    this->runAction(seq);
}

void JewelSprite::f_dropDown(bool callFunc){
    
    if (m_disappear) {
        f_appearAgain(callFunc);
        return;
    }
//    CCWaitAction * wait = CCWaitAction::createWithWaitTime(0.1);
    CCMoveTo * moveTo = CCMoveTo::create(0.1*m_moveCount, f_calcPos());
    if (callFunc) {
        CCCallFunc * callF = CCCallFunc::create(this,
                                                  callfunc_selector(JewelSprite::f_dropEndCallFunc));
        CCWaitAction * wait2 = CCWaitAction::createWithWaitTime(0.1);
        CCSequence * seq = CCSequence::create(moveTo,wait2,callF,NULL);
        seq->setTag(MOVEACTIONTAG);
        this->runAction(seq);
    }else{
        
        moveTo->setTag(MOVEACTIONTAG);
        this->runAction(moveTo);
    }
    m_moveCount = 0;
}

void JewelSprite::f_appearAgain(bool callFunc){
    
    m_disappear = false;
    
    int absY = abs(TOTALY - m_y);
    
    CCWaitAction * wait = CCWaitAction::createWithWaitTime((m_y-1)*0.1);
    CCMoveTo *moveTo = CCMoveTo::create(0.1*absY, f_calcPos());
    if (callFunc) {
        CCCallFunc * callF = CCCallFunc::create(this,
                                                  callfunc_selector(JewelSprite::f_dropEndCallFunc));
        CCWaitAction * wait2 = CCWaitAction::createWithWaitTime(0.1);
        CCSequence * seq = CCSequence::create(wait,moveTo,wait2,callF,NULL);
        seq->setTag(MOVEACTIONTAG);
        this->runAction(seq);
    }else{
        CCSequence * seq = CCSequence::create(wait,moveTo,NULL);
        seq->setTag(MOVEACTIONTAG);
        this->runAction(seq);
    }
    m_moveCount = 0;
}

void JewelSprite::f_dropEndCallFunc(){
    
//    CCLog("calc again!");
    
    JewelDatahandle * par = dynamic_cast<JewelDatahandle*>(this->getParent());
    
    if (par) {
        par->f_doCacleDissappearAgain();
    }
    
}

void JewelSprite::f_disappearCallFunc(){
    
    SKILLTYPE type =(SKILLTYPE)(arc4random()%TOTALSKILL);
    
    f_setType(type);
    
    
    if (m_lastDisappear) {
        
//        CCLog("call parent drop down!");
        
        m_lastDisappear = false;
        
        JewelDatahandle * par = dynamic_cast<JewelDatahandle*>(this->getParent());
        
        if (par) {
            par->f_dropdown();
        }
    }
}

