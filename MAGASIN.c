#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
typedef struct date {
    int j;
    int m;
    int a;
} DATE;

typedef struct mvt {
    DATE d;
    char codeMvt;
    int qt;
    struct mvt *suivm;
} listeMVT;

typedef struct produit {
    int CP;
    char lib[15];
    int QT;
    float PU;
    int QTseuil;
    listeMVT *mvt;
    struct produit *svt;
} PROD;
bool seuil(PROD *nov) {
    return nov->QT >= nov->QTseuil;
}

PROD* cherche_code(PROD *t, int code) {
    PROD *p = t;
    while (p != NULL && p->CP != code) {
        p = p->svt;
    }
    return p;
}

PROD* cree_prod() {
    PROD *nov = malloc(sizeof(PROD));

    if (nov == NULL) {
        printf("Erreur allocation memoire !");
        return NULL;
    }
    nov->mvt = NULL;
    nov->svt = NULL;
    printf("\nDonner le code du produit : ");
    scanf("%d", &nov->CP);
    printf("Donner le libelle : ");
    scanf("%14s", nov->lib);
    printf("Donner le prix unitaire : ");
    scanf("%f", &nov->PU);
    do {
        printf("Donner la quantite en stock : ");
        scanf("%d", &nov->QT);
        printf("Donner la quantite seuil : ");
        scanf("%d", &nov->QTseuil);
        if (nov->QT < 0 || nov->QTseuil < 0 ||
            !seuil(nov)) {
            printf("Erreur : stock doit etre >= au seuil.\n");
        }
    } while (nov->QT < 0 || nov->QTseuil < 0 ||
             !seuil(nov));
    return nov;
}
void ajout_en_tete(PROD **t) {
    PROD *nov;
    int code;
    nov = cree_prod();
    if (nov == NULL)
        return;
    code = nov->CP;
    if (cherche_code(*t, code) != NULL) {
        printf("Ce code existe deja !\n");
        free(nov);
        return;
    }
    nov->svt = *t;
    *t = nov;
    printf("Produit ajoute avec succes !\n");
}

void affichage(PROD *t) {
    PROD *p = t;
    if(t == NULL){
    	printf("\n la liste des produits est vide ");
	}
	else{
		printf("\n");
    printf(" ----------------------------------------------------\n");
    printf("| Code | Libelle       | Prix    | Stock   | Seuil   |\n");
    printf(" ----------------------------------------------------\n");
    while (p != NULL) {
        printf("| %-4d | %-13s | %-7.2f | %-7d | %-7d |\n",
               p->CP, p->lib, p->PU, p->QT, p->QTseuil);

        p = p->svt;
    }
	printf(" ----------------------------------------------------\n");

	}
    }

void ajout_mvt(PROD *t) {
    int code;
    PROD *p;
    listeMVT *m;
    char rep;
    printf("\nDonner le code du produit : ");
    scanf("%d", &code);
    p = cherche_code(t, code);
    if (p == NULL) {
        printf("Produit introuvable !\n");
        return;
    }
    m = malloc(sizeof(listeMVT));
    if (m == NULL) {
        printf("Erreur allocation memoire !\n");
        return;
    }
    printf("\nDonner la date (jour mois annee) : ");
    scanf("%d%d%d", &m->d.j, &m->d.m, &m->d.a);
    printf("Type de mouvement (E = Entree / S = Sortie) : ");
    scanf(" %c", &rep);

    m->codeMvt = toupper(rep);

    if (m->codeMvt != 'E' && m->codeMvt != 'S') {
        printf("Type de mouvement invalide !\n");
        free(m);
        return;
    }

    printf("Donner la quantite : ");
    scanf("%d", &m->qt);

    if (m->qt <= 0) {
        printf("Quantite invalide !\n");
        free(m);
        return;
    }
    if (m->codeMvt == 'E') {
        p->QT = p->QT + m->qt;
    }
    else {
        if (p->QT - m->qt < p->QTseuil) {
            printf("Operation impossible !\n");
            printf("Le stock ne peut pas etre inferieur au seuil.\n");
            free(m);
            return;
        }
        p->QT = p->QT - m->qt;
    }
    m->suivm = p->mvt;
    p->mvt = m;
    printf("\nMouvement enregistre avec succes !\n");
    printf("Nouveau stock : %d\n", p->QT);
}

