#ifndef __THOMPSONNA_H
#define __THOMPSONNA_H
#include "regexp.h"
#include <vector>
#include <set>
using std::vector;
const uchar epsilon=0;
class ZoekNA;
//ThompsonNA: met een begintoestand (index 0)
//en een eindtoestand (index aantalToestanden-1)
//Alleen gebruikt om algoritme ven Thompson te implementeren

class Verbinding{
    friend class ThompsonNA;
    public:
        Verbinding(int _bron,int _doel,uchar _a);                             ;
        int geefBron() const;
        int geefDoel() const;
        int geefKarakter() const;
        friend ostream& operator<<(ostream& os, const Verbinding& verbinding);

    private:
        int bron;
        int doel;
        uchar a;
};

class ThompsonNA{
    //friend class ZoekNA;
    public:
        ThompsonNA(const Regexp& re);
        void schrijf(ostream& os) const;

        int geefAantalVerbindingen() const;
        int geefAantalToestanden() const;
        const Verbinding& operator[](int i) const;

    protected:
        //haalt de Verbindingen van TNA binnen, maar verschuift de knoopnrs;
        void haalbinnen(const ThompsonNA& TNA,int verschuiving);
        void verbind(int bron, int doel, uchar c);
        int aantalToestanden;
        vector<Verbinding> overgang;
};


Verbinding::Verbinding(int _bron,int _doel,uchar _a):bron(_bron),doel(_doel),a(_a){}

int Verbinding::geefBron() const{
    return bron;
}

int Verbinding::geefDoel() const{
    return doel;
}

int Verbinding::geefKarakter() const{
    return a;
}

ostream& operator<<(ostream& os, const Verbinding& verbinding) {
    os << verbinding.bron << " " << verbinding.doel << " ";
    if (verbinding.a==epsilon) {
        os<<"epsilon";
    } else {
        os<< verbinding.a;
    }

    return os;
}





















int ThompsonNA::geefAantalVerbindingen() const{
    return overgang.size();
}

int ThompsonNA::geefAantalToestanden() const{
    return aantalToestanden;
}

const Verbinding& ThompsonNA::operator[](int i) const{
    return overgang[i];
}


void ThompsonNA::schrijf(ostream& os) const{
    os<<"aantal toestanden: "<<aantalToestanden<<" aantal verbindingen: "<<geefAantalVerbindingen()<<'\n';
    for (int i=0; i<overgang.size() ;i++){
        os<<overgang[i].bron<<' '
          <<overgang[i].doel<<' ';
        if (overgang[i].a==epsilon)
            os<<"epsilon"<<'\n';
        else
            os<<overgang[i].a<<'\n';
    }
    os<<"--------\n";
}

void ThompsonNA::haalbinnen(const ThompsonNA& TNA,int verschuiving){
    for (int i=0; i<TNA.overgang.size() ;i++ ){
        const Verbinding& oud=TNA.overgang[i];
        overgang.push_back(Verbinding(oud.bron+verschuiving,
                oud.doel+verschuiving,oud.a));
    }
}
void ThompsonNA::verbind(int bron, int doel, uchar c=epsilon){
    overgang.push_back(Verbinding(bron,doel,c));
}


ThompsonNA::ThompsonNA(const Regexp& re){
    switch (re.geefOpcode()){
     case Regexp::letter:
         aantalToestanden=2;
         verbind(0,1,re.geefLetter());
     break;
     case Regexp::plus:{
            ThompsonNA op1(*re.geefEersteOperand());
            ThompsonNA op2(*re.geefTweedeOperand());
            //beginknoop tweede operand overlapt met eindknoop eerste
            aantalToestanden=op1.aantalToestanden+op2.aantalToestanden-1;
            haalbinnen(op1, 0);
            haalbinnen(op2, op1.aantalToestanden-1);
        }
        break;
        case Regexp::of:{
            ThompsonNA op1(*re.geefEersteOperand());
            ThompsonNA op2(*re.geefTweedeOperand());
            aantalToestanden=op1.aantalToestanden+op2.aantalToestanden+2;
            haalbinnen(op1, 1);
            haalbinnen(op2, 1+op1.aantalToestanden);
            verbind(0, 1);
            verbind(0, op1.aantalToestanden+1);
            verbind(op1.aantalToestanden, aantalToestanden-1);
            verbind(aantalToestanden-2, aantalToestanden-1);
        }
        break;
        case Regexp::ster:{
            ThompsonNA op1(*re.geefEersteOperand());
            aantalToestanden=op1.aantalToestanden+2;
            haalbinnen(op1, 1);
            verbind(0, 1);
            verbind(0, aantalToestanden-1);
            verbind(aantalToestanden-2, aantalToestanden-1);
            verbind(aantalToestanden-2, 1);
        }
    }
};

#endif

