#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"

// #define DEBUG
enum ItemType { ANTIDOTE = 0, PHOENIXI, PHOENIXII, PHOENIXIII, PHOENIXIV };
class BaseItem;
class Antidote; class PhoenixDownI; class PhoenixDownII; class PhoenixDownIII; class PhoenixDownIV;
class BaseBag;
class BaseOpponent;
class NormalKnight;
class BaseKnight;
class ArmyKnights;
struct Node {
    string item;
    Node* next;
    ItemType type;
    BaseItem* Item;

};
class BaseBag {
public:
    string item;
    BaseBag* nextPtr;
    int n = 0;
    int phoenixdownI, phoenixdownII, phoenixdownIII, phoenixdownIV;
    int antidote;
    //ItemType itemType;
    Node* headPtr = nullptr;
    void insertitem(Node*& head, string newitem, ItemType type, BaseItem* item) {
        Node* bag = new Node;
        bag->item = newitem;
        bag->next = nullptr;
        bag->type = type;
        bag->Item = item;
        if (head == nullptr) {
            head = bag;
        }
        else {
            bag->next = head;
            head = bag;
        }
        this->n++;
    }
    void clear(Node*& headPtr) {
        if (headPtr != nullptr) {
            auto currentPtr = headPtr;
            Node* deletePtr = nullptr;
            while (currentPtr != nullptr) {
                deletePtr = currentPtr;
                currentPtr = currentPtr->next;
                delete deletePtr;
                this->n--;
            }
            headPtr = nullptr;
        }
    }
    int checkstt(ItemType itemType) {
        auto current = headPtr;
        int stt = 1;
    //    cout << stt << current->type << '\n';
        while (current != nullptr) {
          //  cout << stt << current->type << '\n';
            if (current->type == itemType)   return stt;
            current = current->next;
            stt++;
        }
        return 0;
    }
    void useitem(ItemType itemType) {
        auto prePtr = headPtr;
        auto afterheadPtr = headPtr->next;
        auto currentPtr = headPtr->next;
        auto a = headPtr;
        Node* deletePtr = nullptr;
        if (headPtr->type == itemType) {
            deletePtr = headPtr;
            headPtr = headPtr->next;
            delete deletePtr;
            this->n--;
        }
        else if (headPtr->next->type == itemType) {
            a->next = afterheadPtr->next;
            afterheadPtr->next = a;
            headPtr = afterheadPtr;
            deletePtr = headPtr;
            headPtr = headPtr->next;
            delete deletePtr;
            this->n--;
        }
        else
            while (currentPtr != nullptr) {
                if (currentPtr->type == itemType) {
                    auto afcurrentPtr = currentPtr->next;
                    headPtr = currentPtr;
                    currentPtr = a;
                    headPtr->next = afterheadPtr;
                    prePtr->next = currentPtr;
                    currentPtr->next = afcurrentPtr;
                    deletePtr = headPtr;
                    headPtr = headPtr->next;
                    delete deletePtr;
                    this->n--;
                    break;
                }
                prePtr = prePtr->next;
                currentPtr = currentPtr->next;
            }
    }
    void dropitem() {
        Node* deletePtr = nullptr;
        if (this->n > 3) {
            for (int i = 0; i < 3; i++) {
                deletePtr = headPtr;
                headPtr = headPtr->next;
                delete deletePtr;
                this->n--;
            }
        }
        else {
            this->clear(this->headPtr); }
    }
    ~BaseBag() {
        clear(headPtr);
    }
    virtual bool insertFirst(BaseItem* item) = 0;
    virtual BaseItem* get(ItemType itemType) = 0;
    virtual string toString() const = 0;
};
class BaseOpponent {
public:
    virtual bool danhquai(ArmyKnights* knights, int levelO) = 0;
};


class Events {
public:
    int soluong, sk[10000];
    Events() {
    };
    Events(const string& file_events) {
        //  cout << file_events;
        ifstream input;
        input.open(file_events);
        input >> this->soluong;
        for (int i = 0; i < soluong; i++) { input >> sk[i]; }
        input.close();
    }
    int count() const {
        return soluong;
    }
    int get(int i) const {
        return this->sk[i];
    }
};
enum KnightType { PALADIN = 0, LANCELOT, DRAGON, NORMAL };
class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;
    BaseBag* bag;
    KnightType knightType;

