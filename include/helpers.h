#ifndef HELPER_H
#define HELPER_H


#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "structure.h"
#include "../library/cJson/cJSON.h"


////// Declarations
characterUserValue* menu();
int  chooser();

// /////////////////

int chooser(int min,int max){

    long valeur;
    char buffer[100];
    char *fin;

    while (1) {
        printf("Insert a number between %d and %d : ", min, max);

        // Lire une ligne entière (pour gérer les entrées invalides)
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Error on write .\n");
            exit(1);
        }

        // Convertir la chaîne en un nombre entier long
        valeur = strtol(buffer, &fin, 10);

        // Vérifier les erreurs de conversion
        if (fin == buffer) {
            printf("Erreur : Entrée invalide (aucun nombre trouvé).\n");
            //system("cls");
            continue;
        }

        if (*fin != '\n' && *fin != '\0') {
            printf("Erreur : Entrée invalide (caractères non numériques).\n");
            // system("cls");
            continue;
        }

        // Vérifier l'intervalle
        if (valeur < min || valeur > max || valeur > INT_MAX || valeur < INT_MIN) {
            printf("Erreur :hors de l'intervalle [%d, %d].\n", min, max);
            
            continue;
        }
        // system("cls");
        return (int)valeur;
    }
}


int* sortPropByCharactere(Property properties[], int size_properties,int charactereId ){

    int result[]={0};

    if(result==NULL){
        exit(1);
    }

    int resultLength=0;
    for (size_t j = 0; j < size_properties; j++)
    {
            if(properties[j].characteristic_id==charactereId){
                printf("%d:%s \n",properties[j].id, properties[j].libelle);
                resultLength++;
                result[resultLength]=properties[j].characteristic_id;
            }
    }

    for (size_t i = 0; i < resultLength; i++)
    {
        printf("%d",result[i]);
    }
    
            
    return result;      
}


int chooserTable(int tableau[], int tailleTableau) {
    long valeur;
    char buffer[100];
    char *fin;
    int i;
    int trouve;

    while (1) {
        printf("Insert a number ");

        // Lire une ligne entière (pour gérer les entrées invalides)
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fprintf(stderr, "Erreur de lecture de l'entrée.\n");
            exit(1);
        }

        // Convertir la chaîne en un nombre entier long
        valeur = strtol(buffer, &fin, 10);
        // Vérifier les erreurs de conversion
        if (fin == buffer) {
            printf("Erreur : Entrée invalide (aucun nombre trouvé).\n");
            continue;
        }

        if (*fin != '\n' && *fin != '\0') {
            printf("Erreur : Entrée invalide (caractères non numériques).\n");
            continue;
        }

        // Vérifier si la valeur est dans le tableau (sauf si c'est 0)
        trouve = 0;
        if (valeur==0){
            return valeur;
            printf("zero");
            break;;
        }
        if (valeur != 0) {
            for (i = 0; i < tailleTableau; i++) {
                if (valeur == tableau[i]) {
                    trouve = 1;
                    break;
                }
            }
            if (!trouve) {
                printf("Erreur : Nombre non trouvé dans le tableau.\n");
                continue;
            }
        }
        // system("cls");
        return (int)valeur;
    }
}

characterUserValue* menu(Character characters[],int size_characters,Property properties[], int size_properties) {

    characterUserValue* values=malloc((size_characters)*sizeof(int));

    if(!values){
        printf("allocation error");
        return NULL;
    }
    int table[size_properties];
    int tableLengh=0;

    for (size_t i = 0; i < size_characters; i++)
    {   
        system("cls");
        printf("%s :%s \n\n",characters[i].name,characters[i].description);
        
        printf("make your choose \n\n");

        for (size_t j = 0; j < size_properties; j++)
        {

            if(properties[j].characteristic_id==characters[i].id){
                table[tableLengh]=properties[j].id;
                tableLengh++;
                printf("%d:%s \n",properties[j].id, properties[j].libelle);
            }   
        }

        if(tableLengh==0){
            continue;
        }
        int c=chooserTable(table,tableLengh);
        tableLengh=0;
        for (size_t k = 0; k < size_properties; k++)
        {
            table[k]=NULL;
        }
        if(c!=0 && c!='0'){
            // printf("%s: %s\n",characters[i].name,properties[c-1].libelle);
            values[i].characterId=properties[c-1].characteristic_id;
            values[i].propertyId=properties[c-1].id;
        }
        if(c==0 || c==NULL){
            values[i].characterId=NULL;
            values[i].propertyId=NULL;
        }

       

    }
  
    // printf("%d",sizeof(values)/sizeof(values[0]));
    // for(size_t z = 0; z <= 2; z++)
    // {
    //    printf("\n character id :%d- property id: %d \n",values[z].characterId,values[z].propertyId);
    // }
    
     
    return values;

  }


