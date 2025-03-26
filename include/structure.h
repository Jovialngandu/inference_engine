#ifndef STRUCTURE_H
#define STRUCTURE_H


typedef struct  {
    int id;
    char name[100];
    char description[100];
 }Character;


 typedef struct {
    int characterId;
    int propertyId;
 }characterUserValue;
 

typedef struct {
   int id;
   char name[100];
} Knowledge;

typedef struct {
   int id;
   char name[100];
   char description[100];
} AnimalClass;

typedef struct {
   int id;
   char name[100];
   int knowledge_id;
   int animal_class_id;
   char description[100];
} Rule;

typedef struct {
   int id;
   char name[100];
   int knowledge_id;
   int animal_class_id;
   char description[100];
} Fact;

typedef struct {
   int id;
   int characteristic_id;
   char libelle[100];
} Property;

typedef struct {
   int id;
   int fact_id;
   int property_id;
} PropertyFact;

typedef struct {
   int id;
   int rule_id;
   int property_id;
   char condition[200];
   char action[200];
} PropertyRule;

// Structure pour représenter un tableau de PropertyRule
typedef struct {
   PropertyRule *data;
   int size;
   int capacity;
} PropertyRuleArray;

// Structure pour représenter un tableau de PropertyRuleArray (le retour de notre fonction)
typedef struct {
   PropertyRuleArray **data;
   int size;
   int capacity;
} PropertyRuleArrayList;

typedef struct {
   int rule_id;
   int nbrMatches;
} Matches;

typedef struct {
   Matches *matches;
   int size;
   int capacity;
} MatchesList;
#endif //STRUCTURE_H