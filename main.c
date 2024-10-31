#define _CRT_SECURE_NO_WARNINGS 

// BASKARAN AMALAN
// DJOMO Elisabeth
// --FINAL VERSION
//02/06/2023

// unique fichier (main.c) qui contienent (Header + les fonction)

// Pour laancer ce program sous un terminal il faudra donner ce ligne de commande avant de lancer ce program( initiation de couleur
// reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f

#include <string.h>    
#include <stdlib.h>   
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#define BUFFERSIZE 80
typedef char STRING[BUFFERSIZE];

// Les structures destiné au produit 
typedef struct _PRODUIT { 
  STRING nom;
  double prix;
  int qte;
}PRODUIT;

typedef struct _NODE {
  PRODUIT produit;
  struct _NODE* prev, * next;
}NODE;

typedef struct _LIST {
  PRODUIT produit;
  NODE* first, * last;
}LIST;

// Les structures destiné au categorie 

typedef struct _CATE {
  STRING nom_cat;
  LIST abclist;
  LIST prixlist;
  LIST qtelist;
  struct CATE* pre;
  struct CATE* nex;
}CATE;

typedef struct _LISTCAT {
  STRING nom_cat;
  LIST abclist;
  LIST prixlist;
  LIST qtelist;
  CATE* head, * tail;
}LISTCAT;

// Majuscule chaine de charactere

char* strtoupper(char* dest, const char* src) {
  char* result = dest;
  while (*dest++ = toupper(*src++));
  return result;
}

// fonction menu
int welcome();

// different print 
void printProduit(PRODUIT produit); //afficher un produit
void printList(LIST list); //afficher une liste des produit
void printCat_nom(LISTCAT* listcat); // afficher une categorie de produit selon ordre alphabetique
void printCat_prix(LISTCAT* listcat); // afficher une categorie de produit selon ordre prix croissant
void printCat_qte(LISTCAT* listcat); // afficher une categorie de produit selon ordre Qte decroissant

void appendToABC_List(LIST* list, PRODUIT produit); //ajouter un produit à une liste en tri alphabetique
void appendToPRIX_List(LIST* sortlist, PRODUIT produit); //ajouter un produit à une liste en tri prix
void appendToQTE_List(LIST* qtelist, PRODUIT produit); //ajouter un produit à une liste en tri qte
void appendToMULTI_List(LIST* list, LIST* sortlist, LIST* qtelist, PRODUIT produit); // ajouter un produit selon les 3 tris precedent

void appendToCat(LISTCAT* listcat, LIST list, LIST sortlist, LIST qtelist, STRING name); // creation d'une categorie
void delToList(LISTCAT* listcat, STRING Nom_CAT, STRING del_name); // supression d'un produit dans une categorie
void delCat(LISTCAT* listcat, STRING Nom_CAT); // supression d'une categorie

void createProdui(LISTCAT* listcat); //creation d'un produit et ajouté dans une catégorie (choix utilisateur)
void supp(LISTCAT* listcat); //supression d'un produit dans une catégorie (choix utilisatuer)
void supp_cat(LISTCAT* listcat); // supression d'une categorie (choix utilisatuer)
void appendProduitToCat(LISTCAT* listcat, PRODUIT produit, STRING name); // ajouter un produit dans une catégorie
int searchProduit(LISTCAT* listcat, STRING nom_produit); // chercher nombre de fois un produit present
int NbElements(LISTCAT* listcat); // nombre de produit total

PRODUIT copyProduit(LISTCAT* listcat, STRING Nom_CAT, STRING nom_produit); //copier un produit

void updatePrice(LISTCAT* listcat, STRING Nom_CAT, STRING nom_produit, float price); // mise a jour prix
void updateQte(LISTCAT* listcat, STRING Nom_CAT, STRING nom_produit, int qte); // mise a jour qte
void aJourPrix(LISTCAT* listcat); // mise a jour prix (choix utilisateur)
void aJourQte(LISTCAT* listcat); // mise a jour qte (choix utilisateur)

void readData(LISTCAT* listcat, FILE* database); //lecture d'un fichier
void writeDate(LISTCAT* listcat); // ecriture d'un fichier

// couleur de terminal

void red();
void yellow();
void blue();
void reset();

// global variable
STRING nom_cas_n1;