public:
    int currenthp() { return hp; }
    int MAXHP() { return maxhp; }
    int currentlv() { return level; }
    int currentgil() { return gil; }
    void changegil(int newgil) { gil = newgil; }
    void changelv(int newlv) { level = newlv; }
    void changehp(int newhp) { hp = newhp; }
    int currentantidote() { return antidote; }
    BaseBag* currentbag() { return bag; }
    void changeantidote(int newantidote) { antidote = newantidote; }
    static BaseKnight* create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    string toString() const;
    KnightType knighttype() { return knightType; }
    bool usingphoenix();
    ~BaseKnight() {
        delete bag;
    }
};
class BaseItem {
public:
    ItemType itemType;
    virtual bool canUse(BaseKnight* knight) = 0;
    virtual void use(BaseKnight* knight) = 0;
};
class Antidote :public BaseItem {
public:
    bool canUse(BaseKnight* knight) {
         if (knight->currentbag()->get(ANTIDOTE) != nullptr ) return 1;
        else return 0;
    }
    void use(BaseKnight* knight) {
        knight->currentbag()->useitem(ANTIDOTE);
    }
};
class PhoenixDownI :public BaseItem {
    bool canUse(BaseKnight* knight) {
        if (knight->currentbag()->get(PHOENIXI) != nullptr && knight->currenthp() <= 0) return 1;
        else return 0;
    }
    void use(BaseKnight* knight) {
        knight->currentbag()->useitem(PHOENIXI);
        knight->changehp(knight->MAXHP());
    }
};
class PhoenixDownII :public BaseItem {
    bool canUse(BaseKnight* knight) {
        if (knight->currentbag()->get(PHOENIXII) != nullptr && knight->currenthp() < knight->MAXHP() / 4) return 1;
        else return 0;
    }
    void use(BaseKnight* knight) {
        knight->currentbag()->useitem(PHOENIXII);
        knight->changehp(knight->MAXHP());
    }
};
class PhoenixDownIII :public BaseItem {
    bool canUse(BaseKnight* knight) {
        if (knight->currentbag()->get(PHOENIXIII) != nullptr && knight->currenthp() < (knight->MAXHP() / 3)) return 1;
        else return 0;
    }
    void use(BaseKnight* knight) {
        knight->currentbag()->useitem(PHOENIXIII);
        if (knight->currenthp() <= 0) knight->changehp(knight->MAXHP() / 3);
        if (knight->currenthp() < knight->MAXHP() / 3 && knight->currenthp() > 0) knight->changehp(knight->currenthp() + knight->MAXHP() / 4);
    }
};
class PhoenixDownIV :public BaseItem {
    bool canUse(BaseKnight* knight) {
        if (knight->currentbag()->get(PHOENIXIV) != nullptr && knight->currenthp() < knight->MAXHP() / 2) return 1;
        else return 0;
    }
    void use(BaseKnight* knight) {
        knight->currentbag()->useitem(PHOENIXIV);
        if (knight->currenthp() <= 0) knight->changehp(knight->MAXHP() / 2);
        if (knight->currenthp() < knight->MAXHP() / 2 && knight->currenthp() > 0) knight->changehp(knight->currenthp() + knight->MAXHP() / 5);
    }
};


