#include <stdio.h>
#include <string.h>
#include <zip.h>

struct zip *openZip(const char *zip_filename) {
    struct zip *zip_archive = zip_open(zip_filename, 0, NULL);
    if (!zip_archive) {
        perror("Failed to open zip archive");
        return NULL;
    }
    return zip_archive;
}

int getNumFiles(struct zip *zip_archive) {
    int num_files = zip_get_num_files(zip_archive);
    if (num_files < 0) {
        perror("Failed to get number of files in zip archive");
        return -1;
    }
    return num_files;
}

void printFileNames(struct zip *zip_archive, const char *zip_filename, int num_files) {
    printf("Files in %s:\n", zip_filename);
    for (int i = 0; i < num_files; ++i) {
        const char *file_name = zip_get_name(zip_archive, i, 0);
        if (!file_name) {
            perror("Failed to get file name");
            return;
        }
        printf("%d - %s\n", i + 1,file_name);
    }
}

void closeZip(struct zip *zip_archive) {
    if (zip_close(zip_archive) < 0) {
        perror("Failed to close zip archive");
        return;
    }
}

void read_zip(const char *zip_filename) {
    struct zip *zip_archive = openZip(zip_filename);
    if (!zip_archive) {
        return;
    }

    int num_files = getNumFiles(zip_archive);
    if (num_files < 0) {
        zip_close(zip_archive);
        return;
    }

    printFileNames(zip_archive, zip_filename, num_files);
    
    closeZip(zip_archive);
}

int main() {
    read_zip("test.zip");
    return 0;
}