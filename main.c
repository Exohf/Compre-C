#include <stdio.h>
#include <string.h>
#include <zip.h>
#include <stdlib.h>

struct zip *openZip(const char *zip_filename) {
    struct zip *zip_archive = zip_open(zip_filename, ZIP_CHECKCONS, NULL);
    if (!zip_archive) {
        const char *error_msg = zip_strerror(zip_archive);
        if (strcmp(error_msg, "password required") == 0) {
            fprintf(stderr, "Failed to open zip archive. The archive is password protected.\n");
        } else {
            fprintf(stderr, "Failed to open zip archive: %s\n", error_msg);
        }
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

int isFileProtected(struct zip *zip_archive, const char *file_name) {
    struct zip_stat file_stat;
    if (zip_stat(zip_archive, file_name, 0, &file_stat) != 0) {
        perror("Failed to get file information");
        return -1;
    }

    return (file_stat.encryption_method != ZIP_EM_NONE) ? 1 : 0;
}

void printFileNames(struct zip *zip_archive, const char *zip_filename, int num_files) {
    printf("Entries in %s:\n", zip_filename);
    int fileIndex = 0;
    for (int i = 0; i < num_files; ++i) {
        const char *file_name = zip_get_name(zip_archive, i, 0);
        if (!file_name) {
            perror("Failed to get file name");
            return;
        }

        // Check if the entry is a directory based on its name
        if (file_name[strlen(file_name) - 1] != '/') {
            fileIndex++;
            if (isFileProtected(zip_archive, file_name)) {
                printf("%d - [Protected] %s\n", fileIndex, file_name);
            } else {
                printf("%d - %s\n", fileIndex, file_name);
            }
        }
    }
}


void closeZip(struct zip *zip_archive) {
    if (zip_close(zip_archive) < 0) {
        perror("Failed to close zip archive");
        return;
    }
}

void displayFileContent(struct zip *zip_archive, int file_index) {
    if (!zip_archive) {
        fprintf(stderr, "Error: ZIP archive is not open.\n");
        return;
    }

    struct zip_file *file = zip_fopen_index(zip_archive, file_index - 1, 0);
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char buffer[1024];
    int len;
    while ((len = zip_fread(file, buffer, sizeof(buffer))) > 0) {
        fwrite(buffer, 1, len, stdout);
    }

    zip_fclose(file);
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
    
    char choice_str[10];
    while (1) {
        printf("Enter the number of the file to display its content (or 'q' to quit): ");
        if (scanf("%s", choice_str) != 1) {
            fprintf(stderr, "Error: Invalid input.\n");
            continue;
        }

        if (strcmp(choice_str, "q") == 0) {
            printf("Quitting...\n");
            break;
        }

        int choice = atoi(choice_str);
        if (choice < 1 || choice > num_files) {
            fprintf(stderr, "Error: Unknown index. Please try again.\n");
            continue;
        }

        displayFileContent(zip_archive, choice);
    }

    closeZip(zip_archive);
}

void displayHelp() {
    printf("Usage: program_name [options]\n");
    printf("Options:\n");
    printf(" -h, --help\t\t\tShow this help\n");
    printf(" -o, --open\t\t\tOpen a zip file for browsing\n");
    printf(" -b, --bruteforce\t\tTry to bruteforce the password\n");
    printf(" -d, --dictionary=FILE\t\tTry to bruteforce the password with a dictionary\n");
    printf(" -p, --password=PASSWORD\tUse this password\n");
    printf(" -e, --extract=FILE\t\tExtract this file\n");
    printf(" -i, --include=FILE\t\tInclude this file\n");
}

int isOption(const char *str) {
    const char *options[] = {"-h", "--help", "-o", "--open", "-b", "--bruteforce",
                             "-d", "--dictionary", "-p", "--password", "-e", "--extract",
                             "-i", "--include"};
    int num_options = sizeof(options) / sizeof(options[0]);

    for (int i = 0; i < num_options; i++) {
        if (strcmp(str, options[i]) == 0) {
            return 1; // Match found
        }
    }
    return 0; // No match found
}

void handleArgs(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if ((strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) && i == 1){
            displayHelp();
            exit(0);
        }

        if ((strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--open") == 0) && i == 1){
            if (argc <= i + 1) {
                fprintf(stderr, "Error: No zip file specified.\n");
                exit(0);
            }

            read_zip(argv[i + 1]);
        }
    }
}

int main(int argc, char *argv[]) {
    handleArgs(argc, argv);

    return 0;
}