//
//  JewelMainLayer.cpp
//  jewel2
//
//  Created by CpyShine on 13-5-22.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#include "JewelMainLayer.h"

#include "JewelDatahandle.h"

#include "JewelSprite.h"

bool JewelMainLayer::init(void){
    if (!CCLayer::init()) {
        return false;
    }
    //set touch enable = yes,and delegate is oneByone
    
    setAnchorPoint(ccp(0, 0));
    setPosition(ccp(0,0));
    
    return true;
}

void JewelMainLayer::onEnter(void){
    CCLayer::onEnter();

    m_dataHandle = JewelDatahandle::create();
    
    this->addChild(m_dataHandle);
    
    m_dataHandle->f_initJewelSprites(45, 45);
    
    this->setContentSize(CCSizeMake(TOTALX * 45, TOTALY * 45));
    
    m_farDistance = 45.0f/3*2;
    
    this->setTouchMode(kCCTouchesOneByOne);
    
    this->setTouchEnabled(true);
    
    m_currentSelectSprite = NULL;
    
    m_jsAlpha = JewelSprite::create();
    m_jsAlpha->setVisible(false);
    
    this->addChild(m_jsAlpha);
//    
//    CCLayerColor * colorlayer = CCLayerColor::create(ccc4(0, 0, 0, 255));
//    
//    colorlayer->setAnchorPoint(ccp(0,0));
//    
//    colorlayer->setPosition(ccp(0, 45*TOTALY));
//    
//    this->addChild(colorlayer);
    
    m_pauseLayer = CCLayerColor::create(ccc4(255, 255, 255, 180));
    
    m_pauseLayer->setVisible(false);
    m_pauseLayer->setPosition(ccp(0, 0));
    m_pauseLayer->setAnchorPoint(ccp(0, 0));
    m_pauseLayer->setContentSize(CCSizeMake(TOTALX*45, TOTALY*45));
    this->addChild(m_pauseLayer);
    
    m_canDoExchange = true;
    
    this->setContentSize(CCSizeMake(TOTALX*45, (TOTALY+1)*45));
}

void JewelMainLayer::onEnterTransitionDidFinish(void){
    CCLayer::onEnterTransitionDidFinish();
}

void JewelMainLayer::onExit(void){
    CCLayer::onExit();
    
    this->setTouchEnabled(false);
    
    m_dataHandle->removeAllChildren();
    
    m_dataHandle->removeFromParentAndCleanup(true);
    
    m_dataHandle = NULL;
}

bool JewelMainLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent){
    
    if (!m_canDoExchange) {
        return false;
    }

    CCPoint touchLocation = pTouch->getLocation();
    
    CCPoint local = this->convertToNodeSpace(touchLocation);
    
    if (!this->boundingBox().containsPoint(local)) {//if the touchposition not in this layer boundBoxRect return false;
//        m_canDoExchange = true;
        CCLog("Touch BegineAt Pos:%f,%f",local.x,local.y);
        
        CCLog("m_dataHandle.children.count:%d",m_dataHandle->f_getStateArrayCount());
        return false;
    }
    
    if (m_currentSelectSprite) {
        m_currentSelectSprite->f_cancelSelect();
        m_currentSelectSprite = NULL;
    }
    
    m_currentSelectSprite = m_dataHandle->f_getCurrentSelectJewel(local);
    
    if(m_currentSelectSprite){
        
        m_currentSelectSprite->f_selected();
        m_jsAlpha->setVisible(true);
        m_jsAlpha->f_setType(m_currentSelectSprite->getSkillType());
        m_jsAlpha->setPosition(m_currentSelectSprite->f_calcPos());
        
        m_jsAlpha->setOpacity(255*0.3);
        
    }
    
    m_startPos = local;//  give the position assignment
    
    
    return true;
}


void JewelMainLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
    if (m_currentSelectSprite==NULL) {
        return;
    }
    
    CCPoint moveLocation = pTouch->getLocation();
    CCPoint local        = this->convertToNodeSpace(moveLocation);
    
    EXCHANGEDIRECTION direction = DIR_NONE;//默认为none
    
//    set m_currentSelectSprite position
    
    m_currentSelectSprite->setPosition(local);
    
    float distance  = ccpDistance(m_startPos, local);
    
    if ( distance > m_farDistance) {// up right down left Judgment
        
        direction = f_theDirectionOfJudgment(m_startPos,
                                                        local);
    }
    
    if (direction != DIR_NONE) { // if the direction is not none
        
        f_doJewelBlockExchange(direction);
    }
}

void JewelMainLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent){
    
    if (m_currentSelectSprite) {
        
        m_jsAlpha->setVisible(false);
        m_currentSelectSprite->f_cancelSelect();
        m_currentSelectSprite =NULL;
        f_gamePause();
        m_dataHandle->f_doCacleDissappearAgain();
    }
}

