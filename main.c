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

            Fact *matchingFacts = findFactsWithAllProperties(datas, selectedProperties, selectedPropertiesSize, &matchingFactsSize);

            if (matchingFacts) {
                printf("Faits correspondants trouvés (%d):\n", matchingFactsSize);
                for (int i = 0; i < matchingFactsSize; i++) {
                    printf("ID: %d, Nom: %s, Description: %s\n",
                        matchingFacts[i].id, matchingFacts[i].name, matchingFacts[i].description);
                }
                free(matchingFacts);
            } else {
                printf("Aucun fait trouvé\n");
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
