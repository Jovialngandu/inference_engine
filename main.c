#include <stdio.h>
#include <stdlib.h>
#include "library/cJson/cJSON.h"
#include "include/helpers.h"
#include "include/structure.h"


int main()
{

    cJSON *datas=pythonExecute();

    int tailleCharacteristics;
    int tailleProperties;
    Character *caracteristiques = (Character *)get(datas, "Characteristic", &tailleCharacteristics);
    Property *property = (Property *)get(datas, "Property", &tailleProperties);
    characterUserValue* userValues= menu(caracteristiques,tailleCharacteristics,property,tailleProperties);


    // for(size_t z = 0; z <tailleCharacteristics ; z++)
    // {
    //    printf("\n character id :%d- property id: %d \n",userValues[z].characterId,userValues[z].propertyId);
    // }


    // characterUserValue userValues[] = {{1, 1}, {1, 2}}; // Exemple de tableau userValues
    int userValuesSize = sizeof(userValues) / sizeof(userValues[0]);
    int matchingFactsSize;
    Fact *matchingFacts = getMatchingFacts(datas, userValues, userValuesSize, &matchingFactsSize);

    if (matchingFacts) {
        printf("Faits correspondants trouvés : %d\n", matchingFactsSize);
        for (int i = 0; i < matchingFactsSize; i++) {
            printf("Fait ID: %d, Nom: %s\n", matchingFacts[i].id, matchingFacts[i].name);
        }
        free(matchingFacts);
    } else {
        printf("Aucun fait correspondant trouvé.\n");
    }

    cJSON_Delete(datas);
    return 0;
}
