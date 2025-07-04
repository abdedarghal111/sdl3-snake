#pragma once

namespace hidden {
    struct FPSRecorder {
        double* fpAVG = new double[60]();
        int pointer = 0;

        void record(double fps) {
            fpAVG[pointer] = fps;
            pointer = (pointer + 1) % 60;
        }

        double getAVG() {
            double sum = 0;
            for (int i = 0; i < 60; i++) {
                sum += fpAVG[i];
            }
            return (sum == 0) ? 0 : sum / 60;
        }
    };
}

hidden::FPSRecorder* fpsRecorder = new hidden::FPSRecorder();