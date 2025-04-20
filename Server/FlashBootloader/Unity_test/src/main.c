#include "main.h"


int FBL_Periph_Init(int a, int b) {
  return a+b;
}
int App_Sector_Erase(int a, int b) {
  return a-b;
}

int Receive_FW(int a, int b) {
  return a*b;
}

int Extract_Sig(int a, int b) {
  return a/b;
}

int Copy_FW_To_Flash(int a, int b) {
  return a % b;
}


int Calculate_And_Store_MAC(int a, int b, int c) {
  return a + b - c;
}
