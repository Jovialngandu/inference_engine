#ifndef HELPER_H
#define HELPER_H


#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include "structure.h"
#include <string.h>
#include "../library/cJson/cJSON.h"


const char* getStringValue(cJSON *item) {
    if (item && cJSON_IsString(item)) {
        return item->valuestring;
    }
    return NULL;
}

// Helper function to safely get integer values from cJSON
int getIntValue(cJSON *item) {
    if (item && cJSON_IsNumber(item)) {
        return item->valueint;
    }
    return 0; // Or some other default value indicating null
}

  char* clean_json(const char* entree) {
    const char *debut = NULL, *fin = NULL;

    // 1. Chercher la première accolade ou crochet ouvrant
    debut = strchr(entree, '{');
    fin = strrchr(entree, '}');

    if (!debut || !fin || fin <= debut) {
        // Sinon, essayer avec des crochets
        debut = strchr(entree, '[');
        fin = strrchr(entree, ']');
        if (!debut || !fin || fin <= debut) {
            return NULL; // Rien à extraire
        }
    }

    // Taille à copier
    int taille = (fin - debut) + 1;
    char* resultat = malloc(taille + 1); // +1 pour \0

    if (!resultat) return NULL;

    strncpy(resultat, debut, taille);
    resultat[taille] = '\0'; // Terminaison

    return resultat;
}

cJSON* pythonExecute(){

    FILE *pipe;
    char *buffer[4096];
    char result[4096]={0};
    char pythonFilePath[]=" C:/Users/DELL/Documents/GitHub/animal_class_knowledge_base ";
    char query[]="cd "; strcat(query,pythonFilePath); strcat(query,"&& python  animal_knowledge all");//

    // Exécuter le script Python
    pipe = popen(query, "r");
    if (!pipe) {
        perror("popen");
        return 1;
    }

    //Lire la sortie du script Python
    while (fgets(buffer, (sizeof(buffer)), pipe) != NULL) {
            strcat(result, buffer);
    }

    pclose(pipe);
    
    // Analyser le JSON avec cJSON
    cJSON *json = cJSON_Parse(clean_json(result));
    if (!json) {
        fprintf(stderr, "Erreur d'analyse de la base de connaissance: %s\n", cJSON_GetErrorPtr());
        return 1;
    }
    
    return json;

}

