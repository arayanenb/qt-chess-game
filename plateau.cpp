#include "plateau.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>


using namespace std;

plateau::plateau()
{
    cp = 0;
    tour = 1;
    srand(time(NULL));
    initPlateau();
}

plateau::~plateau()
{
}

void plateau::initPlateau()
{
    for (int i = 2; i <= 5; i++)
        for (int j = 0; j <= 7; j++)
            plat[i][j] = 0;

    for (int j = 0; j <= 7; j++)
    {
        plat[1][j] = -1;
        plat[6][j] = 1;
    }

    plat[0][0] = -2; plat[0][1] = -3; plat[0][2] = -4;
    plat[0][3] = -5; plat[0][4] = -6; plat[0][5] = -4;
    plat[0][6] = -3; plat[0][7] = -2;

    plat[7][0] = 2; plat[7][1] = 3; plat[7][2] = 4;
    plat[7][3] = 5; plat[7][4] = 6; plat[7][5] = 4;
    plat[7][6] = 3; plat[7][7] = 2;
}

void plateau::afficher()
{
    cout << "    0    1    2    3    4    5    6    7" << endl;
    cout << "  +---------------------------------------------------" << endl;
    for (int i = 0; i <= 7; i++)
    {
        cout << i << " |";
        for (int j = 0; j <= 7; j++)
        {
            cout << " ";
            switch (plat[i][j])
            {
            case 0:  cout << "  "; break;
            case 1:  cout << "PB"; break;
            case 2:  cout << "TB"; break;
            case 3:  cout << "CB"; break;
            case 4:  cout << "FB"; break;
            case 5:  cout << "DB"; break;
            case 6:  cout << "RB"; break;
            case -1: cout << "PN"; break;
            case -2: cout << "TN"; break;
            case -3: cout << "CN"; break;
            case -4: cout << "FN"; break;
            case -5: cout << "DN"; break;
            case -6: cout << "RN"; break;
            }
            cout << " |";
        }
        cout << endl;
        cout << "  +---------------------------------------------------" << endl;
    }
}

int plateau::couleur_piece(int l, int c)
{
    if (plat[l][c] == 0) return -1;
    if (plat[l][c] > 0) return 1;
    return 0;
}

int plateau::get_tour()
{
    return tour;
}

int plateau::verif_tour(int ld, int cd)
{
    if ((get_tour() % 2 != 0) && (couleur_piece(ld, cd) == 1)) return 1;
    if ((get_tour() % 2 == 0) && (couleur_piece(ld, cd) == 0)) return 1;
    return 0;
}

int plateau::chemin_libre(int ld, int cd, int la, int ca)
{
    int dl = 0, dc = 0;

    if (la > ld) dl = 1;
    else if (la < ld) dl = -1;

    if (ca > cd) dc = 1;
    else if (ca < cd) dc = -1;

    int l = ld + dl;
    int c = cd + dc;

    while (l != la || c != ca)
    {
        if (l < 0 || l > 7 || c < 0 || c > 7) return 0;
        if (plat[l][c] != 0) return 0;
        l += dl;
        c += dc;
    }
    return 1;
}

int plateau::eval(int ld, int cd, int la, int ca)
{
    if (ld < 0 || ld > 7 || cd < 0 || cd > 7 ||
        la < 0 || la > 7 || ca < 0 || ca > 7)
        return 0;

    if (plat[ld][cd] == 0) return 0;

    if (plat[la][ca] != 0 && couleur_piece(ld, cd) == couleur_piece(la, ca))
        return 0;

    switch (plat[ld][cd])
    {
    case 1: case -1: return eval_pion(ld, cd, la, ca);
    case 2: case -2: return eval_tour(ld, cd, la, ca);
    case 3: case -3: return eval_cav(ld, cd, la, ca);
    case 4: case -4: return eval_fou(ld, cd, la, ca);
    case 5: case -5: return eval_dame(ld, cd, la, ca);
    case 6: case -6: return eval_roi(ld, cd, la, ca);
    }
    return 0;
}

int plateau::eval_pion(int ld, int cd, int la, int ca)
{
    int imp = 0;
    switch (plat[ld][cd])
    {
    case 1:
        if (la == ld - 1 && ca == cd && plat[la][ca] == 0)
            imp = 1;
        if (ld == 6 && la == ld - 2 && ca == cd && plat[la][ca] == 0 && plat[ld - 1][cd] == 0)
            imp = 1;
        if (la == ld - 1 && (ca == cd + 1 || ca == cd - 1) && plat[la][ca] < 0)
            imp = 1;
        break;

    case -1:
        if (la == ld + 1 && ca == cd && plat[la][ca] == 0)
            imp = 1;
        if (ld == 1 && la == ld + 2 && ca == cd && plat[la][ca] == 0 && plat[ld + 1][cd] == 0)
            imp = 1;
        if (la == ld + 1 && (ca == cd + 1 || ca == cd - 1) && plat[la][ca] > 0)
            imp = 1;
        break;
    }
    return imp;
}

