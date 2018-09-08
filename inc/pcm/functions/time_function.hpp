#pragma once

/* Standard libraries: */
#include <functional>

namespace Cynth::PCM::Functions {

    /*/ Generic Time Function:
    
        For a given period offset returns the corresponding function value.
    /*/
    template<typename output_t = float>
    class TimeFunction {
    protected:
        /* Type definitions: */
        // Function definition:
        using def_t = std::function<output_t(double)>;

        /* Properties: */
        // Function definition:
        def_t def;
        // Starting offset:
        double start;
    
    public:
        /* Constructor: */
        TimeFunction();
        TimeFunction(def_t def);

        /* Accessors: */
        // Call the function:
        virtual output_t operator() (double offset) const;
        double getStart() const;
        void setStart(double offset);

        /* Mutators: */
        // Set the function definition:
        TimeFunction<output_t>& operator=(def_t def);
    };

}