cJSON* pythonExecute(){

    FILE *pipe;
    char buffer[1024];
    char result[4096] = {0};

    // Exécuter le script Python
    pipe = popen("python script2.py", "r");
    if (!pipe) {
        perror("popen");
        return 1;
    }

    // Lire la sortie du script Python
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        strcat(result, buffer);
    }

    pclose(pipe);

    // Analyser le JSON avec cJSON
    cJSON *json = cJSON_Parse(result);
    if (!json) {
        fprintf(stderr, "Erreur d'analyse JSON: %s\n", cJSON_GetErrorPtr());
        return 1;
    }

    return json;

}

void *get(cJSON *data, const char *nomTableau, int *taille) {
    cJSON *tableauJSON = cJSON_GetObjectItemCaseSensitive(data, nomTableau);
    if (!tableauJSON || !cJSON_IsArray(tableauJSON)) {
        return NULL;
    }

    *taille = cJSON_GetArraySize(tableauJSON);

    if (strcmp(nomTableau, "Characteristic") == 0) {
        Character *tableau = (Character *)malloc(sizeof(Character) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            tableau[i].id = cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint;
            strcpy(tableau[i].name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
            strcpy(tableau[i].description, cJSON_GetObjectItemCaseSensitive(elementJSON, "description")->valuestring);
        }
        return tableau;
    }
    // Ajoutez des blocs similaires pour les autres types (Knowledge, AnimalClass, etc.)
    else if (strcmp(nomTableau, "Knowledge") == 0) {
        Knowledge *tableau = (Knowledge *)malloc(sizeof(Knowledge) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            tableau[i].id = cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint;
            strcpy(tableau[i].name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
        }
        return tableau;
    }
    else if (strcmp(nomTableau, "AnimalClass") == 0) {
        AnimalClass *tableau = (AnimalClass *)malloc(sizeof(AnimalClass) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            tableau[i].id = cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint;
            strcpy(tableau[i].name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
            strcpy(tableau[i].description, cJSON_GetObjectItemCaseSensitive(elementJSON, "description")->valuestring);
        }
        return tableau;
    }
    else if (strcmp(nomTableau, "Rule") == 0) {
        Rule *tableau = (Rule *)malloc(sizeof(Rule) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            tableau[i].id = cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint;
            strcpy(tableau[i].name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
            tableau[i].BC_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "BC_id")->valueint;
            tableau[i].animal_class_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "animal_class_id")->valueint;
            strcpy(tableau[i].description, cJSON_GetObjectItemCaseSensitive(elementJSON, "description")->valuestring);
        }
        return tableau;
    }
    else if (strcmp(nomTableau, "Fact") == 0) {
        Fact *tableau = (Fact *)malloc(sizeof(Fact) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            tableau[i].id = cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint;
            strcpy(tableau[i].name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
            tableau[i].BC_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "BC_id")->valueint;
            tableau[i].animal_class_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "animal_class_id")->valueint;
            strcpy(tableau[i].description, cJSON_GetObjectItemCaseSensitive(elementJSON, "description")->valuestring);
        }
        return tableau;
    }
    else if (strcmp(nomTableau, "Property") == 0) {
         Property *tableau = (Property *)malloc(sizeof(Property) * (*taille));
         for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            tableau[i].id = cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint;
            tableau[i].characteristic_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "characteristic_id")->valueint;
            strcpy(tableau[i].libelle, cJSON_GetObjectItemCaseSensitive(elementJSON, "libelle")->valuestring);
        }
        return tableau;
    }
    else if (strcmp(nomTableau, "PropertyFact") == 0) {
        PropertyFact *tableau = (PropertyFact *)malloc(sizeof(PropertyFact) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            tableau[i].id = cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint;
            tableau[i].fact_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "fact_id")->valueint;
            tableau[i].property_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "property_id")->valueint;
        }
        return tableau;
    }
    else if (strcmp(nomTableau, "PropertyRule") == 0) {
        PropertyRule *tableau = (PropertyRule *)malloc(sizeof(PropertyRule) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            tableau[i].id = cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint;
            tableau[i].rule_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "rule_id")->valueint;
            tableau[i].property_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "property_id")->valueint;
            strcpy(tableau[i].condition, cJSON_GetObjectItemCaseSensitive(elementJSON, "condition")->valuestring);
            strcpy(tableau[i].action, cJSON_GetObjectItemCaseSensitive(elementJSON, "action")->valuestring);
        }
        return tableau;
    }
    return NULL;
}



