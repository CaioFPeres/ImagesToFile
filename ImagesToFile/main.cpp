#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <fstream>
#include <locale>
#include <codecvt>

using namespace std;

vector<wstring>* getFilesNames(LPTSTR lpDir) {

    vector<wstring>* fileNames = new vector<wstring>;
    int dirNameSize = 0;

    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    TCHAR szDir[MAX_PATH];
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    // Prepare string for use with FindFile functions. First, copy the
    // string to a buffer, then append '\*' to the directory name.

    wcscpy_s(szDir, lpDir);

    for (; szDir[dirNameSize] != '\0'; dirNameSize++);

    szDir[dirNameSize++] = '\\';
    szDir[dirNameSize++] = '*';
    szDir[dirNameSize] = '\0';


    // Find the first file in the directory.

    hFind = FindFirstFile(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        wprintf_s(L"\nERROR: %lu", dwError);
        return fileNames;
    }

    // List all the files in the directory with some info about them.
    do
    {
        if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
        {
            // Encontrou diret�rios, N�o fazer nada;
            //wprintf_s(TEXT("  %s   <DIR>\n"), ffd.cFileName);
        }
        else
        {
            filesize.LowPart = ffd.nFileSizeLow;
            filesize.HighPart = ffd.nFileSizeHigh;
            //wprintf_s(TEXT("  %s   %ld bytes\n"), ffd.cFileName, (int)filesize.QuadPart);  // printa nomes e tamanhos dos arquivos;
            (*fileNames).push_back(ffd.cFileName);
        }

    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        wprintf_s(L"\nERROR: %lu", dwError);
    }


    FindClose(hFind);

    return fileNames;
}

void ConvertToSingleFile(wstring folder, vector<wstring>* fileNames) {

    ofstream outFile;
    outFile.open("images", std::ios::out | std::ios::binary);

    for (int i = 0; i < fileNames->size(); i++) {

        wcout << folder + wstring(L"\\") + (*fileNames)[i] << '\n';

        ifstream file;
        file.open(folder + wstring(L"\\") + (*fileNames)[i], ios_base::in | ios_base::binary);
        
        while (!file.eof())
        {
            char c = file.get();
            outFile << c;
        }

        outFile << '\n';
        file.close();

    }

    outFile.close();

}

int main(int argc, char** argv){

    if (argc > 2) {
        cout << "Only one argument!";
        return 0;
    }

    wstring folder;

    if (argc == 1) {
        cout << "This tool converts many images into one file, where each line contains each image." << '\n';
        cout << "Insert the path, or you could start this program and provide the path as a parameter: \n";

        wcin >> folder;
    }
    else {
        
        using convert_type = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_type, wchar_t> converter;

        std::wstring converted_str = converter.from_bytes(string(argv[1]));

        folder = converted_str;
    }

    vector<wstring>* fileNames = getFilesNames((LPTSTR)folder.c_str());

    ConvertToSingleFile(folder, fileNames);

    system("pause");

	return 0;
}