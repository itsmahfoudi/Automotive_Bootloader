#ifndef MAIN_H
#define MAIN_H

int FBL_Periph_Init(int a, int b);
int App_Sector_Erase(int a, int b);
int Receive_FW(int a, int b);
int Extract_Sig(int a, int b);
int Copy_FW_To_Flash(int a, int b);
int Calculate_And_Store_MAC(int a, int b, int c);
#endif // MAIN_H
