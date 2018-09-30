/* Exporting Python module for user accessible abstractions: */
#include "cynth.hpp" // Will also include platform.hpp
#ifdef CYNTH_LIB_PY
/* This file is not included in sources when compiling with CMake,
   but this macro prevents it in case of other compilation methods. */

/* Other local libraries: */
#include "pcm/functions/time_function.hpp"
#include "rack/components/receiver_port.hpp"
#include "rack/components/transmitter_port.hpp"
#include "rack/devices/input_device.hpp"
#include "rack/devices/oscillator.hpp"
#include "rack/devices/output_device.hpp"
// User accessible abstractions (from UserLibrary and logger.hpp) are included in cynth.hpp

/* pybind11: */
#include <cmath>
#define _hypot hypot // Fix for hypot in pybind11
#include "pybind11/pybind11.h"
#include "pybind11/operators.h"

using namespace Cynth;
using Cynth::PCM::Functions::TimeFunction;
using Cynth::Rack::Components::ReceiverPort;
using Cynth::Rack::Components::TransmitterPort;
using Cynth::Rack::Devices::InputDevice;
using Cynth::Rack::Devices::Oscillator;
using Cynth::Rack::Devices::OutputDevice;
using namespace Cynth::UserLibrary;
using namespace Cynth::UserLibrary::Devices;
using namespace Cynth::UserLibrary::Functions;
using wave_func_t = Types::wave_func_t;
using freq_type_t = Types::freq_type_t;
using comp_type_t = Types::comp_type_t;

