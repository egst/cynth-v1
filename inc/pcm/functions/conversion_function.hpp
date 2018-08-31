#pragma once

/* Standard libraries: */
#include <functional>

namespace Cynth::PCM::Functions {

    /*/ Conversion Function:
    
        Input and output types are both customizable. It is used to convert
        an output of a device to match an input of another one.
    /*/
    template<typename input_t, typename output_t>
    class ConversionFunction {
    protected:
        /* Type definitions: */
        // Function definition:
        using def_t = std::function<output_t(input_t)>;

        /* Properties: */
        // Function definition:
        def_t def;
        
    public:
        /* Constructor: */
        ConversionFunction();
        ConversionFunction(def_t def);

        /* Accessors: */
        // Calling the function:
        output_t operator()(input_t input);

        /* Mutators: */
        // Setting the function definition:
        ConversionFunction<input_t, output_t>& operator=(def_t def);
    };

}