void *get(cJSON *data, const char *nomTableau, int *taille) {
    cJSON *tableauJSON = cJSON_GetObjectItemCaseSensitive(data, nomTableau);
    if (!tableauJSON || !cJSON_IsArray(tableauJSON)) {
        *taille = 0;
        return NULL;
    }

    *taille = cJSON_GetArraySize(tableauJSON);
    void *tableau = NULL;

    if (strcmp(nomTableau, "Characteristic") == 0) {
        tableau = malloc(sizeof(Character) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            ((Character *)tableau)[i].id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id"));
            const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
            if (name) strcpy(((Character *)tableau)[i].name, name); else strcpy(((Character *)tableau)[i].name, "");
            const char *description = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "description"));
            if (description) strcpy(((Character *)tableau)[i].description, description); else strcpy(((Character *)tableau)[i].description, "");
        }
    } else if (strcmp(nomTableau, "Knowledge") == 0) {
        tableau = malloc(sizeof(Knowledge) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            ((Knowledge *)tableau)[i].id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id"));
            const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
            if (name) strcpy(((Knowledge *)tableau)[i].name, name); else strcpy(((Knowledge *)tableau)[i].name, "");
        }
    } else if (strcmp(nomTableau, "AnimalClass") == 0) {
        tableau = malloc(sizeof(AnimalClass) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            ((AnimalClass *)tableau)[i].id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id"));
            const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
            if (name) strcpy(((AnimalClass *)tableau)[i].name, name); else strcpy(((AnimalClass *)tableau)[i].name, "");
            const char *description = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "description"));
            if (description) strcpy(((AnimalClass *)tableau)[i].description, description); else strcpy(((AnimalClass *)tableau)[i].description, "");
        }
    } else if (strcmp(nomTableau, "Rule") == 0) {
        tableau = malloc(sizeof(Rule) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            ((Rule *)tableau)[i].id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id"));
            ((Rule *)tableau)[i].knowledge_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "knowledge_id"));
            ((Rule *)tableau)[i].animal_class_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "animal_class_id"));
            const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
            if (name) strcpy(((Rule *)tableau)[i].name, name); else strcpy(((Rule *)tableau)[i].name, "");
            const char *description = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "description"));
            if (description) strcpy(((Rule *)tableau)[i].description, description); else strcpy(((Rule *)tableau)[i].description, "");
        }
    } else if (strcmp(nomTableau, "Fact") == 0) {
        tableau = malloc(sizeof(Fact) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            ((Fact *)tableau)[i].id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id"));
            ((Fact *)tableau)[i].knowledge_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "knowledge_id"));
            ((Fact *)tableau)[i].animal_class_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "animal_class_id"));
            const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
            if (name) strcpy(((Fact *)tableau)[i].name, name); else strcpy(((Fact *)tableau)[i].name, "");
            const char *description = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "description"));
            if (description) strcpy(((Fact *)tableau)[i].description, description); else strcpy(((Fact *)tableau)[i].description, "");
        }
    } else if (strcmp(nomTableau, "Property") == 0) {
        tableau = malloc(sizeof(Property) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            ((Property *)tableau)[i].id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id"));
            ((Property *)tableau)[i].characteristic_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "characteristic_id"));
            const char *libelle = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "libelle"));
            if (libelle) strcpy(((Property *)tableau)[i].libelle, libelle); else strcpy(((Property *)tableau)[i].libelle, "");
        }
    } else if (strcmp(nomTableau, "PropertyFact") == 0) {
        tableau = malloc(sizeof(PropertyFact) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            ((PropertyFact *)tableau)[i].id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id"));
            ((PropertyFact *)tableau)[i].fact_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "fact_id"));
            ((PropertyFact *)tableau)[i].property_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "property_id"));
        }
    } else if (strcmp(nomTableau, "PropertyRule") == 0) {
        tableau = malloc(sizeof(PropertyRule) * (*taille));
        for (int i = 0; i < *taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            ((PropertyRule *)tableau)[i].id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id"));
            ((PropertyRule *)tableau)[i].rule_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "rule_id"));
            ((PropertyRule *)tableau)[i].property_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "property_id"));
            const char *condition = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "condition"));
            if (condition) strcpy(((PropertyRule *)tableau)[i].condition, condition); else strcpy(((PropertyRule *)tableau)[i].condition, "");
            const char *action = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "action"));
            if (action) strcpy(((PropertyRule *)tableau)[i].action, action); else strcpy(((PropertyRule *)tableau)[i].action, "");
        }
    }

    return tableau;
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
            if (getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id")) == id) {
                Character *caractere = (Character *)malloc(sizeof(Character));
                caractere->id = id;
                const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
                if (name) strcpy(caractere->name, name); else strcpy(caractere->name, "");
                const char *description = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "description"));
                if (description) strcpy(caractere->description, description); else strcpy(caractere->description, "");
                return caractere;
            }
        }
    } else if (strcmp(nomTableau, "Knowledge") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id")) == id) {
                Knowledge *connaissance = (Knowledge *)malloc(sizeof(Knowledge));
                connaissance->id = id;
                const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
                if (name) strcpy(connaissance->name, name); else strcpy(connaissance->name, "");
                return connaissance;
            }
        }
    } else if (strcmp(nomTableau, "AnimalClass") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id")) == id) {
                AnimalClass *classeAnimale = (AnimalClass *)malloc(sizeof(AnimalClass));
                classeAnimale->id = id;
                const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
                if (name) strcpy(classeAnimale->name, name); else strcpy(classeAnimale->name, "");
                const char *description = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "description"));
                if (description) strcpy(classeAnimale->description, description); else strcpy(classeAnimale->description, "");
                return classeAnimale;
            }
        }
    } else if (strcmp(nomTableau, "Rule") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id")) == id) {
                Rule *regle = (Rule *)malloc(sizeof(Rule));
                regle->id = id;
                regle->knowledge_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "knowledge_id"));
                regle->animal_class_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "animal_class_id"));
                const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
                if (name) strcpy(regle->name, name); else strcpy(regle->name, "");
                const char *description = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "description"));
                if (description) strcpy(regle->description, description); else strcpy(regle->description, "");
                return regle;
            }
        }
    } else if (strcmp(nomTableau, "Fact") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id")) == id) {
                Fact *fait = (Fact *)malloc(sizeof(Fact));
                fait->id = id;
                fait->knowledge_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "knowledge_id"));
                fait->animal_class_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "animal_class_id"));
                const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "name"));
                if (name) strcpy(fait->name, name); else strcpy(fait->name, "");
                const char *description = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "description"));
                if (description) strcpy(fait->description, description); else strcpy(fait->description, "");
                return fait;
            }
        }
    } else if (strcmp(nomTableau, "Property") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id")) == id) {
                Property *propriete = (Property *)malloc(sizeof(Property));
                propriete->id = id;
                propriete->characteristic_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "characteristic_id"));
                const char *libelle = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "libelle"));
                if (libelle) strcpy(propriete->libelle, libelle); else strcpy(propriete->libelle, "");
                return propriete;
            }
        }
    } else if (strcmp(nomTableau, "PropertyFact") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id")) == id) {
                PropertyFact *proprieteFait = (PropertyFact *)malloc(sizeof(PropertyFact));
                proprieteFait->id = id;
                proprieteFait->fact_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "fact_id"));
                proprieteFait->property_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "property_id"));
                return proprieteFait;
            }
        }
    } else if (strcmp(nomTableau, "PropertyRule") == 0) {
        for (int i = 0; i < taille; i++) {
            cJSON *elementJSON = cJSON_GetArrayItem(tableauJSON, i);
            if (getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "id")) == id) {
                PropertyRule *proprieteRegle = (PropertyRule *)malloc(sizeof(PropertyRule));
                proprieteRegle->id = id;
                proprieteRegle->rule_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "rule_id"));
                proprieteRegle->property_id = getIntValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "property_id"));
                const char *condition = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "condition"));
                if (condition) strcpy(proprieteRegle->condition, condition); else strcpy(proprieteRegle->condition, "");
                const char *action = getStringValue(cJSON_GetObjectItemCaseSensitive(elementJSON, "action"));
                if (action) strcpy(proprieteRegle->action, action); else strcpy(proprieteRegle->action, "");
                return proprieteRegle;
            }
        }
    }
    return NULL;
}

