#include <fstream>
#include <iostream>

using namespace std;

int main()
{
    FILE* file;
    fopen_s(&file, "1.miloradov", "r + ");
    char buf[1024] = { 0 };
    fread_s(buf, 1024, sizeof(char), 1024, file);
    printf_s("Content: %s\n\n", buf);
    char buf_to_write[1024] = { "Miloradov Vladimir" };
    fseek(file, 0, 0);
    fwrite(buf_to_write, sizeof(char), 1024, file);
    fclose(file);
    getchar();
    return 0;
}