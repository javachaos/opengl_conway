#include <err.h>
#include <CUnit/Basic.h>
#include <binary_matrix.h>
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

void test_ctor(void)
{
    BinaryMatrix *matrix = ConstructBinaryMatrix(8, 8);
    CU_ASSERT_EQUAL(matrix->num_cols, 8);
    CU_ASSERT_EQUAL(matrix->num_rows, 8);
    CU_ASSERT_EQUAL(matrix->data[0], 0);
    CU_ASSERT_EQUAL(matrix->data[1], 0);
    PrintMatrix(matrix);
    DeleteBinaryMatrix(matrix);
    
    BinaryMatrix *m = ConstructBinaryMatrix(8, -8);
    CU_ASSERT_PTR_NULL(m);
    BinaryMatrix *n = ConstructBinaryMatrix(-8, 8);
    CU_ASSERT_PTR_NULL(n);
    BinaryMatrix *o = ConstructBinaryMatrix(-8, -8);
    CU_ASSERT_PTR_NULL(o);

    //This test might not be 100% future proof, as system memory might change in the future.
    //Tested to work on a system with 32GB of RAM. (Will fail if system memory is greater than 576460.75 Terabytes)
    BinaryMatrix *p = ConstructBinaryMatrix(2147483647, 2147483647);
    CU_ASSERT_PTR_NULL(p);

    // Will fail if system memory is less than 800 megabytes.
    BinaryMatrix *large_matrix = ConstructBinaryMatrix(80000, 80000);
    CU_ASSERT_EQUAL(large_matrix->num_cols, 80000);
    CU_ASSERT_EQUAL(large_matrix->num_rows, 80000);
    for( int i = 0; i < 20000; i++ )
    {
        CU_ASSERT_EQUAL(large_matrix->data[i], 0);
    }
    DeleteBinaryMatrix(large_matrix);
}

int main(void) {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        errx(EXIT_FAILURE, "can't initialize test registry");
    }
    CU_pSuite binary_matrix_test_suite = CU_add_suite("binary_matrix_test", NULL, NULL);
    if (CU_get_error() != CUE_SUCCESS) {
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    }
    CU_add_test(binary_matrix_test_suite, "ctor", test_ctor);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}