// Parameters of input Files.

#define __NumOfSetpoint 4

// Parameters of input waveforms.

#define __WaveLength 21000
// the default length is 50000
#define __NumOfWave 50000

// Peak Search Parameters

#define __PrimaryPeakLeftMost 240
#define __PrimaryPeakRightMost 300
#define __PrimaryPeakSearchLeft -30
#define __PrimaryPeakSearchRight 30

// Primary pulse calculation parameters.

#define __PrimaryPeakIntLeft -19
#define __PrimaryPeakIntRight 55
#define __PrimaryBaselineLeft -220
#define __PrimaryBaselineRight -20

// Average waveform with aligned peak

#define __AlignedPeakLeft -250
#define __AlignedPeakRight 20800

// Afterpulse calculation parameters.

//#define __AfterpulseSearchLeft 150 //original 500
#define __AfterpulseSearchLeft 150
#define __AfterpulseSearchRight 20800
#define __AfterpulseThreshold 3
#define __AfterpulseIntWindow 75

#define __AfterpulsePeakIntLeft -20
#define __AfterpulsePeakIntRight 55
#define __AfterpulseBaselineLeft -95
#define __AfterpulseBaselineRight -20






