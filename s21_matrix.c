#include "s21_matrix.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int flag = 1;
  if (rows <= 0 || columns <= 0) {
    return flag; // Возвращаем флаг, если количество строк или столбцов равно 0
  }

  result->matrix = calloc(rows, sizeof(double *));
  if (result->matrix == NULL) {
    return flag; // Возвращаем флаг, если выделение памяти для строки не удалось
  }

  for (int i = 0; i < rows; i++) {
    result->matrix[i] = calloc(columns, sizeof(double));
    if (result->matrix[i] == NULL) {
      // Освобождаем ранее выделенные ресурсы и возвращаем флаг ошибки
      for (int j = 0; j < i; j++) {
        free(result->matrix[j]);
      }
      free(result->matrix);
      return flag;
    }
  }

  result->rows = rows;
  result->columns = columns;
  flag = 0; // Устанавливаем флаг в 0, если все прошло успешно

  return flag;
}

void s21_remove_matrix(matrix_t *A) {
  if (!A || !A->matrix) { //проверка что матрица не NULL
    return;
  }
  for (int i = 0; i < A->rows; i++) {
    if (A->matrix[i]) { // Проверка, что строка не равна NULL
      free(A->matrix[i]);
    }
  }
  free(A->matrix);
}

int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  int flag = 1;
  if (A->rows == B->rows && A->columns == B->columns) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) > EPC) {
          flag = 1;
          break;
        }
      }
    }
  } else {
    flag = 0;
  }
  return flag;
}

int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int flag = 0;
  if (A->rows == B->rows && A->columns == B->columns) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
      }
    }
  } else {
    flag = 2;
  }
  return flag;
}

int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  int flag = 0;
  if (A->rows == B->rows && A->columns == B->columns) {
    s21_create_matrix(A->rows, A->columns, result);
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
      }
    }
  } else {
    flag = 2;
  }
  return flag;
}

int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  s21_create_matrix(A->rows, A->columns, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return 0;
}


int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (A->rows == 0 || A->columns == 0 || B->rows == 0 || B->columns == 0) {
    return 1;
  }

  int ret = s21_create_matrix(A->rows, B->columns, result);
  if (ret != 0) {
    return 1;
  }

  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      for (int k = 0; k < A->columns; k++) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }

  return 0;
}


///int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
///  if (A->rows == 0 || A->columns == 0 || B->rows == 0 || B->columns == 0) {
///    return 2;
///  }
///  s21_create_matrix(A->rows, B->columns, result);
///  for (int i = 0; i < A->rows; i++) {
///    for (int j = 0; j < B->columns; j++) {
///      for (int k = 0; k < A->columns; k++) {
///        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
///      }
///    }
///  }
///  return 0;
///}

int s21_transpose(matrix_t *A, matrix_t *result) {
    if (A->rows == 0 || A->columns == 0) {
    return 1; 
  }
  s21_create_matrix(A->columns, A->rows, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }
  return 0;
}

void s21_get_calc(int row, int col, matrix_t *A, matrix_t *result) {
  int a_row = 0;
  int a_col = 0;

  for (int i = 0; i < A->rows; i++) {
    if (i == row) {
      continue;
    }

    for (int j = 0; j < A->columns; j++) {
      if (j == col) {
        continue;
      }

      result->matrix[a_row][a_col] = A->matrix[i][j];
      a_col++;

      if (a_col >= A->columns) {
        a_col = 0;
        a_row++;
      }
    }
  }
}

double s21_get_determinant(matrix_t *A) {
  if (A->rows == 0 || A->columns == 0) {
    return 1;
  } else if (A->rows == 1) {
    return A->matrix[0][0];
  } else {
    matrix_t tmp;
    if (s21_create_matrix(A->rows - 1, A->columns - 1, &tmp) != 1) {
      return 1;
    }
    long long flag = 0;
    for (int i = 0; i < A->columns; i++) {
      int is_even = i % 2 == 0;
      s21_get_calc(0, i, A, &tmp);
      flag += (is_even ? 1 : -1) * A->matrix[0][i] * s21_get_determinant(&tmp);
    }
    s21_remove_matrix(&tmp);
    return flag;
  }
}

///double s21_get_determinant(matrix_t *A) {
///  if (A->rows == 1) {
///    return A->matrix[0][0];
///  } else {
///    matrix_t tmp;
///    s21_create_matrix(A->rows - 1, A->columns - 1, &tmp);
///    double flag = 0;
///    for (int i = 0; i < A->columns; i++) {
///      s21_get_calc(0, i, A, &tmp);
///      if (i % 2) {
///        flag -= A->matrix[0][i] * s21_get_determinant(&tmp);
///      } else {
///        flag += A->matrix[0][i] * s21_get_determinant(&tmp);
///      }
///    }
///    s21_remove_matrix(&tmp);
///    return flag;
///  }
///}

int s21_determinant(matrix_t *A, double *result) {
  if (A->rows != A->columns) {
    return 1;
  }

  *result = s21_get_determinant(A);
  return 0;
}

int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (A->rows != A->columns) {
    return 1;
  }

  s21_create_matrix(A->columns, A->rows, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      matrix_t calc;
      double determinant = 0;
      s21_create_matrix(A->columns - 1, A->rows - 1, &calc);
      s21_get_calc(i, j, A, &calc);
      s21_determinant(&calc, &determinant);
      result->matrix[i][j] = pow(-1, (i + j)) * determinant;
      s21_remove_matrix(&calc);
    }
  }

  return 0;
}

int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  int flag = 0;
  if (A == NULL || A->matrix == NULL || A->rows < 0 || A->columns < 0) {
    flag = 1;
  } else if (A->rows != A->columns) {
    flag = 2;
  } else {
    double det = 0;
    s21_determinant(A, &det);
    if (fabs(det) > 1e-7) {
      matrix_t tempM1, tempM2;
      s21_calc_complements(A, &tempM1);
      s21_transpose(&tempM1, &tempM2);
      s21_remove_matrix(&tempM1);
      s21_mult_number(&tempM2, 1.0 / det, result);
      s21_remove_matrix(&tempM2);
    } else {
      flag = 2;
    }
  }
  return flag;
}


void s21_initialize_matrix(matrix_t *A, double start_value,
                           double iteration_step) {
  if (A != NULL && A->matrix != NULL) {
    double value = start_value;
    for (int i = 0; i < A->rows; i++)
      for (int j = 0; j < A->columns; j++) {
        A->matrix[i][j] = value;
        value += iteration_step;
      }
  }
};