int plateau::eval_tour(int ld, int cd, int la, int ca)
{
    if (ld == la || cd == ca)
        return chemin_libre(ld, cd, la, ca);
    return 0;
}

int plateau::eval_cav(int ld, int cd, int la, int ca)
{
    if (la < 0 || la > 7 || ca < 0 || ca > 7) return 0;

    bool vert = (la == ld + 2 && ca == cd + 1) || (la == ld - 2 && ca == cd + 1) ||
                (la == ld + 2 && ca == cd - 1) || (la == ld - 2 && ca == cd - 1);

    bool hori = (la == ld + 1 && ca == cd + 2) || (la == ld + 1 && ca == cd - 2) ||
                (la == ld - 1 && ca == cd + 2) || (la == ld - 1 && ca == cd - 2);

    return (vert || hori) ? 1 : 0;
}

int plateau::eval_fou(int ld, int cd, int la, int ca)
{
    if (abs(ld - la) == abs(cd - ca))
        return chemin_libre(ld, cd, la, ca);
    return 0;
}

int plateau::eval_roi(int ld, int cd, int la, int ca)
{
    if (plat[ld][cd] == 6 && ld == 7 && la == 7 && ca == cd + 2 &&
        plat[7][7] == 2 && chemin_libre(ld, cd, 7, 7))
        return 1;

    if (plat[ld][cd] == 6 && ld == 7 && la == 7 && ca == cd - 2 &&
        plat[7][0] == 2 && chemin_libre(ld, cd, 7, 0))
        return 1;

    if (plat[ld][cd] == -6 && ld == 0 && la == 0 && ca == cd + 2 &&
        plat[0][7] == -2 && chemin_libre(ld, cd, 0, 7))
        return 1;

    if (plat[ld][cd] == -6 && ld == 0 && la == 0 && ca == cd - 2 &&
        plat[0][0] == -2 && chemin_libre(ld, cd, 0, 0))
        return 1;

    bool vertical = (la == ld + 1 || la == ld - 1);
    bool horizontal = (ca == cd + 1 || ca == cd - 1);
    bool diagonal = (abs(ld - la) == 1 && abs(cd - ca) == 1);

    return (vertical || horizontal || diagonal) ? 1 : 0;
}

int plateau::eval_dame(int ld, int cd, int la, int ca)
{
    if (eval_fou(ld, cd, la, ca) == 1 || eval_tour(ld, cd, la, ca) == 1)
        return 1;
    return 0;
}

void plateau::deplacerPiece(int ld, int cd, int la, int ca)
{
    if (plat[ld][cd] == 6 && ca == cd + 2 && la == 7)
    {
        plat[7][5] = 2;
        plat[7][7] = 0;
    }
    else if (plat[ld][cd] == 6 && ca == cd - 2 && la == 7)
    {
        plat[7][3] = 2;
        plat[7][0] = 0;
    }
    else if (plat[ld][cd] == -6 && ca == cd + 2 && la == 0)
    {
        plat[0][5] = -2;
        plat[0][7] = 0;
    }
    else if (plat[ld][cd] == -6 && ca == cd - 2 && la == 0)
    {
        plat[0][3] = -2;
        plat[0][0] = 0;
    }

    plat[la][ca] = plat[ld][cd];
    plat[ld][cd] = 0;

    // AJOUTER ICI - Promotion du pion
    if (plat[la][ca] == 1 && la == 0) {
        plat[la][ca] = 5; // Promotion en Dame (Blanc)
    }
    if (plat[la][ca] == -1 && la == 7) {
        plat[la][ca] = -5; // Promotion en Dame (Noir)
    }

    tour++;
}