class PaladinBag :public BaseBag {
public:
    bool insertFirst(BaseItem* item) {
        if (item->itemType == 0) {
       
            insertitem(headPtr, "Antidote", ANTIDOTE, item);
        }
        if (item->itemType == 1) {
          
            insertitem(headPtr, "PhoenixI", PHOENIXI, item);
        }
        if (item->itemType == 2) {
            
            insertitem(headPtr, "PhoenixII", PHOENIXII, item);
        }
        if (item->itemType == 3) {

            insertitem(headPtr, "PhoenixIII", PHOENIXIII, item);
        }
        if (item->itemType == 4) {
        
            insertitem(headPtr, "PhoenixIV", PHOENIXIV, item);
        }

        return 1;
    }
    BaseItem* get(ItemType itemType) {
        auto current = headPtr;
        while (current != nullptr) {
            if (current->type == itemType)   return current->Item;
            current = current->next;
        }
        return nullptr;
    }
    string toString() const {
        string s("");
        s += "Bag[count=" + to_string(n) + ";";
        Node* current = headPtr;
        while (current != nullptr) {
            s += current->item;
            current = current->next;
            if (current != nullptr) s += ",";
        }
        s += "]";
        return s;

    }
    PaladinBag(BaseKnight* knight, int phoenixdownI, int antidote) {
        for (int i = 0; i < phoenixdownI; i++) {
            PhoenixDownI* item = new PhoenixDownI;
            insertitem(headPtr, "PhoenixI", PHOENIXI, item);
        }
        for (int i = 0; i < antidote; i++) {
            Antidote* item = new Antidote;
            insertitem(headPtr, "Antidote", ANTIDOTE, item);
        }
        this->toString();
    }
};
class LancelotBag :public BaseBag {
public:
    bool insertFirst(BaseItem* item) {
        if (n < 16) {
            if (item->itemType == 0) {
                item = new Antidote;
                insertitem(headPtr, "Antidote", ANTIDOTE, item);
            }
            if (item->itemType == 1) {
           
                insertitem(headPtr, "PhoenixI", PHOENIXI, item);
            }
            if (item->itemType == 2) {
          
                insertitem(headPtr, "PhoenixII", PHOENIXII, item);
            }
            if (item->itemType == 3) {
               
                insertitem(headPtr, "PhoenixIII", PHOENIXIII, item);
            }
            if (item->itemType == 4) {
       
                insertitem(headPtr, "PhoenixIV", PHOENIXIV, item);
            }

            return 1;
        }
        else return 0;
    }
    BaseItem* get(ItemType itemType) {
        auto current = headPtr;
        while (current != nullptr) {
            if (current->type == itemType)   return current->Item;
            current = current->next;
        }
        return nullptr;
    }
    string toString() const {
        string s("");
        s += "Bag[count=" + to_string(n) + ";";
        Node* current = headPtr;
        while (current != nullptr) {
            s += current->item;
            current = current->next;
            if (current != nullptr) s += ",";
        }
        s += "]";
        return s;

    }
    LancelotBag(BaseKnight* knight, int phoenixdownI, int antidote) {
        for (int i = 0; i < phoenixdownI; i++) {
            PhoenixDownI* item = new PhoenixDownI;
            insertitem(headPtr, "PhoenixI", PHOENIXI, item);
        }
        for (int i = 0; i < antidote; i++) {
            Antidote* item = new Antidote;
            insertitem(headPtr, "Antidote", ANTIDOTE, item);
        }
        this->toString();

    }
};
class DragonBag :public BaseBag {
public:
    bool insertFirst(BaseItem* item) {
        if (n < 14) {
          
            if (item->itemType == 0) {
             
                insertitem(headPtr, "Antidote", ANTIDOTE, item);
            }
            if (item->itemType == 1) {
       
                insertitem(headPtr, "PhoenixI", PHOENIXI, item);
            }
            if (item->itemType == 2) {
             
                insertitem(headPtr, "PhoenixII", PHOENIXII, item);
            }
            if (item->itemType == 3) {
              
                insertitem(headPtr, "PhoenixIII", PHOENIXIII, item);
            }
            if (item->itemType == 4) {
        
                insertitem(headPtr, "PhoenixIV", PHOENIXIV, item);
            }

            return 1;
        }
        else return 0;
    }
    BaseItem* get(ItemType itemType) {
        auto current = headPtr;
        while (current != nullptr) {
            if (current->type == itemType)   return current->Item;
            current = current->next;
        }
        return nullptr;
    }
    string toString() const {
        string s("");
        s += "Bag[count=" + to_string(n) + ";";
        Node* current = headPtr;
        while (current != nullptr) {
            s += current->item;
            current = current->next;
            if (current != nullptr) s += ",";
        }
        s += "]";

        return s;

    }
    DragonBag(BaseKnight* knight, int phoenixdownI, int antidote) {
        for (int i = 0; i < phoenixdownI; i++) {
            PhoenixDownI* item = new PhoenixDownI;
            insertitem(headPtr, "PhoenixI", PHOENIXI, item);
        }
       
        this->toString();

    }
};
class NormalBag :public BaseBag {
public:
    bool insertFirst(BaseItem* item) {
        if (n < 19) {
            if (item->itemType == 0) {
               
                insertitem(headPtr, "Antidote", ANTIDOTE, item);
            }
            if (item->itemType == 1) {
          
                insertitem(headPtr, "PhoenixI", PHOENIXI, item);
            }
            if (item->itemType == 2) {
               
                insertitem(headPtr, "PhoenixII", PHOENIXII, item);
            }
            if (item->itemType == 3) {
               
                insertitem(headPtr, "PhoenixIII", PHOENIXIII, item);
            }
            if (item->itemType == 4) {
               
                insertitem(headPtr, "PhoenixIV", PHOENIXIV, item);
            }

            return 1;
        }
        else return 0;
    }
    BaseItem* get(ItemType itemType) {
        auto current = headPtr;
        while (current != nullptr) {
            if (current->type == itemType)   return current->Item;
            current = current->next;
        }
        return nullptr;
    }
    string toString() const {
        string s("");
        s += "Bag[count=" + to_string(n) + ';';
        Node* current = headPtr;
        while (current != nullptr) {
            s += current->item;
            current = current->next;
            if (current != nullptr) s += ',';
        }
        s += ']';
        return s;

    }
    NormalBag(BaseKnight* knight, int phoenixdownI, int antidote) {
        for (int i = 0; i < phoenixdownI; i++) {
            PhoenixDownI* item = new PhoenixDownI;
            insertitem(headPtr, "PhoenixI", PHOENIXI, item);
        }
        for (int i = 0; i < antidote; i++) {
            Antidote* item = new Antidote;
            insertitem(headPtr, "Antidote", ANTIDOTE, item);
        }
        this->toString();
    }
};


