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

    printf("\n--- Récapitulatif des sélections ---:\n");
    for (int i = 0; i < *selectedPropertiesSize; i++) {
        printf("Caractéristique ID (via Property): %d -> Propriété sélectionnée: %s (ID: %d)\n",
               selectedProperties[i].characteristic_id, selectedProperties[i].libelle, selectedProperties[i].id);
    }

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

#endif 