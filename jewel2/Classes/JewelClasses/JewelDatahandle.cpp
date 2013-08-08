//
//  JewelDatahandle.cpp
//  jewel2
//
//  Created by CpyShine on 13-5-22.
//  Copyright 2013年 __MyCompanyName__. All rights reserved.
//

#include "JewelDatahandle.h"

#include "JewelMainLayer.h"
#include "JewelSprite.h"

bool JewelDatahandle::init(void){
    if (!CCNode::init()) {
        return false;
    }
    
    setAnchorPoint(ccp(0, 0));
    setPosition(ccp(0, 0));
    
    return true;
}
void JewelDatahandle::onEnter(void){
    CCNode::onEnter();
    
}

void JewelDatahandle::onEnterTransitionDidFinish(void){
    CCNode::onEnterTransitionDidFinish();
}

void JewelDatahandle::onExit(void){
    
    CCNode::onExit();
    
    m_stateArray->removeAllObjects();
    
    CC_SAFE_DELETE(m_stateArray);
}
JewelDatahandle::~JewelDatahandle(){
    
}

#pragma mark - functions 

void JewelDatahandle::f_showDebugStateArray(){
    printf("abcdefghijklmnopqrstuvwxyz\n");
    
    for (int r=TOTALY-1; r>=0; r--) {
        
        for (int i=0; i<TOTALX; i++) {
            
            int index = r*TOTALX +i;
            
            JewelSprite * js = dynamic_cast<JewelSprite*>( m_stateArray->objectAtIndex(index));
            
            if (js) {
                printf("%d",js->getSkillType());
            }
//            printf("%d",m_jewelSpriteArray[index].getSkillType());
        }
        printf("-----%02d\n",r);
    }
    printf("abcdefghijklmnopqrstuvwxyz\n");
}

void JewelDatahandle::f_initJewelSprites(float width,float height){
    
//    int total = TOTALX * TOTALY;
    
    m_stateArray = new CCArray();
    m_stateArray->init();
    
    SKILLTYPE skillType;
    
    int tableone[TABLE_ONE_COUNT];
    int tabletwo[TABLE_TWO_COUNT];
    
    for (int y = 0; y <TOTALY; y++) {// 生成规则 当前行能%2=0时 生成 表一和表二 里面分别装有ac，bc个元素且里面没没重复的元素\
                                        然后就是生成 技能类型，如果行为奇数的时候交换 表一和表二 交给randskill函数生成一个技能出来
        
        if (y%2 == 0) {
            f_spawnTabel(tableone,TABLE_ONE_COUNT, tabletwo,TABLE_TWO_COUNT);
        }
        
        for (int x = 0; x <TOTALX; x++) {
            
            int index = y * TOTALX + x;
            
            if (y%2 != 0) {// 当为奇数行时 table1和table2 交换
                
                skillType = f_randSkillType(tableone,TABLE_ONE_COUNT, tabletwo,TABLE_TWO_COUNT, x);
            }else{
                
                skillType = f_randSkillType(tabletwo,TABLE_TWO_COUNT, tableone,TABLE_ONE_COUNT, x);
            }
                        
            JewelSprite * js = JewelSprite::create();
            
            this->addChild(js);
            
            js->f_start(x, y, index, width, height,skillType);
            
//            printf("%d ",index);
            m_stateArray->addObject(js);
        }
    }
    
//    f_showDebugStateArray();
}

SKILLTYPE JewelDatahandle::f_randSkillType(int *a,int ac, int *b,int bc,int x){//生成技能类型
    
    int rand = 0;
    
    SKILLTYPE skill;
    
    if ((x + 1)%3 == 0) {
        
        rand = arc4random() % bc;
        skill = (SKILLTYPE)b[rand];
    }else{
        
        rand = arc4random() % ac;
        skill = (SKILLTYPE)a[rand];
    }
    
    return skill;
}

