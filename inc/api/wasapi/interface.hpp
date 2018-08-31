#pragma once

namespace Cynth::API::WASAPI {

    /*/ Generic Interface:
    WASAPI interfaces abstractions.
    
    These provide abstractions for COM interfaces with key features:
        * Safe to work with without managing the memory allocation manually.
        * More modern C++ approach: Avoiding out parameters etc.
        * HRESULT check is done by these abstractions.
    
    COM interfaces get released and all pointer allocated memory is freed
    when an Interface object is destructed.
    This may be suppressed with auto_release property set to false.
    Suppressing the release is used when the object is created
    on the stack, then returned by value.
    When obtaining such objects, use interface_object.autoRelease(),
    or call interface_object->Release() manually when needed.
    
    The COM interface instance is accessible via ptr_instance pointer.
    The -> operator of the Interface class is overloaded to provide
    this instance too:
    Instead of interface_object.ptr_instance->CoMethod()
    use interface_object->CoMethod().
    
    Some COM methods are wrapped in a custom abstraction method.
    These have the same names as the COM methods,
    but they follow local naming convention - camelCase:
    If there is an abstraction for interface_object->CoMethod() provided,
    use interface_object.coMethod().
    
    Some COM interface abstractions contain initialization
    with CoCreateInstance() inside their constructor.
    Some are constructed via passing the interface pointer
    (received from other abstractions) to their constructor.
    
    HRESULT is checked by these abstractions.
    When it fails, the situation is then handled by Cynth::Logger. /*/
    template<typename interface_t>
    class Interface {
    protected:
        /* Properties: */
        // WASAPI interface instance:
        interface_t* ptr_instance;
        // When false, the interface instance
        // is not released on destruction.
        bool auto_release;

    public:
        /* Constructor:
        
        Sets the ptr_instance property to the given interface pointer. */
        Interface();
        Interface(interface_t* ptr_instance);

        /* Move & Copy: */
        // Copy constructor:
        Interface(const Interface<interface_t>& other);
        // Copy assignment:
        Interface<interface_t>& operator=(const Interface<interface_t>& other);
        // Move constructor:
        Interface(Interface<interface_t>&& other);
        // Move assignment:
        Interface<interface_t>& operator=(Interface<interface_t>&& other);

        /* Destructor:

        Releases the WASAPI interface instance. */
        virtual ~Interface();

        // Allow auto release: (default)
        void autoRelease();
        // Suppress auto release:
        void noAutoRelease();

        /* Accessors: */
        // Allows WASAPI instance method to be used directly:
        interface_t* operator->();
    };

}