void print_all(FILE *stream)
{
 int c;
 while ((c = getc(stream)) != EOF)
 putchar(c);
}


Property *getUserSelectedProperties(Character *characters, int numCharacters, Property *properties, int numProperties, int *selectedPropertiesSize) {
    Property *selectedProperties = NULL;
    *selectedPropertiesSize = 0;

    for (int i = 0; i < numCharacters; i++) {
        system("cls");
        printf("\n--- Caractéristique : %s (ID: %d) ---\n", characters[i].name, characters[i].id);
        printf("Veuillez sélectionner une propriété parmi les suivantes :\n");

        int propertyCount = 0;
        int validPropertyIndices[numProperties];

        // Afficher les propriétés correspondantes à la caractéristique actuelle
        for (int j = 0; j < numProperties; j++) {
            if (properties[j].characteristic_id == characters[i].id) {
                printf("%d - %s (ID: %d)\n", propertyCount + 1, properties[j].libelle, properties[j].id);
                validPropertyIndices[propertyCount] = j;
                propertyCount++;
            }
        }
        printf("0 - Ne pas sélectionner de propriété pour cette caractéristique\n");

        int choice;
        while (1) {
            printf("Votre choix : ");
            if (scanf("%d", &choice) != 1) {
                // Clear input buffer if non-integer input
                while (getchar() != '\n');
                printf("Erreur : Veuillez saisir un nombre entier.\n");
                continue;
            }

            if (choice >= 0 && choice <= propertyCount) {
                if (choice > 0) {
                    int selectedPropertyIndex = validPropertyIndices[choice - 1];
                    (*selectedPropertiesSize)++;
                    selectedProperties = realloc(selectedProperties, sizeof(Property) * (*selectedPropertiesSize));
                    if (selectedProperties == NULL) {
                        perror("Erreur d'allocation mémoire");
                        exit(EXIT_FAILURE);
                    }
                    selectedProperties[(*selectedPropertiesSize) - 1] = properties[selectedPropertyIndex];
                    printf("Vous avez sélectionné la propriété : %s (ID: %d) pour la caractéristique : %s.\n",
                           properties[selectedPropertyIndex].libelle, properties[selectedPropertyIndex].id, characters[i].name);
                } else {
                    printf("Aucune propriété sélectionnée pour la caractéristique : %s.\n", characters[i].name);
                }
                break; // Exit the inner while loop after valid input
            } else {
                printf("Erreur : Veuillez saisir un numéro valide parmi les options proposées.\n");
            }
        }
        // Clear input buffer after reading choice
        while (getchar() != '\n');
    }

    // printf("\n--- Récapitulatif des sélections ---:\n");
    // for (int i = 0; i < *selectedPropertiesSize; i++) {
    //     printf("Caractéristique ID (via Property): %d -> Propriété sélectionnée: %s (ID: %d)\n",
    //            selectedProperties[i].characteristic_id, selectedProperties[i].libelle, selectedProperties[i].id);
    // }

    return selectedProperties;
}
    


