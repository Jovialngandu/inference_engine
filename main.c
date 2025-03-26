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

           for (int i = 0; i < selectedPropertiesSize; i++) {
                printf("Caractéristique ID (via Property): %d -> Propriété sélectionnée: %s (ID: %d)\n",
               selectedProperties[i].characteristic_id, selectedProperties[i].libelle, selectedProperties[i].id);
             }
             printf("size %d\n",selectedPropertiesSize);
        
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
                    printf("ID: %d, Nom: %s, Description: %s\n",
                        matchingFacts[i].id, matchingFacts[i].name, matchingFacts[i].description);
                }
                free(matchingFacts);
            } else {
                PropertyRuleArrayList* allPropertyRules = generatePropertyRulesByPropertyFromJSON(datas, properties, numProperties);
                PropertyRuleArrayList* groupedByRule = groupPropertyRulesByRuleId(allPropertyRules);
                MatchesList *rematch= matchPropertyRuleArray(selectedProperties,selectedPropertiesSize,groupedByRule);
                printMatchesList(rematch);
                // afficherPropertyRuleArrayList(groupedByRule);
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
