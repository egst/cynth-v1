#pragma once

namespace Cynth::UserLibrary::Functions {

    /*/ Basic wave functions: /*/
    enum wave_func_t {
        SINE,
        SINE_POS,
        SQUARE,
        SAW,
        TRIANGLE
    };

    /*/ Frequency types: /*/
    enum freq_type_t {
        // Absolute frequency in Hertz:
        HZ,
        // Relative frequency in Measure Fractions:
        // 1 corresponds to one repetition per measure.
        // 1/2 to two repetitions per measure.
        // 2 to one repetition per two measures etc...
        MF,
        // Relative frequency in Measures per Minute:
        MPM,
        // Relative frequency in Beats per Minute:
        // One beat corresponds to a fourth note.
        BPM
    };

}