int main(int argc, char* argv[]) {
  LISTCAT listcat = { NULL , NULL }; // creation d'une liste de categorie
  int choix = 0;
  int nbelem = 0;
  do { 
    int choix = welcome(); //menu
    if (choix == 11) { // boucle tourne jusqu'a utilisateur tape 11
      break;
    }
    switch (choix) // selon chaque choix utilisateur
    {
    case 0: // affichage tri alphabetique
      nbelem = NbElements(&listcat);
      if (nbelem == 0)
      {
        red();
        printf("\n\t>>> NO ITEMS FOUND <<<<\n");
        reset();
      }
      else {
        yellow();
        printCat_nom(&listcat);
        reset();
      }
      break;

    case 1: // affichage tri prix croissant
      nbelem = NbElements(&listcat);
      if (nbelem == 0)
      {
        red();
        printf("\n\t>>> NO ITEMS FOUND <<<<\n");
        reset();
      }
      else {
        yellow();
        printCat_prix(&listcat);
        reset();
      }
      break;

    case 2: // affichage tri qte decroissant
      nbelem = NbElements(&listcat);
      if (nbelem == 0)
      {
        red();
        printf("\n\t>>> NO ITEMS FOUND <<<<\n");
        reset();
      }
      else {
        yellow();
        printCat_qte(&listcat);
        reset();
      }
      break;

    case 3: // lecture d'une ficher qui est fournit au argument
      if (argc <= 1)
      {
        red();
        printf("\n>>>>> vous n'avez pas introduit de fichier comme argument. <<<<<\n");
        reset();
      }
      else {
        FILE* database = fopen(argv[1], "r"); // open in read mode the file set in argument
        yellow();
        printf("\n \t>>> Lecture de : %s \n", argv[1]);
        reset();
        if (database == NULL) {
          red();
          printf("\n ERROR! in reading >> check your file and relunch the program\n");
          reset();
          exit(-1);
        }
        yellow();
        readData(&listcat, database);
        reset();
      }
      break;

    case 4: // ecriture d'un fichier csv
      writeDate(&listcat);
      break;

    case 5: // creé un produit avec prompt utilisateur
      createProdui(&listcat);
      break;

    case 6:// supression d'un produit avec prompt utilisateur
      nbelem = NbElements(&listcat);
      if (nbelem == 0)
      {
        red();
        printf("\n\t>>> NO ITEMS FOUND <<<<\n");
        reset();
      }
      else {
        supp(&listcat);
      }
      break;

    case 7: //supression d'une categorie avec prompt utilisateur
      supp_cat(&listcat);
      break;

    case 8: //mise a jour prix
      nbelem = NbElements(&listcat);
      if (nbelem == 0)
      {
        red();
        printf("\n\t>>> NO ITEMS FOUND <<<<\n");
        reset();
      }
      else {
        aJourPrix(&listcat);
      }
      break;
    case 9: //mise a jour qte
      nbelem = NbElements(&listcat);
      if (nbelem == 0)
      {
        red();
        printf("\n\t>>> NO ITEMS FOUND <<<<\n");
        reset();
      }
      else {
        aJourQte(&listcat);
      }
      break;

    case 10: // clear terminal
      system("cls");
      break;
    default:
      break;
    }
  }while(choix != 10);
  return 0;
}

int welcome() {
  printf("\n \t \t \t !!! Appuyer !!! \n");
  printf("[0] : Aff ordre alphabetique\t");
  printf("[5] : Ajouter un Produit\n");
  printf("[1] : Aff prix croissant\t");
  printf("[6] : Supprimer un Produit\n");
  printf("[2] : Aff qte decroissant\t");
  printf("[7] : Supprimer une categorie\n");
  printf("[3] : Charger le fichier \t");
  printf("[8] : MAJ Prix d'un produit\n");
  printf("[4] : Enregisterer le fichier \t");
  printf("[9] : MAJ Qte d'un produit\n\n");
  printf("[10] : Clear Console\t\t");
  printf("[11] : Quittez \n");
  int choix = 15;
  printf("\nVotre Choix : ");
  scanf("%d", &choix);
  if (choix >= 0 && choix <= 11)//check si user donne une chiifre entre 0 et 11
  {
    return choix;
  }
  else
  {
    do
    {
      while (getchar() != '\n')
        continue;
      printf("\nReesayer Votre Choix : "); // sinon on demande une nouvelle chiffre jusqu'au que ça soit entre 0 et 11
      scanf("%d", &choix);
    } while (choix < 0 || choix > 11);
    return choix;
  }
}