Fact *findFactsWithAllProperties(cJSON *data, Property *selectedProperties, int numSelectedProperties, int *matchingFactsSize) {
    cJSON *propertyFactsArray = cJSON_GetObjectItemCaseSensitive(data, "PropertyFact");
    cJSON *factsArray = cJSON_GetObjectItemCaseSensitive(data, "Fact");

    if (!propertyFactsArray || !cJSON_IsArray(propertyFactsArray) || !factsArray || !cJSON_IsArray(factsArray)) {
        *matchingFactsSize = 0;
        return NULL;
    }

    int propertyFactsArraySize = cJSON_GetArraySize(propertyFactsArray);
    int factsArraySize = cJSON_GetArraySize(factsArray);
    Fact *matchingFacts = NULL;
    *matchingFactsSize = 0;

    for (int i = 0; i < factsArraySize; i++) {
        cJSON *factJSON = cJSON_GetArrayItem(factsArray, i);
        int factId = getIntValue(cJSON_GetObjectItemCaseSensitive(factJSON, "id"));
        int propertyMatchCount = 0;

        for (int j = 0; j < numSelectedProperties; j++) {
            for (int k = 0; k < propertyFactsArraySize; k++) {
                cJSON *propertyFactJSON = cJSON_GetArrayItem(propertyFactsArray, k);
                if (getIntValue(cJSON_GetObjectItemCaseSensitive(propertyFactJSON, "fact_id")) == factId &&
                    getIntValue(cJSON_GetObjectItemCaseSensitive(propertyFactJSON, "property_id")) == selectedProperties[j].id) {
                    propertyMatchCount++;
                    break; // Move to the next selected property for the current fact
                }
            }
        }

        // If all selected properties are associated with the current fact
        if (propertyMatchCount == numSelectedProperties) {
            (*matchingFactsSize)++;
            matchingFacts = realloc(matchingFacts, sizeof(Fact) * (*matchingFactsSize));
            if (matchingFacts == NULL) {
                perror("Erreur d'allocation mémoire");
                exit(EXIT_FAILURE);
            }
            matchingFacts[(*matchingFactsSize) - 1].id = factId;
            const char *name = getStringValue(cJSON_GetObjectItemCaseSensitive(factJSON, "name"));
            if (name) strcpy(matchingFacts[(*matchingFactsSize) - 1].name, name); else strcpy(matchingFacts[(*matchingFactsSize) - 1].name, "");
            matchingFacts[(*matchingFactsSize) - 1].knowledge_id = getIntValue(cJSON_GetObjectItemCaseSensitive(factJSON, "knowledge_id"));
            matchingFacts[(*matchingFactsSize) - 1].animal_class_id = getIntValue(cJSON_GetObjectItemCaseSensitive(factJSON, "animal_class_id"));
            const char *description = getStringValue(cJSON_GetObjectItemCaseSensitive(factJSON, "description"));
            if (description) strcpy(matchingFacts[(*matchingFactsSize) - 1].description, description); else strcpy(matchingFacts[(*matchingFactsSize) - 1].description, "");
        }
    }

    return matchingFacts;
}


