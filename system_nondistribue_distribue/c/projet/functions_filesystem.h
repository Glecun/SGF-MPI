void  parsing( char * s, char ** r, int * c);
int exec_prog (char ** argv);
int print_y_n(char * s);
char * inputString(FILE* fp, size_t size);
void init_bdd();
void if_file_isset_then_delete(char * fileName);
void if_dir_isset_then_delete(char * dirName);
int remove_directory(char * dirName);