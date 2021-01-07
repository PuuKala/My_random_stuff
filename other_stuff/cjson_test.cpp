#include "cJSON.h"

int main(int argc, char const *argv[])
{
    char json_text[] =  "{\"test\":1,\"test2\":2,\"test3\":1.0,\"test4\":true}\0";
    cJSON *json = cJSON_Parse(json_text);

    return 0;
}
