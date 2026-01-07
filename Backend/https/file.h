#ifndef FILE_H
#define FILE_H

typedef struct {
    size_t size;
    char *data;
} FileInfo;

typedef struct {
    char **paths;
    int count;
} FileList;

FileList files;


FileInfo get_file(const char* filename) {
    FileInfo result;
    result.data = NULL;
    result.size = 0;
    char path[1024];
    strcpy(path, HOME);
    strcat(path, filename);
    // printf("%s\n", path);
    FILE* file = fopen(path, "r");
    if (!file) {
        perror("Failed to open file");
        return result;
    }

    if (fseek(file, 0, SEEK_END) != 0) {
        perror("fseek failed");
        fclose(file);
        return result;
    }

    long size = ftell(file);
    if (size < 0) {
        perror("ftell failed");
        fclose(file);
        return result;
    }
    rewind(file);

    unsigned char* buffer = (unsigned char*)calloc(size/sizeof(unsigned char), sizeof(unsigned char));
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return result;
    }

    size_t read = fread(buffer, 1, size, file);
    fclose(file);

    if (read != (size_t)size) {
        fprintf(stderr, "Only read %zu of %ld bytes\n", read, size);
        free(buffer);
        return result;
    }

    result.data = buffer;
    result.size = read;
    //itlc = result;
    return result;
}



void listFilesRecursive(const char *basePath, const char *relativePath, FileList *result) {
    char fullPath[1024];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", basePath, relativePath);

    DIR *dir = opendir(fullPath);
    if (!dir) return;

    struct dirent *entry;
    while ((entry = readdir(dir))) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char newRelPath[1024];
        if (relativePath[0] != '\0')
            snprintf(newRelPath, sizeof(newRelPath), "%s/%s", relativePath, entry->d_name);
        else
            snprintf(newRelPath, sizeof(newRelPath), "/%s", entry->d_name);

        char fullEntryPath[1024];
        snprintf(fullEntryPath, sizeof(fullEntryPath), "/%s/%s", basePath, newRelPath);

        struct stat statbuf;
        if (stat(fullEntryPath, &statbuf) == -1) continue;

        if (S_ISDIR(statbuf.st_mode)) {
            listFilesRecursive(basePath, newRelPath, result);
        } else if (S_ISREG(statbuf.st_mode)) {
            result->paths = realloc(result->paths, sizeof(char*) * (result->count + 1));
            result->paths[result->count] = strdup(newRelPath);
            result->count++;
        }
    }

    closedir(dir);
}

FileList getAllFilePaths(const char *homeDir) {
    FileList result;
    result.paths = NULL;
    result.count = 0;

    listFilesRecursive(homeDir, "", &result);
    return result;
}


int is_file(char * path){
    for(int i = 0; i < files.count; i++)
        if(strcmp(path, files.paths[i]) == 0)
            return i;
    return -1;
}

#endif
