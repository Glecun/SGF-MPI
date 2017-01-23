#define PATH_BIN_FILE "/home/joachim/system/c/projet/bin/" 
#define FILE_INDEX "/home/joachim/system/c/projet/index.jjg"
#define FILE_STOCK "/home/joachim/system/c/projet/stockage.jjg"
#define PATH_FOLDER_SWAP "/home/joachim/system/c/projet/swp/"
#define FILE_BASE_ENV 8+1+8+3*(1+8+255) // base du fichier index, comptant tout l'envirronement
#define FICHIER 1
#define DOSSIER 0

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })
