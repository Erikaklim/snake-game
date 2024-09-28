// #ifndef HIGHSCORE_H
// #define HIGHSCORE_H
//
// #ifdef __cplusplus
// extern "C" {
// #endif
//
//     typedef void (__cdecl *SaveHighScoreFunc)(int);
//     typedef int (__cdecl *LoadHighScoreFunc)(void);
//
//     extern __declspec(dllexport) SaveHighScoreFunc SaveHighScore;
//     extern __declspec(dllexport) LoadHighScoreFunc LoadHighScore;
//
// #ifdef __cplusplus
// }
// #endif
//
// #endif // HIGHSCORE_H
#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#ifdef __cplusplus
extern "C" {
#endif

    __declspec(dllexport) void __stdcall SaveHighScore(int score);
    __declspec(dllexport) int __stdcall LoadHighScore(void);

#ifdef __cplusplus
}
#endif

#endif // HIGHSCORE_H
