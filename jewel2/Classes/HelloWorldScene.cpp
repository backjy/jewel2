#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#include "JewelConfig.h"

#include "JewelMainLayer.h"

enum testEnum {
    one = 1<<0,
    two = 1<<1,
    three = 1<<2,
    four = 1<<3,
    };

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
//    HelloWorld *layer = HelloWorld::create();
    
    JewelMainLayer * layer = JewelMainLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
//    CCLog("%f", CC_RADIANS_TO_DEGREES(ccpToAngle(ccpSub(ccp(0,0),ccp(0,10)))) );
//    CCLog("%f", CC_RADIANS_TO_DEGREES(ccpToAngle(ccpSub(ccp(0,10),ccp(0,0)))) );
//    CCLog("%f", CC_RADIANS_TO_DEGREES(ccpToAngle(ccpSub(ccp(10,0),ccp(0,0)))) );
//    CCLog("%f", CC_RADIANS_TO_DEGREES(ccpToAngle(ccpSub(ccp(0,-10),ccp(0,0)))) );
//    CCLog("%f", CC_RADIANS_TO_DEGREES(ccpToAngle(ccpSub(ccp(-10,0),ccp(0,0)))) );
    
    
//    CCLog("%d",1%2);
//    CCLog("%d",3%2);
//    CCLog("%d",5%2);
//    CCLog("%d",6%2);
//    CCLog("%d",8%2);
//    
//    CCLog("%d",3%3);
//    CCLog("%d",6%3);
//    CCLog("%d",9%3);
    unsigned long long int x = (1<<32)-1;
    CCLog("%ld",x);
    CCLog("%ld",INTMAX_MAX);
//    CCLog("%d",two);
//    CCLog("%d",three);
//    CCLog("%d",four);
//
//    unsigned int flag = one | two;
//    
//    CCLog("%d",flag & one);
//    CCLog("%d",flag & two);
//    CCLog("%d",flag & three);

    
//    std::list<int> x;
//
//    x.push_back(1);
//    x.push_back(2);
//    x.push_back(3);
//    x.push_back(4);
//    x.push_back(5);
//    x.push_back(6);
//    x.push_back(7);
//    
//    
//    std::list<int>::iterator theIterator;
//    
//    for (theIterator = x.begin();
//         theIterator != x.end();
//         theIterator++) {
//        
//        
//        if (*theIterator%2 ==0) {
//            x.remove(*theIterator);
//        }
//    }
//    
//    
//    for (theIterator = x.begin();
//         theIterator != x.end();
//         theIterator++) {
//        
//        printf("%d ",*theIterator);
//    }
//    printf("\n");
    
    return true;
}

void HelloWorld::hebinglist(std::list<int> &list){
    
    std::list<int> x;
    x.push_back(1);
    
    list.merge(x);
    x.clear();
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
