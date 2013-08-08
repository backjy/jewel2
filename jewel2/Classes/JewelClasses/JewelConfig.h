//
//  JewelConfig.h
//  jewel2
//
//  Created by CpyShine on 13-5-22.
//
//

#ifndef jewel2_JewelConfig_h
#define jewel2_JewelConfig_h

#define TOTALX 6
#define TOTALY 5

#define TOTALSKILL 6

#define moveInterval 1

#define TABLE_ONE_COUNT 3
#define TABLE_TWO_COUNT 3

enum EXCHANGEDIRECTION{//
  DIR_NONE      = 0,
    DIR_RIGHT   = 1,
    DIR_UP      = 2,
    DIR_LEFT    = 3,
    DIR_DOWN    = 4,
};

enum SKILLTYPE {
    SKILL_ONE = 0,
    SKILL_TWO,
    SKILL_THREE,
    SKILL_FOUR,
    SKILL_FIVE,
    SKILL_SIX
};

class JewelDisappearSpawnSkill_Protocol {
    
public:
    virtual void f_SpwanSkill(SKILLTYPE type) = 0;
};

#endif
