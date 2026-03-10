#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#ifdef _WIN32
#define CLEAR "cls"
#elif defined(unix)||defined(__unix__)||defined(__unix)||defined(__APPLE__)||defined(__MACH__)
#define CLEAR "clear"
#else
#error "SO no soportado para limpiar pantalla"
#endif

void clearScreen(){
        system(CLEAR);
}

void printSmallSeparator(){
    cout << "-------------------------------------------" << endl;
}

void printLongSeparator(){
    cout << "------------------------------------------------------------------------------------" << endl;
}

#endif // UTIL_H_INCLUDED