void JewelMainLayer::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent){
    
    if (m_currentSelectSprite) {
        m_jsAlpha->setVisible(false);
        m_currentSelectSprite->f_cancelSelect();
        m_currentSelectSprite =NULL;
        f_gamePause();
        m_dataHandle->f_doCacleDissappearAgain();
    }
}

void JewelMainLayer::draw(void){
    
}

JewelMainLayer::~JewelMainLayer(){
    
}

#pragma mark - functions

EXCHANGEDIRECTION JewelMainLayer::f_theDirectionOfJudgment(CCPoint beginePos,                      CCPoint endPos){ // 识别方向
    
    EXCHANGEDIRECTION direction = DIR_NONE;
    
    CCPoint pos = ccpSub(beginePos,endPos);
    
    float degree = CC_RADIANS_TO_DEGREES(ccpToAngle(pos));// ccpAngle(begineSelectPos, currentPos);
    
    degree +=180;
    
    for (int quadrant = 0; quadrant < 4; quadrant++) {
        
        if (degree >= (360-45.0f)) {
            degree -= 360;
        }
        
        if (degree >= 90.0f * quadrant - 45.0f &&
            degree < 90.0f * quadrant + 45.0f) {
            
            direction = (EXCHANGEDIRECTION)(quadrant + 1);//转换为 enum 里面的元素
            
            break;
        }
    }
    
    return direction;
}

void JewelMainLayer::f_doJewelBlockExchange(EXCHANGEDIRECTION exchangeDirection){// 交换
    
    
    switch (exchangeDirection) {//方向相反
        case DIR_RIGHT:
            f_exchangeWithRight();
            break;
        case DIR_UP:
            f_exchangeWithUp();
            break;
        case DIR_LEFT:
            f_exchangeWithLeft();
            break;
        case DIR_DOWN:
            f_exchangeWithDown();
            break;
        default:
            break;
    }
}


void JewelMainLayer::f_exchangeWithRight(){
    
//    CCLog("exchange with right");
    
    int cIndex = m_currentSelectSprite->getIndex();
    
    int x = m_currentSelectSprite->getX();
    
    if (++x >= TOTALX) {
        return;
    }
    
    int toIndex = m_currentSelectSprite->getY() * TOTALX + x;
    
    bool flag = m_dataHandle->f_exchangeTwoJewelSprite(cIndex, false, toIndex,true,false);
    
    if (flag) {
        
        m_startPos = m_currentSelectSprite->f_calcPos();
        m_jsAlpha->setPosition(m_startPos);
    }
}

void JewelMainLayer::f_exchangeWithUp(){
    
//    CCLog("exchange with up");
    
    int cIndex = m_currentSelectSprite->getIndex();
    
    int y = m_currentSelectSprite->getY();
    
    if (++y >=TOTALY) {
        return;
    }
    
    int toIndex = y*TOTALX + m_currentSelectSprite->getX();
    
    bool flag = m_dataHandle->f_exchangeTwoJewelSprite(cIndex, false, toIndex,true,false);
    
    if (flag) {
        
        m_startPos = m_currentSelectSprite->f_calcPos();
        m_jsAlpha->setPosition(m_startPos);
    }
}

void JewelMainLayer::f_exchangeWithLeft(){
    
//    CCLog("exchange with left");
    
    
    int cIndex = m_currentSelectSprite->getIndex();
    
    int x = m_currentSelectSprite->getX();
    
    if (--x < 0) {
        return;
    }
    
    int toIndex = m_currentSelectSprite->getY() * TOTALX + x;
    
    bool flag = m_dataHandle->f_exchangeTwoJewelSprite(cIndex, false, toIndex,true,false);
    
    if (flag) {
        
        m_startPos = m_currentSelectSprite->f_calcPos();
        m_jsAlpha->setPosition(m_startPos);
    }
}

void JewelMainLayer::f_exchangeWithDown(){
    
//    CCLog("exchange with down");
    
    int cIndex = m_currentSelectSprite->getIndex();
    
    int y = m_currentSelectSprite->getY();
    
    if (--y < 0) {
        return;
    }
    
    int toIndex = y*TOTALX + m_currentSelectSprite->getX();
    
    bool flag = m_dataHandle->f_exchangeTwoJewelSprite(cIndex, false, toIndex,true, false);
    
    if (flag) {
        
        m_startPos = m_currentSelectSprite->f_calcPos();
        m_jsAlpha->setPosition(m_startPos);
    }
}

void JewelMainLayer::f_gamePause(){
    
    m_canDoExchange = false;
    
    if (m_pauseLayer) {
        
//        m_pauseLayer->setVisible(true);
    }
}

void JewelMainLayer::f_gameResume(){
    
    m_canDoExchange = true;
    
    if (m_pauseLayer) {
        
//        m_pauseLayer->setVisible(false);
    }
}