void appendToABC_List(LIST* list, PRODUIT produit) {
  STRING maj_name;
  NODE* node = calloc(1, sizeof(NODE)); // allocation de memoire
  node->produit = produit; //load
  strtoupper(maj_name, produit.nom); //passage au majuscule
  strncpy(node->produit.nom, maj_name, BUFFERSIZE); // copy string
  node->next = NULL;
  node->prev = NULL;
  
  if (list->first == NULL) { // check si liste contient pas d'element
    list->first = node;
    list->last = node;
  }
  else if (strcmp(maj_name, list->first->produit.nom) < 0) { // si l'element qu'on veut ajouté est avant le premier element
    node->next = list->first;
    list->first->prev = node;
    list->first = node; // element qu'on veut ajouté devient premier

  }
  else if (strcmp(maj_name, list->last->produit.nom) > 0) { // si l'element qu'on veut ajouté est apres le dernier element
    node->prev = list->last;
    list->last->next = node;
    list->last = node; // element qu'on veut ajouté devient dernier

  }
  else { //sinon  
    NODE* p3 = list->first;
    NODE* p4 = list->first->next;
    while (p4 != NULL) {
      if (strcmp(maj_name, p4->produit.nom) == 0) {
        printf("Produit exite");
        break;
      }
      else if (strcmp(maj_name, p3->produit.nom) > 0 && strcmp(maj_name, p4->produit.nom) < 0) {// chercher bon emplacement dans la liste
        node->prev = p3;
        node->next = p4;
        p3->next = node;
        p4->prev = node; // placer entre deux elements
      }
      p3 = p3->next;
      p4 = p4->next;
    }

  }

}

void appendToPRIX_List(LIST* sortlist, PRODUIT produit) {
  STRING maj_name;
  NODE* node = calloc(1, sizeof(NODE)); // allocation de memoire
  node->produit = produit; //load
  strtoupper(maj_name, produit.nom);
  strncpy(node->produit.nom, maj_name, BUFFERSIZE);
  node->next = NULL;
  node->prev = NULL;

  if (sortlist->first == NULL) {// check si liste contient pas d'element
    sortlist->first = node;
    sortlist->last = node;
  }
  else if (node->produit.prix <= sortlist->first->produit.prix) { // si l'element qu'on veut ajouté dont le prix inf à premier element
    node->next = sortlist->first;
    sortlist->first->prev = node;
    sortlist->first = node;

  }
  else if (node->produit.prix >= sortlist->last->produit.prix) {// si l'element qu'on veut ajouté dont le prix sup à dernier element
    node->prev = sortlist->last;
    sortlist->last->next = node;
    sortlist->last = node;

  }
  else { // sinon
    NODE* p5 = sortlist->first;
    NODE* p6 = sortlist->first->next;
    while (p6 != NULL)
    {
      if (p5->produit.prix <= node->produit.prix && p6->produit.prix >= node->produit.prix)//cherche emplacement correct
      {
        node->prev = p5;
        node->next = p6;
        p5->next = node;
        p6->prev = node;
        break;
      }
      p5 = p5->next;
      p6 = p6->next;
    }
  }
}

void appendToQTE_List(LIST* qtelist, PRODUIT produit) {
  STRING maj_name;
  NODE* node = calloc(1, sizeof(NODE)); // allocation de memoire
  node->produit = produit; //meme fiche
  strtoupper(maj_name, produit.nom);
  strncpy(node->produit.nom, maj_name, BUFFERSIZE);
  node->next = NULL;
  node->prev = NULL;

  if (qtelist->first == NULL) { // check si liste contient pas d'element
    qtelist->first = node;
    qtelist->last = node;
  }
  else if (node->produit.qte >= qtelist->first->produit.qte) // si l'element qu'on veut ajouté dont le qte sup à premier element
  {
    node->next = qtelist->first;
    qtelist->first->prev = node;
    qtelist->first = node;
  }
  else if (node->produit.qte <= qtelist->last->produit.qte) { // si l'element qu'on veut ajouté dont le qte inf à dernier element
    node->prev = qtelist->last;
    qtelist->last->next = node;
    qtelist->last = node;
  }
  else //sinon
  {
    NODE* p7 = qtelist->first;
    NODE* p8 = qtelist->first->next;
    while (p8 != NULL)
    {
      if (p7->produit.qte >= node->produit.qte && p7->produit.qte <= node->produit.qte)//cherche emplacement correct
      {
        node->prev = p7;
        node->next = p8;
        p7->next = node;
        p8->prev = node;
        break;
      }
      p7 = p7->next;
      p8 = p8->next;
    }
  }
}