// Fonction pour initialiser un PropertyRuleArray

PropertyRuleArray* initPropertyRuleArray() {
    PropertyRuleArray* arr = (PropertyRuleArray*)malloc(sizeof(PropertyRuleArray));
    if (arr == NULL) {
        perror("Failed to allocate PropertyRuleArray");
        return NULL;
    }
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    return arr;
}

// Fonction pour ajouter un PropertyRule à un PropertyRuleArray
void addPropertyRule(PropertyRuleArray* arr, PropertyRule rule) {
    if (arr->size >= arr->capacity) {
        arr->capacity = (arr->capacity == 0) ? 10 : arr->capacity * 2;
        PropertyRule* newData = (PropertyRule*)realloc(arr->data, arr->capacity * sizeof(PropertyRule));
        if (newData == NULL) {
            perror("Failed to reallocate PropertyRuleArray");
            return;
        }
        arr->data = newData;
    }
    arr->data[arr->size++] = rule;
}

// Fonction pour initialiser un PropertyRuleArrayList
PropertyRuleArrayList* initPropertyRuleArrayList() {
    PropertyRuleArrayList* arr = (PropertyRuleArrayList*)malloc(sizeof(PropertyRuleArrayList));
    if (arr == NULL) {
        perror("Failed to allocate PropertyRuleArrayList");
        return NULL;
    }
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    return arr;
}


void addMatchesRuleArray(MatchesList* arr, Matches match) {
  
        // printf("%d",arr->size);
        // arr->Matches[arr->size++] = match;
}


// Fonction pour ajouter un PropertyRuleArray à un PropertyRuleArrayList
void addPropertyRuleArray(PropertyRuleArrayList* arr, PropertyRuleArray* propRules) {
    if (arr->size >= arr->capacity) {
        arr->capacity = (arr->capacity == 0) ? 5 : arr->capacity * 2;
        PropertyRuleArray** newData = (PropertyRuleArray**)realloc(arr->data, arr->capacity * sizeof(PropertyRuleArray*));
        if (newData == NULL) {
            perror("Failed to reallocate PropertyRuleArrayList");
            return;
        }
        arr->data = newData;
    }
    arr->data[arr->size++] = propRules;
}


// Fonction pour générer les PropertyRule par Property à partir des données JSON
PropertyRuleArrayList* generatePropertyRulesByPropertyFromJSON(cJSON *data, Property properties[], int numProperties) {
    PropertyRuleArrayList* result = initPropertyRuleArrayList();
    if (result == NULL) return NULL;

    int ruleCount = 0;
    Rule *rules = (Rule *)get(data, "Rule", &ruleCount);

    if (rules == NULL) {
        fprintf(stderr, "Erreur: Impossible de récupérer le tableau 'Rule' du JSON.\n");
        freePropertyRuleArrayList(result);
        return NULL;
    }

    int propertyRuleCount = 0;
    PropertyRule *allPropertyRulesJSON = (PropertyRule *)get(data, "PropertyRule", &propertyRuleCount);

    if (allPropertyRulesJSON == NULL) {
        fprintf(stderr, "Erreur: Impossible de récupérer le tableau 'PropertyRule' du JSON.\n");
        free(rules);
        freePropertyRuleArrayList(result);
        return NULL;
    }

    for (int i = 0; i < numProperties; i++) {
        Property currentProperty = properties[i];
        PropertyRuleArray* propertyRulesForCurrentProperty = initPropertyRuleArray();
        if (propertyRulesForCurrentProperty == NULL) continue;

        for (int j = 0; j < propertyRuleCount; j++) {
            if (allPropertyRulesJSON[j].property_id == currentProperty.id) {
                addPropertyRule(propertyRulesForCurrentProperty, allPropertyRulesJSON[j]);
            }
        }
        addPropertyRuleArray(result, propertyRulesForCurrentProperty);
    }

    free(rules);
    free(allPropertyRulesJSON);

    return result;
}
// Fonction pour libérer la mémoire allouée pour un PropertyRuleArray
void freePropertyRuleArray(PropertyRuleArray* arr) {
    if (arr) {
        free(arr->data);
        free(arr);
    }
}