void *getById(cJSON *data, const char *nomTableau, int id) {
    cJSON *tableauJSON = cJSON_GetObjectItemCaseSensitive(data, nomTableau);
    if (!tableauJSON || !cJSON_IsArray(tableauJSON)) {
        return NULL;
    }

    int taille = cJSON_GetArraySize(tableauJSON);

    if (strcmp(nomTableau, "Characteristic") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint == id) {
                Character *caractere = (Character *)malloc(sizeof(Character));
                caractere->id = id;
                strcpy(caractere->name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
                strcpy(caractere->description, cJSON_GetObjectItemCaseSensitive(elementJSON, "description")->valuestring);
                return caractere;
            }
        }
    }
    else if (strcmp(nomTableau, "Knowledge") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint == id) {
                Knowledge *connaissance = (Knowledge *)malloc(sizeof(Knowledge));
                connaissance->id = id;
                strcpy(connaissance->name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
                return connaissance;
            }
        }
    }
    else if (strcmp(nomTableau, "AnimalClass") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint == id) {
                AnimalClass *classeAnimale = (AnimalClass *)malloc(sizeof(AnimalClass));
                classeAnimale->id = id;
                strcpy(classeAnimale->name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
                strcpy(classeAnimale->description, cJSON_GetObjectItemCaseSensitive(elementJSON, "description")->valuestring);
                return classeAnimale;
            }
        }
    }
    else if (strcmp(nomTableau, "Rule") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint == id) {
                Rule *regle = (Rule *)malloc(sizeof(Rule));
                regle->id = id;
                strcpy(regle->name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
                regle->BC_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "BC_id")->valueint;
                regle->animal_class_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "animal_class_id")->valueint;
                strcpy(regle->description, cJSON_GetObjectItemCaseSensitive(elementJSON, "description")->valuestring);
                return regle;
            }
        }
    }
    else if (strcmp(nomTableau, "Fact") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint == id) {
                Fact *fait = (Fact *)malloc(sizeof(Fact));
                fait->id = id;
                strcpy(fait->name, cJSON_GetObjectItemCaseSensitive(elementJSON, "name")->valuestring);
                fait->BC_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "BC_id")->valueint;
                fait->animal_class_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "animal_class_id")->valueint;
                strcpy(fait->description, cJSON_GetObjectItemCaseSensitive(elementJSON, "description")->valuestring);
                return fait;
            }
        }
    }
    else if (strcmp(nomTableau, "Property") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint == id) {
                Property *propriete = (Property *)malloc(sizeof(Property));
                propriete->id = id;
                propriete->characteristic_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "characteristic_id")->valueint;
                strcpy(propriete->libelle, cJSON_GetObjectItemCaseSensitive(elementJSON, "libelle")->valuestring);
                return propriete;
            }
        }
    }
    else if (strcmp(nomTableau, "PropertyFact") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint == id) {
                PropertyFact *proprieteFait = (PropertyFact *)malloc(sizeof(PropertyFact));
                proprieteFait->id = id;
                proprieteFait->fact_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "fact_id")->valueint;
                proprieteFait->property_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "property_id")->valueint;
                return proprieteFait;
            }
        }
    }
    else if (strcmp(nomTableau, "PropertyRule") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (cJSON_GetObjectItemCaseSensitive(elementJSON, "id")->valueint == id) {
                PropertyRule *proprieteRegle = (PropertyRule *)malloc(sizeof(PropertyRule));
                proprieteRegle->id = id;
                proprieteRegle->rule_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "rule_id")->valueint;
                proprieteRegle->property_id = cJSON_GetObjectItemCaseSensitive(elementJSON, "property_id")->valueint;
                strcpy(proprieteRegle->condition, cJSON_GetObjectItemCaseSensitive(elementJSON, "condition")->valuestring);
                strcpy(proprieteRegle->action, cJSON_GetObjectItemCaseSensitive(elementJSON, "action")->valuestring);
                return proprieteRegle;
            }
        }
    }
    return NULL;
}

