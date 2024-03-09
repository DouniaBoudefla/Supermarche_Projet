#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>


typedef struct produit{
    char nom[64];
    double prix_unit;
    int stocks;
} produit;

typedef struct ID_ET_PROD{ // Structure pour directement relier les produits à leurs id
    int ID;
    produit objet;
}ID_ET_PROD;

typedef struct item_panier{
    int idProd;
    double quantite;
    struct item_panier* suivant;
} item_panier;


typedef struct panier{
    struct item_panier* first;
    struct tm* date_ticket;
    double total;
    int ticketid;
    int clientid;
} panier;

typedef struct tm tm;

void charge(ID_ET_PROD tab[], int n, char* nomFichier){
  FILE* fich=fopen(nomFichier,"r");
  fread(tab,sizeof(ID_ET_PROD),n,fich);
  fclose(fich);
}

typedef struct infos{  // Structure qui contient les informations pour le fichier de Compta
    int date[3];
    int ticketId;
    int clientId;
    double total;
} infos;

panier* init_panier(int id){ // Nouveau panier
    panier* p=malloc(sizeof(panier));
    p->first=NULL;
    p->total=0;
    p->clientid=id;
    return p;
}

void ajout_prod_panier(panier* p, produit* prod, int idProd, double quantite){ // Ajout d'un produit dans le panier
    if(prod->stocks > quantite){   // 0 remplacer par quantite
        item_panier* new=malloc(sizeof(item_panier));
        new->idProd=idProd;
        new->quantite=quantite;
        p->total+=prod->prix_unit * quantite;
        prod->stocks=prod->stocks - quantite;
        new->suivant=p->first;
        p->first=new;
    }
    else{
        printf("Il n'y a plus de %s\n",prod->nom);
    }
}

void retirer_prod_panier(panier* p, produit* prod, int idProd, double quantite){ // Retirer un produit dans le panier
    item_panier* new=p->first;
    for(int i=0; i<idProd-1;i++){
        new=new->suivant;
    }
    free(new->suivant);
    new->suivant=new->suivant->suivant;
    prod->stocks=prod->stocks + (quantite*prod->prix_unit);
}

void panier_free(panier* p){ // Libération de la mémoire allouée pour un panier
  item_panier* i=p->first;
  while(i != NULL){
    free(i);
    i=i->suivant;
  }
  free(p);
}

