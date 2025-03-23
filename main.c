#include <stdio.h>
#include <stdlib.h>
#include "library/cJson/cJSON.h"
int main()
{
    cJSON *a=cJSON_CreateObject();
    cJSON_AddStringToObject(a,"nom","post");
    char *json_str= cJSON_Print(a);
    printf(json_str,"%s");
    return 0;
}