PYBIND11_MODULE(Cynth, m) {

    pybind11::enum_<wave_func_t>(m, "wave_func_t")
        .value("SINE", wave_func_t::SINE)
        .value("SINE_POS", wave_func_t::SINE_POS)
        .value("SQUARE", wave_func_t::SQUARE)
        .value("SAW", wave_func_t::SAW)
        .value("TRIANGLE", wave_func_t::TRIANGLE);
    
    pybind11::enum_<freq_type_t>(m, "freq_type_t")
        .value("HZ", freq_type_t::HZ)
        .value("MF", freq_type_t::MF)
        .value("MPM", freq_type_t::MPM)
        .value("BPM", freq_type_t::BPM);
    
    pybind11::enum_<comp_type_t>(m, "comp_type_t")
        .value("ADD", comp_type_t::ADD)
        .value("MULTIP", comp_type_t::MULTIP);

    pybind11::class_<TimeFunction<float>>(m, "TimeFunction")
        .def("__call__", &TimeFunction<float>::operator(), pybind11::is_operator())
        .def("__lshift__", &TimeFunction<float>::operator<<, pybind11::is_operator());

    // TODO: Templates.
    /*pybind11::class_<TimeFunction<int>>(m, "TimeFunction")
        .def("__lshift__", &TimeFunction<int>::operator<<, pybind11::is_operator());*/
    
    pybind11::class_<ReceiverPort>(m, "ReceiverPort")
        .def("__lshift__", &ReceiverPort::operator<<, pybind11::is_operator());
    
    pybind11::class_<TransmitterPort>(m, "TransmitterPort");
    
    pybind11::class_<InputDevice>(m, "InputDevice")
        .def_readonly("output_port", &InputDevice::output_port);
    
    pybind11::class_<Oscillator, InputDevice>(m, "Oscillator")
        .def_readwrite("freq_port", &Oscillator::freq_port)
        .def_readwrite("amp_port", &Oscillator::amp_port);
    
    pybind11::class_<OutputDevice>(m, "OutputDevice")
        .def("__lshift__", &OutputDevice::operator<<);
    
    pybind11::class_<Envelope, InputDevice, OutputDevice>(m, "Envelope")
        .def(pybind11::init<>());
    
    pybind11::class_<Hub, InputDevice>(m, "Hub")
        .def(pybind11::init<comp_type_t>(),
             pybind11::arg("comp_type") = comp_type_t::ADD)
        .def("__lshift__", &Hub::operator<<, pybind11::is_operator());

    pybind11::class_<LFO, Oscillator>(m, "LFO")
        .def(pybind11::init<WaveFunction, float, float, freq_type_t>(),
             pybind11::arg("wave_f"),
             pybind11::arg("freq") = 1,
             pybind11::arg("amp") = 0,
             pybind11::arg("freq_t") = freq_type_t::MF)
        .def(pybind11::init<wave_func_t, float, float, freq_type_t>(),
             pybind11::arg("wave_f"),
             pybind11::arg("freq") = 1,
             pybind11::arg("amp") = 0,
             pybind11::arg("freq_t") = freq_type_t::MF);
    
    pybind11::class_<Sequencer>(m, "Sequencer")
        .def(pybind11::init<>())
        .def("__lshift__", &Sequencer::operator<<, pybind11::is_operator())
        .def_readonly("amp_port", &Sequencer::amp_port)
        .def_readonly("freq_port", &Sequencer::freq_port);
    
    pybind11::class_<SoundCard, OutputDevice>(m, "SoundCard")
        .def(pybind11::init<>())
        .def("printProperties", &SoundCard::printProperties)
        .def("play", &SoundCard::play)
        .def("stop", &SoundCard::stop);
    
    pybind11::class_<ToneGenerator, Oscillator>(m, "ToneGenerator")
        .def(pybind11::init<WaveFunction, float, float, freq_type_t>(),
             pybind11::arg("wave_f"),
             pybind11::arg("freq") = 440,
             pybind11::arg("amp") = 0,
             pybind11::arg("freq_t") = freq_type_t::HZ)
        .def(pybind11::init<wave_func_t, float, float, freq_type_t>(),
             pybind11::arg("wave_f"),
             pybind11::arg("freq") = 440,
             pybind11::arg("amp") = 0,
             pybind11::arg("freq_t") = freq_type_t::HZ);

    pybind11::class_<SequenceElement>(m, "SequenceElement")
        .def(pybind11::init<double, float, float>(),
             pybind11::arg("duration"),
             pybind11::arg("amp") = 0,
             pybind11::arg("freq") = 0);
    
    pybind11::class_<SequenceFunction>(m, "SequenceFunction")
        .def(pybind11::init<>())
        .def("__lshift__", pybind11::overload_cast<SequenceElement>(&SequenceFunction::operator<<))
        .def("__lshift__", pybind11::overload_cast<SequenceFunction>(&SequenceFunction::operator<<))
        .def("__imul__", &SequenceFunction::operator*=);
    
    pybind11::class_<WaveFunction>(m, "WaveFunction")
        .def(pybind11::init<>())
        .def(pybind11::init<WaveFunction::def_t>());
    
    pybind11::class_<WaveFs>(m, "WaveFs")
        .def(pybind11::init<>())
        .def_readonly("sine", &WaveFs::sine)
        .def_readonly("sine_pos", &WaveFs::sine_pos)
        .def_readonly("square", &WaveFs::square)
        .def_readonly("saw", &WaveFs::saw)
        .def_readonly("triangle", &WaveFs::triangle);
    
    pybind11::class_<Logger>(m, "Logger")
        .def("log", pybind11::overload_cast<std::string>(&Logger::log))
        .def("log", pybind11::overload_cast<int>(&Logger::log))
        .def("log", pybind11::overload_cast<float>(&Logger::log));

    pybind11::class_<PlatformException>(m, "PlatformException")
        .def("what", &PlatformException::what)
        .def("log", &PlatformException::log);

    pybind11::class_<COMException>(m, "COMException")
        .def("what", &COMException::what)
        .def("log", &COMException::log);

    pybind11::class_<WASAPIException>(m, "WASAPIException")
        .def("what", &WASAPIException::what)
        .def("log", &WASAPIException::log);

    pybind11::class_<CynthException>(m, "CynthException")
        .def("what", &CynthException::what)
        .def("log", &CynthException::log);
    
    // TODO: Maybe this syntax will be needed
    /*.def("__lshift__", [](Hub& hub, const TransmitterPort& port) -> Hub& {
        return hub << port;
    }, pybind11::is_operator());*/

}

#endif