int main(){

   //On stocke le catalogue de produits dans un tableau de produits
    FILE *fich = fopen("Catalogue.txt","r");

    // On ouvre le fichier pour la compta
    FILE* fich2=fopen("Compta.csv","a+");

    int taille =200;  
   //On crée un tableau qui va contenir le fichier
    ID_ET_PROD *tab= malloc(taille*sizeof(ID_ET_PROD));

    // On enregistre le fichier dans le tableau
    charge(tab,taille,"Catalogue.txt");
    
    // Initialisation des variables pour que la boucle recommence
    int x = 666; 
    int m = 666;
    
    do{ // Boucle pour recommencer le main
        printf("\e[1;1H\e[2J\n"); // Effacer ecran
        printf("Dans quel mode voulez vous entrez ?\n\n1) Mode caissier\t2) Mode gérant\t\t0) Quitter\n");
        char n[64];
        char *u;
        u = n;  // Pour régler le bug des réponses de type char
        gets(u);
        int x=atoi(n);
        
        while(x != 1 && x != 2 && x != 0){ // Boucle si l'utilisateur se trompe
            printf("Veuillez entrer un nombre valide (1/2/0)\n");
            gets(u);
            x=atoi(n);
        }
        
        if(x == 0){ // Cas fermeture du prog
            return 0;
        }

        if(x == 1){
            int menu1=1; // Variable pour revenir au menu principal
                printf("\e[1;1H\e[2J");
                do{
                    menu1=1; // Variable pour revenir au menu principal
                    printf("\n\nBienvenue dans le mode caissier !\n\n");
                    printf("Appuyez sur\n");
                    printf("1) Pour démarrer un nouveau ticket\n");
                    printf("2) Pour revenir au menu principal\n");

                    char n[500];
                    char *u;
                    u = n;
                    gets(u);
                    int Verif=atoi(n);
                    while(Verif != 1 && Verif != 2){
                        printf("Veuillez entrer un nombre valide (1/2)\n");
                        gets(u);
                        Verif=atoi(n); 
                    }
                    if(Verif==2){
                        FILE* fichier=fopen("Catalogue.txt","w");
                        fwrite(tab,sizeof(ID_ET_PROD),200,fichier);
                        menu1=0;
                    }
                    if(Verif==1){

                        printf("\e[1;1H\e[2J");
                        printf("Veuillez entrer votre numéro de client (Tapez -1 si vous ne possedez pas de numéro de client) :\n");
                        int clientid;
                        scanf("%d",&clientid);
                        panier* p=init_panier(clientid);

                        int quitter=0;
                        while(quitter != 100){
                    
                            //printf("1 : Ajouter un produit\t\t100 : Finir votre panier\n2 : Retirer un produit de votre panier\n");
                            printf("Entrer un numéro de produit :\t\t100 : Finir votre panier\n");
                            int num;
                            scanf("%d",&num);
                            if (num == 100){
                                quitter=100;
                            }
                            /*if (num1 == 2){ // Cas pour retirer un produit
                                printf("Cette option n'est pas encore disponible\n");
                                char nom_prod[512];
                                printf("Veuillez entrer le nom du produit que vous voulez retirer\n");
                                scanf("%s",nom_prod);
                                int id_prod=0;
                                
                                while(strcmp(tab[id_prod].objet.nom,nom_prod) != 0){
                                    id_prod++;
                                }
                                
                                retirer_prod_panier(p,&tab[id_prod].objet,id_prod,tab[id_prod].objet.prix_unit); 
                            } */
                            else{
                                while(num < 0 || num > 199 || tab[num].objet.stocks==0){
                                    if(tab[num].objet.stocks==0){ // Cas si il n'y a plus de stock
                                        printf("Il n'y a plus de stocks de ce produit.\nVeuillez entrer un nombre valide\n");
                                        scanf("%d",&num);
                                    }
                                    else{
                                        printf("Veuillez entrer un nombre valide\n");
                                        scanf("%d",&num);
                                    }
                                }

                                printf("Vous avez choisi %s\n",tab[num].objet.nom);
                                printf("\nstock : %d \t prix unitaire : %.2f euros\n",tab[num].objet.stocks,tab[num].objet.prix_unit);
                                printf("\nQuelle quantité voulez-vous ?\n");
                                int quant;
                                scanf("%d",&quant);
                                while(quant < 0 || quant >  tab[num].objet.stocks ){
                                    printf("Veuillez entrer un nombre valide (>=0)\n");
                                    scanf("%d",&quant);
                                }

                                ajout_prod_panier( p, &tab[num].objet,num, quant);
                                printf("\e[1;1H\e[2J");

                                 // Afficher la liste des produits 
                                printf("Votre panier actuellement est composé de :\n\n");
                                item_panier* i=p->first;
                                while(i != NULL){
                                    printf("%s \t Quantité : %.1f \t Total : %.2f\n",tab[i->idProd].objet.nom,i->quantite,tab[i->idProd].objet.prix_unit*quant);
                                    i=i->suivant;
                                }
                                printf("Le total de votre panier actuellement est %.2f euros\n",p->total);
                                printf("\n");
                            }
                            
                        }
                        printf("\e[1;1H\e[2J");
                        printf("Le total de votre panier est %.2lf euros\n\n",p->total);
                        printf("\nAppuyer sur entrer pour continuer !");
                        char en[64];
                        char *j;
                        j = en;
                        gets(j);
                        
                        // Ici on cherche dans le fichier Compta.csv la dernière date du dernier ticket enregistré
                        FILE* f_in=fopen("Compta.csv","r");

                        int annee;
                        int mois;
                        int jour;
                        int ticket;
                        char line[512];
        
                        while(fgets(line,512,f_in) != NULL){
                            fscanf(f_in,"%d / %d / %d , %d",&annee,&mois,&jour,&ticket);
                            
                        }
                    
                                                

                        // Puis ici on crée un nouveau tableau avec toutes les infos pour la compta
                        infos compta1[1];

                        // Ces deux lignes sont issus de la page web Koor sur la structure tm
                        time_t timestamp=time(NULL);
                        struct tm* now=localtime(&timestamp);

                        if(now->tm_year+1900 == annee && now->tm_mon+1 == mois && now->tm_mday == jour){
                            compta1->ticketId=ticket+1;
                        }
                        else{
                            compta1->ticketId=0;
                        }
                        compta1->date[0]=now->tm_year+1900;
                        compta1->date[1]=now->tm_mon+1;
                        compta1->date[2]=now->tm_mday;
                        compta1->total=p->total;
                        compta1->clientId=clientid;
                        fprintf(fich2,"%d/%d/%d,%d,%d,%.2f\n",compta1->date[0],compta1->date[1],compta1->date[2],compta1->ticketId,compta1->clientId,compta1->total);
                            
                        
                        panier_free(p);
                        fclose(fich2);
                        gets(u);
                        menu1=0;

                    }

                    FILE* fichier=fopen("Catalogue.txt","w");
                    fwrite(tab,sizeof(ID_ET_PROD),200,fichier);
                    
                    
                }while(menu1 !=0);  
                    
        }

        
        if(x == 2){ // Cas mode gerant
            printf("\e[1;1H\e[2J\n");
            char p[64];
            char *j;
            j = p;

            int boucle2 = 1;
            do{
                boucle2 = 1;
                printf("\e[1;1H\e[2J");
                printf("1) Afficher la liste des produits\n2) Modification/Ajout de produit\n3) Supression de produit\n4) Afficher compta\n0) Quitter le mode Gérant\n");
                gets(j);
                int m = atoi(j);
                while(m != 1 && m != 2 && m != 3 && m !=0 && m!=4){ // Cas où l'utilisateur rentre une valeur non valide
                    printf("Veuillez entrer un nombre valide (1/2/3/4/0)\n");
                    gets(j);
                    m = atoi(j);
                }
                if(m ==1){
                    printf("\e[1;1H\e[2J\n");
                    
                    printf("ID : %d\t",tab[0].ID);
                    printf("Nom : %s\t",tab[0].objet.nom);
                    printf("Prix : %.2lf\t",tab[0].objet.prix_unit);
                    printf("Stocks : %d\n",tab[0].objet.stocks);
                    for(int i =1;i<200;i++){
                        if(tab[i].ID !=0){
                            printf("ID : %d\t",tab[i].ID);
                            printf("Nom : %s\t",tab[i].objet.nom);
                            printf("Prix : %.2lf\t",tab[i].objet.prix_unit);
                            printf("Stocks : %d\n",tab[i].objet.stocks);           
                        }
                    }
                    printf("\nAppuyer sur entrer pour continuer !");
                    gets(j);
                    printf("\n\n");
                    boucle2 = 0;
                }
                if(m==2){
                    printf("\e[1;1H\e[2J\n");
                    printf("Saisir l'ID du produit que vous souhaitez enregistrer :\n");
                    gets(j); int o = atoi(j);
                    while(o <0 || o > 200 || sizeof(o) != 4){
                        printf("Veuillez entrer un nombre entier valide (entre 0 et 199 )\n");
                        gets(j);
                        o = atoi(j);
                    }
                    printf("Choisir nom:\n");
                    scanf("%s",tab[o].objet.nom);

                    printf("Choisir prix:\n");
                    double q = 0;
                    scanf("%lf",&q);
                    tab[o].objet.prix_unit = q;

                    printf("Choisir stock:\n");
                    int newStocks;
                    scanf("%d",&newStocks);
                    tab[o].objet.stocks = newStocks;

                    tab[o].ID=o; // Changer l'id et l'associer au produit

                    printf("Nouveau produit ajouté !\n");
                    printf("C'est le produit : %s au prix de %.2lf avec une  quantite de %d\n",tab[o].objet.nom,tab[o].objet.prix_unit,tab[o].objet.stocks);
                    printf("\nAppuyer sur enter pour continuer !");
                    gets(j);gets(j); //Pour attendre une entrée
                    FILE* fichier=fopen("Catalogue.txt","w"); // On enregistre les modifications des produits dans notre catalogue
                    fwrite(tab,sizeof(ID_ET_PROD),200,fichier);
                    boucle2 = 0; //Pour reprendre aux menus précédents
                }
                if(m==3){
                    printf("\e[1;1H\e[2J\n");
                    printf("L'ID du produit à supprimer :\n");
                    gets(j); int AjSup = atoi(j);
                    // Efface un produit
                    tab[AjSup].objet.prix_unit = 0;
                    tab[AjSup].objet.stocks=0;
                    tab[AjSup].ID = 0; 
                    FILE* fichier=fopen("Catalogue.txt","w"); // On enregistre les modifications des produits dans notre catalogue
                    fwrite(tab,sizeof(ID_ET_PROD),200,fichier);
                    boucle2 = 0; //Pour reprendre aux menus précédents
                }
                if(m==4){
                    printf("\e[1;1H\e[2J\n");
                    FILE* ff=fopen("Compta.csv","r");
                    printf("DATE,TICKETID,CLIENTID,TOTAL\n");
                    fseek(ff,29,SEEK_CUR);
                    char line[512];
                    while(fgets(line,512,ff) != NULL){
                        printf("%s",line);
                    }
                    printf("\n\n");
                    boucle2=0;
                    printf("\nAppuyer sur entrer pour continuer !");
                    gets(j);
                    printf("\n\n");
                }
            
            }while(boucle2 == 0);
        }
    }while(x != 0 && m !=0); 
}