class PaladinKnight :public BaseKnight {
public:
    PaladinKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
        this->id = id;
        this->maxhp = maxhp;
        this->level = level; this->gil = gil; this->antidote = antidote; this->hp = maxhp;
        this->knightType = PALADIN;
        this->bag = new PaladinBag(this, phoenixdownI, antidote);
    }
};
class LancelotKnight :public BaseKnight {
public:
    LancelotKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
        this->id = id;
        this->maxhp = maxhp;
        this->level = level; this->gil = gil; this->antidote = antidote; this->hp = maxhp;
        this->knightType = LANCELOT;
        this->bag = new LancelotBag(this, phoenixdownI, antidote);
    }
};
class DragonKnight :public BaseKnight {
public:
    DragonKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
        this->id = id;
        this->maxhp = maxhp;
        this->level = level; this->gil = gil; this->antidote = antidote; this->hp = maxhp;
        this->knightType = DRAGON;
        this->bag = new DragonBag(this, phoenixdownI, antidote);
    }
};
class NormalKnight :public BaseKnight {
public:
    NormalKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI) {
        this->id = id;
        this->maxhp = maxhp;
        this->level = level; this->gil = gil; this->antidote = antidote;
        this->hp = maxhp;
        this->knightType = NORMAL;
        this->bag = new NormalBag(this, phoenixdownI, antidote);
    }
};





class ArmyKnights {
public:
    bool omega = 0, shield = 0, spear = 0, hair = 0, sword = 0, hades = 0;
    int sohs, id, maxhp, level, antidote, phoenixdownI, gil;
    int currentid = 0;
    int normallive = 0;
    int normalfirstid = 0;
    int id99 = 0;
    BaseKnight** knight;
    int levelO = 0;
    void newlevelO(int newlvO) { this->levelO = newlvO; }
    ArmyKnights(const string& file_armyknights) {
        ifstream input;
        input.open(file_armyknights);
        input >> sohs;
        currentid = sohs;
        knight = new BaseKnight * [sohs];
        id = 1;
        for (int i = 0; i < sohs; i++) {
            input >> maxhp >> level >> phoenixdownI >> gil >> antidote;
            knight[i] = BaseKnight::create(id, maxhp, level, gil, antidote, phoenixdownI);
            //cout << knight[i]->toString() << '\n';
            // cout << knight[i]->currenthp() << '\n';
            id++;
        }
        input.close();
    };
    ~ArmyKnights() {
        for (int i = 0; i < sohs; i++) {
            delete knight[i];
        }
        delete[] knight;
    };
    bool fight(BaseOpponent* opponent) {
        if (knight[currentid - 1]->currenthp() <= 0) return 0;
        if (opponent->danhquai(this, levelO) == 1) {        
            return 1; }
        else return 0;
    };
    bool adventure(Events* events) {
        for (int i = 0; i < events->soluong; i++) {
            events->get(i);
            this->printInfo();
        }
        return 1;
    };
    int count() const {
       if (normallive > 0) return (this->currentid + this ->normallive);
        return this->currentid;
    };
    BaseKnight* lastKnight() const {
        if (normallive > 0) return knight[id99 - 1];
        return knight[currentid - 1];
    };
    bool hasPaladinShield() const {
        if (this->shield == 1) return 1;
        else return 0;
    };
    bool hasLancelotSpear() const {
        if (this->spear == 1) return 1;
        else return 0;
    };
    bool hasGuinevereHair() const {
        if (this->hair == 1) return 1;
        else return 0;;
    };
    bool hasExcaliburSword() const {
        if (this->sword == 1) return 1;
        else return 0;
    };
    void printInfo() const;
    void printResult(bool win) const;
};


