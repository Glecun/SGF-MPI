void get_version(char * v);
void get_index_last_cursor(unsigned long long * c);
void get_stock_last_cursor(unsigned long long * c);
void set_stock_last_cursor( unsigned long long cursor );
void get_parent(unsigned long long * p);
void set_parent(unsigned long long p);
unsigned long long file_exist(char arg_type_file, char * arg_file_name);
void put_file(char * path_filename, unsigned long long cursor_stock, unsigned long long * file_size);
void ajouterLigne(unsigned long long cursor, char active, unsigned long long parent, char file_type, char * file_name, unsigned long long file_cursor_stock, unsigned long long file_size);
