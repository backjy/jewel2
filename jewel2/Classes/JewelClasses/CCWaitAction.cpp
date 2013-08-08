//
//  CCWaitAction.cpp
//  jewel2
//
//  Created by CpyShine on 13-5-23.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#include "CCWaitAction.h"


CCWaitAction * CCWaitAction::createWithWaitTime(float d){
    
    CCWaitAction * pRet = new CCWaitAction();
    
    pRet->initWithDuration(d);
    
    pRet->autorelease();
    
    return pRet;
}

bool CCWaitAction::initWithDuration(float d){
    
    if (CCActionInterval::initWithDuration(d)) {
        return true;
    }
    return false;
}

void CCWaitAction::update(float time){
    
}

CCWaitAction::~CCWaitAction(){
    
}