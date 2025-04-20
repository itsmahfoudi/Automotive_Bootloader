#include "main.h"

int Initialize(int a, int b) {
  return a+b;
}

int FBL_Check(int a, int b) {
  return a-b;
}

int CAL_Check(int a, int b) {
  return a*b;
}

int APP_Check(int a, int b) {
  return a/b;
}

int Set_Target_FW(int a, int b) {
  return a%b;
}

int ProgRequest_Check(int a, int b, int c) {
  return a+b-c;
}
