#include "knight2.h"

/* * * BEGIN implementation of class BaseBag * * */

/* * * END implementation of class BaseBag * * */
BaseKnight* (BaseKnight::create)(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
    KnightType knighttype;
    int k = 0;
    BaseKnight* knight = new BaseKnight;
    knighttype = NORMAL;
    for (int i = 2; i < maxhp; i++) {
        if (maxhp % i == 0) { k = 0; break; }
        else k = 1;
    }
    if (maxhp == 2) k = 1;
    if (k == 1) knighttype = PALADIN;
    if (maxhp == 888) knighttype = LANCELOT;
    int a, b, c;
    if (maxhp >= 100) {
        a = maxhp / 100;
        b = (maxhp - a * 100) / 10;
        c = maxhp % 10;
        if ((a * a == b * b + c * c || b * b == a * a + c * c || c * c == a * a + b * b)&& a>0 &&b>0&&c>0) knighttype = DRAGON;
    }
    if (knighttype == PALADIN) knight = new PaladinKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    if (knighttype == LANCELOT) knight = new LancelotKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    if (knighttype == DRAGON) knight = new DragonKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    if (knighttype == NORMAL) knight = new NormalKnight(id, maxhp, level, gil, antidote, phoenixdownI);

    return knight;
};
/* * * BEGIN implementation of class BaseKnight * * */
string BaseKnight::toString() const {
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    // inefficient version, students can change these code
    //      but the format output must be the same

    string s("");
    s += "[Knight:id:" + to_string(id) 
        + ",hp:" + to_string(hp) 
        + ",maxhp:" + to_string(maxhp)
        + ",level:" + to_string(level)
        + ",gil:" + to_string(gil)
        + "," + bag->toString()
        + ",knight_type:" + typeString[knightType]
        + "]";
    return s;
}

/* * * END implementation of class BaseKnight * * */
bool BaseKnight::usingphoenix() {
    int stt[5] = { -1, -1, -1, -1, -1 };
    BaseItem* phoenix1 = new PhoenixDownI;
    BaseItem* phoenix2 = new PhoenixDownII;
    BaseItem* phoenix3 = new PhoenixDownIII;
    BaseItem* phoenix4 = new PhoenixDownIV;
    if (phoenix1->canUse(this) == 1) { stt[1] = this->bag->checkstt(PHOENIXI); }
    if (phoenix2->canUse(this) == 1) { stt[2] = this->bag->checkstt(PHOENIXII); }
    if (phoenix3->canUse(this) == 1) { stt[3] = this->bag->checkstt(PHOENIXIII); }
    if (phoenix4->canUse(this) == 1) { stt[4] = this->bag->checkstt(PHOENIXIV); }
    int min = stt[1];
    for (int i = 2; i < 5; i++) { 
     if (min == -1) min = stt[i];
    else  if (min > stt[i] && stt[i] >= 0) min = stt[i]; 
    }
    if (min >= 0) {
        if (min == stt[1]) phoenix1->use(this);
        if (min == stt[2]) phoenix2->use(this);
        if (min == stt[3]) { phoenix3->use(this); }
        if (min == stt[4]) phoenix4->use(this);
       delete phoenix1;
        delete phoenix2;
        delete phoenix3;
        delete phoenix4;
       
        return 1;
    }
    return 0;
}
/* * * BEGIN implementation of class ArmyKnights * * */
void ArmyKnights::printInfo() const {
    cout << "No. knights: " << this->count();
    if (this->count() > 0) {
        BaseKnight * lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50, '-') << endl;
}

void ArmyKnights::printResult(bool win) const {
    cout << (win ? "WIN" : "LOSE") << endl;
}

/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class KnightAdventure * * */
KnightAdventure::KnightAdventure() {
    armyKnights = nullptr;
    events = nullptr;
}

/* * * END implementation of class KnightAdventure * * */