class MadBear : public BaseOpponent {
public:
    MadBear() {}
    bool danhquai(ArmyKnights* knights, int levelO) {
        int gil_conlai = 0;
        int k = 0;
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int lv = knights->knight[knights->currentid - 1]->currentlv();
        if (lv >= knights->levelO || knights->knight[knights->currentid - 1]->knighttype() == LANCELOT || knights->knight[knights->currentid - 1]->knighttype() == PALADIN) {
            gil += 100;
            while (gil > 999) {
                gil_conlai = gil - 999;
                if (gil_conlai == 0) break;
                knights->knight[knights->currentid - k - 1]->changegil(999);
                if (knights->currentid - k - 1 == 0) break;
                else k++;
                gil = knights->knight[knights->currentid - k - 1]->currentgil();
                gil += gil_conlai;
            }
            if (gil > 999) gil = 999;
            knights->knight[knights->currentid - k - 1]->changegil(gil);
            
        }
        else {
            hp = hp - 10 * (levelO - lv);
            knights->knight[knights->currentid - 1]->changehp(hp);
            if (knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                knights->knight[knights->currentid - 1]->usingphoenix();
               // cout << knights->knight[knights->currentid - 1]->currenthp() << '\n';
                if (gil >= 100 && knights->knight[knights->currentid - 1]->currenthp() <=0) {
                        knights->knight[knights->currentid - 1]->changegil(gil - 100);
                        knights->knight[knights->currentid - 1]->changehp(knights->knight[knights->currentid - 1]->MAXHP() / 2);
                }
                if (knights->knight[knights->currentid - 1]->currenthp() <= 0)   return 0;
            }
            else knights->knight[knights->currentid - 1]->usingphoenix();
        }
       
        return 1;
    }
};
class Bandit : public BaseOpponent {
public:
    Bandit() {}
    bool danhquai(ArmyKnights* knights, int levelO) {
        int gil_conlai = 0;
        int k = 0;
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int lv = knights->knight[knights->currentid - 1]->currentlv();
        if (lv >= knights->levelO || knights->knight[knights->currentid - 1]->knighttype() == LANCELOT || knights->knight[knights->currentid - 1]->knighttype() == PALADIN) {
            gil += 150;
            while (gil > 999) {
                gil_conlai = gil - 999;
                if (gil_conlai == 0) break;
                knights->knight[knights->currentid - k - 1]->changegil(999);
                if (knights->currentid - k - 1 == 0) break;
                else k++;
                gil = knights->knight[knights->currentid - k - 1]->currentgil();
                gil += gil_conlai;
            }
            if (gil > 999) gil = 999;
            knights->knight[knights->currentid - k - 1]->changegil(gil);

        }
        else {
            hp = hp - 15 * (levelO - lv);
            knights->knight[knights->currentid - 1]->changehp(hp);
            if (knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                knights->knight[knights->currentid - 1]->usingphoenix();
                if (gil >= 100 && knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                    knights->knight[knights->currentid - 1]->changegil(gil - 100);
                    knights->knight[knights->currentid - 1]->changehp(knights->knight[knights->currentid - 1]->MAXHP() / 2);
                }
                if (knights->knight[knights->currentid - 1]->currenthp() <= 0)   return 0;
            }
            else knights->knight[knights->currentid - 1]->usingphoenix();
        }

        return 1;
    }
};
class LordLupin : public BaseOpponent {
public:
    LordLupin() {}
    bool danhquai(ArmyKnights* knights, int levelO) {
        int gil_conlai = 0;
        int k = 0;
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int lv = knights->knight[knights->currentid - 1]->currentlv();
        if (lv >= knights->levelO || knights->knight[knights->currentid - 1]->knighttype() == LANCELOT || knights->knight[knights->currentid - 1]->knighttype() == PALADIN) {
            gil += 450;
            while (gil > 999) {
                gil_conlai = gil - 999;
                if (gil_conlai == 0) break;
                knights->knight[knights->currentid - k - 1]->changegil(999);
                if (knights->currentid - k - 1 == 0) break;
                else k++;
                gil = knights->knight[knights->currentid - k - 1]->currentgil();
                gil += gil_conlai;
            }
            if (gil > 999) gil = 999;
            knights->knight[knights->currentid - k - 1]->changegil(gil);        
        } 
        else {
            hp = hp - 45 * (levelO - lv);
            knights->knight[knights->currentid - 1]->changehp(hp);
            knights->knight[knights->currentid - 1]->changehp(hp);
            if (knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                knights->knight[knights->currentid - 1]->usingphoenix();
                if (gil >= 100 && knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                    knights->knight[knights->currentid - 1]->changegil(gil - 100);
                    knights->knight[knights->currentid - 1]->changehp(knights->knight[knights->currentid - 1]->MAXHP() / 2);
                }
                if (knights->knight[knights->currentid - 1]->currenthp() <= 0)   return 0;
            }
            else knights->knight[knights->currentid - 1]->usingphoenix();
        }

        return 1;
    }
};
class Elf : public BaseOpponent {
public:
    Elf() {}
    bool danhquai(ArmyKnights* knights, int levelO) {
        int gil_conlai = 0;
        int k = 0;
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int lv = knights->knight[knights->currentid - 1]->currentlv();
        if (lv >= knights->levelO || knights->knight[knights->currentid - 1]->knighttype() == LANCELOT || knights->knight[knights->currentid - 1]->knighttype() == PALADIN) {
            gil += 750;
            while (gil > 999) {
                gil_conlai = gil - 999;
                if (gil_conlai == 0) break;
                knights->knight[knights->currentid - k - 1]->changegil(999);
                if (knights->currentid - k - 1 == 0) break;
                else k++;
                gil = knights->knight[knights->currentid - k - 1]->currentgil();
                gil += gil_conlai;
            }
            if (gil > 999) gil = 999;
            knights->knight[knights->currentid - k - 1]->changegil(gil);
           
        }
        else {
            hp = hp - 75 * (levelO - lv);
            knights->knight[knights->currentid - 1]->changehp(hp);
            knights->knight[knights->currentid - 1]->changehp(hp);
            if (knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                knights->knight[knights->currentid - 1]->usingphoenix();
                if (gil >= 100 && knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                    knights->knight[knights->currentid - 1]->changegil(gil - 100);
                    knights->knight[knights->currentid - 1]->changehp(knights->knight[knights->currentid - 1]->MAXHP() / 2);
                }
                if (knights->knight[knights->currentid - 1]->currenthp() <= 0)   return 0;
            }
            else knights->knight[knights->currentid - 1]->usingphoenix();
        }

        return 1;
    }
};
class Troll : public BaseOpponent {
public:
    Troll() {}
    bool danhquai(ArmyKnights* knights, int levelO) {
        int gil_conlai = 0;
        int k = 0;
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int lv = knights->knight[knights->currentid - 1]->currentlv();
        if (lv >= knights->levelO || knights->knight[knights->currentid - 1]->knighttype() == LANCELOT || knights->knight[knights->currentid - 1]->knighttype() == PALADIN) {
            gil += 800;
            while (gil > 999) {
                gil_conlai = gil - 999;
                if (gil_conlai == 0) break;
                knights->knight[knights->currentid - k - 1]->changegil(999);
                if (knights->currentid - k - 1 == 0) break;
                else k++;
                gil = knights->knight[knights->currentid - k - 1]->currentgil();
                gil += gil_conlai;
            }
            if (gil > 999) gil = 999;
            knights->knight[knights->currentid - k - 1]->changegil(gil);
    
        }
        else {
            hp = hp - 95 * (levelO - lv);
            knights->knight[knights->currentid - 1]->changehp(hp);
            if (knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                knights->knight[knights->currentid - 1]->usingphoenix();
                if (gil >= 100 && knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                    knights->knight[knights->currentid - 1]->changegil(gil - 100);
                    knights->knight[knights->currentid - 1]->changehp(knights->knight[knights->currentid - 1]->MAXHP() / 2);
                }
                if (knights->knight[knights->currentid - 1]->currenthp() <= 0)   return 0;
            }
            else knights->knight[knights->currentid - 1]->usingphoenix();
        }

        return 1;
    }
};
class Tornbery : public BaseOpponent {
public:
    Tornbery() {}
    bool danhquai(ArmyKnights* knights, int levelO) {
        
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int lv = knights->knight[knights->currentid - 1]->currentlv();
        if (lv >= knights->levelO) {
            if (lv < 10) knights->knight[knights->currentid - 1]->changelv(lv + 1);
           
        }
        else if (knights->knight[knights->currentid - 1]->knighttype() != DRAGON) {
            BaseItem* doc = new Antidote;
            if (doc->canUse(knights->knight[knights->currentid - 1]) == 1) {
                doc->use(knights->knight[knights->currentid - 1]);
                delete doc;
            }
            else {
                delete doc;
                knights->knight[knights->currentid - 1]->currentbag()->dropitem();
                // cout << hp << '\n';
                hp -= 10;
                knights->knight[knights->currentid - 1]->changehp(hp);
                if (knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                    knights->knight[knights->currentid - 1]->usingphoenix();
                    if (gil >= 100 && knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                        knights->knight[knights->currentid - 1]->changegil(gil - 100);
                        knights->knight[knights->currentid - 1]->changehp(knights->knight[knights->currentid - 1]->MAXHP() / 2);
                    }
                    if (knights->knight[knights->currentid - 1]->currenthp() <= 0)   return 0;
                }
                else knights->knight[knights->currentid - 1]->usingphoenix();
            }
        }
            return 1;
    }
};
class QueenOfCards : public BaseOpponent {
public:
    QueenOfCards() {    }
    bool danhquai(ArmyKnights* knights, int levelO) {
        int gil_conlai = 0;
        int k = 0;
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int lv = knights->knight[knights->currentid - 1]->currentlv();
        if (lv >= knights->levelO) {
            gil *= 2;
            while (gil > 999) {
                gil_conlai = gil - 999;
                if (gil_conlai == 0) break;
                knights->knight[knights->currentid - k - 1]->changegil(999);
                if (knights->currentid - k - 1 == 0) break;
                else k++;
                gil = knights->knight[knights->currentid - k - 1]->currentgil();
                gil += gil_conlai;
            }
            if (gil > 999) gil = 999;
            knights->knight[knights->currentid - k - 1]->changegil(gil);
           
            return 1;
        }
        else if (knights->knight[knights->currentid - 1]->knighttype() != PALADIN) {
            gil /= 2;
            knights->knight[knights->currentid - k - 1]->changegil(gil);
        }
      
        return 1;
    }

};
class NinaDeRings : public BaseOpponent {
public:
    NinaDeRings() { }
    bool danhquai(ArmyKnights* knights, int levelO) {
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        if (gil < 50 && knights->knight[knights->currentid - 1]->knighttype() != PALADIN)
        {
          
            return 1;
        }
        else if (hp < knights->knight[knights->currentid - 1]->MAXHP() / 3) {
            if (knights->knight[knights->currentid - 1]->knighttype() == PALADIN)
            {
                knights->knight[knights->currentid - 1]->changehp(hp + knights->knight[knights->currentid - 1]->MAXHP() / 5);
            }
            else {
                knights->knight[knights->currentid - 1]->changegil(gil - 50);
                knights->knight[knights->currentid - 1]->changehp(hp + knights->knight[knights->currentid - 1]->MAXHP() / 5);
            }
           
            return 1;
        }

        return 1;
    }
};
class DurianGarden : public BaseOpponent {
public:
    DurianGarden() { }
    bool danhquai(ArmyKnights* knights, int levelO) {
        knights->knight[knights->currentid - 1]->changehp(knights->knight[knights->currentid - 1]->MAXHP());
        return 1;
    }
};
class OmegaWeapon : public BaseOpponent {
public:
    OmegaWeapon() {}
    bool danhquai(ArmyKnights* knights, int levelO) {
        if (knights->omega == 1) return 1;
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int lv = knights->knight[knights->currentid - 1]->currentlv();
        if ((lv == 10 && hp == knights->knight[knights->currentid - 1]->MAXHP()) || knights->knight[knights->currentid - 1]->knighttype() == DRAGON) {
            knights->knight[knights->currentid - 1]->changelv(10);
            knights->knight[knights->currentid - 1]->changegil(999);
            knights->omega = 1;
        }
        else {
            knights->knight[knights->currentid - 1]->changehp(0);
 
            if (knights->knight[knights->currentid - 1]->currenthp() <= 0) {
           
                knights->knight[knights->currentid - 1]->usingphoenix();
               
                if (gil >= 100 && knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                    knights->knight[knights->currentid - 1]->changegil(gil - 100);
                    knights->knight[knights->currentid - 1]->changehp(knights->knight[knights->currentid - 1]->MAXHP() / 2);
                }
                if (knights->knight[knights->currentid - 1]->currenthp() <= 0)   return 0;
            }
            else knights->knight[knights->currentid - 1]->usingphoenix();
        }

        return 1;
    }
};
class Hades : public BaseOpponent {
public:
    Hades() {  }
    bool danhquai(ArmyKnights* knights, int levelO) {
        if (knights->hades == 1) return 1;
        int gil = knights->knight[knights->currentid - 1]->currentgil();
        int hp = knights->knight[knights->currentid - 1]->currenthp();
        int lv = knights->knight[knights->currentid - 1]->currentlv();
        if (lv == 10 || (knights->knight[knights->currentid - 1]->knighttype() == PALADIN && lv >= 8)) {
            knights->shield = 1;
            knights->hades = 1;
        }
        else {
            knights->knight[knights->currentid - 1]->changehp(0);
            if (knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                knights->knight[knights->currentid - 1]->usingphoenix();
                if (gil >= 100 && knights->knight[knights->currentid - 1]->currenthp() <= 0) {
                    knights->knight[knights->currentid - 1]->changegil(gil - 100);
                    knights->knight[knights->currentid - 1]->changehp(knights->knight[knights->currentid - 1]->MAXHP() / 2);
                }
                if (knights->knight[knights->currentid - 1]->currenthp() <= 0)   return 0;
            }
            else knights->knight[knights->currentid - 1]->usingphoenix();
        }

        return 1;
    }
};


