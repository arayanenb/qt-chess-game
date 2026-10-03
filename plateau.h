#ifndef PLATEAU_H
#define PLATEAU_H
#include <vector>
using namespace std;

struct mov
{
	int ld,cd,la,ca;
};

class plateau
{
	public:
		plateau();
		~plateau();
		void initPlateau();
		void afficher();
		int eval(int ld,int cd,int la,int ca);
		void deplacerPiece(int ld,int cd,int la,int ca);
		int eval_pion(int ld,int cd,int la,int ca);
		int eval_tour(int ld,int cd,int la,int ca);
		int eval_cav(int ld,int cd,int la,int ca);
		int eval_fou(int ld,int cd,int la,int ca);
		int eval_roi(int ld,int cd,int la,int ca);
		int eval_dame(int ld,int cd,int la,int ca);
		int couleur_piece(int l,int c);
		int chemin_libre(int ld,int cd,int la,int ca);
        int get_tour();
		int verif_tour(int,int);
		void deplacerIA();
		void scan_mp_IA();
		int eval_mp_IA(vector<mov> &tm);
        bool estEnEchec(int couleur);
        bool estEchecEtMat(int couleur);
        bool estPat(int couleur);
    public:  // Changed from private to public for GUI access
        int plat[8][8];
    private:
        int tour;
        mov tm[1000];
        int cp;
};

#endif