void JewelDatahandle::f_spawnTabel(int *a,int ac, int *b,int bc){//生成 a列表和b列表 存放技能类型
    int total = ac + bc;
    int index = arc4random() % total;
    for (int i = 0; i < ac; i++) {
        a[i] = index;
        index ++;
        if (index >= total) {
            index = 0;
        }
    }
    for (int i = 0; i < bc; i++) {
        b[i] = index;
        index ++;
        if (index >= total) {
            index = 0;
        }
    }
}

JewelSprite *JewelDatahandle::f_getCurrentSelectJewel(const cocos2d::CCPoint &touchL){//得到当前被选择到的 jewel
    
    CCObject * obj = NULL;
    
    CCARRAY_FOREACH(m_stateArray, obj){
        
        JewelSprite * js = dynamic_cast<JewelSprite*>(obj);
        
        if (js->boundingBox().containsPoint(touchL) &&
            !js->getSelected()) {
            
            return js;
        }
    }
    return NULL;
}

bool JewelDatahandle::f_exchangeTwoJewelSprite(int indexA,bool a,int indexB,int b,bool addCount){
    
    CCObject * objA = m_stateArray->objectAtIndex(indexA);
    CCObject * objB = m_stateArray->objectAtIndex(indexB);
    
    JewelSprite * jsA = dynamic_cast<JewelSprite*>(objA);
    JewelSprite * jsB = dynamic_cast<JewelSprite*>(objB);
    
    if (!jsA || !jsB) {
        CCLog("jsA or jsB is NULL");
        return false;
    }
    
    
    int aX = jsA->getX();
    int aY = jsA->getY();
    int aI = jsA->getIndex();
    
    jsA->f_doExchange(jsB->getX(),
                      jsB->getY(),
                      jsB->getIndex(), a,addCount);
    
    jsB->f_doExchange(aX, aY, aI, b,addCount);
    
    m_stateArray->exchangeObjectAtIndex(indexA, indexB);
    
    m_checkDisappearList.push_back(jsA->getIndex());//将移动过的元素加入消除计算list中
    m_checkDisappearList.push_back(jsB->getIndex());
    
    return true;
}

#pragma mark -  disappear functions--------------------------------------------------

int JewelDatahandle::f_calcDisappear(){
    
    int count = 0;
    
    if (m_checkDisappearList.size() == 0) {
        int total = TOTALX*TOTALY;
        for (int in = 0 ; in < total; in++) {
            
            count += f_doDisappear(in);
        }
        return count;
    }
    
    m_checkDisappearList.sort();
    m_checkDisappearList.unique();//去掉重复的元素
    
    std::list<int>::iterator theIterator;
    for (theIterator = m_checkDisappearList.begin();
         theIterator != m_checkDisappearList.end();
         theIterator++) {

        count += f_doDisappear(*theIterator);
    }
    
//    m_checkDisappearList.clear();
    return count;
}

void JewelDatahandle::f_doCacleDissappearAgain(){
    
    int count = f_calcDisappear();
    
    m_checkDisappearList.clear();
    
    if (count == 0) {
        
        JewelMainLayer * par = dynamic_cast<JewelMainLayer*>(this->getParent());
        if (par) {
            
            par->f_gameResume();
        }
    }
}

int JewelDatahandle::f_doDisappear(int index) {
    
    if (index<0 || index>= TOTALY*TOTALX) {
        return 0;
    }
        
    std::list<int> openList;
    std::list<int> closeList;
    
    openList.push_back(index);
    
    std::list<int>::iterator theIterator;
    
    while (openList.size()>0) {
        
        theIterator = openList.begin();
        
        f_findSameJewelAtIndexNearBy(*theIterator, openList, closeList);
        
        closeList.push_back(*theIterator);
        openList.remove(*theIterator);
        
    }
    f_calcCloseListNoUseIndex(closeList);
    closeList.unique();
    int count = closeList.size();
//    CCLog("count:%d",count);
    if (count>=3) {
        f_doDisappearWithList(closeList);
        return count;
    }
    
    return 0;
}

