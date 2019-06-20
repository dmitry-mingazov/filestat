#include "unity.h"
#include "filestatConf.h"
#include "rw_file.h"

void setUp(void){

}

void tearDown(void){

}

void test_function_getOptions_VS(void){
  
    // filestat_configuration fstest;
    // filestat_configuration *pfstest = &fstest;
    // char *argv[] = {"filestat", "-v", "-s", NULL};
    // int argc = (int) (sizeof(argv)/sizeof(char*) -1);
    // getOptions(argc, argv, &pfstest);
}

int main(void){
    UNITY_BEGIN();
    RUN_TEST(test_function_getOptions_VS);
    return UNITY_END();
}
