// NI DAQ PCIe-6363: base 0.1 V, pulse 2.0 V for 100 us on 'e', quit on 'q'.
// Two-task approach:
//  - Use a short-lived on-demand task to set/hold 0.1 V (then clear it).
//  - Create a fresh finite-sample task for each pulse, then clear it.
// The device holds the last DAC code when tasks stop/clear.

#include <stdio.h>
#include <iostream>
#include <vector>
#include <conio.h>    // _kbhit, _getch (Windows)
#include <windows.h>  // Sleep()
#include "NIDAQmx.h"

#define DAQmxErrChk(functionCall) do { if (DAQmxFailed(error=(functionCall))) goto Error; } while(0)

static const char* PHYS_CHAN = "Dev1/ao0";
static const float64 BASE_V = 1.0;
static const float64 PULSE_V = 2.0;
static const float64 SR = 1.0e6;     // 1 MS/s
static const double  PULSE_US = 1000.0;     // 100 microseconds
static const int32   BASE_TAIL_SAMPS = 10; // a few base samples after pulse
static const float64 TIMEOUT_S = 10.0;

// write base voltage using a short-lived, on-demand task
static int write_base_level(float64 volts) {
    int32 error = 0;
    TaskHandle t = 0;
    char errBuff[2048] = { 0 };

    DAQmxErrChk(DAQmxCreateTask("", &t));
    DAQmxErrChk(DAQmxCreateAOVoltageChan(t, PHYS_CHAN, "",
        -10.0, 10.0, DAQmx_Val_Volts, NULL));
    // On-demand scalar write (no need to StartTask)
    DAQmxErrChk(DAQmxWriteAnalogScalarF64(t, 1, TIMEOUT_S, volts, NULL));

    // Clear the task; device holds the last DAC code (volts)
    DAQmxErrChk(DAQmxClearTask(t));
    return 0;

Error:
    if (DAQmxFailed(error)) {
        DAQmxGetExtendedErrorInfo(errBuff, 2048);
        printf("DAQmx Error (write_base_level): %s\n", errBuff);
    }
    if (t) DAQmxClearTask(t);
    return -1;
}

int main(int, char**) {
    int32 error = 0;
    char errBuff[2048] = { 0 };

    // derived counts
    int32 nPulse = static_cast<int32>(PULSE_US * (SR / 1.0e6));
    if (nPulse < 1) nPulse = 1;
    const int32 nSamps = nPulse + BASE_TAIL_SAMPS;

    printf("NI DAQ Voltage Control (PCIe-6363)\n");
    printf("Channel: %s\n", PHYS_CHAN);
    printf("Base: %.3f V, Pulse: %.3f V for %.1f us @ %.0f S/s\n", BASE_V, PULSE_V, PULSE_US, SR);
    printf("Press 'e' to send pulse; 'q' to quit.\n\n");

    if (write_base_level(BASE_V) != 0) return -1;
    printf("Output set to %.3f V. Waiting for key press...\n", BASE_V);

    for (;;) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'q' || ch == 'Q') break;

            if (ch == 'e' || ch == 'E') {
                // build finite waveform: [PULSE_V x nPulse] then [BASE_V x BASE_TAIL_SAMPS]
                std::vector<float64> wave(nSamps, BASE_V);
                for (int32 i = 0; i < nPulse; ++i) wave[i] = PULSE_V;

                TaskHandle pulseTask = 0;

                // create fresh finite-sample task for the pulse
                DAQmxErrChk(DAQmxCreateTask("", &pulseTask));
                DAQmxErrChk(DAQmxCreateAOVoltageChan(pulseTask, PHYS_CHAN, "",
                    -10.0, 10.0, DAQmx_Val_Volts, NULL));

                DAQmxErrChk(DAQmxCfgSampClkTiming(pulseTask, "",
                    SR, DAQmx_Val_Rising,
                    DAQmx_Val_FiniteSamps,
                    nSamps));
                // match device buffer to our transfer
                DAQmxErrChk(DAQmxCfgOutputBuffer(pulseTask, nSamps));

                // allow regeneration (finite burst of exactly what we queue)
                DAQmxErrChk(DAQmxSetWriteRegenMode(pulseTask, DAQmx_Val_AllowRegen));

                int32 written = 0;
                DAQmxErrChk(DAQmxWriteAnalogF64(pulseTask,
                    nSamps,
                    0,                 // autoStart = 0
                    TIMEOUT_S,
                    DAQmx_Val_GroupByChannel,
                    wave.data(),
                    &written,
                    NULL));
                if (written != nSamps) {
                    printf("Warning: wrote %d of %d samples.\n", written, nSamps);
                }

                DAQmxErrChk(DAQmxStartTask(pulseTask));
                DAQmxErrChk(DAQmxWaitUntilTaskDone(pulseTask, TIMEOUT_S));
                DAQmxErrChk(DAQmxStopTask(pulseTask));
                DAQmxErrChk(DAQmxClearTask(pulseTask));
                pulseTask = 0;

                // explicitly reassert base level (held already, but explicit is nice)
                if (write_base_level(BASE_V) != 0) goto Error;

                printf("Pulse sent: %.3f V for %.1f us. Output returned to %.3f V.\n",
                    PULSE_V, PULSE_US, BASE_V);
            }
        }
        Sleep(1); // avoid busy loop (hardware controls the timing)
    }

    // final: ensure base level
    write_base_level(BASE_V);
    printf("Task cleared. Program ended.\n");
    return 0;

Error:
    if (DAQmxFailed(error)) {
        DAQmxGetExtendedErrorInfo(errBuff, 2048);
        printf("DAQmx Error: %s\n", errBuff);
    }
    // best-effort to restore base (holds even if errors occurred)
    write_base_level(BASE_V);
    return -1;
}
