#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/wait.h>
#include <libgen.h>

#define MAX_PATH_LENGTH 4096
#define MAX_FILENAME_LENGTH 256

int match_pattern(const char *pattern, const char *str)
{
    while (*pattern)
    {
        if (*pattern == '*')
        {
            if (!*++pattern)
                return 1; // Pattern ends with '*'; it matches anything.
            while (*str)
                if (match_pattern(pattern, str++))
                    return 1;
            return 0;
        }
        if (*pattern++ != *str++)
            return 0;
    }
    return !*str && !*pattern;
}

void compile_assembly_file(const char *filename)
{
    char *assembler_path = "../bin/assembler";
    char *arguments[] = {assembler_path, (char *)filename, NULL};
    execvp(assembler_path, arguments);
    perror("execvp failed");
    exit(EXIT_FAILURE);
}

void process_directory(const char *dir_path)
{
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dir_path)) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_REG && strstr(entry->d_name, ".as") != NULL)
            {
                char file_path[MAX_PATH_LENGTH];
                sprintf(file_path, "%s/%s", dir_path, entry->d_name);
                char file_name[MAX_FILENAME_LENGTH];
                strncpy(file_name, entry->d_name, strlen(entry->d_name) - 3);
                file_name[strlen(entry->d_name) - 3] = '\0'; // Remove the .as extension

                pid_t pid = fork();
                if (pid == -1)
                {
                    perror("fork failed");
                    exit(EXIT_FAILURE);
                }
                else if (pid == 0)
                {                                      // Child process
                    freopen("/dev/null", "w", stdout); // Redirect stdout to /dev/null
                    compile_assembly_file(file_name);
                }
                else
                { // Parent process
                    int status;
                    waitpid(pid, &status, 0);
                    if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
                    {
                        // Successfully compiled the assembly file
                        char expected_files_pattern[MAX_FILENAME_LENGTH];
                        sprintf(expected_files_pattern, "%s.*.expected", file_name);

                        DIR *file_dir;
                        if ((file_dir = opendir(dir_path)) != NULL)
                        {
                            struct dirent *file_entry;
                            while ((file_entry = readdir(file_dir)) != NULL)
                            {
                                if (match_pattern(expected_files_pattern, file_entry->d_name))
                                {
                                    char expected_file_path[MAX_PATH_LENGTH];
                                    sprintf(expected_file_path, "%s/%s", dir_path, file_entry->d_name);

                                    char expected_ext[MAX_FILENAME_LENGTH];
                                    strncpy(expected_ext, file_entry->d_name + strlen(file_name), strlen(file_entry->d_name) - strlen(file_name) - 9);
                                    expected_ext[strlen(file_entry->d_name) - strlen(file_name) - 9] = '\0'; // Remove the .expected extension

                                    char result_file_path[MAX_PATH_LENGTH];
                                    sprintf(result_file_path, "%s/%s%s", dir_path, file_name, expected_ext);

                                    if (access(result_file_path, F_OK) != -1)
                                    {
                                        pid_t diff_pid = fork();
                                        if (diff_pid == -1)
                                        {
                                            perror("fork failed");
                                            exit(EXIT_FAILURE);
                                        }
                                        else if (diff_pid == 0)
                                        {
                                            freopen("/dev/null", "w", stdout);
                                            execlp("diff", "diff", result_file_path, expected_file_path, (char *)NULL);
                                            perror("execlp failed");
                                            exit(EXIT_FAILURE);
                                        }
                                        else
                                        {
                                            int diff_status;
                                            waitpid(diff_pid, &diff_status, 0);
                                            if (WIFEXITED(diff_status) && WEXITSTATUS(diff_status) == EXIT_SUCCESS)
                                            {
                                                printf("%s: passed\n", basename(result_file_path));
                                            }
                                            else
                                            {
                                                printf("%s: failed\n", basename(result_file_path));
                                            }
                                        }
                                    }
                                    else
                                    {
                                        printf("Test failed: %s does not exist\n", result_file_path);
                                    }
                                }
                            }
                            closedir(file_dir);
                        }
                    }
                }
            }
            else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 &&
                     strcmp(entry->d_name, "..") != 0)
            {
                char sub_dir_path[MAX_PATH_LENGTH];
                sprintf(sub_dir_path, "%s/%s", dir_path, entry->d_name);
                process_directory(sub_dir_path); // Recursively process subdirectory
            }
        }
        closedir(dir);
    }
    else
    {
        perror("opendir failed");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    char current_path[MAX_PATH_LENGTH];
    if (getcwd(current_path, sizeof(current_path)) == NULL)
    {
        perror("getcwd error");
        return EXIT_FAILURE;
    }

    process_directory(current_path);

    return EXIT_SUCCESS;
}
