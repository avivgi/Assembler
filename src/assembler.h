#ifndef __ASSEMBLER__
#define __ASSEMBLER__

#include "language.h"
#include "datamodel.h"

void handleSourceFile(char *arg, Data_model data_model, Language language);
int main(int argc, char *argv[]);
void initlize_language(Language *language);
#endif