void JewelDatahandle::f_findSameJewelAtIndexNearBy(int index, std::list<int> &resultList,
                                                   std::list<int> &closeList){
    
    
    JewelSprite * js = dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(index));
    
    if (!js || js->getDisappear() ) {
        return;
    }
    SKILLTYPE type = js->getSkillType();
    int x = js->getX();
    int y = js->getY();
    
    f_doCheckSameJewelSprite(x+1, y, type, resultList, closeList);
    f_doCheckSameJewelSprite(x-1, y, type, resultList, closeList);
    
    f_doCheckSameJewelSprite(x, y+1, type, resultList, closeList);
    f_doCheckSameJewelSprite(x, y-1, type, resultList, closeList);
}


void JewelDatahandle::f_doCheckSameJewelSprite(int x,int y,SKILLTYPE type,std::list<int> &resultList,
                                               std::list<int> &closeList){
    
    if (x < 0 || x >= TOTALX || y < 0 || y>=TOTALY) {
        return;
    }
    
    int index = f_calcIndex(x, y);
    
    JewelSprite * js = dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(index));
    
    if (js ||
        !js->getDisappear()) {
        
        if (js->getSkillType() != type) {
            return;
        }
        
        if (!f_judgeListExitNumber(closeList,index)) {//如果不在 close中才能加入openglist中
            
            resultList.push_back(index);
        }
    }
}


bool JewelDatahandle::f_judgeListExitNumber(std::list<int> &list,int value){
    
    std::list<int>::iterator theIterator;
    
    for (theIterator = list.begin();
         theIterator != list.end();
         theIterator++){
        if (*theIterator == value) {
            return true;
        }
    }
    return false;
}

void JewelDatahandle::f_calcCloseListNoUseIndex(std::list<int> &list){
    
    list.sort();
    std::list<int> resultList;
    
    std::list<int>::iterator theIteratorA;
    std::list<int>::iterator theIteratorB;
    
    JewelSprite * js = NULL;
    
    std::list<int> listX;
    std::list<int> listY;
    
    for (theIteratorA = list.begin(); theIteratorA != list.end(); theIteratorA++) {
        
        js= dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(*theIteratorA));
        if (!js) {
            continue;
        }
        int aX = js->getX();
        int aY = js->getY();
        
        listY.push_back(*theIteratorA);
        listX.push_back(*theIteratorA);
        
        for (theIteratorB = list.begin(); theIteratorB != list.end(); theIteratorB++) {
            
            if (*theIteratorA == *theIteratorB) {
                continue;
            }
            js= dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(*theIteratorB));
            if (!js) {
                continue;
            }
            int bX = js->getX();
            int bY = js->getY();
            
            if (aX == bX) {
                listY.push_back(js->getIndex());
            }
            if (aY == bY) {
                listX.push_back(js->getIndex());
            }
        }
        if (listX.size()<2 && listY.size()<2) {
//            list.remove(*theIteratorA);
            continue;
        }
        if (listX.size()>=2 && f_xListIsContinue(listX)) {
            resultList.merge(listX);
        }
        if (listY.size()>=2 && f_yListIsContinue(listY)) {
            resultList.merge(listY);
        }
        listX.clear();
        listY.clear();
    }
    list.clear();
    resultList.sort();
    list.merge(resultList);
}
bool JewelDatahandle::f_xListIsContinue(std::list<int> &list){
    list.sort();
    std::list<int>::iterator theIterator;
    int count= 1;
    bool begin =true;
    int x = 0;
    JewelSprite * js = NULL;
    for (theIterator = list.begin(); theIterator!=list.end(); theIterator++) {
        
        js= dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(*theIterator));
        if (!js) {
            return false;
        }
        if (begin) {
            begin = false;
            x = js->getX();
            continue;
        }
        
        int jx = js->getX();
        if ((x+count)==jx) {
            count++;
        }
    }
    if (count>=3) {
        return true;
    }
    return false;
}
bool JewelDatahandle::f_yListIsContinue(std::list<int> &list){
    list.sort();
    std::list<int>::iterator theIterator;
    int count= 1;
    bool begin = true;
    int y =0;
    JewelSprite * js = NULL;
    for (theIterator = list.begin(); theIterator!=list.end(); theIterator++) {
        
        js= dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(*theIterator));
        if (!js) {
            return false;
        }
        if (begin) {
            begin = false;
            y = js->getY();
            continue;
        }
        int jy = js->getY();
        if ((y+count)==jy) {
            count++;
        }
    }
    if (count>=3) {
        return true;
    }
    return false;
}


