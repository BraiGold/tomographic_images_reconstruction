#include <string>
#include <iostream>
#include <dirent.h>
#include <cstdlib>
#include <stdio.h>
#include <vector>

using namespace std;


int main () {

    vector < string > nombresDCM;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("/home/botmaker/Desktop/TP3MetNum/imgCornell")) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir(dir)) != NULL)
        {
            // printf("%s\n", ent->d_name);
            if (ent->d_name != "" && ent->d_name[0] != "."[0])
            {
                string strDCMName(ent->d_name);
                nombresDCM.push_back(strDCMName.substr(0, strDCMName.find(".dcm")));
            }
        }
        closedir(dir);
    }
    else
    {
        /* could not open directory */
        perror("");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < nombresDCM.size(); i++)
    {
        string run = "dcm2pnm " + nombresDCM[i] + ".dcm" + " " + nombresDCM[i] + ".pgm";
        system(run.c_str());
    }

    return 0;

}