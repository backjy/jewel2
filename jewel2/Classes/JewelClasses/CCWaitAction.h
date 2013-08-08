//
//  CCWaitAction.h
//  jewel2
//
//  Created by CpyShine on 13-5-23.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#ifndef _CCWaitAction_H
#define _CCWaitAction_H

#include "cocos2d.h"

using namespace cocos2d;


class CCWaitAction :public CCActionInterval {
    
    
public:
    virtual void update(float time);

    virtual ~CCWaitAction();
    
    bool initWithDuration(float d);
    
    static CCWaitAction * createWithWaitTime(float d);
    
//    if (CCActionInterval::initWithDuration(duration))
};

#endif