#pragma once

#include <functional> // -> std::function

namespace Cynth {

    /*/ Generic Time Function:
    
        For a given period offset returns the corresponding function value.
    /*/
    template<typename output_t>
    class TimeFunction {
    protected:
        /* Typedefs: */
        // Function definition:
        typedef std::function<output_t(float)> def_t;

        /* Properties: */
        // Function definition:
        def_t def;
    
    public:
        /* Constructor: */
        TimeFunction();
        TimeFunction(def_t def);

        /* Interaction with the function: */
        // Call the function:
        output_t operator() (float offset);
        // For int input:
        output_t operator() (int offset);
        // Set the function definition:
        void setDef(def_t def);
    };

    /*/ Wave Function:
    
        Generates continuous values.
    /*/
    class WaveFunction: public TimeFunction<float> {
    protected:
        /* Typedefs: */
        // Function definition:
        typedef TimeFunction<float>::def_t def_t;
    
    public:
        /* Constructor: */
        WaveFunction();
        WaveFunction(def_t def);
    };

    /*/ Basic wave functions: /*/
    enum wave_t {
        SINE,
        SQUARE,
        SAW,
        TRIANGLE
    };

    class WaveFs {
    public:
        /* Constructor: */
        // Initializes the functions.
        WaveFs();

        /* Functions: */
        // Sine Wave:
        WaveFunction sine;
        // Square Wave:
        WaveFunction square;
        // Saw Wave:
        WaveFunction saw;
        // Triangle Wave:
        WaveFunction triangle;
    };

    /*/ Sequence Function:
    
        Generates discrete values.
    /*/
    template<typename output_t>
    class SequenceFunction: public TimeFunction<output_t> {};

    /*/ Conversion Function:
    
        Input and output types are both customizable. It is used to convert
        an output of a device to match an input of another one.
    /*/
    template<typename input_t, typename output_t>
    class ConversionFunction {
    protected:
        /* Typedefs: */
        // Function definition:
        typedef std::function<output_t(input_t)> def_t;

        /* Properties: */
        // Function definition:
        def_t def;
        
    public:
        /* Constructor: */
        ConversionFunction();
        ConversionFunction(def_t def);

        /* Interaction with the function: */
        // Calling the function:
        output_t operator() (input_t input);
        // Setting the function definition:
        void setDef(def_t def);
    };

    /*/ Frequency conversion functions: /*/
    class ConvFs {
    public:
        /* Constructor: */
        // Initializes the functions.
        ConvFs();

        /* Functions: */
        // No conversion dummy function:
        ConversionFunction<float, float> doNotConvert;
        // Convert Measure Fractions to Hertz:
        ConversionFunction<float, float> convertMF;
        // Convert Measures per Minute to Hertz:
        ConversionFunction<float, float> convertMPM;
        // Convert Beats per Minute to Hertz:
        ConversionFunction<float, float> convertBPM;
    };
}