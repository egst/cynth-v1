#pragma once

namespace Cynth {

    /*/ Generic Input Device: /*/
    class InputDevice {};

    /*/ MIDI device: /*/
    class MIDIDevice: public InputDevice {};

    /*/ Generig Output Device: /*/
    class OutputDevice {};

    /*/ Sound Card Device: /*/
    class SoundCard: public OutputDevice {};

}