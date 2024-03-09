#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct produit{
    char nom[64];
    double prix_unit;
    int stocks;
} produit;

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

struct node{
  char chaine[64];
  struct node* next;
};

typedef struct node node;

struct list{
  node* head;
};

typedef struct list list;

list* list_init(){
  list* l =malloc(sizeof(list));
  l->head=NULL;
  return l;
}

void list_add_front(list* l, char c[]){
  node* n=malloc(sizeof(node));
  strcpy(n->chaine,c);
  n->next=l->head;
  l->head=n;
}

void list_print(list* l){
  node* n=l->head;
  while(n != NULL){
    printf("%s ",n->chaine);
    n=n->next;
  }
  printf("\n");
}


typedef struct ID_ET_PROD{
    int ID;
    produit objet;
}ID_ET_PROD;

int main(){
    FILE* fich=fopen("Catalogue.txt","w");

    list*l =list_init();
    list_add_front(l,"Lait 1L");
    list_add_front(l,"Oeuf x6");
    list_add_front(l,"Eau 1L");
    list_add_front(l,"Farine 1kg");
    list_add_front(l,"Jus 1L");
    list_add_front(l,"Tomate");
    list_add_front(l,"Cola 1L");
    list_add_front(l,"Glaces x6");
    list_add_front(l,"Orange");
    list_add_front(l,"Café");
    list_add_front(l,"Chips salé");
    list_add_front(l,"Chips barbecue");
    list_add_front(l,"Pâtes 500g");
    list_add_front(l,"Sel");
    list_add_front(l,"Pain");
    list_add_front(l,"Pain de mie x10");
    list_add_front(l,"Céreales");
    list_add_front(l,"Pâte à tartiner");
    list_add_front(l,"Beurre");
    list_add_front(l,"Fromage");

    int nbr=20;
    ID_ET_PROD tab[nbr]; 
    int i=0; 
    node* n=l->head;
    while(n != NULL){ // Dans cette boucle on écrit dans le fichier "Catalogue.txt" les caractéristiques des produits (le stock et le prix unitaire sont choisi aléatoirement)
        strcpy(tab[i].objet.nom,n->chaine);
        tab[i].objet.prix_unit=rand()/((double)RAND_MAX)+1;
        tab[i].objet.stocks=(rand()%100)+100;
        n=n->next; 
        tab[i].ID=i;
        i++;
    }

    
    


    fwrite(tab,sizeof(ID_ET_PROD),nbr,fich);
    fclose(fich);

    

    



}