class KnightAdventure {
private:
    ArmyKnights* armyKnights;
    Events* events;

public:
    KnightAdventure() ;
    ~KnightAdventure() {
        delete armyKnights;
        delete events;
    }; // TODO:

    void loadArmyKnights(const string& file_armyknights) {
        armyKnights = new ArmyKnights(file_armyknights);

    };
    void loadEvents(const string& file_events) {
        events = new Events(file_events);

    };
    void run() {
       
       // cout << 1;
        int hpUlti = 5000;
        //cout << events->soluong;
        for (int i = 0; i < events->soluong; i++) {
          //     cout << i <<' '<< events->get(i) << '\n';
            int newlv = (i + events->get(i)) % 10 + 1;
            armyKnights->newlevelO(newlv);
            //      cout << armyKnights->levelO << '\n';
            if (events->get(i) > 0 && events->get(i) <= 11) {
                BaseOpponent* opponent = nullptr;
                if (events->get(i) == 1)  opponent = new MadBear;
                if (events->get(i) == 2)  opponent = new Bandit;
                if (events->get(i) == 3)  opponent = new LordLupin;
                if (events->get(i) == 4)  opponent = new Elf;
                if (events->get(i) == 5)  opponent = new Troll;
                if (events->get(i) == 6)  opponent = new Tornbery;
                if (events->get(i) == 7)  opponent = new QueenOfCards;
                if (events->get(i) == 8)  opponent = new NinaDeRings;
                if (events->get(i) == 9)  opponent = new DurianGarden;
                if (events->get(i) == 10)  opponent = new OmegaWeapon;
                if (events->get(i) == 11)  opponent = new Hades;
                if (armyKnights->fight(opponent) != 1 || armyKnights ->knight[armyKnights->currentid-1]->currenthp()<=0) {
                 //   cout << armyKnights->knight[armyKnights->currentid - 1]->currenthp() << '\n';
                    armyKnights->currentid--;
                 //   cout << armyKnights->currentid << '\n';                
                }
                delete  opponent;
            }
           // cout << armyKnights->currentid << '\n';
            if (events->get(i) == 95) armyKnights->shield = 1;
            if (events->get(i) == 96) armyKnights->spear = 1;
            if (events->get(i) == 97) armyKnights->hair = 1;
            if (events->get(i) == 98 && armyKnights->shield == 1 && armyKnights->spear == 1 && armyKnights->hair == 1) armyKnights->sword = 1;
            if (events->get(i) >= 112 && events->get(i) <= 114) {
                int q = armyKnights->currentid;
                BaseItem* item = nullptr;
                if (events->get(i) == 112) { item = new PhoenixDownII; item->itemType = PHOENIXII; }
                if (events->get(i) == 113) { item = new PhoenixDownIII; item->itemType = PHOENIXIII; }
                if (events->get(i) == 114) { item = new PhoenixDownIV; item->itemType = PHOENIXIV; }
                while (armyKnights->knight[q - 1]->currentbag()->insertFirst(item) == 0 && q >= 1)q--;
            }
            bool firstid = 0;
         //   int q = 0;
            if (events->get(i) == 99) {
            //    int idnow = armyKnights->currentid;
                if (armyKnights->sword != 1) {
                    if (armyKnights->shield == 1 && armyKnights->spear == 1 && armyKnights->hair == 1) {
                        while (armyKnights->currentid >= 1) {
                            int damage = 0; 
                           
                            if (armyKnights->knight[armyKnights->currentid - 1]->knighttype() == NORMAL) {
                                if (firstid == 0) {
                                    firstid = 1; armyKnights->normalfirstid = armyKnights->currentid;
                                }
                                armyKnights->normallive++;
                              //  cout << armyKnights->normallive << ' ' << armyKnights->currentid << '\n';
                                if (armyKnights->currentid >= 1) armyKnights->currentid--;
                                else break;
                            }
                            else {
                                if (armyKnights->knight[armyKnights->currentid - 1]->knighttype() == LANCELOT) {
                                    damage = armyKnights->knight[armyKnights->currentid - 1]->currenthp() * armyKnights->knight[armyKnights->currentid - 1]->currentlv() * 0.05;
                                    hpUlti -= damage;
                                }
                                else if (armyKnights->knight[armyKnights->currentid - 1]->knighttype() == PALADIN) {
                                    damage = armyKnights->knight[armyKnights->currentid - 1]->currenthp() * armyKnights->knight[armyKnights->currentid - 1]->currentlv() * 0.06;
                                    hpUlti -= damage;
                                }
                                else if (armyKnights->knight[armyKnights->currentid - 1]->knighttype() == DRAGON) {
                                    damage = armyKnights->knight[armyKnights->currentid - 1]->currenthp() * armyKnights->knight[armyKnights->currentid - 1]->currentlv() * 0.075;
                                    hpUlti -= damage;
                                }

                              //  cout << q << ' ' << armyKnights->currentid << ' ' << damage << ' ' << hpUlti << '\n';
                             //   q++;

                                if (hpUlti > 0 && armyKnights->currentid >= 1)  armyKnights->currentid--;
                                else if (hpUlti <= 0 && armyKnights->currentid >= 1) {
                                    if (armyKnights->normalfirstid > armyKnights->currentid) armyKnights->id99 = armyKnights->normalfirstid;
                                    else armyKnights->id99 = armyKnights->currentid;
                                    break;
                                }
                                else      break;
                            }
                        }
                        if (hpUlti > 0) { 
                            armyKnights->normallive = 0;
                            armyKnights->currentid = 0; }
                    }
                    else armyKnights->currentid = 0;
                }
                else hpUlti = 0;
            }

            //    if (armyKnights->currentid>=2)  cout << armyKnights->knight[armyKnights->currentid - 2]->toString() << '\n' << '\n';
          //  cout << armyKnights->normalfirstid << ' ' << hpUlti << '\n';
          //  if (armyKnights->currentid - 1>=0 && armyKnights->knight[armyKnights->currentid -1]->currenthp()<=0) armyKnights->currentid--;
            armyKnights->printInfo();
            if (armyKnights->count() == 0) { armyKnights->printResult(0);  break; }
            if (events->get(i) == 99) {
                if (hpUlti > 0)  armyKnights->printResult(0);
                else armyKnights->printResult(1);
            }
            else if (i == events->soluong -1) {
                if (armyKnights->count() > 0) armyKnights->printResult(1);
            }
 
        }

    }
};

#endif // __KNIGHT2_H__