void appendToMULTI_List(LIST* list, LIST* sortlist, LIST* qtelist, PRODUIT produit){
  appendToABC_List(list, produit);
  appendToPRIX_List(sortlist, produit); // ajour d'un produit dans les 3 liste
  appendToQTE_List(qtelist, produit);
}

void appendToCat(LISTCAT* listcat, LIST list, LIST sortlist, LIST qtelist, STRING name){
  STRING maj_name;
  CATE* temp = calloc(1, sizeof(CATE));
  temp->abclist = list;
  temp->prixlist = sortlist;
  temp->qtelist = qtelist;
  strtoupper(maj_name, name); // majuscule
  strncpy(temp->nom_cat, maj_name, BUFFERSIZE);
  
  if (listcat->head == NULL) { // creation de 1er categorie
    temp->pre = NULL;
    temp->nex = NULL;
    listcat->head = temp;
    listcat->tail = temp;
  }
  else if(strcmp(maj_name, listcat->head->nom_cat) < 0) { //condition ordre alphabetique
    temp->pre = NULL;
    temp->nex = listcat->head;
    listcat->head->pre = temp;
    listcat->head = temp;
  }
  else if (strcmp(maj_name, listcat->tail->nom_cat) > 0){//condition ordre alphabetique
    temp->pre = listcat->tail;
    temp->nex = NULL;
    listcat->tail->nex = temp;
    listcat->tail = temp;
  }
  else
  {
    CATE* p1 = listcat->head;
    CATE* p2 = listcat->head->nex;
    while (p2 != NULL)
    {
      if (strcmp(maj_name, p1->nom_cat) > 0 && strcmp(maj_name, p2->nom_cat) < 0) //condition ordre alphabetique
      {
        temp->pre = p1;
        temp->nex = p2;
        p1->nex = temp;
        p2->pre = temp;
      }
      p1 = p1->nex;
      p2 = p2->nex;
    }
  }
}

void delToList(LISTCAT* listcat, STRING Nom_CAT , STRING del_name) {
  STRING del_maj_name;
  STRING maj_nom_cat;
  strtoupper(del_maj_name, del_name);
  strtoupper(maj_nom_cat, Nom_CAT);

  if (listcat->head == NULL)
  {
    red();
    printf("\n No Category Found \n");
    reset();
  }
  else {
    CATE* b1 = listcat->head;
    while (b1 != NULL) {
      if (strcmp(maj_nom_cat, b1->nom_cat) == 0)
      { 
        //_______________________________________________ Delete from ABC LIST__________________________________
        if (strcmp(b1->abclist.first->produit.nom,del_maj_name)==0) {
          if (b1->abclist.first->next == NULL) {
            b1->abclist.first = NULL;
            b1->abclist.last = NULL;
            delCat(listcat, maj_nom_cat);
            return;
          }
          else {
            b1->abclist.first = b1->abclist.first->next; // check chaque cas de supression 
            b1->abclist.first->prev = NULL;
          }
          
        }
        else if (strcmp(b1->abclist.last->produit.nom, del_maj_name) == 0) {
          b1->abclist.last = b1->abclist.last->prev;
          b1->abclist.last->next = NULL;
        }
        else {
          NODE* x1 = b1->abclist.first;
          while (x1 != NULL) {
            if (strcmp(x1->produit.nom,del_maj_name)==0)
            {
              x1->prev->next = x1->next;
              x1->next->prev = x1->prev;
              break;
            }
            x1 = x1->next;
          }
        }
        //_______________________________________________ Delete from PRIX LIST__________________________________
        if (strcmp(b1->prixlist.first->produit.nom, del_maj_name) == 0) {
          if (b1->prixlist.first->next == NULL) {
            b1->prixlist.first = NULL;
            b1->prixlist.last = NULL;
          }
          else {
            b1->prixlist.first = b1->prixlist.first->next;
            b1->prixlist.first->prev = NULL;
          }
        }
        else if (strcmp(b1->prixlist.last->produit.nom, del_maj_name) == 0) { // check chaque cas de supression 
          b1->prixlist.last = b1->prixlist.last->prev;
          b1->prixlist.last->next = NULL;
        }
        else {
          NODE* x2 = b1->prixlist.first;
          while (x2 != NULL) {
            if (strcmp(x2->produit.nom, del_maj_name) == 0)
            {
              x2->prev->next = x2->next;
              x2->next->prev = x2->prev;
              break;
            }
            x2 = x2->next;
          }
        }
        //_______________________________________________ Delete from QTE LIST__________________________________
        if (strcmp(b1->qtelist.first->produit.nom, del_maj_name) == 0) {
          if (b1->qtelist.first->next == NULL) {
            b1->qtelist.first = NULL;
            b1->qtelist.last = NULL;
          }
          else {
            b1->qtelist.first = b1->qtelist.first->next;
            b1->qtelist.first->prev = NULL;
          }
        }
        else if (strcmp(b1->qtelist.last->produit.nom, del_maj_name) == 0) { // check chaque cas de supression 
          b1->qtelist.last = b1->qtelist.last->prev;
          b1->qtelist.last->next = NULL;
        }
        else {
          NODE* x3 = b1->qtelist.first;
          while (x3 != NULL) {
            if (strcmp(x3->produit.nom, del_maj_name) == 0)
            {
              x3->prev->next = x3->next;
              x3->next->prev = x3->prev;
              break;
            }
            x3 = x3->next;
          }
        }
      }
      b1 = b1->nex;
    }
  }
}

