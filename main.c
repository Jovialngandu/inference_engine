#include <stdio.h>
#include <stdlib.h>
#include "library/cJson/cJSON.h"
#include "include/helpers.h"
#include "include/structure.h"


int main()
{

    int numCharacters;
    int matchingFactsSize;
    int numProperties;
    int selectedPropertiesSize;


    cJSON *datas=pythonExecute();

    Character *characters = (Character *)get(datas, "Characteristic", &numCharacters);
    Property *properties = (Property *)get(datas, "Property", &numProperties);

    if (characters && properties) {
        Property *selectedProperties = getUserSelectedProperties(characters, numCharacters, properties, numProperties, &selectedPropertiesSize);

        if (selectedProperties) {
            system("cls");
           for (int i = 0; i < selectedPropertiesSize; i++) {
                printf("_ : %s\n",selectedProperties[i].libelle);
             }        
        // if (allPropertyRules) {
        //     printf("PropertyRules regroupées par Property:\n");
        //     for (int i = 0; i < allPropertyRules->size; i++) {
        //         printf("\nProperty ID: %d, Libelle: %s\n", properties[i].id, properties[i].libelle);
        //         afficherPropertyRuleArray(allPropertyRules->data[i]);
        //     }
        //     freePropertyRuleArrayList(allPropertyRules);

            Fact *matchingFacts = findFactsWithAllProperties(datas, selectedProperties, selectedPropertiesSize, &matchingFactsSize);

            if (matchingFacts) {
                printf("Faits correspondants trouvés (%d):\n", matchingFactsSize);
                for (int i = 0; i < matchingFactsSize; i++) {
                    printf("__%s\n",matchingFacts[i].description);
                }
                free(matchingFacts);
            } else {
                PropertyRuleArrayList* allPropertyRules = generatePropertyRulesByPropertyFromJSON(datas, properties, numProperties);
                PropertyRuleArrayList* groupedByRule = groupPropertyRulesByRuleId(allPropertyRules);
                MatchesList *rematch= matchPropertyRuleArray(selectedProperties,selectedPropertiesSize,groupedByRule);

                int numRules;

                Rule *rules = (Rule *)get(datas, "Rule", &numRules);
                
                // printMatchesList(rematch);

                // printf("Affichage des noms de règles pour chaque MatchesList:\n");
                // displayRuleNamesForMatchesList(rules, numRules, rematch);                // afficherPropertyRuleArrayList(groupedByRule);
                displayUniqueRuleNamesForMatchesList(rules, numRules, rematch);
                freePropertyRuleArrayList(groupedByRule);
            }

            free(selectedProperties);
        } else {
                printf("\nAucune propriété sélectionnée.\n");
        }
        free(characters);
        free(properties);
    } else {
        fprintf(stderr, "Erreur lors de la récupération des caractéristiques ou des propriétés depuis la base de connaissance.\n");
    }

    cJSON_Delete(datas);
  
}