Fact *getMatchingFacts(cJSON *data, characterUserValue *userValues, int userValuesSize, int *matchingFactsSize) {
    cJSON *propertiesArray = cJSON_GetObjectItemCaseSensitive(data, "Property");
    cJSON *propertyFactsArray = cJSON_GetObjectItemCaseSensitive(data, "PropertyFact");
    cJSON *factsArray = cJSON_GetObjectItemCaseSensitive(data, "Fact");

    if (!propertiesArray || !cJSON_IsArray(propertiesArray) ||
        !propertyFactsArray || !cJSON_IsArray(propertyFactsArray) ||
        !factsArray || !cJSON_IsArray(factsArray)) {
        return NULL;
    }

    int propertiesArraySize = cJSON_GetArraySize(propertiesArray);
    int propertyFactsArraySize = cJSON_GetArraySize(propertyFactsArray);
    int factsArraySize = cJSON_GetArraySize(factsArray);
    Fact *matchingFacts = NULL;
    *matchingFactsSize = 0;

    int *propertyIds = (int *)malloc(sizeof(int) * userValuesSize);
    for (int i = 0; i < userValuesSize; i++) {
        for (int j = 0; j < propertiesArraySize; j++) {
            cJSON *propertyJSON = cJSON_GetArrayItem(propertiesArray, j);
            if (cJSON_GetObjectItemCaseSensitive(propertyJSON, "characteristic_id")->valueint == userValues[i].characterId &&
                cJSON_GetObjectItemCaseSensitive(propertyJSON, "id")->valueint == userValues[i].propertyId) {
                propertyIds[i] = cJSON_GetObjectItemCaseSensitive(propertyJSON, "id")->valueint;
                break;
            }
        }
    }

    for (int i = 0; i < factsArraySize; i++) {
        cJSON *factJSON = cJSON_GetArrayItem(factsArray, i);
        int factId = cJSON_GetObjectItemCaseSensitive(factJSON, "id")->valueint;
        int matchCount = 0;

        for (int j = 0; j < userValuesSize; j++) {
            for (int k = 0; k < propertyFactsArraySize; k++) {
                cJSON *propertyFactJSON = cJSON_GetArrayItem(propertyFactsArray, k);
                if (cJSON_GetObjectItemCaseSensitive(propertyFactJSON, "fact_id")->valueint == factId &&
                    cJSON_GetObjectItemCaseSensitive(propertyFactJSON, "property_id")->valueint == propertyIds[j]) {
                    matchCount++;
                    break;
                }
            }
        }

        if (matchCount == userValuesSize) {
            (*matchingFactsSize)++;
            matchingFacts = realloc(matchingFacts, sizeof(Fact) * (*matchingFactsSize));
            matchingFacts[(*matchingFactsSize) - 1].id = factId;
            strcpy(matchingFacts[(*matchingFactsSize) - 1].name, cJSON_GetObjectItemCaseSensitive(factJSON, "name")->valuestring);
            matchingFacts[(*matchingFactsSize) - 1].BC_id = cJSON_GetObjectItemCaseSensitive(factJSON, "BC_id")->valueint;
            matchingFacts[(*matchingFactsSize) - 1].animal_class_id = cJSON_GetObjectItemCaseSensitive(factJSON, "animal_class_id")->valueint;
            strcpy(matchingFacts[(*matchingFactsSize) - 1].description, cJSON_GetObjectItemCaseSensitive(factJSON, "description")->valuestring);
        }
    }

    free(propertyIds);
    return matchingFacts;
}
#endif 