void delCat(LISTCAT* listcat, STRING Nom_CAT) {
  STRING maj_nom_cat;
  strtoupper(maj_nom_cat, Nom_CAT);

  if (listcat->head == NULL)
  {
    red();
    printf("\n No Category Found \n");
    reset();
  }
  else
  {
    if (strcmp(maj_nom_cat, listcat->head->nom_cat) == 0) // conditon for check the postion of category to delete
    {
      if (listcat->head->nex == NULL) {
        listcat->head = NULL;
        listcat->tail = NULL;
      }
      else
      {
        listcat->head = listcat->head->nex;
        listcat->head->pre = NULL;
      }
    }
    else if (strcmp(maj_nom_cat, listcat->tail->nom_cat) == 0)
    {
      listcat->tail = listcat->tail->pre;
      listcat->tail->nex = NULL;
    }
    else
    {
      CATE* b1 = listcat->head;
      CATE* b2 = listcat->head->nex;
      CATE* b3 = b2->nex;
      while (b3 != NULL) {
        if (strcmp(maj_nom_cat, b2->nom_cat) == 0) {
          b1->nex = b3;
          b3->pre = b1;
        }
        b1 = b1->nex;
        b2 = b2->nex;
        b3 = b3->nex;
      }
    }
  }
}


// les affichage 
void printProduit(PRODUIT produit) {
  printf("\n\t\t-- Contenue de produit --\n");
  printf("Nom \t: %s \n", produit.nom);
  printf("Prix \t: %.1f \n", produit.prix);
  printf("Qte \t: %d \n", produit.qte);
}

void printList(LIST list) {
  printf("*********************** LISTE **********************\n");
  NODE* p = list.first; // selection de premier node
  while (p != NULL)
  {
    printProduit(p->produit); // print chaque node
    p = p->next; //next node
  }
  printf("\n");
}

void printCat_nom(LISTCAT* listcat){
  CATE* n = listcat->head;
  while (n != NULL) {
    printf("\n~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ %s ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n",n->nom_cat);
    printList(n->abclist);
    n = n->nex;
  }
}

void printCat_prix(LISTCAT* listcat) {
  CATE* n = listcat->head;
  while (n != NULL) {
    printf("\n~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ %s ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n", n->nom_cat);
    printList(n->prixlist);
    n = n->nex;
  }
}

void printCat_qte(LISTCAT* listcat) {
  CATE* n = listcat->head;
  while (n != NULL) {
    printf("\n~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ %s ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~\n", n->nom_cat);
    printList(n->qtelist);
    n = n->nex;
  }
}




void appendProduitToCat(LISTCAT* listcat, PRODUIT produit, STRING name) {
  STRING maj_name;
  strtoupper(maj_name, name);
  LIST list = { NULL , NULL };
  LIST sortlist = { NULL , NULL };
  LIST qtelist = { NULL , NULL };
  CATE* m = listcat->head;
  while (m != NULL) {
    if (strcmp(maj_name, m->nom_cat)==0)
    {
      appendToMULTI_List(&m->abclist,&m->prixlist,&m->qtelist,produit);// ajout d'un produit dans different liste
      return;
    }
    m = m->nex;
  }
  appendToMULTI_List(&list, &sortlist, &qtelist,produit);
  appendToCat(listcat, list, sortlist, qtelist, name); // ces different liste forme une categorie avec un nom
  return;
}