// Fonction pour libérer la mémoire allouée pour un PropertyRuleArrayList
void freePropertyRuleArrayList(PropertyRuleArrayList* list) {
    if (list) {
        if (list->data) {
            for (int i = 0; i < list->size; i++) {
                freePropertyRuleArray(list->data[i]);
            }
            free(list->data);
        }
        free(list);
    }
}

void afficherPropertyRuleArray(const PropertyRuleArray* arr) {
    if (arr == NULL) {
        printf("PropertyRuleArray est NULL.\n");
        return;
    }
    printf("  Taille du PropertyRuleArray: %d\n", arr->size);
    for (int i = 0; i < arr->size; i++) {
        printf("    - ID: %d, Rule ID: %d, Property ID: %d, Condition: %s, Action: %s\n\n\n",
               arr->data[i].id,
               arr->data[i].rule_id,
               arr->data[i].property_id,
               arr->data[i].condition,
               arr->data[i].action);
    }
}

void afficherPropertyRuleArrayList(const PropertyRuleArrayList* list) {
    if (list == NULL) {
        printf("PropertyRuleArrayList est NULL.\n");
        return;
    }
    printf("Taille du PropertyRuleArrayList: %d\n", list->size);
    for (int i = 0; i < list->size; i++) {
        printf("  --- Tableau de PropertyRule à l'index %d ---\n", i);
        afficherPropertyRuleArray(list->data[i]);
    }
}


PropertyRuleArrayList* groupPropertyRulesByRuleId(PropertyRuleArrayList* list) {
    PropertyRuleArrayList* groupedResult = initPropertyRuleArrayList();
    if (groupedResult == NULL || list == NULL) {
        return NULL;
    }

    if (list->size == 0) {
        return groupedResult;
    }

    int *processedRuleIds = NULL;
    int processedCount = 0;
    int processedCapacity = 0;

    for (int i = 0; i < list->size; i++) {
        PropertyRuleArray* currentPropertyRules = list->data[i];
        for (int j = 0; j < currentPropertyRules->size; j++) {
            int currentRuleId = currentPropertyRules->data[j].rule_id;
            int alreadyProcessed = 0;
            for (int k = 0; k < processedCount; k++) {
                if (processedRuleIds[k] == currentRuleId) {
                    alreadyProcessed = 1;
                    break;
                }
            }

            if (!alreadyProcessed) {
                PropertyRuleArray* newRuleGroup = initPropertyRuleArray();
                if (newRuleGroup == NULL) {
                    freePropertyRuleArrayList(groupedResult);
                    free(processedRuleIds);
                    return NULL;
                }

                for (int x = 0; x < list->size; x++) {
                    PropertyRuleArray* innerArray = list->data[x];
                    for (int y = 0; y < innerArray->size; y++) {
                        if (innerArray->data[y].rule_id == currentRuleId) {
                            addPropertyRule(newRuleGroup, innerArray->data[y]);
                        }
                    }
                }
                addPropertyRuleArray(groupedResult, newRuleGroup);

                if (processedCount >= processedCapacity) {
                    processedCapacity = (processedCapacity == 0) ? 5 : processedCapacity * 2;
                    int *newProcessedIds = (int*)realloc(processedRuleIds, processedCapacity * sizeof(int));
                    if (newProcessedIds == NULL) {
                        freePropertyRuleArrayList(groupedResult);
                        free(processedRuleIds);
                        return NULL;
                    }
                    processedRuleIds = newProcessedIds;
                }
                processedRuleIds[processedCount++] = currentRuleId;
            }
        }
    }

    free(processedRuleIds);
    return groupedResult;
}

