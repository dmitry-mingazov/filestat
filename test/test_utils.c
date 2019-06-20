#include "../unity/src/unity.h"
#include "utils.h"

void setUp(void){

}

void tearDown(void){

}

void test_function_parse_mode777(void){
    char * perms = malloc(sizeof(char)*11);
    mode_t mode = 0100777;
    parse_mode(mode, perms);
    TEST_ASSERT_EQUAL_STRING(perms, "rwxrwxrwx");
}   

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_function_parse_mode777);
    return UNITY_END();
}