void affichage_mvt(PROD *t) {
    int code;
    PROD *p;
    listeMVT *m;
    printf("\nDonner le code du produit : ");
    scanf("%d", &code);
    p = cherche_code(t, code);
    if (p == NULL) {
        printf("Produit introuvable !\n");
        return;
    }
    m = p->mvt;
    printf("\nHistorique du produit : %s\n", p->lib);
    printf("-------------------------------------------\n");
    printf("| Date       | Type       | Quantite       |\n");
    printf("-------------------------------------------\n");
    if (m == NULL) {
        printf("Aucun mouvement pour ce produit.\n");
    }
    while (m != NULL) {
        printf("| %02d/%02d/%04d | %-10c | %-14d |\n",
               m->d.j, m->d.m, m->d.a,
               m->codeMvt, m->qt);
        m = m->suivm;
    }
    printf("-------------------------------------------\n");
}

void afficher_un_produit(PROD *t) {
    int code;
    PROD *p;
    printf("\nDonner le code a rechercher : ");
    scanf("%d", &code);
    p = cherche_code(t, code);
    if (p == NULL) {
        printf("Produit introuvable !\n");
    }
    else {
        printf("\nCode : %d", p->CP);
        printf("\nLibelle : %s", p->lib);
        printf("\nPrix : %.2f", p->PU);
        printf("\nStock : %d", p->QT);
        printf("\nSeuil : %d\n", p->QTseuil);
        if (p->QT <= p->QTseuil) {
            printf("ATTENTION : Stock au seuil !\n");
        }
    }
}
void liberer(PROD *t) {
    PROD *p;
    listeMVT *m;
    listeMVT *tmp;
    while (t != NULL) {
        p = t;
        t = t->svt;
        m = p->mvt;
        while (m != NULL) {
            tmp = m;
            m = m->suivm;
            free(tmp);
        }
        free(p);
    }
}
void sauvegarder(PROD *t) {
    FILE *fprod;
    FILE *fmvt;
    PROD *p = t;
    listeMVT *m;
    fprod = fopen("produits.txt", "w");
    fmvt = fopen("mouvements.txt", "w");
    if (fprod == NULL || fmvt == NULL) {
        printf("Erreur ouverture fichier !");
        if (fprod != NULL) fclose(fprod);
        if (fmvt != NULL) fclose(fmvt);
        return;
    }
    while (p != NULL) {
        fprintf(fprod, "%d %s %d %.2f %d\n",
                p->CP, p->lib, p->QT,
                p->PU, p->QTseuil);
        m = p->mvt;
        while (m != NULL) {
            fprintf(fmvt,"%d,%d,%d,%d,%c,%d\n",
                    p->CP,
                    m->d.j, m->d.m, m->d.a,
                    m->codeMvt, m->qt);
            m = m->suivm;
        }
        p = p->svt;
    }
    fclose(fprod);
    fclose(fmvt);
    printf("\nSauvegarde effectuee avec succes !");
}



int main() {
    PROD *t = NULL;
    int choix;
    do {
        printf("\n\n========== GESTION DE MAGASIN ==========\n");
        printf("0. Sauvgarder les modification\n");
        printf("1. Ajouter un produit\n");
        printf("2. Afficher tous les produits\n");
        printf("3. Rechercher un produit\n");
        printf("4. Enregistrer un mouvement de stock\n");
        printf("5. Afficher l'historique des mouvements\n");
        printf("6. Quitter\n");
        printf("========================================\n");
        printf("Donner votre choix : ");
        scanf("%d", &choix);
        switch (choix) {
        	case 0:
    			sauvegarder(t);
    			break;
            case 1:
                ajout_en_tete(&t);
                break;
            case 2:
                affichage(t);
                break;
            case 3:
                afficher_un_produit(t);
                break;
            case 4:
                ajout_mvt(t);
                break;
            case 5:
                affichage_mvt(t);
                break;
            case 6:
                sauvegarder(t);
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide !\n");
        }
    } while (choix !=6);
    liberer(t);
    return 0;
}