MatchesList* initMatchesList() {
    MatchesList* list = (MatchesList*)malloc(sizeof(MatchesList));
    if (list == NULL) {
        perror("Failed to allocate MatchesList");
        return NULL;
    }
    list->matches = NULL;
    list->size = 0;
    list->capacity = 0;
    return list;
}

// Fonction pour trouver les Matches avec le nbrMatches le plus élevé
MatchesList* findTopMatches(MatchesList* list) {
    MatchesList* result = initMatchesList();
    if (result == NULL || list == NULL || list->size == 0) {
        return result; // Retourne une liste vide en cas d'erreur ou de liste d'entrée vide
    }

    int maxNbrMatches = -1; // Initialiser avec une valeur inférieure à toute nbrMatches possible

    // Trouver la valeur maximale de nbrMatches
    for (int i = 0; i < list->size; i++) {
        if (list->matches[i].nbrMatches > maxNbrMatches) {
            maxNbrMatches = list->matches[i].nbrMatches;
        }
    }

    // Ajouter tous les Matches ayant la valeur maximale de nbrMatches à la liste de résultat
    for (int i = 0; i < list->size; i++) {
        if (list->matches[i].nbrMatches == maxNbrMatches) {
            addMatches(result, list->matches[i]);
        }
    }

    return result;
}

// Fonction pour afficher le contenu d'une MatchesList (pour le test)
void printMatchesList(MatchesList* list) {
    if (list == NULL) {
        printf("MatchesList is NULL.\n");
        return;
    }
    printf("MatchesList (size: %d):\n", list->size);
    for (int i = 0; i < list->size; i++) {
        printf("  Rule ID: %d, Nbr Matches: %d\n", list->matches[i].rule_id, list->matches[i].nbrMatches);
    }
}

MatchesList* matchPropertyRuleArray(Property* properties, int propertiesSize,const PropertyRuleArrayList* arr) {

    MatchesList* result = initMatchesList();
    if (result == NULL) {
        return 1;
    }

    for (int j = 0; j < arr->size; j++)
    {
        Matches match;
        match.nbrMatches=0;
        match.rule_id=arr->data[j]->data->rule_id;
        for (int i = 0; i < arr->data[j]->size; i++) {
            
            for(int k = 0; k < propertiesSize; k++)
            {
                if(arr->data[j]->data[i].property_id==properties[k].id){
                    match.nbrMatches++;                   
                }
            }

        }

        addMatches(result,match);
    }
    
    // printMatchesList(result);

    MatchesList* topMatchesList = findTopMatches(result);
    // printf("\nListe des Matches avec le nbrMatches le plus élevé:\n");
    // printMatchesList(topMatchesList);

    freeMatchesList(result);
    freeMatchesList(topMatchesList);
    return topMatchesList;
    
}

// Implémentation de la fonction pour ajouter un Matches à une MatchesList
void addMatches(MatchesList* list, Matches match) {
    if (list == NULL) {
        fprintf(stderr, "Error: MatchesList pointer is NULL in addMatches.\n");
        return;
    }
    if (list->size >= list->capacity) {
        list->capacity = (list->capacity == 0) ? 10 : list->capacity * 2;
        Matches* newMatches = (Matches*)realloc(list->matches, list->capacity * sizeof(Matches));
        if (newMatches == NULL) {
            perror("Failed to reallocate Matches array");
            return;
        }
        list->matches = newMatches;
    }
    list->matches[list->size++] = match;
}

// Implémentation de la fonction pour libérer la mémoire d'une MatchesList
void freeMatchesList(MatchesList* list) {
    if (list) {
        free(list->matches);
        free(list);
    }
}






#endif 