int searchProduit(LISTCAT* listcat, STRING nom_produit){
  STRING maj_nom_produit;
  strtoupper(maj_nom_produit, nom_produit);
  int i = 0;
  CATE* n = listcat->head;
  while (n != NULL) {
    NODE* p = n->abclist.first; 
    while (p != NULL)
    {
      if (strcmp(maj_nom_produit, p->produit.nom) == 0) {
        printf("  || ----- This product was found in %s ----- ||  ",n->nom_cat); // print the category where the product present
        printProduit(p->produit);
        strcpy(nom_cas_n1, n->nom_cat);
        i++;
      }
      p = p->next;
    }
    n = n->nex;
  }
  return i; // return number of same produit present in diffrent list
}

PRODUIT copyProduit(LISTCAT* listcat, STRING Nom_CAT, STRING nom_produit) {
  STRING maj_nom_produit;
  strtoupper(maj_nom_produit, nom_produit);
  STRING maj_nom_cat;
  strtoupper(maj_nom_cat, Nom_CAT);

  CATE* n = listcat->head;
  while (n != NULL) {
    if (strcmp(maj_nom_cat, n->nom_cat) == 0) { // copier un produit dans une liste de produit
      NODE* p = n->abclist.first;
      while (p != NULL) {
        if (strcmp(maj_nom_produit, p->produit.nom) == 0)
        {
          return p->produit;
        }
        p = p->next;
      }
    }
    n = n->nex;
  }
  printf("\n Produit Non trouvé ! \n");
}

void updatePrice(LISTCAT* listcat, STRING Nom_CAT, STRING nom_produit, float price) {
  STRING maj_nom_produit;
  strtoupper(maj_nom_produit, nom_produit);
  STRING maj_nom_cat;
  strtoupper(maj_nom_cat, Nom_CAT);
  PRODUIT remp = copyProduit(listcat, maj_nom_cat, maj_nom_produit); // copier le produit qu'on va mettre a jour
  remp.prix = price; // changer le prix de produit copié
  delToList(listcat, maj_nom_cat, maj_nom_produit); // supprimer le produit
  appendProduitToCat(listcat,remp, maj_nom_cat); // reinserer le produit avec le prix change
  return;
}

void updateQte(LISTCAT* listcat, STRING Nom_CAT, STRING nom_produit, int qte) {
  STRING maj_nom_produit;
  strtoupper(maj_nom_produit, nom_produit);
  STRING maj_nom_cat;
  strtoupper(maj_nom_cat, Nom_CAT);
  PRODUIT remp = copyProduit(listcat, maj_nom_cat, maj_nom_produit); // copier le produit qu'on va mettre a jour
  remp.qte = qte; // changer le qte de produit copié
  delToList(listcat, maj_nom_cat, maj_nom_produit); // supprimer le produit
  appendProduitToCat(listcat, remp, maj_nom_cat); // reinserer le produit avec le prix change
  return;
}

void readData(LISTCAT* listcat, FILE* database){
  char temp[80];
  char nom_cat[80];
  PRODUIT pro;
  char c[500];
  int nb_line = 1;
  while (fgets(c, sizeof(c), database) != NULL) { // read each line
    if (c[0] == '[') {
      char* strToken = strtok(c, "[]"); // split using ;
      strcpy(nom_cat, strToken);
      //printf("%s \n", strToken);
    }
    else {
      char* strToken = strtok(c, ";"); // split using ;
      int i = 0;
      while (strToken != NULL) {
        if (i == 0) {
          //printf("%s :", strToken); // 1st variable is name, so affect  variable to name
          strcpy(pro.nom, strToken);
        }
        else if (i == 1) {
          //printf("%.1f :", atof(strToken));
          pro.prix = atof(strToken);
        }
        else if (i == 2) {
          //printf("%d \n", atoi(strToken));
          pro.qte = atoi(strToken);
        }
        strToken = strtok(NULL, ";"); // pass next variable
        i++;
      }
      appendProduitToCat(listcat, pro, nom_cat); //ajouter le produit chaque fois dans la categorie
    }
  }
  blue();
  printf("\n--------------------------");
  printf("\n>>> Reading Successful <<<\n");
  printf("--------------------------\n");
  reset();
}

