#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <libgen.h>
#include <signal.h>
#include <sys/wait.h> // For waitpid

#define MAX_PATH_LENGTH 4096
#define MAX_FILENAME_LENGTH 256

int compile_assembly_file(char *filename)
{
    pid_t pid = fork();
    filename[strlen(filename) - 3] = '\0'; // Remove the .as extension
    if (pid == -1)
    {
        // Fork failed
        perror("fork");
        return -1;
    }
    else if (pid == 0)
    {
        // Child process executes the assembler

        // silent stdout
        freopen("/dev/null", "w", stdout);
        execl("../bin/assembler", "assembler", filename, (char *)NULL);

        perror("execl"); // If execl returns, it means an error occurred
        // resume stdout
        freopen("/dev/tty", "w", stdout);
    }
    else
    {
        // Parent process waits for the child to finish
        int status;
        waitpid(pid, &status, 0);

        // Check the exit status of the assembler
        if (WIFEXITED(status))
        {
            int exit_code = WEXITSTATUS(status);
            if (exit_code == 0)
            {
                //    printf("Assembler exited gracefully.\n");
                return 0;
            }
            else
            {
                printf("Error by assembler (progrematic) %s crashed with error code: %d\n", filename, exit_code);
                return 1;
            }
        }
        else if (WIFSIGNALED(status))
        {
            printf("CRASH! %s crashed with error code: %d\n", filename, WTERMSIG(status));
            return 1;
        }
        else
        {
            printf("Assembler exited abnormally.\n");
            return 1;
        }
    }
    return 0;
}

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

void compare_expected_files_in_directory(const char *dir_path)
{
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dir_path)) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_REG && strstr(entry->d_name, ".expected") != NULL)
            {
                char expected_file_path[MAX_PATH_LENGTH];
                sprintf(expected_file_path, "%s/%s", dir_path, entry->d_name);

                // Extract filename without .expected extension
                char file_name[MAX_FILENAME_LENGTH];
                strncpy(file_name, entry->d_name, strlen(entry->d_name) - strlen(".expected"));
                file_name[strlen(entry->d_name) - strlen(".expected")] = '\0';

                // Construct relative file path
                char result_file_path[MAX_PATH_LENGTH];
                sprintf(result_file_path, "%s/%s", dir_path, file_name);

                // Check if the corresponding file exists
                if (access(result_file_path, F_OK) != -1)
                {
                    // Use a diff tool (replace with your preferred diff implementation)
                    pid_t diff_pid = fork();
                    if (diff_pid == -1)
                    {
                        perror("fork failed");
                        exit(EXIT_FAILURE);
                    }
                    else if (diff_pid == 0)
                    {
                        freopen("/dev/null", "w", stdout); // Optional: Suppress diff output on success
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
                            printf("%s: passed\n", file_name);
                        }
                        else
                        {
                            printf("%s: failed\n", file_name);
                        }
                    }
                }
                else
                {
                    printf("Test failed: %s does not exist\n", result_file_path);
                }
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

void compile_assembly_files_in_directory(const char *dir_path)
{
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(dir_path)) != NULL)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == DT_REG && strstr(entry->d_name, ".as") != NULL)
            {
                char full_file_path[MAX_PATH_LENGTH];
                sprintf(full_file_path, "%s/%s", dir_path, entry->d_name);

                // Call compile_assembly_file with the full path
                compile_assembly_file(full_file_path);
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

    compile_assembly_files_in_directory(current_path);
    compare_expected_files_in_directory(current_path);

    return EXIT_SUCCESS;
}
