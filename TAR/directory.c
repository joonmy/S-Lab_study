#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
void finddir(DIR *dp, int layer, char path[])
{
    dp = opendir(path);
    struct dirent *ep;

    if (dp != NULL)
    {
        while (ep = readdir(dp))
        {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)
            {
                continue;
            }
            char s[30];
            strcpy(s, path);
            strcat(s, ep->d_name);
            DIR *p = opendir(s);
            if (p != NULL)
            {
                DIR *a;
                char s1[10] = "/";
                strcat(s, s1);
                for (int i = 0; i < layer; i++)
                {
                    printf("    ");
                }
                //printf("Directory : ");
                puts(ep->d_name);
                layer += 1;
                finddir(a, layer, s);
                layer -= 1;
            }
            else
            {
                for (int i = 0; i < layer; i++)
                {
                    printf("    ");
                }
                char b[100];
                strcpy(b, path);
                strcat(b, ep->d_name);
                //printf("%s  ", b);
                //printf("File : ");
                puts(ep->d_name);
            }
        }
        closedir(dp);
    }
    else
    {
        perror("Couldn't open the directory\n");
    }
}
int main(void)
{
    DIR *dp;
    char path[200] = "C:\\Users\\JunMung\\Desktop\\mystudy\\";
    finddir(dp, 0, path);
}