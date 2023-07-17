#include <check.h>
#include <stdio.h>

#include "parse.h"

int eqData(data_t *res, data_t *dupl);
int fillData(data_t *data, char *fileName);

START_TEST(shift_X_min_test) {
  data_t original = {0};
  int error = fillData(&original, "test_file/min.obj");
  // data_t dupl = {0};
  // dup(&original, &dupl);
  shifting(&original, 5, 0, 0);
  // shifting(&original, &dupl, Ax, 5);
  ck_assert_int_eq(error, 0);
  if (!error) {
    data_t res = {0};
    error = fillData(&res, "test_file/minX5.obj");
    error = !eqData(&res, &original);
    ck_assert_int_eq(error, 0);
    structFree(&res);
  }
  structFree(&original);
  // structFree(&dupl);
}
END_TEST

START_TEST(shift_Y_min_test) {
  data_t original = {0};
  int error = fillData(&original, "test_file/min.obj");
  // data_t dupl = {0};
  // dup(&original, &dupl);
  shifting(&original, 0, -5.9, 0);
  // shifting(&original, &dupl, Ay, -5.9);
  if (!error) {
    data_t res = {0};
    fillData(&res, "test_file/minY59.obj");
    ck_assert_int_eq(eqData(&res, &original), 1);
    structFree(&res);
  }
  ck_assert_int_eq(error, 0);
  structFree(&original);
  // structFree(&dupl);
}
END_TEST

START_TEST(shift_Z_min_test) {
  data_t original = {0};
  int error = fillData(&original, "test_file/min.obj");
  // data_t dupl = {0};
  // dup(&original, &dupl);

  shifting(&original, 0, 0, 6.4);
  // shifting(&original, &dupl, Az, 6.4);

  if (!error) {
    data_t res = {0};
    fillData(&res, "test_file/minZ6.obj");
    ck_assert_int_eq(eqData(&res, &original), 1);
    structFree(&res);
  }
  ck_assert_int_eq(error, 0);
  structFree(&original);
  // structFree(&dupl);
}
END_TEST

START_TEST(scale_plus_min_test) {
  data_t original = {0};
  int error = fillData(&original, "test_file/min.obj");
  data_t dupl = {0};
  dup(&original, &dupl);
  scaling(&original, &dupl, 1.56);
  if (!error) {
    data_t res = {0};
    fillData(&res, "test_file/minScal156.obj");
    ck_assert_int_eq(eqData(&res, &dupl), 1);
    structFree(&res);
  }
  ck_assert_int_eq(error, 0);
  structFree(&original);
  structFree(&dupl);
}
END_TEST

START_TEST(scale_zero_min_test) {
  data_t original = {0};
  int error = fillData(&original, "test_file/min.obj");
  data_t dupl = {0};
  dup(&original, &dupl);
  scaling(&original, &dupl, 0.63);
  if (!error) {
    data_t res = {0};
    fillData(&res, "test_file/minScal063.obj");
    ck_assert_int_eq(eqData(&res, &dupl), 1);
    structFree(&res);
  }
  ck_assert_int_eq(error, 0);
  structFree(&original);
  structFree(&dupl);
}
END_TEST

START_TEST(scale_minus_min_test) {
  data_t original = {0};
  int error = fillData(&original, "test_file/min.obj");
  data_t dupl = {0};
  dup(&original, &dupl);
  scaling(&original, &dupl, -2);
  if (!error) {
    data_t res = {0};
    fillData(&res, "test_file/minScal-2.obj");
    ck_assert_int_eq(eqData(&res, &dupl), 1);
    structFree(&res);
  }
  ck_assert_int_eq(error, 0);
  structFree(&original);
  structFree(&dupl);
}
END_TEST

START_TEST(rotation_X_min_test) {
  data_t original = {0};
  int error = fillData(&original, "test_file/min.obj");
  data_t dupl = {0};
  dup(&original, &dupl);
  rotation(&original, &dupl, Ax, 10);
  if (!error) {
    data_t res = {0};
    int error = fillData(&res, "test_file/minRotX10.obj");
    ck_assert_int_eq(eqData(&res, &dupl), 1);
    structFree(&res);
  }
  ck_assert_int_eq(error, 0);
  structFree(&original);
  structFree(&dupl);
}
END_TEST

START_TEST(rotation_Y_min_test) {
  data_t original = {0};
  int error = fillData(&original, "test_file/min.obj");
  data_t dupl = {0};
  dup(&original, &dupl);
  rotation(&original, &dupl, Ay, 25.3);
  if (!error) {
    data_t res = {0};
    int error = fillData(&res, "test_file/minRotY25.obj");
    ck_assert_int_eq(eqData(&res, &dupl), 1);
    structFree(&res);
  }
  ck_assert_int_eq(error, 0);
  structFree(&original);
  structFree(&dupl);
}
END_TEST

START_TEST(rotation_Z_min_test) {
  data_t original = {0};
  int error = fillData(&original, "test_file/min.obj");
  data_t dupl = {0};
  dup(&original, &dupl);
  rotation(&original, &dupl, Az, -90);
  if (!error) {
    data_t res = {0};
    int error = fillData(&res, "test_file/minRotZ-90.obj");
    ck_assert_int_eq(eqData(&res, &dupl), 1);
    structFree(&res);
  }
  ck_assert_int_eq(error, 0);
  structFree(&original);
  structFree(&dupl);
}
END_TEST

Suite *Viewer_suite(void) {
  Suite *suite;
  TCase *core;
  suite = suite_create("3dViewer");
  core = tcase_create("Core");

  tcase_add_test(core, shift_X_min_test);
  tcase_add_test(core, shift_Y_min_test);
  tcase_add_test(core, shift_Z_min_test);
  tcase_add_test(core, scale_plus_min_test);
  tcase_add_test(core, scale_zero_min_test);
  tcase_add_test(core, scale_minus_min_test);
  tcase_add_test(core, rotation_X_min_test);
  tcase_add_test(core, rotation_Y_min_test);
  tcase_add_test(core, rotation_Z_min_test);

  suite_add_tcase(suite, core);

  return (suite);
}

int main(void) {
  int failed = 0;
  Suite *suite;

  SRunner *runner;

  suite = Viewer_suite();
  runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

int eqData(data_t *res, data_t *dupl) {
  int eq = 1;
  eq = (res->numVertex == dupl->numVertex && res->numEdges == dupl->numEdges)
           ? 1
           : 0;
  if (eq) {
    for (int i = 0; i < res->numVertex; i++)
      for (int y = 0; y < 3; y++) {
        if (round(res->vertexes[i][y] * pow(10, 6)) !=
            round(dupl->vertexes[i][y] * pow(10, 6))) {
          eq = 0;
          break;
        }
        if (!eq) break;
      }
  }
  return eq;
}

int fillData(data_t *data, char *fileName) {
  int error = readFile(fileName, data);
  callocStruct(data);
  scanFile(fileName, data);
  return error;
}