void plateau::scan_mp_IA()
{
    cp = 0;
    // L'IA joue toujours les pièces noires (couleur 0)
    // MAIS on doit vérifier que le mouvement ne met pas le roi en échec
    for (int i = 0; i <= 7; i++)
    {
        for (int j = 0; j <= 7; j++)
        {
            // Ne prendre que les pièces noires (couleur 0)
            if (couleur_piece(i, j) != 0) continue;

            for (int k = 0; k <= 7; k++)
            {
                for (int l = 0; l <= 7; l++)
                {
                    if (eval(i, j, k, l) == 1)
                    {
                        // AJOUTER CETTE VÉRIFICATION - Le mouvement ne doit pas mettre le roi en échec
                        // Simuler le mouvement
                        int pieceCapturee = plat[k][l];
                        int pieceDeplacee = plat[i][j];
                        plat[k][l] = pieceDeplacee;
                        plat[i][j] = 0;

                        bool roiEnEchec = estEnEchec(0); // Vérifier pour les noirs (couleur 0)

                        // Annuler le mouvement
                        plat[i][j] = pieceDeplacee;
                        plat[k][l] = pieceCapturee;

                        // Si le mouvement ne met pas le roi en échec, on l'ajoute
                        if (!roiEnEchec && cp < 1000)
                        {
                            tm[cp].ld = i;
                            tm[cp].cd = j;
                            tm[cp].la = k;
                            tm[cp].ca = l;
                            cp++;
                        }
                    }
                }
            }
        }
    }
}
void plateau::deplacerIA()
{
    scan_mp_IA();

    if (cp == 0)
    {
        cout << "Aucun mouvement possible pour l'IA !" << endl;
        tour++;
        return;
    }

    // Remplacer l'index aléatoire par une sélection intelligente
    int meilleurScore = -9999;
    int meilleurIndex = 0;

    for (int i = 0; i < cp; i++) {
        // Simuler le mouvement
        int pieceCapturee = plat[tm[i].la][tm[i].ca];
        int pieceDeplacee = plat[tm[i].ld][tm[i].cd];
        plat[tm[i].la][tm[i].ca] = pieceDeplacee;
        plat[tm[i].ld][tm[i].cd] = 0;

        // Calculer le score (plus il est élevé, meilleur est le mouvement)
        int score = 0;
        int valeurs[7] = {0, 1, 5, 3, 3, 9, 100};

        // Bonus pour la capture
        if (pieceCapturee != 0) {
            score += valeurs[abs(pieceCapturee)] * 10;
        }

        // Bonus pour la promotion
        if (pieceDeplacee == -1 && tm[i].la == 7) {
            score += 80; // Promotion en dame
        }

        // Bonus pour avancer les pions
        if (pieceDeplacee == -1) {
            score += (tm[i].la - tm[i].ld) * 2; // Avancer est bon
        }

        // Pénalité pour les mouvements de roi au début
        if (abs(pieceDeplacee) == 6 && tour < 10) {
            score -= 50;
        }

        if (score > meilleurScore) {
            meilleurScore = score;
            meilleurIndex = i;
        }

        // Annuler le mouvement
        plat[tm[i].ld][tm[i].cd] = pieceDeplacee;
        plat[tm[i].la][tm[i].ca] = pieceCapturee;
    }

    // Exécuter le meilleur mouvement
    plat[tm[meilleurIndex].la][tm[meilleurIndex].ca] =
        plat[tm[meilleurIndex].ld][tm[meilleurIndex].cd];
    plat[tm[meilleurIndex].ld][tm[meilleurIndex].cd] = 0;

    // Gérer la promotion du pion noir
    if (plat[tm[meilleurIndex].la][tm[meilleurIndex].ca] == -1 && tm[meilleurIndex].la == 7) {
        plat[tm[meilleurIndex].la][tm[meilleurIndex].ca] = -5; // Promotion en Dame
    }

    tour++;
}

bool plateau::estEnEchec(int couleur) {
    // Trouver la position du roi
    int roiRow = -1, roiCol = -1;
    int roiValue = (couleur == 1) ? 6 : -6;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (plat[i][j] == roiValue) {
                roiRow = i;
                roiCol = j;
                break;
            }
        }
        if (roiRow != -1) break;
    }

    if (roiRow == -1) return true; // Roi capturé

    // Vérifier si une pièce adverse menace le roi
    int adversaire = (couleur == 1) ? 0 : 1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (couleur_piece(i, j) == adversaire) {
                if (eval(i, j, roiRow, roiCol)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool plateau::estEchecEtMat(int couleur) {
    if (!estEnEchec(couleur)) return false;

    // Vérifier si un mouvement peut sortir de l'échec
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (couleur_piece(i, j) == couleur) {
                for (int k = 0; k < 8; k++) {
                    for (int l = 0; l < 8; l++) {
                        if (eval(i, j, k, l)) {
                            // Simuler le mouvement
                            int pieceCapturee = plat[k][l];
                            int pieceDeplacee = plat[i][j];
                            plat[k][l] = pieceDeplacee;
                            plat[i][j] = 0;

                            bool toujoursEchec = estEnEchec(couleur);

                            // Annuler le mouvement
                            plat[i][j] = pieceDeplacee;
                            plat[k][l] = pieceCapturee;

                            if (!toujoursEchec) return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool plateau::estPat(int couleur) {
    if (estEnEchec(couleur)) return false;

    // Vérifier si au moins un mouvement est possible
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (couleur_piece(i, j) == couleur) {
                for (int k = 0; k < 8; k++) {
                    for (int l = 0; l < 8; l++) {
                        if (eval(i, j, k, l)) {
                            // Simuler le mouvement
                            int pieceCapturee = plat[k][l];
                            int pieceDeplacee = plat[i][j];
                            plat[k][l] = pieceDeplacee;
                            plat[i][j] = 0;

                            bool enEchec = estEnEchec(couleur);

                            // Annuler le mouvement
                            plat[i][j] = pieceDeplacee;
                            plat[k][l] = pieceCapturee;

                            if (!enEchec) return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}