void writeDate(LISTCAT* listcat) {
  FILE* database = fopen("export.csv", "w"); // open write mode
  if (database == NULL) {
    printf("ERROR!");
    exit(-1);
  }
  CATE* n = listcat->head;
  while (n != NULL) {
    fprintf(database,"[");
    fprintf(database, "%s",n->nom_cat);
    fprintf(database, "]\n");

    NODE* p = n->abclist.first;
    while (p != NULL) {
      fprintf(database, "%s;%.1f;%d\n", p->produit.nom,p->produit.prix,p->produit.qte); // ecriture dans fichier
      p = p->next;
    }
    n = n->nex;
  }
  blue();
  printf("\n--------------------------");
  printf("\n>>> Writing Successful <<<\n");
  printf("--------------------------\n");
  reset();
}

void createProdui(LISTCAT* listcat) {
  PRODUIT temp;
  STRING nom_cat;
  STRING maj_nom_cat;
  STRING nom;
  STRING maj_nom;
  float prix;
  int qte;

  if (NbElements(listcat) == 0)
  {
    red();
    printf("\n>>>> No category Found <<<<");
    reset();
  }
  else
  {
    yellow();
    printf("\n>>>> List of Category <<<<\n");
    reset();
    int i = 0;
    CATE* n = listcat->head;
    while (n != NULL) { // afficher tous les nom de produit pour que l'utilisateur choisir le quel doit supprimé
      blue();
      printf("%s\t\t", n->nom_cat);
      reset();
      i++;
      if (i == 3)
      {
        printf("\n");
      }
      n = n->nex;
    }
  }
  printf("\n >>> PS : Mentionnerent d'une categorie ne figurant pas dans la liste entraine creation d'une nouvelle categorie <<<");
  while (getchar() != '\n')
    continue;

  printf("\n>>Entrer le Nom de categorie : "); //demandé à utlisateur
  scanf(" %s", &nom_cat);
  strtoupper(maj_nom_cat, nom_cat);

  while (getchar() != '\n')
    continue;

  printf("\n>>Entrer le Nom de Produit : "); //demandé à utlisateur
  scanf(" %s", &nom);
  strtoupper(maj_nom, nom);
  strncpy(temp.nom, maj_nom, BUFFERSIZE);

  while (getchar() != '\n')
    continue;

  printf("\n>>Entrer le Prix de Produit : ");
  scanf("%f", &prix);
  printf("\n>>Entrer le Qte de Produit : "); //demandé à utlisateur
  scanf("%d", &qte);
  temp.prix = prix;
  temp.qte = qte;
  appendProduitToCat(listcat, temp, maj_nom_cat); //ajouté le produit crée par utlisateur
  yellow();
  printf("\n--------Product created successfully");
  printf("\n----------------[ %s ; %.1f ; %d ]",temp.nom,temp.prix,temp.qte);
  printf("\n------------------------ADDED to %s\n\n\n", maj_nom_cat);
  reset();
}

void supp(LISTCAT* listcat) {
  STRING nom_cat;
  STRING maj_nom_cat;
  STRING nom;
  STRING maj_nom;
  int n = 0;
  printf("\nEntrer le Nom de Produit : ");//demandé à utlisateur 
  scanf(" %s", &nom);
  strtoupper(maj_nom, nom);
  printf("\n Votre Recherche : \n");
  n = searchProduit(listcat, nom);
  if (n == 0) // si pas d'element
  {
    printf("\nIl n'y a pas de produit disponible sous le nom\n");
  }
  else {
    printf("\n>>>> Pour confirmer la supression");
    printf("\n>>>> Entrer le Nom de categorie : ");
    scanf(" %s", &nom_cat);
    strtoupper(maj_nom_cat, nom_cat);
    delToList(listcat, maj_nom_cat, maj_nom); // supprimé
    printf("\n>> The product has been Deleted <<\n");
  }
}

