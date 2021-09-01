
#ifndef SRC_SETTINGS_H_
#define SRC_SETTINGS_H_


#ifdef SIMULATOR
#define RPiLAB_WIN
#undef RPiLAB_RPi
#else
#define RPiLAB_RPi
#undef RPiLAB_WIN
#endif

#endif /* SRC_SETTINGS_H_ */