void JewelDatahandle::f_doDisappearWithList(std::list<int> &list){// 
    
    
    SKILLTYPE skillType;
    int count = list.size();
    
    printf("begin do dissappear withCount:%d\n",count);
    
    std::list<int>::iterator theIterator;
    
    theIterator = --list.end();
    int theEnd = *theIterator;
    for (theIterator = list.begin();
         theIterator != list.end();
         theIterator++) {
        
        JewelSprite * tJ = dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(*theIterator));
        
        skillType = tJ->getSkillType();
        printf(" ,%d " ,skillType);
        if (tJ){
            
            if (*theIterator == theEnd) {
                tJ->f_disappear(true);
            }else{
                
                tJ->f_disappear(false);
            }
        }
    }
    printf(" end \n");
}

int JewelDatahandle::f_calcIndex(int x, int y){
    
    return (y * TOTALX )+ x;
}

bool JewelDatahandle::f_dropdown(){
    
    if (m_checkDisappearList.size()>0) {
        m_checkDisappearList.clear();
    }
    
    for (int x = 0; x < TOTALX; x++) {
        
        for (int y = 0; y < TOTALY; y++) {
            
            int index = f_calcIndex(x, y);
            
            JewelSprite *js = dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(index));
            
            if (js &&
                js->getDisappear()) {
                m_checkDisappearList.push_back(index);
            }
            if (!js->getDisappear()) {
                f_calcDropDown(index);
            }
        }
        
    }
    
    m_checkDisappearList.sort();
    m_checkDisappearList.unique();
    
    if (m_checkDisappearList.size()>0) {
        
        f_doDropDown();
        return false;
    }
    return true;
}

void JewelDatahandle::f_calcDropDown(int index){
    
    int tIndex = index;
    
    while (true) {
        
        JewelSprite *js = dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(tIndex));
        
        if (!js) {
            return;
        }
        
        int sX = js->getX();
        int sY = js->getY();
        int sI = js->getIndex();
        
        if (sY==0) {
            
            break;
        }
        
        int tI = f_calcIndex(sX, sY - 1);
        
        JewelSprite *jsdown = dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(tI));
        
        if (!jsdown) {
            CCLog("error Down JewelSprite is NULL!"); break;
        }
        
        if(jsdown->getDisappear()){
            f_exchangeTwoJewelSprite( sI, false, tI, false,true);
        }else{
            
            break;
        }
        
        tIndex = js->getIndex();
    }
}

void JewelDatahandle::f_doDropDown(){
    
    std::list<int>::iterator theIterator;
    
    theIterator = --m_checkDisappearList.end();
    int theEnd = *theIterator;
    
    for (theIterator = m_checkDisappearList.begin();
         theIterator != m_checkDisappearList.end();
         theIterator++) {
        
        JewelSprite *js = dynamic_cast<JewelSprite*>(m_stateArray->objectAtIndex(*theIterator));
        
        if (!js) {
            CCLog("error js JewelSprite is NULL!"); continue;
        }
        
        if (*theIterator!=theEnd) {
            js->f_dropDown(false);
        }else{
            js->f_dropDown(true);
        }
    }
}