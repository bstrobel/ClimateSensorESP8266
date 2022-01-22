# Do not use 'using namespace std' with Platform.io and Arduino Framework 3.x.x+

<https://github.com/things4u/ESP-1ch-Gateway/issues/82>

## Backround

Arduino and C++ 2017 define type 'byte' differently and conflictingly. This can only be avoided if 'using namespace std' is not used.