#define PATH_GLOBAL_DIR "/home/joachim/system/c/projet2/"

#define FILE_NAME_INDEX "index.jjg"
#define FILE_NAME_STOCKAGE "stockage.jjg"
#define FILE_INDEX PATH_GLOBAL_DIR FILE_NAME_INDEX
#define FILE_STOCK PATH_GLOBAL_DIR FILE_NAME_STOCKAGE

#define DIR_SWP_NAME "swp"
#define PATH_FOLDER_SWAP PATH_GLOBAL_DIR DIR_SWP_NAME "/"

#define DIR_BIN_NAME "bin"
#define PATH_BIN_FILE PATH_GLOBAL_DIR DIR_BIN_NAME "/"

#define FILE_BASE_ENV 8+1+8+3*(1+8+255) // base du fichier index, comptant tout l'envirronement
#define INDEX_LINE_SIZE (1 + 8 + 1 + 255 + 8 + 8)

#define FICHIER 1
#define DOSSIER 0

// /home/joachim/system/c/projet/

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _b : _a; })
