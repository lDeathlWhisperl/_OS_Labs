#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[])
{
    for(int i = 0; i < argc; ++i)
    {
        std::cout << argv[i]; // <-- специально без std::flush, чтобы был "более красивый" вывод на экран
        sleep(1.5);
    }
    return 0;
}