void supp_cat(LISTCAT* listcat) {
  STRING nom_cat;
  STRING maj_nom_cat;
  if (NbElements(listcat)==0)
  {
    red();
    printf("\n>>>> No category Found <<<<");
    reset();
  }
  else
  {
    yellow();
    printf("\n>>>> List of Category <<<<\n"); 
    reset();
    int i = 0;
    CATE* n = listcat->head;
    while (n != NULL) { // afficher tous les nom de produit pour que l'utilisateur choisir le quel doit supprimé
      blue();
      printf("%s\t\t",n->nom_cat);
      reset();
      i++;
      if (i==3)
      {
        printf("\n");
      }
      n = n->nex;
    }
  }
  printf("\n>>>> Entrer le Nom de categorie : ");
  scanf(" %s", &nom_cat);
  strtoupper(maj_nom_cat, nom_cat);
  delCat(listcat, maj_nom_cat); // deletion
  printf("\n >> The category has been Deleted << \n");
}

void aJourPrix(LISTCAT* listcat) {
  STRING nom_cat;
  STRING maj_nom_cat;
  STRING nom;
  STRING maj_nom;
  STRING nom_c;
  int n = 0;
  float prix = 0.0;
  while (getchar() != '\n')
    continue;

  printf("\n>>>> Entrer le Nom de Produit : ");
  scanf(" %s", &nom);
  strtoupper(maj_nom, nom);

  n = searchProduit(listcat, maj_nom);

  if (n == 0) // pas d'element
  {
    red();
    printf("\n>>>> Il n'y a pas de produit disponible sous le nom <<<<\n");
    reset();
  }
  else if (n == 1) // si un seul element de ce nom present dans la liste 
  {
    printf("\n>>>> Entrer le Nouveau prix : ");
    scanf("%f", &prix);

    updatePrice(listcat, nom_cas_n1, maj_nom, prix);
    yellow();
    printf("\n>>>>Votre produit a bien ete MAJ<<<<");
    reset();
  }
  else { // si plusieur elements present, on demant dans quel categories il va supprimer
    printf(">>>> Quel cathegorie corespond a votre recherche : \n");
    printf("\n>>>> Entrer le Nom de categorie : ");
    scanf(" %s", &nom_cat);
    strtoupper(maj_nom_cat, nom_cat);

    printf("\n>>>> Entrer le Nouveau prix : ");
    scanf("%f", &prix);

    updatePrice(listcat, maj_nom_cat, maj_nom, prix);
    yellow();
    printf("\n>>>>Votre produit a bien ete MAJ<<<<");
    reset();
  }
}

void aJourQte(LISTCAT* listcat) {
  STRING nom_cat;
  STRING maj_nom_cat;
  STRING nom;
  STRING maj_nom;
  STRING nom_c;
  int n = 0;
  int qte = 0;

  while (getchar() != '\n')
    continue;

  printf("\n>>>> Entrer le Nom de Produit : ");
  scanf(" %s", &nom);
  strtoupper(maj_nom, nom);

  n = searchProduit(listcat, maj_nom);
  if (n == 0) // pas d'element
  {
    red();
    printf("\nIl n'y a pas de produit disponible sous le nom\n");
    reset();
  }
  else if (n == 1) // si un seul element de ce nom present dans la liste 
  {
    printf("\n>>>> Entrer le Nouveau prix : ");
    scanf("%f", &qte);

    updateQte(listcat, nom_cas_n1, maj_nom, qte);
    yellow();
    printf("\n>>>>Votre produit a bien ete MAJ<<<<");
    reset();
  }
  else { // si plusieur elements present, on demant dans quel categories il va supprimer
    printf(">>>> Quel cathegorie corespond a votre recherche :");
    printf("\n>>>> Entrer le Nom de categorie : ");
    scanf(" %s", &nom_cat);
    strtoupper(maj_nom_cat, nom_cat);

    printf("\n>>>> Entrer le Nouveau prix : ");
    scanf("%f", &qte);

    updateQte(listcat, maj_nom_cat, maj_nom, qte);
    yellow();
    printf("\n>>>>Votre produit a bien ete MAJ<<<<");
    reset();
  }
}

int NbElements(LISTCAT* listcat) {
  int i = 0;
  CATE* n = listcat->head;
  while (n != NULL) {
    NODE* p = n->abclist.first;
    while (p != NULL)
    {
      i++;  // nombre d'element total de liste des categorie
      p = p->next;
    }
    n = n->nex;
  }
  return i;
}


// couleur
void red() {
  printf("\033[1;31m");
}

void yellow(){
  printf("\033[1;33m");
}

void blue(){
  printf("\033[1;34m");
}

void reset() {
  printf("\033[0m");
}
